#include <iostream>
#include <Windows.h>
#include <limits>
#include <ctime>
#include <chrono>
#include <fstream>
#undef max
using namespace std;
using namespace chrono;


void clearStream() {
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cin.sync();
}


int randint(bool randArrSize){
    if(randArrSize){
        return rand() % 20 + 1;
    }
    return rand() % 198 - 99;
}


void mainMenu(){
    cout << "1) Сформировать дерево\n" <<
         "2) Вывести дерево\n" <<
         "3) Действия с деревом\n" <<
         "4) Проверка на сбалансированность\n" <<
         "5) Генерация заданий\n" <<
         "6) Время выполнения\n" <<
         "7) Очистка экрана\n" <<
         "8) Выход\n-->> ";
}


void actionMenu(){
    cout << "1) Вставка\n" <<
         "2) Удаление\n" <<
         "3) Получение\n-->> ";
}

//..........................................................................
struct Time{
    int rand = 0;
    int fill = 0;
    int insert = 0;
    int erase = 0;
    int search = 0;
    int balance = 0;
    void print(){
        cout << "\nВремя:";
        if (rand) cout << "\nЗаполнения случайными числами = " << rand;
        if (fill) cout << "\nЗаполения числами с клавиатуры = " << fill;
        if (insert) cout << "\nВставки = " << insert;
        if (erase) cout << "\nУдаления = " << erase;
        if (search) cout << "\nПолучения = " << search;
        if (balance) cout << "\nПроверки на сбалансированность = " << balance;
    }
};

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

struct trunk{
    string str;
    trunk* prev;
    trunk(trunk* prevAdr, string prev_str){
        str = prev_str;
        prev = prevAdr;
    }
};

struct Tree {
    Node* root;

    void createTree(){
        root = nullptr;
    }

    void updHeight(Node* node){
        if (node == nullptr) return;
        node->height = (getHeight(node->left) >= getHeight(node->right)) ? getHeight(node->left) + 1 : getHeight(node->right) + 1;
    }

    int getHeight(Node* node){
        return node == nullptr ? -1 : node->height;
    }

    int getBalance(Node* node){
        return(node == nullptr) ? 0 : getHeight(node->right) - getHeight(node->left);
    }

    void rotateRight(Node* &node){
        Node* lst = node->left;
        Node* lrst = lst->right;
        lst->right = node;
        node->left = lrst;
        node = lst;
        updHeight(node->right);
        updHeight(node);
    }

    void rotateLeft(Node* &node){
        Node* rst = node->right;
        Node* rlst = rst->left;
        rst->left = node;
        node->right = rlst;
        node = rst;
        updHeight(node->left);
        updHeight(node);
    }

    Node* balance(Node* node){
        updHeight(node);
        if(getBalance(node) == 2){
            if(getBalance(node->right) < 0)
                rotateRight(node->right);
            rotateLeft(node);
        }
        if(getBalance(node) == -2){
            if(getBalance(node->left) > 0)
                rotateLeft(node->left);
            rotateRight(node);
        }
        return node;
    }

    Node* insert(Node* node, int key){
        if(!node) return new Node(key);
        if( key < node->key )
            node->left = insert(node->left,key);
        else
            node->right = insert(node->right,key);
        return balance(node);
    }

    Node* deleteNode(Node* node, int key){
        if(node == nullptr) return nullptr;
        else if(key < node->key) node->left = deleteNode(node->left, key);
        else if(key > node->key) node->right = deleteNode(node->right, key);
        else{
            if(node->left == nullptr || node->right == nullptr)
                node = (node->left == nullptr) ? node->right : node->left;
            else{
                Node* maxInLeft = getMaxTree(node->left);
                node->key = maxInLeft->key;
                node->left = deleteNode(node->left, maxInLeft->key);
            }
        }
        if(node != nullptr){
            updHeight(node);
        }
        return balance(node);
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
        return ptr;
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

    void showTrunk(trunk* p, int &count, ofstream &fout){
        if(p == nullptr) return;
        showTrunk(p->prev, count, fout);
        count++;
        fout << p->str;
    }

    void printTree(Node* node, trunk* prev, bool isRight, ofstream &fout){
        if(node == nullptr) return;
        string prev_str = "    ";
        trunk* tmp = new trunk(prev, prev_str);
        printTree(node->right, tmp, true, fout);
        if (!prev)
            tmp->str = "-->";
        else if (isRight) {
            tmp->str = ".-->";
            prev_str = "   |";
        }
        else {
            tmp->str = "`-->";
            prev->str = prev_str;
        }
        int count = 0;
        showTrunk(tmp, count, fout);
        fout << "{" << node->key << "}" << "\n";
        if (prev)
            prev->str = prev_str;
        tmp->str = "   |";
        printTree(node->left, tmp, false, fout);
    }

    void directBypass(Node* node, bool &isBalance, bool balancing) {
        if (node && isBalance && !balancing) {
            if(getBalance(node) == 2 || getBalance(node) == -2){
                isBalance = false;
            }
            directBypass(node->left, isBalance, balancing);
            directBypass(node->right, isBalance, balancing);
        }
    }

    void fillTree(int numOfNodes, int* arr, bool rand){
        if(rand){
            for (int i = 0; i < numOfNodes; i++){
                root = insert(root, randint(false));
            }
        }
        else{
            for (int i = 0; i < numOfNodes; i++){
                root = insert(root, arr[i]);
            }
        }
    }
};

//.................................................................
int fillArray(int* &arr){
    int number;
    int sizeArr = 0;
    while (cin >> number){
        int  *rez = new int[sizeArr+1];
        for (int i = 0; i < sizeArr; i++) {
            rez[i] = arr[i];
        }
        rez[sizeArr] = number;
        sizeArr++;
        delete[] arr;
        arr = rez;
        if (cin.peek() == '\n') {
            break;
        }
    }
    clearStream();
    return sizeArr;
}


int randArray(int &sizeArr, int* &arr){
    int  *rez = new int[sizeArr];
    for (int i = 0; i < sizeArr; i++){
        *(rez + i) = randint(false);
    }
    delete[] arr;
    arr = rez;
    return sizeArr;
}


void clearArray(int* &arr){
    int  *rez = new int[0];
    delete[] arr;
    arr = rez;
}

//................................................................

void completeTask(ofstream &task, ofstream &taskKey, ofstream &taskAns, int* &arr, Tree &avl){
    avl.createTree();
    int numOfNodes;
    short choise;
    short key;
    cout << "\nВведите количество вариантов: ";
    cin >> choise;
    task.open("C:\\Users\\fedos\\CLionProjects\\AVL-tree\\output_task.txt");
    taskKey.open("C:\\Users\\fedos\\CLionProjects\\AVL-tree\\output_key.txt");
    taskAns.open("C:\\Users\\fedos\\CLionProjects\\AVL-tree\\output_ans.txt");
    if (!task.is_open() || !taskKey.is_open() || !taskAns.is_open()){
        cout << "Ошибка открытия файла!!!";
        task.close();
        taskKey.close();
        taskAns.close();
        return;
    }
    for (int i = 1; i <= choise; i++){
        clearArray(arr);
        avl.deleteTree(avl.root);
        avl.createTree();
        task << "ВАРИАНТ № " << i << "\n\n";
        taskKey << "ВАРИАНТ № " << i << "\n";

        task << "Задание № 1\nСоздать АВЛ-дерево из следующего массива:";
        taskKey << "Задание № 1\n";
        numOfNodes = randint(true);
        randArray(numOfNodes, arr);
        for (int j = 0; j < numOfNodes; j++){
            task << " " << arr[j];
        }
        avl.fillTree(numOfNodes, arr, false);
        avl.printTree(avl.root, nullptr, true, taskKey);

        key = randint(false);
        task << "\nЗадание № 2\nВставить в дерево элемент с ключом: " << key << "\n";
        taskKey << "\nЗадание № 2\n";
        avl.insert(avl.root, key);
        avl.printTree(avl.root, nullptr, true, taskKey);

        key = randint(false);
        task << "Задание № 3\nУдалить из дерева элемент с ключом: " << key << "\n\n";
        taskKey << "\nЗадание № 3\n";
        avl.deleteNode(avl.root, key);
        avl.printTree(avl.root, nullptr, true, taskKey);
        taskKey << "\n\n";
    }
    task.close();
    taskKey.close();
    taskAns.close();
}
//.................................................................
int main() {
    SetConsoleOutputCP(CP_UTF8);
    srand(time(NULL));
    ofstream fout, task,taskKey, taskAns;
    time_point<steady_clock, duration<__int64, ratio<1, 1000000000>>> start, end;
    nanoseconds result;
    Tree avl;
    Time time;
    avl.createTree();
    short choise;
    bool isBalance = true;
    int* arr = new int [0];
    while(true){
        system("cls");
        mainMenu();
        cin >> choise;
        switch(choise){
            case 1:
                avl.deleteTree(avl.root);
                avl.createTree();
                clearArray(arr);
                cout << "\nВыберите способ заполнения:\n" <<
                     "1) Случайные числа\n" <<
                     "2) Ручной ввод\n-->> ";
                cin >> choise;
                if(choise == 1){
                    cout << "\nВведите количество элементов: ";
                    cin >> choise;
                    start = steady_clock::now();
                    avl.fillTree(choise, nullptr, true);
                    end = steady_clock::now();
                    result = duration_cast<nanoseconds>(end - start);
                    time.rand = result.count();
                }
                else if(choise == 2){
                    cout << "\nЗаполните массив:\n";
                    choise = fillArray(arr);
                    start = steady_clock::now();
                    avl.fillTree(choise, arr, false);
                    end = steady_clock::now();
                    result = duration_cast<nanoseconds>(end - start);
                    time.fill = result.count();
                }
                else cout << "\nНеправильно введен номер!\n";
                break;
            case 2:
                fout.open("C:\\Users\\fedos\\CLionProjects\\AVL-tree\\Tree.txt");
                if (!fout.is_open()) {
                    cout << "Ошибка открытия файла!!!";
                    fout.close();
                    break;
                }
                else{
                    avl.printTree(avl.root, nullptr, true, fout);
                    fout.close();
                }
                break;
            case 3:
                actionMenu();
                cin >> choise;
                switch(choise){
                    case 1:
                        cout << "\nВведите ключ: ";
                        cin >> choise;
                        start = steady_clock::now();
                        avl.insert(avl.root, choise);
                        end = steady_clock::now();
                        result = duration_cast<nanoseconds>(end - start);
                        time.insert = result.count();
                        break;
                    case 2:
                        cout << "\nВведите ключ: ";
                        cin >> choise;
                        start = steady_clock::now();
                        avl.root = avl.deleteNode(avl.root, choise);
                        end = steady_clock::now();
                        result = duration_cast<nanoseconds>(end - start);
                        time.erase = result.count();
                        break;
                    case 3:
                        cout << "\nВведите ключ: ";
                        cin >> choise;
                        start = steady_clock::now();
                        cout << avl.search(choise, avl.root);
                        end = steady_clock::now();
                        result = duration_cast<nanoseconds>(end - start);
                        time.search = result.count();
                        break;
                    default:
                        cout << "\nНеправильно введен номер!\n";
                }
                break;
            case 4:
                start = steady_clock::now();
                avl.directBypass(avl.root, isBalance, false);
                end = steady_clock::now();
                result = duration_cast<nanoseconds>(end - start);
                time.balance = result.count();
                if (isBalance){
                    cout << "\nДерево сбалансированно!\n";
                }
                else cout << "\nДерево не сбалансированно!\n";
                isBalance = true;
                break;
            case 5:
                completeTask(task, taskKey, taskAns, arr, avl);
                clearArray(arr);
                break;
            case 6:
                time.print();
                break;
            case 7:
                system("cls");
                break;
            case 8:
                delete [] arr;
                arr = nullptr;
                avl.deleteTree(avl.root);
                exit(0);
                break;
            default:
                cout << "\nНеправильно введен номер!\n";
        }
    }
    return 0;
}