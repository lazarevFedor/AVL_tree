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


int main() {
    SetConsoleOutputCP(CP_UTF8);

    return 0;
}
