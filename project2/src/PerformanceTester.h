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
#include <filesystem>
#include "Pair.h"

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
        [raw]     structure, operation, n, repetition, time_us
        [summary] structure, operation, n, repetitions, avg_us, min_us, max_us

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
    using us    = std::chrono::microseconds;

    struct Result {
        std::string operation;
        int         n;
        int         repetition;
        long long   time_us;
    };

    // ── Timing ────────────────────────────────────────────────────────────────

    template <typename Func>
    long long measure(Func&& f) {
        auto t0 = Clock::now();
        f();
        auto t1 = Clock::now();
        return std::chrono::duration_cast<us>(t1 - t0).count();
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

        // Raw section
        file << "structure,operation,n,repetition,time_us\n";
        for (const auto& r : results)
            file << implName     << ","
                 << r.operation  << ","
                 << r.n          << ","
                 << r.repetition << ","
                 << r.time_us    << "\n";

        // Summary section — aggregate per (operation, n)
        file << "\n";
        file << "structure,operation,n,repetitions,avg_us,min_us,max_us\n";

        std::vector<std::pair<std::string, int>> keys;
        for (const auto& r : results) {
            auto key = std::make_pair(r.operation, r.n);
            if (std::find(keys.begin(), keys.end(), key) == keys.end())
                keys.push_back(key);
        }

        for (const auto& key : keys) {
            std::vector<long long> times;
            for (const auto& r : results)
                if (r.operation == key.first && r.n == key.second)
                    times.push_back(r.time_us);

            if (times.empty()) continue;
            long long sum = std::accumulate(times.begin(), times.end(), 0LL);
            long long mn  = *std::min_element(times.begin(), times.end());
            long long mx  = *std::max_element(times.begin(), times.end());

            file << implName                                  << ","
                 << key.first                                 << ","
                 << key.second                                << ","
                 << times.size()                              << ","
                 << static_cast<long long>(static_cast<double>(sum) / times.size()) << ","
                 << mn                                        << ","
                 << mx                                        << "\n";
        }
    }

    // ── Per-size benchmark ────────────────────────────────────────────────────

    void runForSize(int n, int reps,
                    const std::vector<Pair<T>>& data,
                    std::vector<Result>&         results) {
        int useN = std::min(n, static_cast<int>(data.size()));

        if (useN < n)
            std::cout << "  UWAGA: dostepnych " << data.size()
                      << " elementow, testowanie dla n=" << useN << "\n";

        std::cout << "    n=" << useN
                  << "  [" << reps << " powtorzen]... ";
        std::cout.flush();

        for (int rep = 1; rep <= reps; rep++) {

            // enqueue — n insertions into empty queue
            {
                QueueImpl q;
                long long t = measure([&]() {
                    for (int i = 0; i < useN; i++) q.enqueue(data[i]);
                });
                results.push_back({"enqueue", useN, rep, t});
            }

            // peek — single read on populated queue
            // peek() returns const Pair<T>& — copy to a local variable to prevent
            // the compiler from optimising the call away entirely.
            {
                QueueImpl q;
                for (int i = 0; i < useN; i++) q.enqueue(data[i]);
                Pair<T> peekResult;
                long long t = measure([&]() {
                    if (!q.isEmpty()) peekResult = q.peek();
                });
                (void)peekResult;
                results.push_back({"peek", useN, rep, t});
            }

            // extractMax — n/2 extractions
            {
                int xCount = useN / 2;
                QueueImpl q;
                for (int i = 0; i < useN; i++) q.enqueue(data[i]);
                long long t = measure([&]() {
                    for (int i = 0; i < xCount && !q.isEmpty(); i++)
                        q.extractMax();
                });
                results.push_back({"extractMax_x" + std::to_string(xCount),
                                   useN, rep, t});
            }
        }

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