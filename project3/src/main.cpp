#include "../include/TestConfig.h"
#include "../include/TestRunner.h"

#include <exception>
#include <iostream>

int main() {
    const TestConfig config {
        {10000, 50000, 100000, 500000, 1000000},
        {0.25, 0.50, 0.70},
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
        "data",
        "results/benchmark_results.csv",
        5
    };

    try {
        TestRunner runner(config);
        runner.run();
    }
    catch (const std::exception& exception) {
        std::cerr << "Test error: "
                  << exception.what() << '\n';
        return 1;
    }

    return 0;
}