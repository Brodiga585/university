#include <iostream>
#include <cassert>
#include <sstream>

#include "Dictionary.hpp"

void testAddAndSize() {
    Dictionary dictionary;
    assert(dictionary.size() == 0);
    assert(dictionary.isEmpty());

    dictionary += std::make_pair("apple", "яблоко");
    dictionary += std::make_pair("banana", "банан");

    assert(dictionary.size() == 2);
    assert(!dictionary.isEmpty());

    dictionary += std::make_pair("apple", "яблочко");
    assert(dictionary["apple"] == "яблочко");

    dictionary.clear();
    assert(dictionary.isEmpty());
    std::cout << "[PASSED] testAddAndSize\n";
}

void testCharStarOverload() {
    Dictionary dictionary;
    const char* english = "cat";
    const char* russian = "кот";
    dictionary += std::make_pair(english, russian);

    assert(dictionary.size() == 1);
    assert(dictionary["cat"] == "кот");

    dictionary -= english;
    assert(dictionary["cat"] == "");
    std::cout << "[PASSED] testCharStarOverload\n";
}

void testFindAndReplace() {
    Dictionary dictionary;
    dictionary += std::make_pair("dog", "собака");

    assert(dictionary["dog"] == "собака");

    dictionary["dog"] = "пес";
    assert(dictionary["dog"] == "пес");

    assert(dictionary["mouse"] == "");
    assert(dictionary.size() == 2);

    const char* dog = "dog";
    assert(dictionary[dog] == "пес");
    std::cout << "[PASSED] testFindAndReplace\n";
}

void testRemove() {
    Dictionary dictionary;
    dictionary += std::make_pair("sun", "солнце");
    dictionary += std::make_pair("moon", "луна");
    dictionary += std::make_pair("star", "звезда");
    assert(dictionary.size() == 3);

    dictionary -= "sun";
    assert(dictionary["sun"] == "");

    dictionary -= "moon";
    assert(dictionary["moon"] == "");

    dictionary -= "nonexistent";
    std::cout << "[PASSED] testRemove\n";
}

void testCopyAndAssignment() {
    Dictionary firstDictionary;
    firstDictionary += std::make_pair("car", "машина");

    Dictionary secondDictionary(firstDictionary);
    assert(secondDictionary.size() == 1);
    assert(secondDictionary["car"] == "машина");

    Dictionary thirdDictionary;
    thirdDictionary = firstDictionary;
    assert(thirdDictionary.size() == 1);
    assert(thirdDictionary["car"] == "машина");

    thirdDictionary = thirdDictionary;
    assert(thirdDictionary.size() == 1);
    std::cout << "[PASSED] testCopyAndAssignment\n";
}

void testStreams() {
    Dictionary firstDictionary;
    firstDictionary += std::make_pair("house", "дом");

    std::stringstream stream;
    stream << firstDictionary;

    Dictionary secondDictionary;
    stream >> secondDictionary;
    assert(secondDictionary.size() == 1);
    assert(secondDictionary["house"] == "дом");
    std::cout << "[PASSED] testStreams\n";
}

void testEqualityAndFileIO() {
    Dictionary firstDictionary;
    Dictionary secondDictionary;
    firstDictionary += std::make_pair("a", "b");
    secondDictionary += std::make_pair("a", "b");

    assert(firstDictionary == secondDictionary);
    assert(!(firstDictionary != secondDictionary));

    secondDictionary += std::make_pair("c", "d");
    assert(firstDictionary != secondDictionary);

    firstDictionary.saveToFile("test_dict.txt");

    Dictionary thirdDictionary;
    thirdDictionary.loadFromFile("test_dict.txt");
    assert(thirdDictionary.size() == 1);
    assert(thirdDictionary["a"] == "b");

    try {
        thirdDictionary.loadFromFile("nonexistent_file.txt");
        assert(false);
    } catch (const std::runtime_error&) {
    }

    try {
        thirdDictionary.saveToFile("/invalid_path/test.txt");
        assert(false);
    } catch (const std::runtime_error&) {
    }

    std::cout << "[PASSED] testEqualityAndFileIO\n";
}

int main() {
    std::cout << "=== RUNNING DICTIONARY UNIT TESTS ===\n";
    try {
        testAddAndSize();
        testCharStarOverload();
        testFindAndReplace();
        testRemove();
        testCopyAndAssignment();
        testStreams();
        testEqualityAndFileIO();
        std::cout << "=== ALL TESTS PASSED SUCCESSFULLY ===\n";
    } catch (const std::exception& error) {
        std::cerr << "[FAILED] Test failed with exception: " << error.what() << "\n";
        return 1;
    }
    return 0;
}