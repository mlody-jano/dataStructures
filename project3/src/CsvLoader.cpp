#include "../include/CsvLoader.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

std::vector<TestRecord> loadDataFromCsv(
    const std::string& filePath,
    int maxRecords
) {
    if (maxRecords <= 0) {
        throw std::runtime_error("maxRecords must be positive.");
    }

    std::ifstream file(filePath);

    if (!file.is_open()) {
        throw std::runtime_error("Cannot open input file: " + filePath);
    }

    std::vector<TestRecord> data;
    data.reserve(maxRecords);

    std::string line;

    // Pomijamy nagłówek: key,value
    if (!std::getline(file, line)) {
        throw std::runtime_error("Input file is empty: " + filePath);
    }

    int lineNumber = 1;

    while (
        static_cast<int>(data.size()) < maxRecords &&
        std::getline(file, line)
    ) {
        ++lineNumber;

        if (line.empty()) {
            continue;
        }

        std::stringstream stream(line);
        std::string keyString;
        std::string valueString;

        if (!std::getline(stream, keyString, ',') ||
            !std::getline(stream, valueString, ',')) {
            throw std::runtime_error(
                "Invalid CSV format in file " + filePath +
                " at line " + std::to_string(lineNumber)
            );
        }

        try {
            TestRecord record;
            record.key = std::stoi(keyString);
            record.value = std::stoi(valueString);
            data.push_back(record);
        }
        catch (const std::exception&) {
            throw std::runtime_error(
                "Invalid integer value in file " + filePath +
                " at line " + std::to_string(lineNumber)
            );
        }
    }

    if (static_cast<int>(data.size()) < maxRecords) {
        throw std::runtime_error(
            "Not enough records in file: " + filePath +
            ". Expected at least: " + std::to_string(maxRecords) +
            ", got: " + std::to_string(data.size())
        );
    }

    return data;
}