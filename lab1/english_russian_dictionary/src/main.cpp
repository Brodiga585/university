#include <iostream>
#include <string>
#include "Dictionary.hpp"
#ifdef _WIN32
#include <windows.h>
#endif

void printMenu() {
    std::cout << "\n=== ENGLISH-RUSSIAN DICTIONARY CLI ===\n"
              << "1. Добавить слово (std::string или char*)\n"
              << "2. Удалить слово\n"
              << "3. Найти / Изменить перевод ([])\n"
              << "4. Показать количество слов\n"
              << "5. Загрузить словарь из файла\n"
              << "6. Вывести весь словарь\n"
              << "7. Выход\n"
              << "Выберите пункт меню: ";
}

int main() {
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    #endif
    Dictionary dict;
    int choice = 0;

    while (true) {
        printMenu();
        if (!(std::cin >> choice)) break;

        if (choice == 1) {
            std::string eng, rus;
            std::cout << "Введите английское слово: ";
            std::cin >> eng;
            std::cout << "Введите перевод: ";
            std::cin >> rus;
            dict += std::make_pair(eng, rus);
            std::cout << "Слово успешно добавлено.\n";
        } else if (choice == 2) {
            std::string eng;
            std::cout << "Введите английское слово для удаления: ";
            std::cin >> eng;
            dict -= eng;
            std::cout << "Операция удаления завершена.\n";
        } else if (choice == 3) {
            std::string eng;
            std::cout << "Введите английское слово: ";
            std::cin >> eng;
            try {
                std::string& translation = dict[eng];
                std::cout << "Текущий перевод: " << (translation.empty() ? "[пусто]" : translation) << "\n";
                std::cout << "Хотите изменить перевод? (y/n): ";
                char ans;
                std::cin >> ans;
                if (ans == 'y' || ans == 'Y') {
                    std::string new_rus;
                    std::cout << "Введите новый перевод: ";
                    std::cin >> new_rus;
                    translation = new_rus;
                    std::cout << "Перевод обновлен.\n";
                }
            } catch (const std::exception& e) {
                std::cerr << "Ошибка: " << e.what() << "\n";
            }
        } else if (choice == 4) {
            std::cout << "Количество слов в словаре: " << dict.size() << "\n";
        } else if (choice == 5) {
            std::string filepath;
            std::cout << "Введите путь к файлу: ";
            std::cin >> filepath;
            try {
                dict.loadFromFile(filepath);
                std::cout << "Словарь успешно загружен из файла.\n";
            } catch (const std::exception& e) {
                std::cerr << "Ошибка: " << e.what() << "\n";
            }
        } else if (choice == 6) {
            std::cout << "\n--- СОДЕРЖИМОЕ СЛОВАРЯ ---\n";
            std::cout << dict;
            std::cout << "---------------------------\n";
        } else if (choice == 7) {
            std::cout << "Выход из программы.\n";
            break;
        } else {
            std::cout << "Неверный выбор. Попробуйте снова.\n";
        }
    }

    return 0;
}