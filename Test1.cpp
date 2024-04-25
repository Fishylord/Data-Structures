#include <iostream>
#include <ctime>
#include <cstdlib>

//Testing testing 123
// Define the Node for the CDLL
struct Node {
    std::string value;
    Node *next;
    Node *prev;
    
    Node(std::string val) : value(val), next(nullptr), prev(nullptr) {}
};

// Define the Circular Doubly Linked List
class CircularDoublyLinkedList {
private:
    Node *head;
    int size;

public:
    CircularDoublyLinkedList() : head(nullptr), size(0) {}

    void addNode(std::string value) {
        Node *newNode = new Node(value);
        if (!head) {
            head = newNode;
            head->next = head;
            head->prev = head;
        } else {
            Node *tail = head->prev;
            tail->next = newNode;
            newNode->prev = tail;
            newNode->next = head;
            head->prev = newNode;
        }
        size++;
    }

    void displayList() {
        Node *current = head;
        do {
            std::cout << current->value << " -> ";
            current = current->next;
        } while (current != head);
        std::cout << "HEAD" << std::endl;
    }

    std::string spinWheel() {
        int spins = rand() % (3 * size) + 1; // Random spins as per the rule
        std::cout << "The wheel is spun " << spins << " times." << std::endl;
        while (spins--) {
            head = head->next;
        }
        return head->value;
    }
};

int main() {
    srand(time(NULL)); // Seed for random number generation

    CircularDoublyLinkedList wheel;
    wheel.addNode("$100");
    wheel.addNode("$200");
    wheel.addNode("Good Luck");
    wheel.addNode("$300");
    wheel.addNode("Try Again");
    wheel.addNode("$400");

    std::cout << "Initial Wheel: ";
    wheel.displayList();

    std::cout << "Spinning the wheel..." << std::endl;
    std::string result = wheel.spinWheel();
    std::cout << "The wheel stopped at: " << result << std::endl;

    return 0;
}