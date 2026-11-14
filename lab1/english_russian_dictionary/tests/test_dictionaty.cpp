#include <iostream>
#include <cassert>
#include <sstream>
#include <fstream>
#include "Dictionary.hpp"

void testAddAndSize() {
    Dictionary dict;
    assert(dict.size() == 0);
    assert(dict.isEmpty());

    dict += std::make_pair("apple", "яблоко");
    dict += std::make_pair("banana", "банан");

    assert(dict.size() == 2);
    assert(!dict.isEmpty());

    dict += std::make_pair("apple", "яблочко");
    assert(dict["apple"] == "яблочко");
    
    dict.clear();
    assert(dict.isEmpty());
    std::cout << "[PASSED] testAddAndSize\n";
}

void testCharStarOverload() {
    Dictionary dict;
    const char* eng = "cat";
    const char* rus = "кот";
    dict += std::make_pair(eng, rus);

    assert(dict.size() == 1);
    assert(dict["cat"] == "кот");

    dict -= eng;
    assert(dict["cat"] == "");
    std::cout << "[PASSED] testCharStarOverload\n";
}

void testFindAndReplace() {
    Dictionary dict;
    dict += std::make_pair("dog", "собака");

    assert(dict["dog"] == "собака");

    dict["dog"] = "пес";
    assert(dict["dog"] == "пес");

    assert(dict["mouse"] == "");
    assert(dict.size() == 2);

    const char* dog = "dog";
    assert(dict[dog] == "пес");
    std::cout << "[PASSED] testFindAndReplace\n";
}

void testRemove() {
    Dictionary dict;
    dict += std::make_pair("sun", "солнце");
    dict += std::make_pair("moon", "луна");
    dict += std::make_pair("star", "звезда");
    assert(dict.size() == 3); 

    dict -= "sun";
    assert(dict["sun"] == ""); 

    dict -= "moon";
    assert(dict["moon"] == "");

    dict -= "nonexistent";
    std::cout << "[PASSED] testRemove\n";
}

void testCopyAndAssignment() {
    Dictionary dict1;
    dict1 += std::make_pair("car", "машина");

    Dictionary dict2(dict1);
    assert(dict2.size() == 1);
    assert(dict2["car"] == "машина");

    Dictionary dict3;
    dict3 = dict1;
    assert(dict3.size() == 1);
    assert(dict3["car"] == "машина");

    dict3 = dict3;
    assert(dict3.size() == 1);
    std::cout << "[PASSED] testCopyAndAssignment\n";
}

void testStreams() {
    Dictionary dict1;
    dict1 += std::make_pair("house", "дом");

    std::stringstream ss;
    ss << dict1;

    Dictionary dict2;
    ss >> dict2;
    assert(dict2.size() == 1);
    assert(dict2["house"] == "дом");
    std::cout << "[PASSED] testStreams\n";
}

void testEqualityAndFileIO() {
    Dictionary dict1, dict2;
    dict1 += std::make_pair("a", "b");
    dict2 += std::make_pair("a", "b");
    
    assert(dict1 == dict2);
    assert(!(dict1 != dict2));

    dict2 += std::make_pair("c", "d");
    assert(dict1 != dict2);

    dict1.saveToFile("test_dict.txt");
    
    Dictionary dict3;
    dict3.loadFromFile("test_dict.txt");
    assert(dict3.size() == 1);
    assert(dict3["a"] == "b");

    try {
        dict3.loadFromFile("nonexistent_file.txt");
        assert(false);
    } catch (const std::runtime_error& e) {}

    try {
        dict3.saveToFile("/invalid_path/test.txt");
        assert(false);
    } catch (const std::runtime_error& e) {}

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
    } catch (const std::exception& e) {
        std::cerr << "[FAILED] Test failed with exception: " << e.what() << "\n";
        return 1;
    }
    return 0;
}