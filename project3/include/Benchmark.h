#pragma once

#include "CsvLoader.h"

#include <chrono>
#include <stdexcept>
#include <vector>

template <typename DictionaryType>
double benchmarkSingleInsert(
    const std::vector<TestRecord>& initialData,
    const TestRecord& elementToInsert,
    int initialCapacity,
    int repetitions
) {
    using Clock = std::chrono::high_resolution_clock;

    double totalNanoseconds = 0.0;

    for (int repetition = 0; repetition < repetitions; ++repetition) {
        DictionaryType dictionary(initialCapacity);

        // Wypełnienie struktury do zadanego poziomu — poza pomiarem.
        for (const TestRecord& record : initialData) {
            dictionary.insert(record.key, record.value);
        }

        const auto start = Clock::now();

        dictionary.insert(elementToInsert.key, elementToInsert.value);

        const auto end = Clock::now();

        totalNanoseconds +=
            std::chrono::duration<double, std::nano>(end - start).count();
    }

    return totalNanoseconds / repetitions;
}

template <typename DictionaryType>
double benchmarkSingleRemove(
    const std::vector<TestRecord>& initialData,
    const TestRecord& elementToRemove,
    int initialCapacity,
    int repetitions
) {
    using Clock = std::chrono::high_resolution_clock;

    double totalNanoseconds = 0.0;

    for (int repetition = 0; repetition < repetitions; ++repetition) {
        DictionaryType dictionary(initialCapacity);

        // Wypełnienie struktury do zadanego poziomu — poza pomiarem.
        for (const TestRecord& record : initialData) {
            dictionary.insert(record.key, record.value);
        }

        const auto start = Clock::now();

        dictionary.remove(elementToRemove.key);

        const auto end = Clock::now();

        totalNanoseconds +=
            std::chrono::duration<double, std::nano>(end - start).count();
    }

    return totalNanoseconds / repetitions;
}