#pragma once
#include <iostream>
#include <limits>
#include <string>

/**
 * class InteractiveMenu
 * @tparam DictImpl odnosi się do implementacji słownika, @tparam V typ wartości
 * Zapewnia interaktywny interfejs do testowania słowników i ich operacji
 */
template <typename DictImpl, typename V>
class InteractiveMenu {
private:
    DictImpl    dict;
    std::string implName;

    void clearInput() {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    void pause() {
        std::cout << "\n  [Nacisnij Enter aby kontynuowac...]";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    void printSeparator(char ch = '-', int width = 50) { 
        std::cout << "  " << std::string(width, ch) << "\n";
    }

    void printHeader(const std::string& title) {
        std::cout << "\n";
        printSeparator();
        std::cout << "  >> " << title << "\n";
        printSeparator();
    }

    void doInsert() {
        printHeader("INSERT - Dodaj / aktualizuj element");
        int key;
        V val;
        std::cout << "  Podaj klucz (int): "; std::cin >> key;
        std::cout << "  Podaj wartosc     : "; std::cin >> val;
        clearInput();
        try {
            dict.insert(key, val);
            std::cout << "  OK: Dodano pare [" << key << " -> " << val << "].\n";
        } catch (const std::exception& e) { 
            std::cout << "  BLAD: " << e.what() << "\n";
        }
    }

    void doRemove() {
        printHeader("REMOVE - Usun element o podanym kluczu");
        int key;
        std::cout << "  Podaj klucz do usuniecia: "; std::cin >> key;
        clearInput();
        try {
            if (!dict.exists(key)) {
                std::cout << "  Informacja: Klucz " << key << " nie istnieje w slowniku.\n";
                return;
            }
            dict.remove(key);
            std::cout << "  OK: Element o kluczu " << key << " zostal usuniety.\n";
        } catch (const std::exception& e) { 
            std::cout << "  BLAD: " << e.what() << "\n";
        }
    }

    void doFind() {
        printHeader("FIND - Wyszukaj wartosc po kluczu");
        int key;
        std::cout << "  Podaj szukany klucz: "; std::cin >> key;
        clearInput();
        try {
            V val = dict.find(key);
            std::cout << "  Znaleziono: [" << key << " -> " << val << "]\n";
        } catch (const std::exception& e) { 
            std::cout << "  BLAD: " << e.what() << "\n";
        }
    }

    void doExists() {
        printHeader("EXISTS - Sprawdz istnienie klucza");
        int key;
        std::cout << "  Podaj klucz: "; std::cin >> key;
        clearInput();
        if (dict.exists(key)) {
            std::cout << "  TAK: Klucz " << key << " istnieje w slowniku.\n";
        } else {
            std::cout << "  NIE: Klucz " << key << " nie istnieje w slowniku.\n";
        }
    }

    void doDisplay() {
        printHeader("DISPLAY - Zawartosc slownika");
        if (dict.empty()) {
            std::cout << "  [Slownik jest pusty]\n";
            return;
        }
        dict.display();
    }

    void doStatus() {
        printHeader("STATUS - Informacje o slowniku");
        std::cout << "  Implementacja : " << implName             << "\n";
        std::cout << "  Rozmiar (count): " << dict.size()         << "\n";
        std::cout << "  Czy pusty     : " << (dict.empty() ? "TAK" : "NIE") << "\n";
    }

    void doLoadSample() {
        printHeader("DANE TESTOWE - Zaladuj przykladowe elementy");
        
        int sampleKeys[] = {15, 32, 7, 49, 23, 99, 44, 2};
        int sampleVals[] = {100, 200, 300, 400, 500, 700, 800, 900};
        int n = 8;

        for (int i = 0; i < n; i++) {
            try { 
                dict.insert(sampleKeys[i], static_cast<V>(sampleVals[i]));
            } catch(const std::exception& e) { 
                std::cout << "  BLAD przy kluczu " << sampleKeys[i] << ": " << e.what() << "\n";
            }
        }
        std::cout << "  Zaladowano przykladowe elementy testowe.\n\n";
        dict.display();
    }

    void printMenu() {
        std::cout << "\n";
        std::cout << "  +--------------------------------------------------+\n";
        std::cout << "  | Implementacja: " << implName;
        int pad = 34 - static_cast<int>(implName.size());
        if (pad > 0) std::cout << std::string(pad, ' ');
        std::cout << "|\n";
        std::cout << "  +--------------------------------------------------+\n";
        std::cout << "  | 1. Insert        - dodaj / nadpisz element       |\n";
        std::cout << "  | 2. Remove        - usun element po kluczu        |\n";
        std::cout << "  | 3. Find          - wyszukaj wartosc              |\n";
        std::cout << "  | 4. Exists        - sprawdz czy klucz istnieje    |\n";
        std::cout << "  | 5. Display       - wyswietl zawartosc slownika   |\n";
        std::cout << "  | 6. Status        - rozmiar i informacje          |\n";
        std::cout << "  | 7. Zaladuj dane testowe                          |\n";
        std::cout << "  | 0. Powrot do menu glownego                       |\n";
        std::cout << "  +--------------------------------------------------+\n";
        std::cout << "  Wybor: ";
    }

public:
    explicit InteractiveMenu(const std::string& name) : implName(name) {}

    void run() {
        int choice = -1;
        while (choice != 0) {
            printMenu();
            if (!(std::cin >> choice)) { clearInput(); choice = -1; continue; }
            clearInput();
            switch (choice) {
                case 1: doInsert();     break;
                case 2: doRemove();     break;
                case 3: doFind();       break;
                case 4: doExists();     break;
                case 5: doDisplay();    break;
                case 6: doStatus();     break;
                case 7: doLoadSample(); break;
                case 0: std::cout << "  Powrot do menu glownego.\n"; break;
                default: std::cout << "  Nieprawidlowy wybor. Podaj liczbe 0-7.\n";
            }
            if (choice != 0) { pause(); }
        }
    }
};