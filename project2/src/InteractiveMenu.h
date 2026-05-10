#pragma once
#include <iostream>
#include <limits>
#include <string>
#include "Pair.h"

/**
 * class InteractiveMenu
 * @tparam QueueImpl refers to implementation of queue, @tparam value type of Pair
 * provides a interactive interface to test all implemented queues and their operations
 */
template <typename QueueImpl, typename T>
class InteractiveMenu {
private:
    QueueImpl   queue;
    std::string implName;

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
    void printSeparator(char ch = '-', int width = 50) { std::cout << "  " << std::string(width, ch) << "\n";}

    /**
     * private method for printing the title of operation being carried out
     * uses @fn printSeparator() to separate the title
     * @param title specifies the title meant to be printed
     */
    void printHeader(const std::string& title) {
        std::cout << "\n";
        printSeparator();
        std::cout << "  >> " << title << "\n";
        printSeparator();
    }

    /**
     * private method for displaying the queue
     * creates a snapshot of queue, then deletes it
     */
    void displayQueue() {
        if (queue.isEmpty()) {
            std::cout << "  [Kolejka jest pusta]\n";
            return;
        }

        int n = queue.size();
        Pair<T>* snap = new Pair<T>[n];

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

    // polymorphic queue operations

    /** 
     * private method for enqueuing an element
     * uses @fn enqueue() to add element to queue
     * uses try-catch to prevent program crashes
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
        } catch (const std::exception& e) { std::cout << "  BLAD: " << e.what() << "\n";}
    }

    /** 
     * private method for extracting the maximum element
     * uses @fn extractMax() to extract the maximum element
     * uses try-catch to prevent program crashes
     */
    void doExtractMax() {
        printHeader("EXTRACT MAX - Usun element o najwyzszym priorytecie");
        try {
            const Pair<T>& top = queue.peek();
            std::cout << "  Usuwany: wartosc=" << top.getValue()
                      << ", priorytet=" << top.getPriority() << "\n";
            queue.extractMax();
            std::cout << "  OK: element usuniety.\n";
        } catch (const std::exception& e) { std::cout << "  BLAD: " << e.what() << "\n";}
    }

    /** 
     * private method for peeking the maximum element
     * uses @fn peek() to extract the maximum element
     * uses try-catch to prevent program crashes
     */
    void doPeek() {
        printHeader("PEEK - Podejrzyj szczyt kolejki (bez usuwania)");
        try {
            const Pair<T>& top = queue.peek();
            std::cout << "  Szczyt: wartosc=" << top.getValue()
                      << ", priorytet=" << top.getPriority() << "\n";
        } catch (const std::exception& e) { std::cout << "  BLAD: " << e.what() << "\n";}
    }

    /** 
     * private method for decreasing the key value of specific pair
     * uses @fn decreaseKey() to decrease key value
     * uses try-catch to prevent program crashes
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
        } catch (const std::exception& e) { std::cout << "  BLAD: " << e.what() << "\n";}
    }

    /** 
     * private method for increasing the key value of specific pair
     * uses @fn increaseKey() to extract the maximum element
     * uses try-catch to prevent program crashes
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
        } catch (const std::exception& e) { std::cout << "  BLAD: " << e.what() << "\n";}
    }

    /** 
     * private method for displaying queue
     * uses @fn displayQueue() to display queue
     */
    void doDisplay() {
        printHeader("DISPLAY - Zawartosc kolejki (od najwyzszego priorytetu)");
        displayQueue();
    }

    /** 
     * private method for showing summary of queue
     */
    void doStatus() {
        printHeader("STATUS - Informacje o kolejce");
        std::cout << "  Implementacja : " << implName             << "\n";
        std::cout << "  Rozmiar       : " << queue.size()         << "\n";
        std::cout << "  Czy pusta     : " << (queue.isEmpty() ? "TAK" : "NIE") << "\n";
    }

    /** 
     * private method for loading sample data to visualize performance
     * clears the queue first, then loads sample data
     * uses try-catch to prevent program crashes
     */
    void doLoadSample() {
        printHeader("DANE TESTOWE - Zaladuj przykladowe elementy");
        while (!queue.isEmpty()) {queue.extractMax();}

        // 9 elements with varied priorities including duplicates
        int samples[][2] = {
            {10, 1}, {30, 3}, {50, 5}, {20, 2},
            {40, 4}, {15, 1}, {55, 6}, {25, 3}, {70, 7}
        };
        for (auto& s : samples) {
            Pair<T> p(static_cast<T>(s[0]), s[1]);
            try { queue.enqueue(p);} catch(const std::exception& e) { std::cout << "  BLAD: " << e.what() << "\n";}
            
        }
        std::cout << "  Zaladowano 9 elementow testowych.\n\n";
        displayQueue();
    }

    // menu display func

    /** 
     * private method for printing the menu
     * uses try-catch to prevent program crashes
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

    /**
     * constructor of class InteractiveMenu
     * @param name specifies the name of queue implementation to be displayed in menu
     */
    explicit InteractiveMenu(const std::string& name) : implName(name) {}

    /** 
     * public method for running interactive menu
     */
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