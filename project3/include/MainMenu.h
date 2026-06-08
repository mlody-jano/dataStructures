#pragma once
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "HashTableList.h"
#include "HashTableOpenAddressing.h"
#include "HashTableAVL.h"
#include "InteractiveMenu.h"
#include "TestConfig.h"
#include "TestRunner.h"

/**
 * class MainMenu
 * Zapewnia interfejs wyboru trybu programu oraz wprowadzania konfiguracji testów.
 */
class MainMenu {
private:
    void clearInput() {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    void pause() {
        std::cout << "\n  [Nacisnij Enter aby kontynuowac...]";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    void printLine(char ch = '-', int w = 56) const { 
        std::cout << "  " << std::string(w, ch) << "\n";
    }

    int readChoice() {
        int c = -1;
        if (!(std::cin >> c)) { clearInput(); }
        else clearInput();
        return c;
    }

    void runInteractive() {
        int choice = -1;
        while (choice != 0) {
            std::cout << "\n";
            printLine();
            std::cout << "  | TRYB INTERAKTYWNY - Wybierz implementacje slownika: |\n";
            printLine();
            std::cout << "  | 1. Lancuchowa (HashTableList)                       |\n";
            std::cout << "  | 2. Adresowanie otwarte (HashTableOpenAddressing)     |\n";
            std::cout << "  | 3. Drzewa AVL w kubelkach (HashTableAVL)            |\n";
            std::cout << "  | 0. Powrot                                           |\n";
            printLine();
            std::cout << "  Wybor: ";

            choice = readChoice();
            switch (choice) {
                case 1: {
                    InteractiveMenu<HashTableList<int>, int> m("HashTableList (Lancuchowa)");
                    m.run();
                    choice = 0;
                    break;
                }
                case 2: {
                    InteractiveMenu<HashTableOpenAddressing<int>, int> m("HashTableOpenAddressing");
                    m.run();
                    choice = 0;
                    break;
                }
                case 3: {
                    InteractiveMenu<HashTableAVL<int>, int> m("HashTableAVL (Kubelki AVL)");
                    m.run();
                    choice = 0;
                    break;
                }
                case 0: 
                    break;
                default: 
                    std::cout << "  Nieprawidlowy wybor.\n";
                    pause();
            }
        }
    }

    /**
     * Interaktywne budowanie konfiguracji TestConfig pod TestRunner
     */
    TestConfig askConfig() {
        TestConfig cfg;

        std::cout << "  Katalog z plikami danych (dataDirectory): ";
        std::cin >> cfg.dataDirectory;
        clearInput();

        std::cout << "  Plik wynikowy CSV (outputFile)          : ";
        std::cin >> cfg.outputFile;
        clearInput();

        std::cout << "  Liczba powtorzen (repetitions)          : ";
        std::cin >> cfg.repetitions;
        clearInput();
        if (cfg.repetitions < 1) cfg.repetitions = 1;

        // Wczytywanie wektora pojemności (capacities)
        std::cout << "  Podaj pojemnosci (capacities) do testu (rozdzielone spacja, 0 konczy):\n  ";
        int cap;
        while (std::cin >> cap && cap != 0) {
            if (cap > 0) cfg.capacities.push_back(cap);
        }
        clearInput();

        // Wczytywanie wektora load factors
        std::cout << "  Podaj wspolczynniki zapelnienia (np. 0.25 0.50 0.75 0 konczy):\n  ";
        double lf;
        while (std::cin >> lf && lf != 0.0) {
            if (lf > 0.0) cfg.loadFactors.push_back(lf);
        }
        clearInput();

        // Wczytywanie wektora seeds
        std::cout << "  Podaj ziarna losowosci (seeds) (np. 1 2 3 0 konczy):\n  ";
        int sd;
        while (std::cin >> sd && sd != 0) {
            if (sd > 0) cfg.seeds.push_back(sd);
        }
        clearInput();

        return cfg;
    }

    void runPerformance() {
        printLine();
        std::cout << "  | TRYB TESTOW WYDAJNOSCIOWYCH                          |\n";
        printLine();
        
        TestConfig cfg = askConfig();
        if (cfg.capacities.empty() || cfg.loadFactors.empty() || cfg.seeds.empty()) {
            std::cout << "  Anulowano - konfiguracja nie zawiera wszystkich potrzebnych danych.\n";
            pause();
            return;
        }

        std::cout << "  Inicjalizacja testow za pomoca TestRunner...\n";
        try {
            TestRunner runner(cfg);
            runner.run();
            std::cout << "  [OK] Testy ukonczone. Wyniki zapisano w: " << cfg.outputFile << "\n";
        } catch (const std::exception& e) {
            std::cout << "  [BLAD] Wystapil blad podczas wykonywania testow: " << e.what() << "\n";
        }
        pause();
    }

    void printMainMenu() const {
        std::cout << "\n";
        std::cout << "  +----------------------------------------------------+\n";
        std::cout << "  |               SLOWNIKI - MENU GLOWNE               |\n";
        std::cout << "  +----------------------------------------------------+\n";
        std::cout << "  | 1. Tryb interaktywny                               |\n";
        std::cout << "  | 2. Testy wydajnosciowe                             |\n";
        std::cout << "  | 0. Wyjscie                                         |\n";
        std::cout << "  +----------------------------------------------------+\n";
        std::cout << "  Wybor: ";
    }

public:
    void run() {
        int choice = -1;
        while (choice != 0) {
            printMainMenu();
            choice = readChoice();
            switch (choice) {
                case 1: runInteractive();  break;
                case 2: runPerformance();  break;
                case 0: std::cout << "\n  Koniec programu.\n\n"; break;
                default: std::cout << "  Nieprawidlowy wybor. Podaj 0, 1 lub 2.\n"; pause();
            }
        }
    }
};