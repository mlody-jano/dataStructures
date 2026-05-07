#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include <limits>
#include <numeric>
#include <random>
#include <filesystem>
#include "Pair.h"
#include "HeapQueue.h"

/*
    Performance tester for any Queue<T> implementation.

    Scans an input directory for all *.csv files matching a given prefix,
    runs tests on each one separately, and writes one result CSV per input
    file into a designated output directory.

    Input CSV format (with header):
        value,priority
        42,5
        ...

    Output CSV per input file — two sections:
        [raw]     structure, operation, n, repetition, time_ns
        [summary] structure, operation, n, repetitions, avg_ns, min_ns, max_ns

    Template parameters:
        QueueImpl — concrete queue class (must be default-constructible)
        T         — value type stored in the queue

    Tested operations per size n:
        enqueue     — n insertions into empty queue
        peek        — 1 call on populated queue of size n
        extractMax  — n/2 extractions from populated queue
*/
/*
    Plain-data struct shared by all PerformanceTester instantiations.
    Defined outside the template so DTQueue and HeapQueue testers can
    exchange config objects without type-conversion errors.
*/

std::random_device rd;
std::mt19937 gen(rd());

struct BenchmarkConfig {
    std::string      inputDir;
    std::string      filePrefix;
    std::string      outputDir;
    std::vector<int> sizes;
    int              repetitions = 1;
};

template <typename QueueImpl, typename T>
class PerformanceTester {
private:
    std::string implName;

    using Clock = std::chrono::high_resolution_clock;
    using ns    = std::chrono::nanoseconds;

    struct Result {
        std::string operation;
        int         n;
        int         sample_count;   // reps * COPIES — total number of measurements averaged
        double   avg_ns;         // averaged time across all samples
    };

    // ── Timing ────────────────────────────────────────────────────────────────

    template <typename Func>
    double measure(Func&& f) {
        auto t0 = Clock::now();
        f();
        auto t1 = Clock::now();
        return std::chrono::duration_cast<ns>(t1 - t0).count();
    }

    // ── Directory scan ────────────────────────────────────────────────────────

    /*
        Returns a sorted list of all *.csv files in `dir` whose filename
        starts with `prefix` (empty prefix = all csv files).
    */
    std::vector<std::string> scanDirectory(const std::string& dir,
                                            const std::string& prefix) {
        std::vector<std::string> files;
        std::error_code ec;

        if (!std::filesystem::exists(dir, ec) || !std::filesystem::is_directory(dir, ec)) {
            std::cerr << "  BLAD: Katalog nie istnieje lub nie jest katalogiem: " << dir << "\n";
            return files;
        }

        for (const auto& entry : std::filesystem::directory_iterator(dir, ec)) {
            if (ec) break;
            if (!entry.is_regular_file()) continue;

            std::string fname = entry.path().filename().string();

            // Must end with .csv
            if (fname.size() < 4 || fname.substr(fname.size() - 4) != ".csv") continue;

            // Must start with prefix (if given)
            if (!prefix.empty() && fname.substr(0, prefix.size()) != prefix) continue;

            files.push_back(entry.path().string());
        }

        std::sort(files.begin(), files.end());
        return files;
    }

    // ── CSV I/O ───────────────────────────────────────────────────────────────

    std::vector<Pair<T>> loadCSV(const std::string& path) {
        std::vector<Pair<T>> data;
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "  BLAD: Nie mozna otworzyc: " << path << "\n";
            return data;
        }

        std::string line;
        std::getline(file, line);   // skip header

        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;
            // Strip trailing CR (Windows line endings)
            if (!line.empty() && line.back() == '\r') line.pop_back();

            std::stringstream ss(line);
            std::string valStr, prioStr;
            if (!std::getline(ss, valStr,  ',')) continue;
            if (!std::getline(ss, prioStr, ',')) continue;
            try {
                T   val  = static_cast<T>(std::stoi(valStr));
                int prio = std::stoi(prioStr);
                data.emplace_back(val, prio);
            } catch (...) { continue; }
        }
        return data;
    }

    void saveCSV(const std::string& path,
                 const std::string& sourceFile,
                 const std::vector<Result>& results) {
        std::ofstream file(path);
        if (!file.is_open()) {
            std::cerr << "  BLAD: Nie mozna zapisac: " << path << "\n";
            return;
        }

        // Metadata header
        file << "# structure: " << implName << "\n";
        file << "# source:    " << sourceFile << "\n";
        file << "\n";

        // Raw section — one averaged row per (operation, n)
        file << "structure,operation,n,sample_count,avg_ns\n";
        for (const auto& r : results)
            file << implName       << ","
                 << r.operation    << ","
                 << r.n            << ","
                 << r.sample_count << ","
                 << r.avg_ns       << "\n";

        // Summary section — identical to raw (already averaged over reps*COPIES)
        file << "# summary \n";
        file << "structure,operation,n,sample_count,avg_ns\n";
        for (const auto& r : results)
            file << implName       << ","
                 << r.operation    << ","
                 << r.n            << ","
                 << r.sample_count << ","
                 << r.avg_ns       << "\n";
    }

    // ── Per-size benchmark ────────────────────────────────────────────────────

    static constexpr int COPIES = 100;   // number of independent queue instances per rep

    /*
        For each operation:
          1. Build COPIES pre-populated queues (identical state, independent objects).
          2. Repeat `reps` times:
               for each copy — measure the operation once, record the time.
          3. Compute avg / min / max over all (COPIES * reps) samples and push
             a single Result row to `results`.

        Using multiple copies eliminates measurement bias caused by cache warm-up
        or queue-state side effects (e.g. extractMax empties the queue, so each
        copy provides a fresh independent starting point for the next rep).
    */
    void runForSize(int n, int reps,
                    const std::vector<Pair<T>>& data,
                    std::vector<Result>&         results) {

        int useN = std::min(n, static_cast<int>(data.size()));

        if (useN < n)
            std::cout << "  UWAGA: dostepnych " << data.size()
                      << " elementow, testowanie dla n=" << useN << "\n";

        std::cout << "    n=" << useN
                  << "  [" << reps << " rep x " << COPIES << " kopii]... ";
        std::cout.flush();

        // ── Collect per-rep averaged times ────────────────────────────────────
        //
        // Each repetition:
        //   1. Build ONE reference queue via enqueue (O(n^2) for DTQueue) — once.
        //   2. Clone it COPIES times using the copy constructor — O(n) per clone.
        //   3. Execute the target operation on ALL COPIES in one timed block,
        //      then divide: avg = (end - start) / COPIES.
        //
        // Cost per rep: O(n^2) build + O(COPIES*n) clone — instead of the
        // previous O(COPIES*n^2) which dominated the entire measurement.
        //
        std::vector<double> enqTimes, peekTimes, extTimes, decTimes, incTimes, sizeTimes;
        enqTimes .reserve(reps);
        peekTimes.reserve(reps);
        extTimes .reserve(reps);
        decTimes .reserve(reps);
        incTimes .reserve(reps);
        sizeTimes.reserve(reps);

        const Pair<T> extraElement = data[useN > 0 ? useN - 1 : 0];

        for (int rep = 0; rep < reps; rep++) {

            // Build reference queues once per rep
            QueueImpl refFull;                              // n elements  — for peek / extractMax
            for (int j = 0; j < useN; j++)
                refFull.enqueue(data[j]);

            QueueImpl refEnq;                               // n-1 elements — for enqueue
            for (int j = 0; j < useN - 1; j++)
                refEnq.enqueue(data[j]);

            // ── enqueue: clone refEnq, measure n-th insertion ────────────────
            {
                std::vector<QueueImpl> q(COPIES, refEnq);  // copy-constructor x COPIES — O(n) each
                double t = measure([&]() {
                    for (int i = 0; i < COPIES; i++)
                        q[i].enqueue(extraElement);
                });
                enqTimes.push_back(t / COPIES);
            }

            // ── peek: clone refFull, measure single peek ──────────────────────
            {
                std::vector<QueueImpl> q(COPIES, refFull);
                Pair<T> peekResult;
                double t = measure([&]() {
                    for (int i = 0; i < COPIES; i++)
                        peekResult = q[i].peek();
                });
                (void)peekResult;
                peekTimes.push_back(t / COPIES);
            }

            // ── extractMax: clone refFull, measure single removal ─────────────
            {
                std::vector<QueueImpl> q(COPIES, refFull);
                double t = measure([&]() {
                    for (int i = 0; i < COPIES; i++)
                        q[i].extractMax();
                });
                extTimes.push_back(t / COPIES);
            }
            // -- decreaseKey
            {
                std::vector<QueueImpl> q(COPIES, refFull);
                Pair<T> element = data[gen() % useN];
                double t = measure([&]() {
                    for (int i = 0; i < COPIES; i++)
                        q[i].decreaseKey(element, element.getPriority() - 1);
                });
                decTimes.push_back(t / COPIES);
            }
            // -- increaseKey
            {
                Pair<T> element = data[gen() % useN];
                std::vector<QueueImpl> q(COPIES, refFull);
                double t = measure([&]() {
                    for (int i = 0; i < COPIES; i++)
                        q[i].increaseKey(element, element.getPriority() + 1);
                });
                incTimes.push_back(t / COPIES);
            }
            // -- size
            {
                std::vector<QueueImpl> q(COPIES, refFull);
                double t = measure([&]() {
                    for (int i = 0; i < COPIES; i++)
                        q[i].size();
                });
                sizeTimes.push_back(t / COPIES);
            }

        }

        // ── Aggregate: one Result row per operation ───────────────────────────
        auto pushResult = [&](const std::string& op,
                               std::vector<double>& times) {
            double sum = std::accumulate(times.begin(), times.end(), 0LL);
            double avg = static_cast<double>(
                                static_cast<double>(sum) / times.size());
            results.push_back({op, useN, reps * COPIES, avg});
        };

        pushResult("enqueue",    enqTimes);
        pushResult("peek",       peekTimes);
        pushResult("extractMax", extTimes);
        pushResult("decreaseKey", decTimes);
        pushResult("increaseKey", incTimes);
        pushResult("size",       sizeTimes);

        std::cout << "OK\n";
    }

    // ── Per-file run ──────────────────────────────────────────────────────────

    void runForFile(const std::string& inputPath,
                    const std::string& outputDir,
                    const std::vector<int>& sizes,
                    int reps) {

        // Derive output filename: results_{implName}_{inputStem}.csv
        std::string stem = std::filesystem::path(inputPath).stem().string();
        std::string outPath = outputDir + "/results_" + implName + "_" + stem + ".csv";

        std::cout << "\n  [" << stem << "]\n";
        std::cout << "  Wczytywanie... ";

        std::vector<Pair<T>> data = loadCSV(inputPath);
        if (data.empty()) {
            std::cout << "BLAD: brak danych — pomijam.\n";
            return;
        }
        std::cout << data.size() << " elementow zaladowano.\n";

        std::vector<Result> results;
        for (int n : sizes) runForSize(n, reps, data, results);

        // Ensure output directory exists
        std::filesystem::create_directories(outputDir);
        saveCSV(outPath, inputPath, results);
        std::cout << "  -> Wyniki: " << outPath << "\n";
    }

    // ── Console helpers ───────────────────────────────────────────────────────

    void clearInput() {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::vector<int> askSizes() {
        std::cout << "  Rozmiary n (oddzielone spacjami, zakoncz 0)\n";
        std::cout << "  np. 1000 2000 5000 0: ";
        std::vector<int> sizes;
        int s;
        while (std::cin >> s && s != 0)
            if (s > 0) sizes.push_back(s);
        clearInput();
        return sizes;
    }

public:
    explicit PerformanceTester(const std::string& name) : implName(name) {}

    /*
        Run with a pre-built config — no console prompting.
        Used by MainMenu when both implementations share the same session config.
    */
    void runWith(const BenchmarkConfig& cfg) {
        std::cout << "\n  === TESTY WYDAJNOSCIOWE: " << implName << " ===\n";

        std::vector<std::string> files = scanDirectory(cfg.inputDir, cfg.filePrefix);
        if (files.empty()) {
            std::cout << "  Nie znaleziono plikow CSV w: " << cfg.inputDir << "\n";
            return;
        }
        std::cout << "  Znaleziono " << files.size() << " plik(ow).\n";

        int done = 0;
        for (const auto& f : files) {
            std::cout << "\n  (" << ++done << "/" << files.size() << ") ";
            runForFile(f, cfg.outputDir, cfg.sizes, cfg.repetitions);
        }
        std::cout << "\n  Testy zakonczone. Wyniki w: " << cfg.outputDir << "\n";
    }

    void run() {
        std::cout << "\n  === TESTY WYDAJNOSCIOWE: " << implName << " ===\n\n";

        // Input directory
        std::string inputDir;
        std::cout << "  Katalog z plikami CSV (dane wejsciowe) : ";
        std::cin >> inputDir;
        clearInput();

        // Optional filename prefix filter
        std::string prefix;
        std::cout << "  Prefix nazwy pliku (Enter = wszystkie)  : ";
        std::getline(std::cin, prefix);
        if (!prefix.empty() && prefix.back() == '\n') prefix.pop_back();

        // Output directory
        std::string outputDir;
        std::cout << "  Katalog na pliki wynikowe              : ";
        std::cin >> outputDir;
        clearInput();

        // Scan for files
        std::vector<std::string> files = scanDirectory(inputDir, prefix);
        if (files.empty()) {
            std::cout << "  Nie znaleziono plikow CSV w: " << inputDir << "\n";
            return;
        }
        std::cout << "\n  Znaleziono " << files.size() << " plik(ow):\n";
        for (const auto& f : files)
            std::cout << "    " << std::filesystem::path(f).filename().string() << "\n";

        // Test parameters
        std::cout << "\n";
        std::vector<int> sizes = askSizes();
        if (sizes.empty()) {
            std::cout << "  Brak rozmiarow — testy anulowane.\n"; return;
        }

        int reps;
        std::cout << "  Liczba powtorzen na rozmiar            : ";
        std::cin >> reps;
        clearInput();
        if (reps < 1) reps = 1;

        // Run per file
        int done = 0;
        for (const auto& f : files) {
            std::cout << "\n  (" << ++done << "/" << files.size() << ") ";
            runForFile(f, outputDir, sizes, reps);
        }

        std::cout << "\n  Testy zakonczone. Wyniki w katalogu: " << outputDir << "\n";
    }
};