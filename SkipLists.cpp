#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
using namespace std;

class Node {
public:
    int value;
    Node** forward;
    Node(int level, int value) {
        this->value = value;
        forward = new Node*[level + 1];
        memset(forward, 0, sizeof(Node*) * (level + 1));
    }
    ~Node() {
        delete[] forward;
    }
};

class SkipList {
    int maxLevel;
    float p;
    int level;
    Node* header;

public:
    SkipList(int maxLevel, float p) {
        this->maxLevel = maxLevel;
        this->p = p;
        level = 0;
        header = new Node(maxLevel, -1);
    }

    ~SkipList() {
        Node* current = header;
        while (current != nullptr) {
            Node* next = current->forward[0];
            delete current;
            current = next;
        }
    }

    int randomLevel() {
        int lvl = 0;
        while (((float) rand() / RAND_MAX) < p && lvl < maxLevel) {
            lvl++;
        }
        return lvl;
    }

    void insertElement(int value) {
        Node* current = header;
        Node* update[maxLevel + 1];
        memset(update, 0, sizeof(Node*) * (maxLevel + 1));

        for (int i = level; i >= 0; i--) {
            while (current->forward[i] != nullptr && current->forward[i]->value < value) {
                current = current->forward[i];
            }
            update[i] = current;
        }

        current = current->forward[0];

        if (current == nullptr || current->value != value) {
            int rlevel = randomLevel();

            if (rlevel > level) {
                for (int i = level + 1; i < rlevel + 1; i++) {
                    update[i] = header;
                }
                level = rlevel;
            }

            Node* n = new Node(rlevel, value);

            for (int i = 0; i <= rlevel; i++) {
                n->forward[i] = update[i]->forward[i];
                update[i]->forward[i] = n;
            }
        }
    }

    void deleteElement(int value) {
        Node* current = header;
        Node* update[maxLevel + 1];
        memset(update, 0, sizeof(Node*) * (maxLevel + 1));

        for (int i = level; i >= 0; i--) {
            while (current->forward[i] != nullptr && current->forward[i]->value < value) {
                current = current->forward[i];
            }
            update[i] = current;
        }

        current = current->forward[0];

        if (current != nullptr && current->value == value) {
            for (int i = 0; i <= level; i++) {
                if (update[i]->forward[i] != current) {
                    break;
                }
                update[i]->forward[i] = current->forward[i];
            }

            while (level > 0 && header->forward[level] == nullptr) {
                level--;
            }

            delete current;
        }
    }

    bool searchElement(int value) {
        Node* current = header;
        for (int i = level; i >= 0; i--) {
            while (current->forward[i] && current->forward[i]->value < value) {
                current = current->forward[i];
            }
        }

        current = current->forward[0];

        if (current && current->value == value) {
            return true;
        }
        return false;
    }

    void displayList() {
        for (int i = 0; i <= level; i++) {
            Node* node = header->forward[i];
            cout << "Level " << i << ": ";
            while (node != nullptr) {
                cout << node->value << " ";
                node = node->forward[i];
            }
            cout << "\n";
        }
    }
};

// Test cases
int main() {
    srand((unsigned)time(0));

    SkipList lst(3, 0.5);

    lst.insertElement(3);
    lst.insertElement(6);
    lst.insertElement(7);
    lst.insertElement(9);
    lst.insertElement(12);
    lst.insertElement(19);
    lst.insertElement(17);
    lst.insertElement(26);
    lst.insertElement(21);

    lst.displayList();

    cout << "Searching for 3: " << (lst.searchElement(3) ? "Found" : "Not Found") << endl;
    cout << "Searching for 7: " << (lst.searchElement(7) ? "Found" : "Not Found") << endl;
    cout << "Searching for 15: " << (lst.searchElement(15) ? "Found" : "Not Found") << endl;

    lst.deleteElement(3);
    cout << "Searching for 3 after deletion: " << (lst.searchElement(3) ? "Found" : "Not Found") << endl;

    lst.displayList();

    return 0;
}