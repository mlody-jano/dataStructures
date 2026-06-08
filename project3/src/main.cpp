#include "../include/MainMenu.h"
#include <exception>
#include <iostream>

int main() {
    try {
        MainMenu menu;
        menu.run();
    }
    catch (const std::exception& exception) {
        std::cerr << "Krytyczny blad programu: "
                  << exception.what() << '\n';
        return 1;
    }

    return 0;
}