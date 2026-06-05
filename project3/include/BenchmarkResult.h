#pragma once

#include <fstream>
#include <stdexcept>
#include <string>

inline void appendAverageResult(
    const std::string& outputFile,
    const std::string& structure,
    const std::string& operation,
    int capacity,
    double loadFactor,
    int elementCount,
    double averageTimeNanoseconds,
    bool writeHeader = false
) {
    std::ofstream file(outputFile, std::ios::app);

    if (!file.is_open()) {
        throw std::runtime_error("Cannot open output file: " + outputFile);
    }

    if (writeHeader) {
        file << "structure,operation,capacity,load_factor,n,average_time_ns\n";
    }

    file << structure << ','
         << operation << ','
         << capacity << ','
         << loadFactor << ','
         << elementCount << ','
         << averageTimeNanoseconds << '\n';
}