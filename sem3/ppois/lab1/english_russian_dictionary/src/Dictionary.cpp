#include "Dictionary.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>

Dictionary::Node::Node(const std::string& k, const std::string& v)
    : key(k), value(v), left(nullptr), right(nullptr) {}

Dictionary::Node::Node(const char* k, const char* v)
    : key(k), value(v), left(nullptr), right(nullptr) {}

Dictionary::Dictionary() : root_(nullptr), size_(0) {}

Dictionary::~Dictionary() {
    clear();
}

Dictionary::Node* Dictionary::copyTree(Node* node) const {
    if (!node) return nullptr;
    Node* newNode = new Node(node->key, node->value);
    newNode->left = copyTree(node->left);
    newNode->right = copyTree(node->right);
    return newNode;
}

Dictionary::Dictionary(const Dictionary& other) : root_(nullptr), size_(0) {
    root_ = copyTree(other.root_);
    size_ = other.size_;
}

Dictionary& Dictionary::operator=(const Dictionary& other) {
    if (this == &other) return *this;
    clear();
    root_ = copyTree(other.root_);
    size_ = other.size_;
    return *this;
}

void Dictionary::clearTree(Node* node) {
    if (!node) return;
    clearTree(node->left);
    clearTree(node->right);
    delete node;
}

void Dictionary::clear() {
    clearTree(root_);
    root_ = nullptr;
    size_ = 0;
}

Dictionary::Node* Dictionary::insert(Node* node, const std::string& key, const std::string& value, bool& inserted) {
    if (!node) {
        inserted = true;
        return new Node(key, value);
    }
    if (key < node->key) {
        node->left = insert(node->left, key, value, inserted);
    } else if (key > node->key) {
        node->right = insert(node->right, key, value, inserted);
    } else {
        node->value = value;
        inserted = false;
    }
    return node;
}

Dictionary& Dictionary::operator+=(const std::pair<std::string, std::string>& item) {
    bool inserted = false;
    root_ = insert(root_, item.first, item.second, inserted);
    if (inserted) {
        size_++;
    }
    return *this;
}

Dictionary& Dictionary::operator+=(const std::pair<const char*, const char*>& item) {
    bool inserted = false;
    root_ = insert(root_, std::string(item.first), std::string(item.second), inserted);
    if (inserted) {
        size_++;
    }
    return *this;
}

Dictionary::Node* Dictionary::findMin(Node* node) const {
    while (node && node->left) {
        node = node->left;
    }
    return node;
}

Dictionary::Node* Dictionary::remove(Node* node, const std::string& key, bool& removed) {
    if (!node) return nullptr;
    if (key < node->key) {
        node->left = remove(node->left, key, removed);
    } else if (key > node->key) {
        node->right = remove(node->right, key, removed);
    } else {
        removed = true;
        if (!node->left) {
            Node* temp = node->right;
            delete node;
            return temp;
        } else if (!node->right) {
            Node* temp = node->left;
            delete node;
            return temp;
        }
        Node* temp = findMin(node->right);
        node->key = temp->key;
        node->value = temp->value;
        node->right = remove(node->right, temp->key, removed);
    }
    return node;
}

Dictionary& Dictionary::operator-=(const std::string& key) {
    bool removed = false;
    root_ = remove(root_, key, removed);
    if (removed && size_ > 0) {
        size_--;
    }
    return *this;
}

Dictionary& Dictionary::operator-=(const char* key) {
    return *this -= std::string(key);
}

Dictionary::Node* Dictionary::find(Node* node, const std::string& key) const {
    if (!node || node->key == key) return node;
    if (key < node->key) return find(node->left, key);
    return find(node->right, key);
}

std::string& Dictionary::operator[](const std::string& key) {
    Node* found = find(root_, key);
    if (found) {
        return found->value;
    }
    bool inserted = false;
    root_ = insert(root_, key, "", inserted);
    if (inserted) {
        size_++;
    }
    return find(root_, key)->value;
}

std::string& Dictionary::operator[](const char* key) {
    return (*this)[std::string(key)];
}

size_t Dictionary::size() const {
    return size_;
}

bool Dictionary::isEmpty() const {
    return size_ == 0;
}

void Dictionary::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    clear();
    std::string eng, rus;
    while (file >> eng >> rus) {
        *this += std::make_pair(eng, rus);
    }
}

void Dictionary::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not write to file: " + filename);
    }
    file << *this;
}

void Dictionary::serialize(Node* node, std::ostream& os) const {
    if (!node) return;
    serialize(node->left, os);
    os << node->key << " " << node->value << "\n";
    serialize(node->right, os);
}

bool Dictionary::operator==(const Dictionary& other) const {
    if (this->size_ != other.size_) return false;
    std::ostringstream oss1, oss2;
    oss1 << *this;
    oss2 << other;
    return oss1.str() == oss2.str();
}

bool Dictionary::operator!=(const Dictionary& other) const {
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const Dictionary& dict) {
    dict.serialize(dict.root_, os);
    return os;
}

std::istream& operator>>(std::istream& is, Dictionary& dict) {
    dict.clear();
    std::string eng, rus;
    while (is >> eng >> rus) {
        dict += std::make_pair(eng, rus);
    }
    return is;
}