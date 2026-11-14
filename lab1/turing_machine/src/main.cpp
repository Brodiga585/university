#include <iostream>
#include <string>
#include "TuringMachine.hpp"
#ifdef _WIN32
#include <windows.h>
#endif

void printMenu() {
    std::cout << "\n=== TURING MACHINE CLI ===\n"
              << "1. Загрузить и запустить машину из файла\n"
              << "2. Запустить с флагом -log\n"
              << "3. Выход\n"
              << "Выберите пункт меню: ";
}

int main(int argc, char* argv[]) {
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    #endif
    if (argc > 1) {
        std::string filepath = argv[1];
        bool log_flag = false;
        if (argc > 2 && std::string(argv[2]) == "-log") {
            log_flag = true;
        }

        try {
            TuringMachine tm;
            tm.loadFromFile(filepath);
            tm.run(log_flag);
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
            return 1;
        }
        return 0;
    }

    int choice = 0;
    while (true) {
        printMenu();
        if (!(std::cin >> choice)) break;

        if (choice == 1 || choice == 2) {
            std::string filepath;
            std::cout << "Введите путь к файлу конфигурации (например, input.txt): ";
            std::cin >> filepath;

            try {
                TuringMachine tm;
                tm.loadFromFile(filepath);
                bool log_flag = (choice == 2);
                tm.run(log_flag);
            } catch (const std::exception& e) {
                std::cerr << "Ошибка: " << e.what() << "\n";
            }
        } else if (choice == 3) {
            std::cout << "Выход из программы.\n";
            break;
        } else {
            std::cout << "Неверный выбор. Попробуйте снова.\n";
        }
    }

    return 0;
}