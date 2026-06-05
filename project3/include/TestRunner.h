#pragma once

#include "CsvLoader.h"
#include "TestConfig.h"

#include <string>
#include <vector>

class TestRunner {
private:
    TestConfig config;

    std::string buildInputFilePath(int seed) const;

    static bool containsKey(
        const std::vector<TestRecord>& data,
        int key
    );

    static TestRecord makeInsertCandidate(
        const std::vector<TestRecord>& data,
        int capacity,
        int loadPercent,
        int seed
    );

    static TestRecord chooseElementToRemove(
        const std::vector<TestRecord>& data
    );

public:
    explicit TestRunner(const TestConfig& config);

    void run() const;
};