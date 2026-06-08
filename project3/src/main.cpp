#include "../include/TestConfig.h"
#include "../include/TestRunner.h"

#include <exception>
#include <iostream>

int main() {
    const TestConfig config {
        {50021, 100003, 150001, 200003, 250007, 300007, 350003, 400009,
            450001, 500009, 550007, 600011, 650011, 700001, 750019, 800011,
            850009, 900007, 950009, 999983},
        {0.25, 0.50, 0.75, 1.0},
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
        "data",
        "results/benchmark_results.csv",
        100
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