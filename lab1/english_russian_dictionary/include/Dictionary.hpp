#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP

#include <string>
#include <iostream>
#include <utility>

class Dictionary {
private:
    struct Node {
        std::string key;
        std::string value;
        Node* left;
        Node* right;

        Node(const std::string& k, const std::string& v);
        Node(const char* k, const char* v);
    };

    Node* root_;
    size_t size_;

    Node* copyTree(Node* node) const;
    void clearTree(Node* node);
    Node* insert(Node* node, const std::string& key, const std::string& value, bool& inserted);
    Node* remove(Node* node, const std::string& key, bool& removed);
    Node* find(Node* node, const std::string& key) const;
    Node* findMin(Node* node) const;
    void serialize(Node* node, std::ostream& os) const;

public:
    Dictionary();
    ~Dictionary();
    Dictionary(const Dictionary& other);
    Dictionary& operator=(const Dictionary& other);

    Dictionary& operator+=(const std::pair<std::string, std::string>& item);
    Dictionary& operator+=(const std::pair<const char*, const char*>& item);

    Dictionary& operator-=(const std::string& key);
    Dictionary& operator-=(const char* key);

    std::string& operator[](const std::string& key);
    std::string& operator[](const char* key);

    size_t size() const;
    bool isEmpty() const;
    void clear();

    void loadFromFile(const std::string& filename);
    void saveToFile(const std::string& filename) const;

    bool operator==(const Dictionary& other) const;
    bool operator!=(const Dictionary& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Dictionary& dict);
    friend std::istream& operator>>(std::istream& is, Dictionary& dict);
};

#endif