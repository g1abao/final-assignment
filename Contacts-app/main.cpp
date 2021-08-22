//Telephone directory
#include <iostream>
#include <string>
#include <fstream>

struct people {
	std::string fullname;
	std::string phoneNumber;
};

struct Node {
	people data;
	Node* left;
	Node* right;
	int height;
};

Node* createNode(people x) {
	Node* p = new Node;
	p->data = x;
	p->left = p->right = NULL;
	p->height = 1;
	return p;
}

int height(Node* N) {
	if (N == NULL)
		return 0;
	return N->height;
}

int max(int a, int b) {
	if (a > b)
		return a;
	return b;
}

Node* rightRotate(Node* N) {
	Node* l = N->left;
	Node* lr = l->right;

	l->right = N;
	N->left = lr;

	N->height = max(height(N->left), height(N->right));
	l->height = max(height(l->left), height(l->right));

	return l;
}

Node* leftRotate(Node* N) {
	Node* r = N->right;
	Node* rl = r->left;

	r->right = N;
	N->left = rl;

	N->height = max(height(N->left), height(N->right));
	r->height = max(height(r->left), height(r->right));

	return r;
}

int balanceTree(Node* N) {
	if (N == NULL)
		return 0;
	return (height(N->left) - height(N->right));
}

Node* insertNode(Node* N, people x) {
	if (N == NULL)
		return createNode(x);

	if (x.fullname < N->data.fullname)
		N->left = insertNode(N->left, x);
	else if (x.fullname >= N->data.fullname)
		N->right = insertNode(N->right, x);
	else
		return N;

	N->height = max(height(N->left), height(N->right));
	int balance = balanceTree(N);

	//LL
	if (balance > 1 && x.fullname < N->data.fullname)
		return rightRotate(N);
	//RR
	if (balance < -1 && x.fullname > N->data.fullname)
		return leftRotate(N);
	//LR
	if (balance > 1 && x.fullname > N->data.fullname) {
		N->left = leftRotate(N->left);
		return rightRotate(N);
	}
	//RL
	if (balance < -1 && x.fullname < N->data.fullname) {
		N->right = rightRotate(N->right);
		return leftRotate(N);
	}

	return N;

}

void readFromFile(Node*& Tree) {
	people x;
	std::fstream fs;
	fs.open("input.txt", std::ios::in);

	while (!fs.eof()) {
		getline(fs, x.fullname, ',');
		fs.seekg(1, 1);
		getline(fs, x.phoneNumber);
		Tree = insertNode(Tree, x);
	}
}

Node* findNode(Node* Tree, std::string s) {
	if (s[0] != '0') {
		if (Tree) {
			if (Tree->data.fullname == s)
				return Tree;
			if (s < Tree->data.fullname)
				return findNode(Tree->left, s);
			return findNode(Tree->right, s);
		}
	}
	else {
		if (Tree) {
			if (Tree->data.phoneNumber == s)
				return Tree;
			if (s < Tree->data.phoneNumber)
				return findNode(Tree->left, s);
			return findNode(Tree->right, s);
		}
	}
	return NULL;
}

void searchPeople(Node* Tree) {
	std::string s;
	std::cout << "Enter: ";
	getline(std::cin, s);
	Node* p;

	//input string is name
	if (s[0] != '0') {
		p = findNode(Tree, s);
		if (p != NULL)
			std::cout << "Phone number: " << p->data.phoneNumber << std::endl;
		else
			std::cout << "Can not find that number!" << std::endl;
	}
	else {
		p = findNode(Tree, s);
		if (p != NULL)
			std::cout << "Contact: " << p->data.fullname << std::endl;
		else
			std::cout << "Can not find that contact!" << std::endl;
	}
	delete p;
}

void addNewContact(Node*& Tree) {
	people x;

	std::cout << "Enter full name: ";
	getline(std::cin, x.fullname);
	std::cout << "Enter phone number: ";
	getline(std::cin, x.phoneNumber);

	Node* name = findNode(Tree, x.fullname);
	Node* number = findNode(Tree, x.phoneNumber);

	if (name == NULL && number == NULL) {
		Tree = insertNode(Tree, x);
		std::cout << "Add new contact successfully!\n";
	}
	else if (name != NULL && number == NULL) {
		char ans;
		std::cout << "Your list already have that contact. Do you want to add another phone number to that contact ? (y/n): ";
		std::cin >> ans;
		if (ans == 'y') {
			name->data.phoneNumber = name->data.phoneNumber + " or " + x.phoneNumber;
			std::cout << "Add new phone number successfully!\n";
		}
	}
	else {
		std::cout << "Your list already have that contact!\n";
	}
}

void editPhoneNumber(Node*& Tree) {
	std::string name;
	std::cout << "Enter name: ";
	getline(std::cin, name);
	
	Node* p = findNode(Tree, name);
	if (p != NULL) {
		std::string num;
		std::cout << "Enter phone number: ";
		getline(std::cin, num);
		p->data.phoneNumber = num;
		std::cout << "Edit successfully!\n";
	}
	else
		std::cout << "Can not find that contact, please try again.\n";
}

void findAndReplace(Node*& x, Node*& y) {
	if (x->left)
		findAndReplace(x, y->left);
	else {
		x->data = y->data;
		x = y;
		y = y->right;
	}
}

void deleteNode(Node*& Tree, std::string name) {
	if (Tree) {
		if (name > Tree->data.fullname)
			deleteNode(Tree->right, name);
		else if (name < Tree->data.fullname)
			deleteNode(Tree->left, name);
		else {
			Node* p = Tree;
			if (!Tree->left)
				Tree = Tree->right;
			else if (!Tree->right)
				Tree = Tree->left;
			else
				findAndReplace(p, Tree->left);
			delete p;
		}
	}
}

void deleteContact(Node*& Tree) {
	std::string name;
	std::cout << "Enter name: ";
	getline(std::cin, name);

	if (findNode(Tree, name)) {
		deleteNode(Tree, name);
		std::cout << "Delete successfully!\n";
	}
	else
		std::cout << "Can not find that contact!\n";
}

void printToFile(std::fstream& fs, Node* Tree) {
	if (Tree == NULL)
		return;
	fs << Tree->data.fullname << ", " << Tree->data.phoneNumber << std::endl;
	printToFile(fs, Tree->left);
	printToFile(fs, Tree->right);
}

int main() {
	//initialize tree
	Node* Tree = NULL;
	readFromFile(Tree);

	while (1) {
		int choice;
		std::cout << "1. Search list\n";
		std::cout << "2. Add new contact\n";
		std::cout << "3. Edit phone number\n";
		std::cout << "4. Print to new file\n";
		std::cout << "0. Exit\n";
		std::cout << "Choice: ";
		std::cin >> choice;
		std::cin.ignore();
		if (choice == 0) {
			std::cout << "Have a nice day!\n";
			break;
			EXIT_SUCCESS;
		}
		else if (choice == 1) {
			searchPeople(Tree);
			system("pause");
			system("cls");
		}
		else if (choice == 2) {
			addNewContact(Tree);
			system("pause");
			system("cls");
		}
		else if (choice == 3) {
			editPhoneNumber(Tree);
			system("pause");
			system("cls");
		}
		else if (choice == 4) {
			deleteContact(Tree);
			system("pause");
			system("cls");
		}
		else if (choice == 5) {
			std::fstream fs;
			fs.open("newList.txt", std::ios::out);
			printToFile(fs, Tree);
			std::cout << "Print success! Please check newList.txt.\n";
			system("pause");
			system("cls");
		}
	}

	delete Tree;
	return 0;
}