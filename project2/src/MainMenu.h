#pragma once
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "Pair.h"
#include "Queue.h"
#include "DTQueue.h"
#include "HeapQueue.h"
#include "InteractiveMenu.h"
#include "PerformanceTester.h"


/**
 * class MainMenu
 * provides a interface for choosing thich mode of program to enter
 */
class MainMenu {
private:
    // utility functions

    /**
     * private method that clears the input buffer after any operation
     */
    void clearInput() {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    /**
     * private method that forces the program to wait for user interaction
     */
    void pause() {
        std::cout << "\n  [Nacisnij Enter aby kontynuowac...]";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    /**
     * private method that prints a separating line for better viewing
     * @param ch specifies the separator, @param width specifies the width of line 
     */
    void printLine(char ch = '-', int w = 52) const { std::cout << "  " << std::string(w, ch) << "\n";}

    /**
     * private method that reads user choice
     * @return user choice
     */
    int readChoice() {
        int c = -1;
        if (!(std::cin >> c)) {clearInput();}
        else clearInput();
        return c;
    }

    // interactive mode menu

    /**
     * private method for printing the interactive mode menu
     * after user choice, uses @fn run() on new object of @class InteractiveMenu
     */
    void runInteractive() {
        std::cout << "\n";
        printLine();
        std::cout << "  | TRYB INTERAKTYWNY - wybierz implementacje:   |\n";
        printLine();
        std::cout << "  | 1. Kolejka na posortowanej liscie (DTQueue)  |\n";
        std::cout << "  | 2. Kolejka na kopcu binarnym    (HeapQueue)  |\n";
        std::cout << "  | 0. Powrot                                    |\n";
        printLine();
        std::cout << "  Wybor: ";

        switch (readChoice()) {
            case 1: {
                InteractiveMenu<DTQueue<int>, int> m("DTQueue (posortowana lista)");
                m.run();
                break;
            }
            case 2: {
                InteractiveMenu<HeapQueue<int>, int> m("HeapQueue (kopiec binarny)");
                m.run();
                break;
            }
            case 0: break;
            default: std::cout << "  Nieprawidlowy wybor.\n";
        }
    }

    // performance mode menu

    /**
     * private method asking user for test parameters
     */
    BenchmarkConfig askConfig(const std::string& sharedOutputDir = "") {
        using Cfg = BenchmarkConfig;
        Cfg cfg;

        std::cout << "  Katalog z plikami CSV (dane wejsciowe) : ";
        std::cin >> cfg.inputDir;
        clearInput();

        std::cout << "  Prefix nazwy pliku (Enter = wszystkie)  : ";
        std::getline(std::cin, cfg.filePrefix);

        if (!cfg.filePrefix.empty() && cfg.filePrefix.back() == '\r') { cfg.filePrefix.pop_back();}

        if (sharedOutputDir.empty()) {
            std::cout << "  Katalog na pliki wynikowe              : ";
            std::cin >> cfg.outputDir;
            clearInput();
        } else {
            cfg.outputDir = sharedOutputDir;
            std::cout << "  Katalog wynikow (wspolny)              : "
                      << cfg.outputDir << "\n";
        }

        std::cout << "  Rozmiary n (oddzielone spacjami, zakoncz 0)\n";
        std::cout << "  np. 1000 2000 5000 0: ";
        int s;
        while (std::cin >> s && s != 0) { if (s > 0) cfg.sizes.push_back(s);}
        clearInput();

        std::cout << "  Liczba powtorzen na rozmiar            : ";
        std::cin >> cfg.repetitions;
        clearInput();
        if (cfg.repetitions < 1) {cfg.repetitions = 1;}

        return cfg;
    }

    /**
     * private method for printing the performance test mode menu
     * after user choice, uses @fn runWith() on new object of @class PerformanceTester
     */
    void runPerformance() {
        std::cout << "\n";
        printLine();
        std::cout << "  | TESTY WYDAJNOSCIOWE - wybierz implementacje: |\n";
        printLine();
        std::cout << "  | 1. Kolejka na posortowanej liscie (DTQueue)  |\n";
        std::cout << "  | 2. Kolejka na kopcu binarnym    (HeapQueue)  |\n";
        std::cout << "  | 3. Obie implementacje (wspolny katalog)      |\n";
        std::cout << "  | 0. Powrot                                    |\n";
        printLine();
        std::cout << "  Wybor: ";

        switch (readChoice()) {
            case 1: {
                auto cfg = askConfig();
                if (cfg.sizes.empty()) { std::cout << "  Anulowano.\n"; break; }
                PerformanceTester<DTQueue<int>, int> t("DTQueue");
                t.runWith(cfg);
                pause();
                break;
            }
            case 2: {
                auto cfg = askConfig();
                if (cfg.sizes.empty()) { std::cout << "  Anulowano.\n"; break; }
                PerformanceTester<HeapQueue<int>, int> t("HeapQueue");
                t.runWith(cfg);
                pause();
                break;
            }
            case 3: {
                // Ask for shared output directory first, then collect the
                // remaining config once — both testers reuse the same settings.
                std::string sharedOut;
                std::cout << "  Wspolny katalog na wyniki obu implementacji: ";
                std::cin >> sharedOut;
                clearInput();

                auto cfg = askConfig(sharedOut);
                if (cfg.sizes.empty()) { std::cout << "  Anulowano.\n"; break; }

                // DTQueue uses TestConfig directly; HeapQueue reuses same cfg
                // (TestConfig is defined on DTQueue's tester but the layout is
                //  identical — sizes, dirs, reps — so we forward it manually).
                {
                    PerformanceTester<DTQueue<int>, int> t("DTQueue");
                    t.runWith(cfg);
                }
                {
                    // Reconstruct equivalent config for HeapQueue tester
                    BenchmarkConfig hCfg;
                    hCfg.inputDir    = cfg.inputDir;
                    hCfg.filePrefix  = cfg.filePrefix;
                    hCfg.outputDir   = cfg.outputDir;
                    hCfg.sizes       = cfg.sizes;
                    hCfg.repetitions = cfg.repetitions;

                    PerformanceTester<HeapQueue<int>, int> t("HeapQueue");
                    t.runWith(hCfg);
                }
                pause();
                break;
            }
            case 0: break;
            default: std::cout << "  Nieprawidlowy wybor.\n";
        }
    }

    // main menu 

    /**
     * private method for printing main menu of program
     */
    void printMainMenu() const {
        std::cout << "\n";
        std::cout << "  +----------------------------------------------------+\n";
        std::cout << "  |        KOLEJKA PRIORYTETOWA - MENU GLOWNE          |\n";
        std::cout << "  +----------------------------------------------------+\n";
        std::cout << "  | Dostepne implementacje:                            |\n";
        std::cout << "  |   - DTQueue   : posortowana lista (tablica dyn.)   |\n";
        std::cout << "  |   - HeapQueue : kopiec binarny (max-heap)          |\n";
        std::cout << "  +----------------------------------------------------+\n";
        std::cout << "  | 1. Tryb interaktywny                               |\n";
        std::cout << "  | 2. Testy wydajnosciowe                             |\n";
        std::cout << "  | 0. Wyjscie                                         |\n";
        std::cout << "  +----------------------------------------------------+\n";
        std::cout << "  Wybor: ";
    }

public:
    /**
     * public method for running main menu
     */
    void run() {
        std::cout << "\n";
        std::cout << "  Witaj w testerze kolejek priorytetowych!\n";
        std::cout << "  Typ wartosci: int  |  Typ priorytetu: int\n";

        int choice = -1;
        while (choice != 0) {
            printMainMenu();
            choice = readChoice();
            switch (choice) {
                case 1: runInteractive();  break;
                case 2: runPerformance();  break;
                case 0: std::cout << "\n  Do widzenia!\n\n"; break;
                default: std::cout << "  Nieprawidlowy wybor. Podaj 0, 1 lub 2.\n";
            }
        }
    }
};