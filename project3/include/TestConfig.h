#pragma once

#include <string>
#include <vector>

struct TestConfig {
    std::vector<int> capacities;
    std::vector<double> loadFactors;
    std::vector<int> seeds;

    std::string dataDirectory;
    std::string outputFile;

    int repetitions;
};