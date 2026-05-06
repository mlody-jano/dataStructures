#pragma once
#include <iostream>
#include <limits>
#include <string>
#include "Pair.h"

/*
    Interactive console menu for any Queue<T> implementation.
    Expects two template parameters: QueueImpl -> implementation class, T -> value type.
    Provides a interactive interface to test all queue operations and display the contents.
*/
template <typename QueueImpl, typename T>
class InteractiveMenu {
private:
    QueueImpl   queue;
    std::string implName;

    // ── Utilities ─────────────────────────────────────────────────────────────

    /*
        Clears the input.
    */
    void clearInput() {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    /*
        Pauses the execution and waits for user input.
    */
    void pause() {
        std::cout << "\n  [Nacisnij Enter aby kontynuowac...]";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    /*
        Prints a separating line.
    */
    void printSeparator(char ch = '-', int width = 50) {
        std::cout << "  " << std::string(width, ch) << "\n";
    }

    /*
        Prints a header using the printSeparator() function.
    */
    void printHeader(const std::string& title) {
        std::cout << "\n";
        printSeparator();
        std::cout << "  >> " << title << "\n";
        printSeparator();
    }

    /*
        Non-destructive display: snapshot via peek/extractMax, then re-insert.
        Works for any Queue implementation without requiring iterator access.
    */
    void displayQueue() {
        if (queue.isEmpty()) {
            std::cout << "  [Kolejka jest pusta]\n";
            return;
        }

        int n = queue.size();
        Pair<T>* snap = new Pair<T>[n];                         // Creates a snapshot of queue, for displaying purposes.

        // Extract all in priority order (highest first)
        for (int i = 0; i < n; i++) {
            snap[i] = queue.peek();
            queue.extractMax();
        }

        std::cout << "  Nr  | Wartosc | Priorytet\n";
        printSeparator('-', 30);
        for (int i = 0; i < n; i++) {
            std::cout << "  [" << i << "]  | "
                      << snap[i].getValue()    << "\t | "
                      << snap[i].getPriority() << "\n";
            queue.enqueue(snap[i]);   // re-insert to restore queue
        }
        delete[] snap;
    }

    // ── Menu operations ───────────────────────────────────────────────────────

    /*
        Unified function that enables the user to insert a self-defined key-value pair to the queue.
        Uses try-catch statements to handle exceptions, which could crash the program.    
    */
    void doEnqueue() {
        printHeader("ENQUEUE - Dodaj element");
        T   val;
        int prio;
        std::cout << "  Podaj wartosc  : "; std::cin >> val;
        std::cout << "  Podaj priorytet: "; std::cin >> prio;
        clearInput();
        Pair<T> p(val, prio);
        try {
            queue.enqueue(p);
            std::cout << "  OK: dodano (" << val << ", priorytet=" << prio << ").\n";
        } catch (const std::exception& e) {
            std::cout << "  BLAD: " << e.what() << "\n";
        }
    }

    /*
        Unified function that enables the user to extract the element the the top of the queue (maximum priority).
        Uses try-catch statements to handle exceptions, which could crash the program.
    */
    void doExtractMax() {
        printHeader("EXTRACT MAX - Usun element o najwyzszym priorytecie");
        try {
            const Pair<T>& top = queue.peek();
            std::cout << "  Usuwany: wartosc=" << top.getValue()
                      << ", priorytet=" << top.getPriority() << "\n";
            queue.extractMax();
            std::cout << "  OK: element usuniety.\n";
        } catch (const std::exception& e) {
            std::cout << "  BLAD: " << e.what() << "\n";
        }
    }

    /*
        Unified function that enables the user to peek the element at the top of the queue (maximum priority).
        Uses try-catch statements to handle exceptions, which could crash the program.
    */
    void doPeek() {
        printHeader("PEEK - Podejrzyj szczyt kolejki (bez usuwania)");
        try {
            const Pair<T>& top = queue.peek();
            std::cout << "  Szczyt: wartosc=" << top.getValue()
                      << ", priorytet=" << top.getPriority() << "\n";
        } catch (const std::exception& e) {
            std::cout << "  BLAD: " << e.what() << "\n";
        }
    }

    /*
        Unified function that enables the user to decrease the priority of a specific key-value pair.
        Uses try-catch statements to handle exceptions, which could crash the program.
    */
    void doDecreaseKey() {
        printHeader("DECREASE KEY - Zmniejsz priorytet elementu");
        T   val;
        int prio, newPrio;
        std::cout << "  Wartosc szukanego elementu: "; std::cin >> val;
        std::cout << "  Obecny priorytet elementu : "; std::cin >> prio;
        std::cout << "  Nowy (nizszy) priorytet   : "; std::cin >> newPrio;
        clearInput();
        Pair<T> p(val, prio);
        try {
            queue.decreaseKey(p, newPrio);
            std::cout << "  OK: priorytet zmieniony na " << newPrio << ".\n";
        } catch (const std::exception& e) {
            std::cout << "  BLAD: " << e.what() << "\n";
        }
    }

    /*
        Unified function that enables the user to increase the priority of a specific key-value pair.
        Uses try-catch statements to handle exceptions, which could crash the program.
    */
    void doIncreaseKey() {
        printHeader("INCREASE KEY - Zwieksz priorytet elementu");
        T   val;
        int prio, newPrio;
        std::cout << "  Wartosc szukanego elementu: "; std::cin >> val;
        std::cout << "  Obecny priorytet elementu : "; std::cin >> prio;
        std::cout << "  Nowy (wyzszy) priorytet   : "; std::cin >> newPrio;
        clearInput();
        Pair<T> p(val, prio);
        try {
            queue.increaseKey(p, newPrio);
            std::cout << "  OK: priorytet zmieniony na " << newPrio << ".\n";
        } catch (const std::exception& e) {
            std::cout << "  BLAD: " << e.what() << "\n";
        }
    }

    void doDisplay() {
        printHeader("DISPLAY - Zawartosc kolejki (od najwyzszego priorytetu)");
        displayQueue();
    }

    void doStatus() {
        printHeader("STATUS - Informacje o kolejce");
        std::cout << "  Implementacja : " << implName             << "\n";
        std::cout << "  Rozmiar       : " << queue.size()         << "\n";
        std::cout << "  Czy pusta     : " << (queue.isEmpty() ? "TAK" : "NIE") << "\n";
    }

    /*
        Function loading pre-defined data for visualization purposes.
    */
    void doLoadSample() {
        printHeader("DANE TESTOWE - Zaladuj przykladowe elementy");
        while (!queue.isEmpty()) queue.extractMax();

        // 9 elements with varied priorities including duplicates
        int samples[][2] = {
            {10, 1}, {30, 3}, {50, 5}, {20, 2},
            {40, 4}, {15, 1}, {55, 6}, {25, 3}, {70, 7}
        };
        for (auto& s : samples) {
            Pair<T> p(static_cast<T>(s[0]), s[1]);
            try {
                queue.enqueue(p);
            } catch(const std::exception& e) {
                std::cout << "  BLAD: " << e.what() << "\n";
            }
            
        }
        std::cout << "  Zaladowano 9 elementow testowych.\n\n";
        displayQueue();
    }

    // ── Menu display ──────────────────────────────────────────────────────────

    /*
        Function for printing the visual interactive menu for visualization purposes.
    */
    void printMenu() {
        std::cout << "\n";
        std::cout << "  +--------------------------------------------------+\n";
        std::cout << "  | Implementacja: " << implName;
        // Padding to align right border
        int pad = 34 - static_cast<int>(implName.size());
        if (pad > 0) std::cout << std::string(pad, ' ');
        std::cout << "|\n";
        std::cout << "  +--------------------------------------------------+\n";
        std::cout << "  | 1. Enqueue       - dodaj element                 |\n";
        std::cout << "  | 2. ExtractMax    - usun element o max priorytecie|\n";
        std::cout << "  | 3. Peek          - podejrzyj szczyt              |\n";
        std::cout << "  | 4. DecreaseKey   - zmniejsz priorytet elementu   |\n";
        std::cout << "  | 5. IncreaseKey   - zwieksz priorytet elementu    |\n";
        std::cout << "  | 6. Display       - wyswietl cala kolejke         |\n";
        std::cout << "  | 7. Status        - rozmiar i informacje          |\n";
        std::cout << "  | 8. Zaladuj dane testowe                          |\n";
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
                case 1: doEnqueue();     break;
                case 2: doExtractMax();  break;
                case 3: doPeek();        break;
                case 4: doDecreaseKey(); break;
                case 5: doIncreaseKey(); break;
                case 6: doDisplay();     break;
                case 7: doStatus();      break;
                case 8: doLoadSample();  break;
                case 0: std::cout << "  Powrot do menu glownego.\n"; break;
                default: std::cout << "  Nieprawidlowy wybor. Podaj liczbe 0-8.\n";
            }
            if (choice != 0) pause();
        }
    }
};