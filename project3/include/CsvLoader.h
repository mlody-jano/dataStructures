#pragma once

#include <string>
#include <vector>

struct TestRecord {
    int key;
    int value;
};

std::vector<TestRecord> loadDataFromCsv(
    const std::string& filePath,
    int maxRecords
);