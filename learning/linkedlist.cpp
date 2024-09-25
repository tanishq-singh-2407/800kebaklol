#include <iostream>
using namespace std;

struct Node {
	int data;
	struct Node *next;
};

class LinkedList {
	struct Node *head;
	struct Node *last;
	int elements;

public:
	LinkedList() {
		head = NULL;
		last = NULL;
		elements = 0;
	};

	void insert(int position, int data) {
		struct Node *ele = new Node;
		ele->data = data;
		elements++;
		
		if (!position) { // first position
			ele->next = head;
			head = ele;
		} else { // insert in between 
			struct Node *temp = head;

			for (int i=0; i<position-1; i++)
				temp = temp->next;

			ele->next = temp->next;
			temp->next = ele;

			if (ele->next == NULL) last = ele;
		};
	};

	void insertLast(int data) {
		struct Node *ele = new Node;
		ele->data = data;
		ele->next = NULL;
		last->next = ele;
		last = ele;
		elements++;
	};

	Node get(int position) {
		struct Node *temp = head;

		for (int i=0; i<position; i++)
			temp = temp->next;

		return *temp;
	};

	void remove(int position) {};
	void update(int position, int newdata) {};		

	void display() {
		struct Node *temp = head;
		
		while (temp) {
			cout << temp->data << " ";
			temp = temp->next;
		};

		cout << endl;
	};

	void info() {
		cout << "Number of Elements: " << elements << endl;
		cout << "First Element (at " << head << "): " << head->data << endl;
		cout << "Last  Element (at " << last << "): " << last->data << endl;
	};
};

int main() {
	LinkedList l1;

	l1.insert(0, 10);
	l1.insert(1, 20);
	l1.insert(2, 30);
	l1.insert(3, 44);
	l1.insertLast(88);
	l1.insert(0, -22);
	l1.insert(1, -10);
	l1.insert(6, 55);
	l1.insertLast(90);

	l1.display();
	cout << l1.get(5).data << endl;


	return 0;
};
