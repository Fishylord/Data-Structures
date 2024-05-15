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
    string* questionIds; // Dynamically allocated array of question IDs
    int questionCount; // Number of question IDs
    int totalScore;
    string cardCategory;
    Player* next;

    Player(const string& pid, const string& nm)
        : id(pid), name(nm), questionIds(nullptr), questionCount(0), totalScore(0), cardCategory(""), next(nullptr) {}
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
            delete[] temp->questionIds; // Free the dynamically allocated question IDs array
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

    void addPlayer(const string& id, const string& name) {
        Player* newPlayer = new Player(id, name);
        newPlayer->next = pHead;
        pHead = newPlayer;
    }

    void assignScoresToPlayer(const string& playerId, int totalScore, string* questionIds, int questionCount, const string& cardCategory) {
        Player* current = pHead;
        while (current != nullptr) {
            if (current->id == playerId) {
                current->totalScore = totalScore;
                current->questionIds = questionIds;
                current->questionCount = questionCount;
                current->cardCategory = cardCategory;
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

            std::string marksStr;
            getline(ss, marksStr, ',');
            try {
                q.marks = std::stoi(marksStr);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid marks format for question ID: " << q.id << std::endl;
                continue;
            } catch (const std::out_of_range& e) {
                std::cerr << "Marks value out of range for question ID: " << q.id << std::endl;
                continue;
            }

            list.addNode(q);
        }
        file.close();
    }

    static void loadPlayers(const string& filename, LinkedList& list) {
        ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
            throw std::runtime_error("Failed to open file.");
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string id, name, questionIdsStr, totalScoreStr, cardCategory;
            getline(ss, id, ',');
            getline(ss, name, ',');

            // Skip question IDs for now
            getline(ss, questionIdsStr, ',');

            getline(ss, totalScoreStr, ',');
            int totalScore = 0;
            if (!totalScoreStr.empty()) {
                try {
                    totalScore = stoi(totalScoreStr);
                } catch (const std::invalid_argument&) {
                    std::cerr << "Invalid score format for player ID: " << id << std::endl;
                    continue;
                }
            }

            getline(ss, cardCategory, ',');

            // Parse question IDs
            int questionCount = 0;
            stringstream qss(questionIdsStr);
            string questionId;
            while (getline(qss, questionId, '-')) {
                questionCount++;
            }

            string* questionIds = nullptr;
            if (questionCount > 0) {
                questionIds = new string[questionCount];
                qss.clear();
                qss.seekg(0, ios::beg);
                int index = 0;
                while (getline(qss, questionId, '-')) {
                    questionIds[index++] = questionId;
                }
            }

            list.addPlayer(id, name);
            list.assignScoresToPlayer(id, totalScore, questionIds, questionCount, cardCategory);
        }
        file.close();
    }
};

void shuffleQuestions(LinkedList& list) {
    int count = 0;
    Node* current = list.head;
    while (current != nullptr) {
        count++;
        current = current->next;
    }

    Question* questions = new Question[count];
    current = list.head;
    for (int i = 0; i < count; ++i) {
        questions[i] = current->data;
        current = current->next;
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(questions, questions + count, g);

    current = list.head;
    for (int i = 0; i < count; ++i) {
        current->data = questions[i];
        current = current->next;
    }

    delete[] questions;
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
         << setw(15) << "Question IDs" << setw(10) << "Score"
         << setw(15) << "Card Category" << endl;
    cout << string(85, '-') << endl;

    while (player != nullptr) {
        cout << setw(15) << player->id << setw(25) << player->name;
        for (int i = 0; i < player->questionCount; ++i) {
            cout << player->questionIds[i] << " ";
        }
        cout << setw(10) << player->totalScore << setw(15) << player->cardCategory << endl;
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
        std::cout << "Questions loaded successfully.\n";
    } catch (const std::runtime_error& e) {
        std::cerr << "Error loading questions: " << e.what() << std::endl;
        return 1;
    }

    try {
        FileHandler::loadPlayers(playerFilename, playerList);
        std::cout << "Players loaded successfully.\n";
    } catch (const std::runtime_error& e) {
        std::cerr << "Error loading players: " << e.what() << std::endl;
        return 1;
    }

    mainMenu(unansweredDeck, answeredDeck, discardPile, playerList);

    return 0;
}
