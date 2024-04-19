#include <iostream>
#include <Windows.h>
#include <limits>
#undef max
using namespace std;

void clearStream() {
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cin.sync();
}


void mainMenu(){
    cout << "1) Сформировать дерево\n" <<
    "2) Вывести дерево\n" <<
    "3) Действия с деревом\n-->> ";
}


struct Node {
    int key;
    int height = 0;
    Node* left;
    Node* right;
    Node(int inputData) {
        key = inputData;
        left = right = nullptr;
    }
};


struct Tree {
    Node* root;
    int sizeCount;

    void createTree(){
        root = nullptr;
    }

    void updHeight(Node* node){
        node->height = (getHeight(node->left) >= getHeight(node->right)) ? node->left->height + 1 : node->right->height + 1;
    }

    int getHeight(Node* node){
        return node == nullptr ? -1 : node->height;
    }

    int getBalance(Node* node){
        return(node == nullptr) ? 0 : getHeight(node->right) - getHeight(node->left);
    }

    void rightRotate(Node* node){
        swap(node->key, node->left->key);
        Node* ptr = node->right;
        node->right = node->left;
        node->left = node->right->left;
        node->right->left = node->right->right;
        node->right->right = ptr;
        updHeight(node->right);
        updHeight(node);
    }

    void leftRotate(Node* node){
        swap(node->key, node->right->key);
        Node* ptr = node->left;
        node->left = node->right;
        node->right = node->left->right;
        node->right->left = node->right->right;
        node->left->right = node->left->left;
        node->left->left = ptr;
        updHeight(node->left);
        updHeight(node);
    }

    void balance(Node* node){
        int balance = getBalance(node);
        if(balance == -2){
            if(getBalance(node->left) == 1) leftRotate(node->left);
            rightRotate(node);
        }
        else if(balance == 2){
            if(getBalance(node->right) == -1) rightRotate(node->right);
            leftRotate(node);
        }
    }

    void insert(Node* node, int key){
        if(key < node->key){
            if(node->left == nullptr) node->left = new Node(key);
            else insert(node->left, key);
        }
        else if(key >= node->key){
            if(node->right == nullptr) node->right = new Node(key);
            else insert(node->right, key);
        }
        updHeight(node);
        balance(node);
    }

    void deleteTree(Node* &ptr){
        if (ptr != nullptr){
            deleteTree(ptr->right);
            deleteTree(ptr->left);
            delete ptr;
        }
    }

    Node* search(int key, Node* ptr){
        if (ptr == nullptr) return ptr;
        if(key == ptr->key) return ptr;
        else if (key < ptr->key) search(key, ptr->left);
        else search(key, ptr->right);
    }

    Node* getMinTree(Node* ptr) {
        while (ptr->left) {
            ptr = ptr->left;
        }
        return ptr;
    }

    Node* getMaxTree(Node* ptr) {
        while (ptr->right) {
            ptr = ptr->right;
        }
        return ptr;
    }

    void sizeTree(Node* ptr) {
        sizeCount = 0;
        if (ptr){
            sizeCount++;
            sizeTree(root->left);
            sizeTree(root->right);
        }
    }
};


int main() {
    SetConsoleOutputCP(CP_UTF8);
    Tree avl;
    avl.createTree();

    return 0;
}
