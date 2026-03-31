#include "Test.h"

// ─────────────────────────────────────────────
//  Konstruktor
// ─────────────────────────────────────────────
Test::Test(const std::string &dataFilePath,
           int repetitions,
           int startSize,
           int sizeStep,
           int numberOfSizes)
    : dataFilePath(dataFilePath),
      repetitions(repetitions),
      startSize(startSize),
      sizeStep(sizeStep),
      numberOfSizes(numberOfSizes)
{
    // Największy rozmiar jaki będziemy testować
    int maxSize = startSize + sizeStep * (numberOfSizes - 1);

    // Losujemy pozycję i wartość wyszukiwania raz — używamy ich przez cały test
    std::mt19937 rng(42);                                       // stały seed = powtarzalne wyniki
    std::uniform_int_distribution<int> posDist(1, maxSize - 2); // środek, nie skrajne
    fixedPosition = posDist(rng);
    fixedSearchValue = 0; // ustawimy po wczytaniu danych (patrz runAll)

    std::cout << "[Test] fixedPosition    = " << fixedPosition << "\n";
}

// ─────────────────────────────────────────────
//  Wczytywanie danych
// ─────────────────────────────────────────────
std::vector<int> Test::loadData(int n) const
{
    std::ifstream file(dataFilePath);
    if (!file.is_open())
    {
        std::cerr << "[Test] Nie można otworzyć pliku: " << dataFilePath << "\n";
        return {};
    }

    std::vector<int> data;
    data.reserve(n);
    int value;
    while (data.size() < static_cast<size_t>(n) && file >> value)
    {
        data.push_back(value);
    }

    if (static_cast<int>(data.size()) < n)
    {
        std::cerr << "[Test] Plik zawiera tylko " << data.size()
                  << " liczb, a wymagano " << n << "\n";
    }

    return data;
}

// ─────────────────────────────────────────────
//  Wypełnianie struktury (poza pomiarem)
// ─────────────────────────────────────────────
template <typename T>
void Test::fillStructure(T &structure, const std::vector<int> &data) const
{
    for (int val : data)
    {
        structure.addElementAtEnd(val);
    }
}

// ─────────────────────────────────────────────
//  Tworzenie 'repetitions' kopii
// ─────────────────────────────────────────────
template <typename T>
std::vector<T *> Test::createCopies(const std::vector<int> &data) const
{
    std::vector<T *> copies;
    copies.reserve(repetitions);

    for (int i = 0; i < repetitions; ++i)
    {
        T *copy = new T();
        fillStructure(*copy, data);
        copies.push_back(copy);
    }

    return copies;
}

// ─────────────────────────────────────────────
//  Zwalnianie kopii
// ─────────────────────────────────────────────
template <typename T>
void Test::deleteCopies(std::vector<T *> &copies) const
{
    for (T *ptr : copies)
    {
        delete ptr;
    }
    copies.clear();
}

// ─────────────────────────────────────────────
//  Pomiar czasu
//
//  Każda iteracja:
//    1. Bierze gotową kopię (dane już są)
//    2. Start stopera
//    3. Wykonuje operację
//    4. Stop stopera
//  Suma / repetitions = średni czas [ns]
// ─────────────────────────────────────────────
template <typename T>
long long Test::measure(std::vector<T *> &copies,
                        std::function<void(T &)> operation) const
{
    long long totalNs = 0;

    for (int i = 0; i < repetitions; ++i)
    {
        auto start = std::chrono::high_resolution_clock::now();

        operation(*copies[i]);

        auto stop = std::chrono::high_resolution_clock::now();
        totalNs += std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();
    }

    return totalNs / repetitions;
}

// ─────────────────────────────────────────────
//  Uruchamia wszystkie operacje dla jednej
//  struktury i jednego rozmiaru danych
// ─────────────────────────────────────────────
template <typename T>
void Test::runForStructure(const std::string &structureName,
                           const std::vector<int> &data,
                           int dataSize)
{
    // Pozycja nie może przekraczać rozmiaru danych
    int pos = fixedPosition % (dataSize > 1 ? dataSize - 1 : 1);
    if (pos == 0)
        pos = 1;

    std::cout << "  [" << structureName << "] rozmiar=" << dataSize
              << " pozycja=" << pos << "\n";

    // ── ADD AT BEGINNING ──────────────────────
    {
        auto copies = createCopies<T>(data);
        long long t = measure<T>(copies, [](T &s)
                                 { s.addElementAtBeginning(999); });
        results.push_back({structureName, "addBeginning", dataSize, t});
        deleteCopies(copies);
    }

    // ── ADD AT END ───────────────────────────
    {
        auto copies = createCopies<T>(data);
        long long t = measure<T>(copies, [](T &s)
                                 { s.addElementAtEnd(999); });
        results.push_back({structureName, "addEnd", dataSize, t});
        deleteCopies(copies);
    }

    // ── ADD AT POSITION ───────────────────────
    {
        auto copies = createCopies<T>(data);
        long long t = measure<T>(copies, [pos](T &s)
                                 { s.addElementAtPosition(999, pos); });
        results.push_back({structureName, "addPosition", dataSize, t});
        deleteCopies(copies);
    }

    // ── DELETE AT BEGINNING ───────────────────
    {
        auto copies = createCopies<T>(data);
        long long t = measure<T>(copies, [](T &s)
                                 { s.deleteElementAtBeginning(); });
        results.push_back({structureName, "deleteBeginning", dataSize, t});
        deleteCopies(copies);
    }

    // ── DELETE AT END ─────────────────────────
    {
        auto copies = createCopies<T>(data);
        long long t = measure<T>(copies, [](T &s)
                                 { s.deleteElementAtEnd(); });
        results.push_back({structureName, "deleteEnd", dataSize, t});
        deleteCopies(copies);
    }

    // ── DELETE AT POSITION ────────────────────
    {
        auto copies = createCopies<T>(data);
        long long t = measure<T>(copies, [pos](T &s)
                                 { s.deleteElementAtPosition(pos); });
        results.push_back({structureName, "deletePosition", dataSize, t});
        deleteCopies(copies);
    }

    // ── SEARCH ────────────────────────────────
    {
        auto copies = createCopies<T>(data);
        int searchVal = fixedSearchValue;
        long long t = measure<T>(copies, [searchVal](T &s)
                                 { s.searchElement(searchVal); });
        results.push_back({structureName, "search", dataSize, t});
        deleteCopies(copies);
    }
}

// ─────────────────────────────────────────────
//  Główna metoda — uruchamia wszystko
// ─────────────────────────────────────────────
void Test::runAll()
{
    for (int i = 0; i < numberOfSizes; ++i)
    {
        int currentSize = startSize + i * sizeStep;
        std::cout << "\n[Test] === Rozmiar: " << currentSize << " ===\n";

        std::vector<int> data = loadData(currentSize);
        if (static_cast<int>(data.size()) < currentSize)
            continue;

        // Ustalamy wartość do wyszukiwania: element ze środka danych
        fixedSearchValue = data[currentSize / 2];

        runForStructure<DynamicTable>("DynamicTable", data, currentSize);
        runForStructure<SinglyLinkedList>("SinglyLinkedList", data, currentSize);
        runForStructure<DoublyLinkedList>("DoublyLinkedList", data, currentSize);
    }

    std::cout << "\n[Test] Gotowe. Wyników: " << results.size() << "\n";
}

// ─────────────────────────────────────────────
//  Zapis do CSV
// ─────────────────────────────────────────────
void Test::saveToCSV(const std::string &outputFilePath) const
{
    std::ofstream file(outputFilePath);
    if (!file.is_open())
    {
        std::cerr << "[Test] Nie można zapisać pliku: " << outputFilePath << "\n";
        return;
    }

    // Nagłówek
    file << "Struktura;Operacja;Rozmiar;SredniCzasNs\n";

    for (const auto &r : results)
    {
        file << r.structure << ";"
             << r.operation << ";"
             << r.dataSize << ";"
             << r.avgTimeNs << "\n";
    }

    std::cout << "[Test] CSV zapisany do: " << outputFilePath << "\n";
}