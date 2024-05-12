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

class FileHandler {
public:
    static void loadQuestions(const std::string& filename, LinkedList& list) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
            throw std::runtime_error("Failed to open file.");
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
            list.addNode(q);
        }
        file.close();
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

void handleUserInteraction(LinkedList& unansweredDeck, LinkedList& answeredDeck, LinkedList& discardPile) {
    char response;
    while (unansweredDeck.head != nullptr) {
        unansweredDeck.displayFirst();  // Show only the current question
        std::cout << "Do you want to answer this question? (y/n): ";
        std::cin >> response;

        if (response == 'y') {
            answeredDeck.addNode(unansweredDeck.removeFront());
        } else {
            discardPile.addNode(unansweredDeck.removeFront());
        }
    }
}

void handleMenuOptions(LinkedList& answeredDeck, LinkedList& discardPile) {
    char view = '9';
    while (view != '0') {
        std::cout << "Select what to do:\n1)View all Answered Questions\n2)View all Discarded Questions\n3)Most Recent Answered Card\n4)Most Recent Discarded Card\n5)Earliest Answered Card\n6)Earliest Discarded Card\n0) Exit\n";
        std::cin >> view;

        switch(view) {
            case '1':
                std::cout << "\nAnswered Questions:\n";
                answeredDeck.display();
                break;
            case '2':
                std::cout << "\nDiscarded Questions:\n";
                discardPile.display();
                break;
            case '3':
                std::cout << "\nMost Recent Answered Card:\n";
                answeredDeck.displayLast();
                break;
            case '4':
                std::cout << "\nMost Recent Discarded Card:\n";
                discardPile.displayLast();
                break;
            case '5':
                std::cout << "\nEarliest Answered Card:\n";
                answeredDeck.displayLast();
                break;
            case '6':
                std::cout << "\nEarliest Discarded Card:\n";
                discardPile.displayLast();
                break;
            case '0':
                break;
            default:
                std::cout << "\nNot a valid Choice\n\n";
        }
    }
}

void showLeaderboard() {
    std::cout << "Displaying Leaderboard...\n";
    // Implementation for displaying leaderboard
}

void showChart() {
    std::cout << "Displaying Chart...\n";
    // Implementation for displaying charts
}

void startGame(LinkedList& unansweredDeck, LinkedList& answeredDeck, LinkedList& discardPile) {
    std::cout << "Starting Game...\n";
    shuffleQuestions(unansweredDeck);
    handleUserInteraction(unansweredDeck, answeredDeck, discardPile);
    handleMenuOptions(answeredDeck, discardPile);
}

void mainMenu(LinkedList& unansweredDeck, LinkedList& answeredDeck, LinkedList& discardPile) {
    char choice;
    do {
        std::cout << "Main Menu:\n";
        std::cout << "1. Leaderboard\n";
        std::cout << "2. Chart\n";
        std::cout << "3. Game\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case '1':
                showLeaderboard();
                break;
            case '2':
                showChart();
                break;
            case '3':
                startGame(unansweredDeck, answeredDeck, discardPile);
                break;
            case '0':
                std::cout << "Exiting...\n";
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != '0');
}



int main() {
    LinkedList unansweredDeck, answeredDeck, discardPile;
    std::string filename = "question.txt";

    try {
        FileHandler::loadQuestions(filename, unansweredDeck);
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    mainMenu(unansweredDeck, answeredDeck, discardPile);

    return 0;
}


