#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <random>
#include <limits>
#include <ctime>
#include <cctype>

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
    string questionIds;
    string cardCategories;
    int totalScore;
    Player* next;

    Player(const string& pid, const string& nm, const string& qIds, const string& cCategories, int score)
        : id(pid), name(nm), questionIds(qIds), cardCategories(cCategories), totalScore(score), next(nullptr) {}
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
        } else {
            Node* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

    void addNodeToFront(Question q) {
        Node* newNode = new Node{ q, head };
        head = newNode;
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

    void removeNode(int index) {
        if (head == nullptr) {
            throw std::runtime_error("List is empty");
        }
        if (index == 0) {
            Node* temp = head;
            head = head->next;
            delete temp;
            return;
        }
        Node* temp = head;
        for (int i = 0; i < index - 1; ++i) {
            if (temp->next == nullptr) {
                throw std::runtime_error("Index out of bounds");
            }
            temp = temp->next;
        }
        Node* toDelete = temp->next;
        temp->next = toDelete->next;
        delete toDelete;
    }

    Question getNodeData(int index) {
        if (head == nullptr) {
            throw std::runtime_error("List is empty");
        }
        Node* temp = head;
        for (int i = 0; i < index; ++i) {
            if (temp->next == nullptr) {
                throw std::runtime_error("Index out of bounds");
            }
            temp = temp->next;
        }
        return temp->data;
    }

    int getCount() const {
        int count = 0;
        Node* temp = head;
        while (temp != nullptr) {
            count++;
            temp = temp->next;
        }
        return count;
    }

    void display() const {
        Node* temp = head;
        while (temp != nullptr) {
            std::cout << "ID: " << temp->data.id << "\nQuestion: " << temp->data.question
                << "\nChoices: " << temp->data.choices << "\nAnswer: " << temp->data.answer
                << "\nMarks: " << temp->data.marks << "\n\n";
            temp = temp->next;
        }
    }

    void addPlayer(const string& id, const string& name, const string& questionIds, const string& cardCategories, int totalScore) {
        Player* newPlayer = new Player(id, name, questionIds, cardCategories, totalScore);
        newPlayer->next = pHead;
        pHead = newPlayer;
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
            string id, name;
            getline(ss, id, ',');
            getline(ss, name, ',');

            list.addPlayer(id, name, "", "", 0);
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

void displayDiscardPile(const LinkedList& discardPile) {
    discardPile.display();
}

void displayLeaderboard(const LinkedList& list) {
    const Player* player = list.pHead;
    cout << left << setw(15) << "Player ID" << setw(25) << "Name"
         << setw(30) << "Question IDs" << setw(20) << "Card Categories" << setw(10) << "Score" << endl;
    cout << string(100, '-') << endl;

    while (player != nullptr) {
        cout << setw(15) << player->id << setw(25) << player->name
             << setw(30) << player->questionIds << setw(20) << player->cardCategories << setw(10) << player->totalScore << endl;
        player = player->next;
    }
}

void showChart() {
    std::cout << "Displaying Chart...\n";
    // Implementation for displaying charts
}

void askQuestion(LinkedList& questionDeck, LinkedList& answeredDeck, LinkedList& discardPile, bool isDiscarded, string& questionIds, string& cardCategories, int& totalScore) {
    Question currentQuestion;
    int questionIndex = 0;

    if (isDiscarded) {
        currentQuestion = discardPile.removeFront();
    } else {
        currentQuestion = questionDeck.removeFront();
    }

    std::string answer;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer
    std::cout << "Question: " << currentQuestion.question << "\nChoices: " << currentQuestion.choices << "\nEnter your answer: ";
    std::getline(std::cin, answer);

    if (answer == currentQuestion.answer) {
        int earnedMarks = isDiscarded ? static_cast<int>(currentQuestion.marks * 0.8) : currentQuestion.marks;
        std::cout << "Correct! You earned " << earnedMarks << " points.\n";
        totalScore += earnedMarks;
        answeredDeck.addNodeToFront(currentQuestion);
        if (!cardCategories.empty()) {
            cardCategories.append("-");
        }
        cardCategories.append(isDiscarded ? "P" : "F");
    } else {
        std::cout << "Incorrect. The correct answer is: " << currentQuestion.answer << "\n";
        discardPile.addNode(currentQuestion);
        if (!cardCategories.empty()) {
            cardCategories.append("-");
        }
        cardCategories.append("W");
    }

    if (!questionIds.empty()) {
        questionIds.append("-");
    }
    questionIds.append(currentQuestion.id);
}

void handleDiscardPileSelection(LinkedList& discardPile, LinkedList& answeredDeck, string& questionIds, string& cardCategories, int& totalScore) {
    displayDiscardPile(discardPile);
    std::cout << "Pick a question or (0) to go back: ";
    std::string selectionStr;
    std::cin >> selectionStr;

    // Validate input
    if (!all_of(selectionStr.begin(), selectionStr.end(), ::isdigit)) {
        std::cout << "Invalid selection. Please try again.\n";
        handleDiscardPileSelection(discardPile, answeredDeck, questionIds, cardCategories, totalScore);
        return;
    }

    int selection = std::stoi(selectionStr);

    if (selection == 0) {
        return;  // Go back to the main round choice
    } else if (selection > 0 && selection <= discardPile.getCount()) {
        // Remove the selected question from the discard pile
        Question selectedQuestion = discardPile.getNodeData(selection - 1);
        discardPile.removeNode(selection - 1);
        // Ask the question
        askQuestion(discardPile, answeredDeck, discardPile, true, questionIds, cardCategories, totalScore);
    } else {
        std::cout << "Invalid selection. Please try again.\n";
        handleDiscardPileSelection(discardPile, answeredDeck, questionIds, cardCategories, totalScore);
    }
}

void handleUnansweredPileSelection(LinkedList& unansweredDeck, LinkedList& answeredDeck, LinkedList& discardPile, string& questionIds, string& cardCategories, int& totalScore) {
    Question currentQuestion = unansweredDeck.removeFront();
    std::string answer;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer
    std::cout << "Question: " << currentQuestion.question << "\nChoices: " << currentQuestion.choices << "\nEnter your answer (type 'SKIP' to skip): ";
    std::getline(std::cin, answer);

    if (answer == "SKIP") {
        std::cout << "You skipped this round.\n";
        discardPile.addNode(currentQuestion);
        if (!cardCategories.empty()) {
            cardCategories.append("-");
        }
        cardCategories.append("W");
    } else {
        if (answer == currentQuestion.answer) {
            std::cout << "Correct! You earned " << currentQuestion.marks << " points.\n";
            totalScore += currentQuestion.marks;
            answeredDeck.addNodeToFront(currentQuestion);
            if (!cardCategories.empty()) {
                cardCategories.append("-");
            }
            cardCategories.append("F");
        } else {
            std::cout << "Incorrect. The correct answer is: " << currentQuestion.answer << "\n";
            discardPile.addNode(currentQuestion);
            if (!cardCategories.empty()) {
                cardCategories.append("-");
            }
            cardCategories.append("W");
        }
    }

    if (!questionIds.empty()) {
        questionIds.append("-");
    }
    questionIds.append(currentQuestion.id);
}

void handleUserInteraction(LinkedList& unansweredDeck, LinkedList& answeredDeck, LinkedList& discardPile, LinkedList& playerList) {
    std::string name;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer
    std::cout << "Enter your name: ";
    std::getline(std::cin, name);

    std::string questionIds, cardCategories;
    int totalScore = 0;

    for (int i = 0; i < 3; ++i) {
        bool validChoice = false;
        while (!validChoice) {
            char choice;
            std::cout << "Round " << i + 1 << "\nChoose a deck (d: discard pile, u: unanswered pile, s: skip): ";
            std::cin >> choice;

            if (choice == 'd') {
                handleDiscardPileSelection(discardPile, answeredDeck, questionIds, cardCategories, totalScore);
                validChoice = true;
            } else if (choice == 'u') {
                handleUnansweredPileSelection(unansweredDeck, answeredDeck, discardPile, questionIds, cardCategories, totalScore);
                validChoice = true;
            } else if (choice == 's') {
                std::cout << "You skipped this round.\n";
                validChoice = true;
            } else {
                std::cout << "Invalid choice. Please try again.\n";
            }
        }
    }

    // Add the player to the player list with question IDs and F/P indicators
    playerList.addPlayer("ID", name, questionIds, cardCategories, totalScore);
}

void startGame(LinkedList& unansweredDeck, LinkedList& answeredDeck, LinkedList& discardPile, LinkedList& playerList) {
    std::cout << "Starting Game...\n";
    handleUserInteraction(unansweredDeck, answeredDeck, discardPile, playerList);
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
                startGame(unansweredDeck, answeredDeck, discardPile, playerList);
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

    // Move 12 questions from unansweredDeck to discardPile
    for (int i = 0; i < 12; ++i) {
        discardPile.addNode(unansweredDeck.removeFront());
    }

    mainMenu(unansweredDeck, answeredDeck, discardPile, playerList);

    return 0;
}