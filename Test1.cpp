#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <random>
#include <ctime>

struct Question {
    std::string id;
    std::string question;
    std::string choices;
    std::string answer;
    int marks;
};

struct Node {
    Question data;
    Node* next;
};

class LinkedList {
public:
    Node* head;
    LinkedList() : head(nullptr) {}

    ~LinkedList() {
        Node* current = head;
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
        head = nullptr;
    }

    void addNode(Question q) {
        Node* newNode = new Node{ q, nullptr };
        if (head == nullptr) {
            head = newNode;
        }
        else {
            Node* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

    Question removeFront() {
        if (head == nullptr) {
            throw std::runtime_error("List is empty");
        }
        Node* temp = head;
        Question q = temp->data;
        head = head->next;
        delete temp;
        return q;
    }

    void displayFirst() {
       Node* temp = head;
       std::cout << "ID: " << temp->data.id << "\nQuestion: " << temp->data.question
           << "\nChoices: " << temp->data.choices << "\nAnswer: " << temp->data.answer
           << "\nMarks: " << temp->data.marks << "\n\n";
    }

    void displayLast() {
       Node* temp = head;
       while (temp->next != nullptr) {
           temp = temp->next;
       }
       std::cout << "ID: " << temp->data.id << "\nQuestion: " << temp->data.question
           << "\nChoices: " << temp->data.choices << "\nAnswer: " << temp->data.answer
           << "\nMarks: " << temp->data.marks << "\n\n";
    }

    void display() {
        Node* temp = head;
        while (temp != nullptr) {
            std::cout << "ID: " << temp->data.id << "\nQuestion: " << temp->data.question
                << "\nChoices: " << temp->data.choices << "\nAnswer: " << temp->data.answer
                << "\nMarks: " << temp->data.marks << "\n\n";
            temp = temp->next;
        }
    }
};

void shuffleQuestions(LinkedList& list) {
    std::vector<Question> questions;
    Node* current = list.head;
    while (current != nullptr) {
        questions.push_back(current->data);
        current = current->next;
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(questions.begin(), questions.end(), g);

    current = list.head;
    for (auto& q : questions) {
        current->data = q;
        current = current->next;
    }
}

int main() {
    LinkedList unansweredDeck, answeredDeck, discardPile;
    std::string filename = "question.txt";
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return 1;
    }

    std::string line;
    while (getline(file, line)) {
        std::istringstream ss(line);
        Question q;
        getline(ss, q.id, ',');
        getline(ss, q.question, ',');
        getline(ss, q.choices, ',');
        getline(ss, q.answer, ',');
        ss >> q.marks;
        unansweredDeck.addNode(q);
    }

    file.close();
    shuffleQuestions(unansweredDeck);

    // User interaction
    char response;
    while (unansweredDeck.head != nullptr) {
        unansweredDeck.displayFirst();  // Show only the current question
        std::cout << "Do you want to answer this question? (y/n): ";
        std::cin >> response;

        if (response == 'y') {
            answeredDeck.addNode(unansweredDeck.removeFront());
        }
        else {
            discardPile.addNode(unansweredDeck.removeFront());
        }
    }

    char view;
    view = '9';
    while (view != '0') {
        std::cout << "Select what to do:\n1)View all Answered Questions\n2)View all Discarded Questions\n3)Most Recent Answered Card\n4)Most Recent Discarded Card\n5)Earliest Answered Card\n6)Earliest Discarded Card\n0) Exit\n";
        std::cin >> view;
        
        if (view == '1') {
            std::cout << "\nAnswered Questions:\n";
            answeredDeck.display();
        }
        else if (view == '2') {
            std::cout << "\nDiscarded Questions:\n";
            discardPile.display();
        }
        else if (view == '3') {
            std::cout << "\nMost Recent Answered Card:\n";
            answeredDeck.displayLast();
        }
        else if (view == '4') {
            std::cout << "\nMost Recent Discarded Card:\n";
            discardPile.displayLast();
        }
        else if (view == '5') {
            std::cout << "\nEarliest Answered Card:\n";
            answeredDeck.displayLast();
        }
        else if (view == '6') {
            std::cout << "\nEarliest Discarded Card:\n";
            discardPile.displayLast();
        }
        else if (view == '0') {
            break;
        }
        else {
            std::cout << "\nNot a valid Choice\n\n";
        }
    }

    return 0;
}
