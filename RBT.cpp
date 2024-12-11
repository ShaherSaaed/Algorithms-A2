#include <iostream>

using namespace std;

enum Color {
    RED, BLACK
};

struct Node {
    int data;
    Color color;
    Node *left;
    Node *right;
    Node *parent;

    Node(int data) : data(data), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

class RedBlackTree {
private:
    Node *root;
    Node *TNULL;

    void balanceInsert(Node *node);

    void balanceDelete(Node *node);

    void leftRotate(Node *x);

    void rightRotate(Node *x);

    void transplant(Node *u, Node *v) {
        if (u->parent == nullptr) {
            root = v;
        } else if (u == u->parent->left) {
            u->parent->left = v;
        } else {
            u->parent->right = v;
        }
        if (v != TNULL) {
            v->parent = u->parent;
        }
    }

    Node *minimum(Node *node) {
        while (node->left != TNULL) {
            node = node->left;
        }
        return node;
    }

    void inorderTraversalHelper(Node *node) {
        if (node != TNULL) {
            inorderTraversalHelper(node->left);
            cout << node->data << " ";
            inorderTraversalHelper(node->right);
        }
    }

    int getHeightHelper(Node *node) {
        if (node == TNULL) {
            return 0;
        }
        int leftHeight = getHeightHelper(node->left);
        int rightHeight = getHeightHelper(node->right);
        return 1 + max(leftHeight, rightHeight);
    }

public:
    RedBlackTree();

    void insert(int key);

    void deleteNode(int data);

    void inorderTraversal(Node *node);

    void printTree();

    Node *getRoot();

    int getTreeHeight();
};

RedBlackTree::RedBlackTree() {
    TNULL = new Node(0);
    TNULL->color = BLACK;
    root = TNULL;
}

void RedBlackTree::leftRotate(Node *x) {
    Node *y = x->right;
    x->right = y->left;
    if (y->left != TNULL) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == nullptr) {
        root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

void RedBlackTree::rightRotate(Node *x) {
    Node *y = x->left;
    x->left = y->right;
    if (y->right != TNULL) {
        y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == nullptr) {
        root = y;
    } else if (x == x->parent->right) {
        x->parent->right = y;
    } else {
        x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
}

void RedBlackTree::balanceInsert(Node *k) {
    while (k->parent->color == RED) {
        if (k->parent == k->parent->parent->left) {
            Node *u = k->parent->parent->right;
            if (u->color == RED) {
                u->color = BLACK;
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                k = k->parent->parent;
            } else {
                if (k == k->parent->right) {
                    k = k->parent;
                    leftRotate(k);
                }
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                rightRotate(k->parent->parent);
            }
        } else {
            Node *u = k->parent->parent->left;
            if (u->color == RED) {
                u->color = BLACK;
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                k = k->parent->parent;
            } else {
                if (k == k->parent->left) {
                    k = k->parent;
                    rightRotate(k);
                }
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                leftRotate(k->parent->parent);
            }
        }
        if (k == root) {
            break;
        }
    }
    root->color = BLACK;
}

void RedBlackTree::balanceDelete(Node *x) {
    while (x != root && x->color == BLACK) {
        if (x == x->parent->left) {
            Node *s = x->parent->right;
            if (s->color == RED) {
                s->color = BLACK;
                x->parent->color = RED;
                leftRotate(x->parent);
                s = x->parent->right;
            }
            if (s->left->color == BLACK && s->right->color == BLACK) {
                s->color = RED;
                x = x->parent;
            } else {
                if (s->right->color == BLACK) {
                    s->left->color = BLACK;
                    s->color = RED;
                    rightRotate(s);
                    s = x->parent->right;
                }
                s->color = x->parent->color;
                x->parent->color = BLACK;
                s->right->color = BLACK;
                leftRotate(x->parent);
                x = root;
            }
        } else {
            Node *s = x->parent->left;
            if (s->color == RED) {
                s->color = BLACK;
                x->parent->color = RED;
                rightRotate(x->parent);
                s = x->parent->left;
            }
            if (s->right->color == BLACK && s->left->color == BLACK) {
                s->color = RED;
                x = x->parent;
            } else {
                if (s->left->color == BLACK) {
                    s->right->color = BLACK;
                    s->color = RED;
                    leftRotate(s);
                    s = x->parent->left;
                }
                s->color = x->parent->color;
                x->parent->color = BLACK;
                s->left->color = BLACK;
                rightRotate(x->parent);
                x = root;
            }
        }
    }
    x->color = BLACK;
}

void RedBlackTree::deleteNode(int data) {
    Node *z = root;
    Node *x;
    Node *y;
    while (z != TNULL) {
        if (z->data == data) {
            break;
        }
        if (data < z->data) {
            z = z->left;
        } else {
            z = z->right;
        }
    }

    if (z == TNULL) {
        cout << "Node not found in the tree!" << endl;
        return;
    }

    y = z;
    Color yOriginalColor = y->color;
    if (z->left == TNULL) {
        x = z->right;
        transplant(z, z->right);
    } else if (z->right == TNULL) {
        x = z->left;
        transplant(z, z->left);
    } else {
        y = minimum(z->right);
        yOriginalColor = y->color;
        x = y->right;
        if (y->parent == z) {
            x->parent = y;
        } else {
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
    }
}

int main() {
    RedBlackTree rbt;

    // Test cases for inserting nodes
    cout << "Inserting nodes: 10, 20, 30, 15, 25" << endl;
    rbt.insert(10);
    rbt.insert(20);
    rbt.insert(30);
    rbt.insert(15);
    rbt.insert(25);

    cout << "In-order traversal of the tree:" << endl;
    rbt.printTree();

    // Test deleting a node
    cout << "Deleting node 20" << endl;
    rbt.deleteNode(20);

    cout << "In-order traversal after deletion:" << endl;
    rbt.printTree();

    // Print the root of the tree
    Node *root = rbt.getRoot();
    if (root != nullptr) {
        cout << "Root of the tree: " << root->data << endl;
    } else {
        cout << "The tree is empty." << endl;
    }

    // Check the height of the tree
    cout << "Height of the tree: " << rbt.getTreeHeight() << endl;

    return 0;
}