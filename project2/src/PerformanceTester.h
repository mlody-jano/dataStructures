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

/**
 * random numbers generator based on Mersenne Twister algorythm
 */
std::random_device rd;
std::mt19937 gen(rd());

/**
 * structure BenchmarkConfig
 * provides a structure of test starting conditions to easily pass them to testing functions
 */
struct BenchmarkConfig {
    std::string      inputDir;
    std::string      filePrefix;
    std::string      outputDir;
    std::vector<int> sizes;
    int              repetitions = 1;
};

/**
 * class PerformanceTester
 * @tparam QueueImpl refers to implementation of queue, @tparam T referes to the type of value in @class Pair
 * provides a testing interface to effectively carry out tests of operations on priority queues
 * input -> reads .csv files in source directory @dir
 * output -> saves .csv files in target directory @dir including 2 sections: raw_data and summary
 * output format -> structure, operation, n, repetition, time_ns
 */
template <typename QueueImpl, typename T>
class PerformanceTester {
private:
    std::string implName;

    using Clock = std::chrono::high_resolution_clock;
    using ns    = std::chrono::nanoseconds;

    /**
     * structure Result
     * provides a structure of data meant to be saved to .csv file after test
     */
    struct Result {
        std::string operation;
        int         n;
        int         sample_count;   // reps * COPIES — total number of measurements averaged
        double      avg_ns;         // averaged time across all samples
    };

    // time measurement

    /**
     * private method used to measure time of operation
     * @tparam specifies the function being currently measured
     * @return time taken to finish function
     */
    template <typename Func>
    double measure(Func&& f) {
        auto t0 = Clock::now();
        f();
        auto t1 = Clock::now();
        return std::chrono::duration_cast<ns>(t1 - t0).count();
    }

    // directory scan

    /**
     * private method to search for files in source directory
     * @param dir refers to source directory, @param prefix specifies target files
     * @return sorted vector of files in source directory
     */
    std::vector<std::string> scanDirectory(const std::string& dir, const std::string& prefix) {
        std::vector<std::string> files;
        std::error_code ec;

        if (!std::filesystem::exists(dir, ec) || !std::filesystem::is_directory(dir, ec)) {
            std::cerr << "  BLAD: Katalog nie istnieje lub nie jest katalogiem: " << dir << "\n";
            return files;
        }

        for (const auto& entry : std::filesystem::directory_iterator(dir, ec)) {
            if (ec) {break;}
            if (!entry.is_regular_file()) {continue;}

            std::string fname = entry.path().filename().string();

            // Must end with .csv
            if (fname.size() < 4 || fname.substr(fname.size() - 4) != ".csv") {continue;}

            // Must start with prefix (if given)
            if (!prefix.empty() && fname.substr(0, prefix.size()) != prefix) {continue;}

            files.push_back(entry.path().string());
        }

        std::sort(files.begin(), files.end());
        return files;
    }

    // loading data from .csv

    /**
     * private method to load data from .csv file into structure
     * @param path refers to the source data file
     * @return vector of type @class Pair of loaded data
     */
    std::vector<Pair<T>> loadCSV(const std::string& path) {
        std::vector<Pair<T>> data;
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "  BLAD: Nie mozna otworzyc: " << path << "\n";
            return data;
        }

        std::string line;
        std::getline(file, line);   // skip header row from input file

        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') {continue;}
            if (!line.empty() && line.back() == '\r') {line.pop_back();}

            std::stringstream ss(line);
            std::string valStr, prioStr;
            if (!std::getline(ss, valStr,  ',')) {continue;}
            if (!std::getline(ss, prioStr, ',')) {continue;}
            try {
                T   val  = static_cast<T>(std::stoi(valStr));
                int prio = std::stoi(prioStr);
                data.emplace_back(val, prio);
            } catch (...) { continue; }
        }
        return data;
    }

    /**
     * private method to save data from tests to target .csv file
     * @param path refers to target file path, @param sourceFile refers to source file, @param result refers to the vector of type @class Pair of results
     */
    void saveCSV(const std::string& path,
                 const std::string& sourceFile,
                 const std::vector<Result>& results) {
        std::ofstream file(path);
        if (!file.is_open()) {
            std::cerr << "  BLAD: Nie mozna zapisac: " << path << "\n";
            return;
        }

        // header
        file << "# structure: " << implName << "\n";
        file << "# source:    " << sourceFile << "\n";
        file << "\n";

        // raw data section
        file << "structure,operation,n,sample_count,avg_ns\n";
        for (const auto& r : results)
            file << implName       << ","
                 << r.operation    << ","
                 << r.n            << ","
                 << r.sample_count << ","
                 << r.avg_ns       << "\n";

        // summary section
        file << "# summary \n";
        file << "structure,operation,n,sample_count,avg_ns\n";
        for (const auto& r : results)
            file << implName       << ","
                 << r.operation    << ","
                 << r.n            << ","
                 << r.sample_count << ","
                 << r.avg_ns       << "\n";
    }

    // test per size

    static constexpr int COPIES = 100;   // number of independent queue instances per rep

    /**
     * private method for running test for a specific data structure size
     * @param n refers to size of structure, @param reps refers to repetition times, @param data refers to original data structure, @param results refers to vector of results
     */
    void runForSize(int n, int reps,
                    const std::vector<Pair<T>>& data,
                    std::vector<Result>&         results) {

        int useN = std::min(n, static_cast<int>(data.size()));              // used elements

        if (useN < n) {std::cout << "  UWAGA: dostepnych " << data.size() << " elementow, testowanie dla n=" << useN << "\n";}

        std::cout << "    n=" << useN << "  [" << reps << " rep x " << COPIES << " kopii]... ";
        std::cout.flush();

        // vectors for times of operations
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
            for (int j = 0; j < useN; j++) {refFull.enqueue(data[j]);}

            QueueImpl refEnq;                               // n-1 elements — for enqueue
            for (int j = 0; j < useN - 1; j++) {refEnq.enqueue(data[j]);}

            // measuring enqueue
            {
                std::vector<QueueImpl> q(COPIES, refEnq);  // copy-constructor x COPIES — O(n) each
                double t = measure([&]() { for (int i = 0; i < COPIES; i++) { q[i].enqueue(extraElement);}});
                enqTimes.push_back(t / COPIES);
            }

            // measuring peek
            {
                std::vector<QueueImpl> q(COPIES, refFull);
                Pair<T> peekResult;
                double t = measure([&]() {
                    for (int i = 0; i < COPIES; i++) { peekResult = q[i].peek();}});
                (void)peekResult;
                peekTimes.push_back(t / COPIES);
            }

            // measuring extractMax
            {
                std::vector<QueueImpl> q(COPIES, refFull);
                double t = measure([&]() {
                    for (int i = 0; i < COPIES; i++) { q[i].extractMax();}});
                extTimes.push_back(t / COPIES);
            }
            // measuring decreaseKey
            {
                std::vector<QueueImpl> q(COPIES, refFull);
                Pair<T> element = data[gen() % useN];       // selecting random element
                double t = measure([&]() { for (int i = 0; i < COPIES; i++) {q[i].decreaseKey(element, element.getPriority() - 1);}});
                decTimes.push_back(t / COPIES);
            }
            // measuring increaseKey
            {
                std::vector<QueueImpl> q(COPIES, refFull);
                Pair<T> element = data[gen() % useN];       // selecting random element
                double t = measure([&]() {
                    for (int i = 0; i < COPIES; i++) { q[i].increaseKey(element, element.getPriority() + 1);}});
                incTimes.push_back(t / COPIES);
            }
            // measuring size
            {
                std::vector<QueueImpl> q(COPIES, refFull);
                double t = measure([&]() {
                    for (int i = 0; i < COPIES; i++) { q[i].size();}});
                sizeTimes.push_back(t / COPIES);
            }

        }

        // accumulate data into one row
        auto pushResult = [&](const std::string& op, std::vector<double>& times) {
            double sum = std::accumulate(times.begin(), times.end(), 0.0);
            double avg = static_cast<double>(sum) / times.size();
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

    /**
     * private method used for running test for entire data file
     * @param inputPath refers to path to source file, @param outputDir refers to target directory, @param sizes is a list of sizes to test, @param reps amount of reps
     */
    void runForFile(const std::string& inputPath,
                    const std::string& outputDir,
                    const std::vector<int>& sizes,
                    int reps) {

        // output filename -> results_{implName}_{inputStem}.csv

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

    // utility functions

    /**
     * private method for clearing input after carrying out operations
     */
    void clearInput() {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    /**
     * private method to ask user for sizes to test
     * @return vector of sizes
     */
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
    /**
     * constructor of class PerformanceTester
     * @param name refers to implementation of queue
     */
    explicit PerformanceTester(const std::string& name) : implName(name) {}

    /**
     * public method to run test for source data file with a user defined test config
     * @param cfg uses @struct BenchmarkConfig to pass test parameters to testing functions
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

    /**
     * public method to run test without a config, requiring user to define test parameters from console
     */
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