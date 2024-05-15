#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <random>
#include <ctime>
using namespace std;

struct Question {
    string id;
    string question;
    string choices;
    string answer;
    int marks;
};

struct Player {
    string id;
    string name;
    int scores[3];  // Assuming there are always three rounds
    string status;
    Player* next;

    Player(const string& pid, const string& nm, const string& sts) : id(pid), name(nm), status(sts), next(nullptr) {
        scores[0] = scores[1] = scores[2] = 0;  // Initialize scores to zero
    }
};

struct Node {
    Question data;
    Node* next;
};

class LinkedList {
public:
    Node* head;
    Player* pHead;

    LinkedList() : head(nullptr), pHead(nullptr) {}

    ~LinkedList() {
        Node* current = head;
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
        head = nullptr;
        
        while (pHead) {
            Player* temp = pHead;
            pHead = pHead->next;
            delete temp;
        }
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
        if (head == nullptr) {
            std::cout << "List is empty\n";
            return;
        }
        Node* temp = head;
        std::cout << "ID: " << temp->data.id << "\nQuestion: " << temp->data.question
            << "\nChoices: " << temp->data.choices << "\nAnswer: " << temp->data.answer
            << "\nMarks: " << temp->data.marks << "\n\n";
    }

    void displayLast() {
        if (head == nullptr) {
            std::cout << "List is empty\n";
            return;
        }
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

    void addPlayer(const string& id, const string& name, const string& status) {
        Player* newPlayer = new Player(id, name, status);
        newPlayer->next = pHead;
        pHead = newPlayer;
    }

    void assignScoresToPlayer(const string& playerId, int roundIndex, int score) {
        Player* current = pHead;
        while (current != nullptr) {
            if (current->id == playerId) {
                current->scores[roundIndex] = score;
                break;
            }
            current = current->next;
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

    static void loadPlayers(const string& filename, LinkedList& list) {
        ifstream file(filename);
        string line, id, name, status;
        int scores[3]; // Temporary storage for scores

        while (getline(file, line)) {
            stringstream ss(line);
            getline(ss, id, ',');
            getline(ss, name, ',');
            for (int i = 0; i < 3; ++i) {
                getline(ss, line, ','); scores[i] = stoi(line);
            }
            getline(ss, status, ',');
            list.addPlayer(id, name, status);
            for (int i = 0; i < 3; ++i) {
                list.assignScoresToPlayer(id, i, scores[i]);
            }
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
        std::cout << "Select what to do:\n1) View all Answered Questions\n2) View all Discarded Questions\n3) Most Recent Answered Card\n4) Most Recent Discarded Card\n5) Earliest Answered Card\n6) Earliest Discarded Card\n0) Exit\n";
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
                answeredDeck.displayFirst();
                break;
            case '6':
                std::cout << "\nEarliest Discarded Card:\n";
                discardPile.displayFirst();
                break;
            case '0':
                break;
            default:
                std::cout << "\nNot a valid Choice\n\n";
        }
    }
}

void displayLeaderboard(const LinkedList& list) {
    const Player* player = list.pHead;
    cout << left << setw(15) << "Player ID" << setw(25) << "Name"
         << setw(10) << "Round 1" << setw(10) << "Round 2"
         << setw(10) << "Round 3" << setw(15) << "Status" << endl;
    cout << string(85, '-') << endl;

    while (player != nullptr) {
        cout << setw(15) << player->id << setw(25) << player->name
             << setw(10) << player->scores[0] << setw(10) << player->scores[1]
             << setw(10) << player->scores[2] << setw(15) << player->status << endl;
        player = player->next;
    }
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

void mainMenu(LinkedList& unansweredDeck, LinkedList& answeredDeck, LinkedList& discardPile, LinkedList& playerList) {
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
                displayLeaderboard(playerList);
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
    LinkedList unansweredDeck, answeredDeck, discardPile, playerList;
    std::string questionFilename = "question.txt";
    std::string playerFilename = "player.txt";

    try {
        FileHandler::loadQuestions(questionFilename, unansweredDeck);
        FileHandler::loadPlayers(playerFilename, playerList);
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    mainMenu(unansweredDeck, answeredDeck, discardPile, playerList);

    return 0;
}