#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <sstream> // Include <sstream> for stringstream

using namespace std;

template <typename K>
int divisionHash(K key, int tableSize) {
    stringstream ss;
    ss << key;
    string keyStr = ss.str();
    int hashValue = 0;
    for (char c : keyStr) {
        hashValue = (hashValue * 10 + (c - '0')) % tableSize;
    }
    return hashValue;
}

template <typename K>
int multiplicationHash(K key, int tableSize) {
    stringstream ss;
    ss << key;
    string keyStr = ss.str();
    const double A = 0.6180339887;
    double sum = 0;
    for (char c : keyStr) {
        sum += c;
    }
    double temp = sum * A;
    double fractionalPart = temp - floor(temp);
    return (int) (fractionalPart * tableSize);
}

template <typename K>
int midSquareHash(K key, int tableSize) {
    stringstream ss;
    ss << key; // Convert key to string using stringstream
    string keyStr = ss.str();
    long long square = stoll(keyStr) * stoll(keyStr);
    string squareStr = to_string(square);
    int midIndex = (squareStr.length() - keyStr.length()) / 2;
    string midPart = squareStr.substr(midIndex, keyStr.length());
    return stoi(midPart) % tableSize;
}

template <typename K>
int foldingHash(K key, int tableSize, int foldSize) {
    stringstream ss;
    ss << key; // Convert key to string using stringstream
    string keyStr = ss.str();
    int hashValue = 0;
    for (int i = 0; i < keyStr.length(); i += foldSize) {
        string fold = keyStr.substr(i, foldSize);
        hashValue += stoi(fold);
    }
    return hashValue % tableSize;
}


// Chaining Method
template <typename K>
class Node {
public:
    K key;
    Node *next;

    Node(K key) : key(key), next(nullptr) {}
};

template <typename K>
class HashTable {
private:
    vector<Node<K> *> table;
    int tableSize;

public:
    HashTable(int size) : table(size, nullptr), tableSize(size) {}

    int hash(K key) {
        return divisionHash(key, tableSize);
    }

    void insert(K key) {
        int index = hash(key);
        Node<K> *newNode = new Node<K>(key);
        newNode->next = table[index];
        table[index] = newNode;
    }

    Node<K> *search(K key) {
        int index = hash(key);
        Node<K> *current = table[index];
        while (current != nullptr) {
            if (current->key == key) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }
};

template <typename K>
class HashTableOpenAddressing {
private:
    vector<K> table;
    int tableSize;

public:
    HashTableOpenAddressing(int size) : table(size, ""), tableSize(size) {}

    int hash(K key) {
        return divisionHash(key, tableSize);
    }

    void insert(K key) {
        int index = hash(key);
        int probe = 0;
        while (table[index] != "") {
            index = (index + probe) % tableSize;
            probe++;
        }
        table[index] = key;
    }

    K search(K key) {
        int index = hash(key);
        int probe = 0;
        while (table[index] != "" && table[index] != key) { // Linear probing to find the key
            index = (index + probe) % tableSize;
            probe++;
        }
        return table[index];
    }
};

// Double Hashing
template <typename K>
class HashTableDoubleHashing {
private:
    vector<K> table;
    int tableSize;

public:
    HashTableDoubleHashing(int size) : table(size, ""), tableSize(size) {}

    int hash1(K key) {
        return midSquareHash(key, tableSize);
    }

    int hash2(K key) {
        return foldingHash(key, tableSize, 2);
    }

    void insert(K key) {
        int index = hash1(key);
        int step = hash2(key);
        int probe = 0;
        while (table[index] != "") {
            index = (index + probe * step) % tableSize;
            probe++;
        }
        table[index] = key;
    }

    K search(K key) {
        int index = hash1(key);
        int step = hash2(key);
        int probe = 0;
        while (table[index] != "" && table[index] != key) {
            index = (index + probe * step) % tableSize;
            probe++;
        }
        return table[index];
    }
};

int main() {
    string key = "12345";
    int tableSize = 10;

    cout << "Division Hash: " << divisionHash(key, tableSize) << endl;
    cout << "Multiplication Hash: " << multiplicationHash(key, tableSize) << endl;
    cout << "Mid-Square Hash: " << midSquareHash(key, tableSize) << endl;
    cout << "Folding Hash: " << foldingHash(key, tableSize, 2) << endl;

    HashTable<string> chainingTable(10);
    chainingTable.insert("apple");
    chainingTable.insert("banana");
    chainingTable.insert("apple");
    Node<string> *foundNode = chainingTable.search("banana");
    if (foundNode) {
        cout << "Found banana: " << foundNode->key << endl;
    } else {
        cout << "banana not found" << endl;
    }

    HashTableOpenAddressing<string> openAddressingTable(10);
    openAddressingTable.insert("apple");
    openAddressingTable.insert("banana");
    string foundKey = openAddressingTable.search("banana");
    if (foundKey != "") {
        cout << "Found banana: " << foundKey << endl;
    } else {
        cout << "banana not found" << endl;
    }

    HashTableDoubleHashing<string> doubleHashingTable(10);
    doubleHashingTable.insert("apple");
    doubleHashingTable.insert("banana");
    foundKey = doubleHashingTable.search("banana");
    if (foundKey != "") {
        cout << "Found banana: " << foundKey << endl;
    } else {
        cout << "banana not found" << endl;
    }

    return 0;
}