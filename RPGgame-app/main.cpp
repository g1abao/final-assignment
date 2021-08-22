//unknown app
//simple rpg game
#include <iostream>
#include <string>
#include <fstream>

struct mainChar {
    int hp;
    int atk;
    int def;
};

struct Boss {
    std::string name;
    int hp;
    int atk;
    int skillPoint;
};

struct Node {
    Boss boss;
    Node* next;
};

struct List {
    Node* head;
    Node* tail;
};

Node* createNode(Boss x) {
    Node* p = new Node;
    p->boss = x;
    p->next = NULL;
    return p;
}

void insertNode(List& l, Node* p) {
    if (l.head == NULL)
        l.head = l.tail = p;
    else {
        l.tail->next = p;
        l.tail = p;
    }
}

void iniList(List& l) {
    l.head = NULL;
}

void play(List l, mainChar mChar, Boss boss) {

}

void loadFile(List& l) {
    std::fstream fs;
    fs.open("rpg.txt", std::ios::in);
    Boss boss;

    while (!fs.eof()) {
        getline(fs, boss.name, '-');
        fs.seekg(1, std::ios::cur);
        fs >> boss.hp;
        fs.seekg(1, std::ios::cur);
        fs >> boss.atk;
        fs.seekg(1, std::ios::cur);
        fs >> boss.skillPoint;
        insertNode(l, createNode(boss));
    }
}

void startGame() {
    List l;
    iniList(l);
    loadFile(l);
    int level;
    // std::cout << "Select level\n";
    // std::cout << "1. Easy\n" << "2. Medium\n" << "3. Hard\n";
    // std::cin >> level;
    for (Node* p = l.head; p != NULL; p = p->next) {
        std::cout << p->boss.name << ", "
            << p->boss.hp << ", "
            << p->boss.atk << ", "
            << p->boss.skillPoint << std::endl;
    }
}

void menu() {
    int choice;
    std::cout << "\tRPG Game\n";
    std::cout << "1. Play\n" << "2. Quit\n";
    std::cin >> choice;

    while (1) {
        if (choice == 1) {
            system("cls");
            startGame();
        }
        else if (choice == 2) {
            std::cout << "Have a nice day!\n";
            break;
            EXIT_SUCCESS;
        }
    }
}

int main() {
    //menu();
    startGame();
    std::cout << "check";
    return 0;
}