#include "../include/TestRunner.h"

#include "../include/Benchmark.h"
#include "../include/BenchmarkResult.h"
#include "../include/CsvLoader.h"
#include "../include/HashTableList.h"
#include "../include/HashTableOpenAddressing.h"
#include "../include/HashTableAVL.h"

#include <cmath>
#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

TestRunner::TestRunner(const TestConfig& config)
    : config(config) {
}

std::string TestRunner::buildInputFilePath(int seed) const {
    return config.dataDirectory
        + "/s"
        + std::to_string(seed)
        + ".csv";
}

bool TestRunner::containsKey(
    const std::vector<TestRecord>& data,
    int key
) {
    for (const TestRecord& record : data) {
        if (record.key == key) {
            return true;
        }
    }

    return false;
}

TestRecord TestRunner::makeInsertCandidate(
    const std::vector<TestRecord>& data,
    int capacity,
    int loadPercent,
    int seed
) {
    int key = 1'000'000'000 - capacity - loadPercent - seed;

    while (containsKey(data, key)) {
        --key;
    }

    return {key, key * 10};
}

TestRecord TestRunner::chooseElementToRemove(
    const std::vector<TestRecord>& data
) {
    if (data.empty()) {
        throw std::runtime_error("Cannot choose element to remove from empty data.");
    }

    return data[data.size() / 2];
}

void TestRunner::run() const {
    if (config.capacities.empty()) {
        throw std::runtime_error("Capacity list cannot be empty.");
    }

    if (config.loadFactors.empty()) {
        throw std::runtime_error("Load factor list cannot be empty.");
    }

    if (config.seeds.empty()) {
        throw std::runtime_error("Seed list cannot be empty.");
    }

    if (config.repetitions <= 0) {
        throw std::runtime_error("Number of repetitions must be positive.");
    }

    std::remove(config.outputFile.c_str());

    bool writeHeader = true;

    for (int capacity : config.capacities) {
        for (double loadFactor : config.loadFactors) {
            const int loadPercent =
                static_cast<int>(std::round(loadFactor * 100.0));

            const int elementCount =
                static_cast<int>(std::round(capacity * loadFactor));

            double listInsertTotal = 0.0;
            double listRemoveTotal = 0.0;

            double openInsertTotal = 0.0;
            double openRemoveTotal = 0.0;

            double avlInsertTotal = 0.0;
            double avlRemoveTotal = 0.0;

            for (int seed : config.seeds) {
                const std::string inputFile =
                    buildInputFilePath(seed);

                const std::vector<TestRecord> data =
                    loadDataFromCsv(inputFile, elementCount);

                const TestRecord elementToInsert =
                    makeInsertCandidate(data, capacity, loadPercent, seed);

                const TestRecord elementToRemove =
                    chooseElementToRemove(data);

                listInsertTotal +=
                    benchmarkSingleInsert<HashTableList<int>>(
                        data,
                        elementToInsert,
                        capacity,
                        config.repetitions
                    );

                listRemoveTotal +=
                    benchmarkSingleRemove<HashTableList<int>>(
                        data,
                        elementToRemove,
                        capacity,
                        config.repetitions
                    );

                openInsertTotal +=
                    benchmarkSingleInsert<HashTableOpenAddressing<int>>(
                        data,
                        elementToInsert,
                        capacity,
                        config.repetitions
                    );

                openRemoveTotal +=
                    benchmarkSingleRemove<HashTableOpenAddressing<int>>(
                        data,
                        elementToRemove,
                        capacity,
                        config.repetitions
                    );

                avlInsertTotal +=
                    benchmarkSingleInsert<HashTableAVL<int>>(
                        data,
                        elementToInsert,
                        capacity,
                        config.repetitions
                    );

                avlRemoveTotal +=
                    benchmarkSingleRemove<HashTableAVL<int>>(
                        data,
                        elementToRemove,
                        capacity,
                        config.repetitions
                    );
                }

            const double seedCount =
                static_cast<double>(config.seeds.size());

            const double listInsertAverage =
                listInsertTotal / seedCount;

            const double listRemoveAverage =
                listRemoveTotal / seedCount;

            const double openInsertAverage =
                openInsertTotal / seedCount;

            const double openRemoveAverage =
                openRemoveTotal / seedCount;

            const double avlInsertAverage =
                avlInsertTotal / seedCount;

            const double avlRemoveAverage =
                avlRemoveTotal / seedCount;

            appendAverageResult(
                config.outputFile,
                "HashTableList",
                "insert",
                capacity,
                loadFactor,
                elementCount,
                listInsertAverage,
                writeHeader
            );

            writeHeader = false;

            appendAverageResult(
                config.outputFile,
                "HashTableList",
                "remove",
                capacity,
                loadFactor,
                elementCount,
                listRemoveAverage
            );

            appendAverageResult(
                config.outputFile,
                "HashTableOpenAddressing",
                "insert",
                capacity,
                loadFactor,
                elementCount,
                openInsertAverage
            );

            appendAverageResult(
                config.outputFile,
                "HashTableOpenAddressing",
                "remove",
                capacity,
                loadFactor,
                elementCount,
                openRemoveAverage
            );

            appendAverageResult(
                config.outputFile,
                "HashTableAVL",
                "insert",
                capacity,
                loadFactor,
                elementCount,
                avlInsertAverage
            );

            appendAverageResult(
                config.outputFile,
                "HashTableAVL",
                "remove",
                capacity,
                loadFactor,
                elementCount,
                avlRemoveAverage
            );

            std::cout << "Completed: capacity = "
                      << capacity
                      << ", load = "
                      << loadPercent
                      << "%, n = "
                      << elementCount
                      << '\n';
        }
    }

    std::cout << "All benchmark results saved to: "
              << config.outputFile << '\n';
}