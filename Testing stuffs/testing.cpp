#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <random>
#include <limits>
#include <vector>

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
        }
        else {
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

    Player* getPlayer(int index) {
        Player* temp = pHead;
        for (int i = 0; i < index; ++i) {
            if (temp == nullptr) {
                throw std::runtime_error("Index out of bounds");
            }
            temp = temp->next;
        }
        return temp;
    }

    int getPlayerCount() const {
        int count = 0;
        Player* temp = pHead;
        while (temp != nullptr) {
            count++;
            temp = temp->next;
        }
        return count;
    }
};

class Stack {
public:
    struct StackNode {
        Question data;
        StackNode* next;
    };

    StackNode* top;

    Stack() : top(nullptr) {}

    ~Stack() {
        while (top) {
            StackNode* temp = top;
            top = top->next;
            delete temp;
        }
    }

    void push(Question q) {
        StackNode* newNode = new StackNode{ q, top };
        top = newNode;
    }

    Question pop() {
        if (top == nullptr) {
            throw std::runtime_error("Stack is empty");
        }
        StackNode* temp = top;
        Question q = temp->data;
        top = top->next;
        delete temp;
        return q;
    }

    Question peek() const {
        if (top == nullptr) {
            throw std::runtime_error("Stack is empty");
        }
        return top->data;
    }

    bool isEmpty() const {
        return top == nullptr;
    }

    int getCount() const {
        int count = 0;
        StackNode* temp = top;
        while (temp != nullptr) {
            count++;
            temp = temp->next;
        }
        return count;
    }
};

class Queue {
public:
    struct QueueNode {
        Question data;
        QueueNode* next;
    };

    QueueNode* front;
    QueueNode* rear;

    Queue() : front(nullptr), rear(nullptr) {}

    ~Queue() {
        while (front) {
            QueueNode* temp = front;
            front = front->next;
            delete temp;
        }
    }

    void enqueue(Question q) {
        QueueNode* newNode = new QueueNode{ q, nullptr };
        if (rear == nullptr) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
    }

    Question dequeue() {
        if (front == nullptr) {
            throw std::runtime_error("Queue is empty");
        }
        QueueNode* temp = front;
        Question q = temp->data;
        front = front->next;
        if (front == nullptr) {
            rear = nullptr;
        }
        delete temp;
        return q;
    }

    Question peek() const {
        if (front == nullptr) {
            throw std::runtime_error("Queue is empty");
        }
        return front->data;
    }

    bool isEmpty() const {
        return front == nullptr;
    }

    int getCount() const {
        int count = 0;
        QueueNode* temp = front;
        while (temp != nullptr) {
            count++;
            temp = temp->next;
        }
        return count;
    }
};

class FileHandler {
public:
    static void loadQuestions(const std::string& filename, Queue& queue) {
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
            }
            catch (const std::invalid_argument& e) {
                std::cerr << "Invalid marks format for question ID: " << q.id << std::endl;
                continue;
            }
            catch (const std::out_of_range& e) {
                std::cerr << "Marks value out of range for question ID: " << q.id << std::endl;
                continue;
            }

            queue.enqueue(q);
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

void shuffleQuestions(Queue& queue) {
    int count = queue.getCount();
    Question* questions = new Question[count];

    for (int i = 0; i < count; ++i) {
        questions[i] = queue.dequeue();
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(questions, questions + count, g);

    for (int i = 0; i < count; ++i) {
        queue.enqueue(questions[i]);
    }

    delete[] questions;
}

void displayPage(const vector<Player*>& players, int page) {
    const int pageSize = 10;
    int start = page * pageSize;
    int end = min(start + pageSize, static_cast<int>(players.size()));

    cout << left << setw(15) << "Player ID" << setw(25) << "Name" << setw(15) << "Round 1"
        << setw(15) << "Round 2" << setw(15) << "Round 3" << setw(15) << "Status" << setw(10) << "Marks" << endl;
    cout << string(110, '-') << endl;

    for (int i = start; i < end; ++i) {
        const Player* player = players[i];
        stringstream ss(player->questionIds);
        string round1, round2, round3;
        getline(ss, round1, '-');
        getline(ss, round2, '-');
        getline(ss, round3, '-');

        cout << setw(15) << player->id << setw(25) << player->name << setw(15) << round1
            << setw(15) << round2 << setw(15) << round3 << setw(15) << player->cardCategories << setw(10) << player->totalScore << endl;
    }
}

void quicksort(vector<Player*>& players, int low, int high) {
    if (low < high) {
        int pivotIndex = low + (high - low) / 2;
        Player* pivotValue = players[pivotIndex];
        swap(players[pivotIndex], players[high]);
        int storeIndex = low;

        for (int i = low; i < high; ++i) {
            if (players[i]->totalScore > pivotValue->totalScore) {
                swap(players[i], players[storeIndex]);
                ++storeIndex;
            }
        }
        swap(players[storeIndex], players[high]);

        quicksort(players, low, storeIndex - 1);
        quicksort(players, storeIndex + 1, high);
    }
}

struct BSTNode {
    Player* player;
    BSTNode* left;
    BSTNode* right;

    BSTNode(Player* p) : player(p), left(nullptr), right(nullptr) {}
};

class BST {
public:
    BSTNode* root;

    BST() : root(nullptr) {}

    ~BST() {
        clear(root);
    }

    void clear(BSTNode* node) {
        if (node) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

    void insert(Player* player) {
        root = insert(root, player);
    }

    BSTNode* insert(BSTNode* node, Player* player) {
        if (node == nullptr) {
            return new BSTNode(player);
        }
        if (player->totalScore < node->player->totalScore) {
            node->left = insert(node->left, player);
        } else {
            node->right = insert(node->right, player);
        }
        return node;
    }

    void inorder(vector<Player*>& players) {
        inorder(root, players);
    }

    void inorder(BSTNode* node, vector<Player*>& players) {
        if (node) {
            inorder(node->left, players);
            players.push_back(node->player);
            inorder(node->right, players);
        }
    }
};

void leaderboardMenu(LinkedList& playerList) {
    int page = 0;
    vector<Player*> players;
    Player* current = playerList.pHead;

    while (current) {
        players.push_back(current);
        current = current->next;
    }

    // Initially display the first 10 players unsorted
    displayPage(players, page);

    char choice;
    do {
        cout << "Leaderboard Menu:\n";
        cout << "1. Sort by Highest\n";
        cout << "2. Sort by Lowest\n";
        cout << "3. Exit\n";
        cout << "4. Page Down\n";
        cout << "5. Page Up\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case '1':
            quicksort(players, 0, players.size() - 1);
            page = 0; // Reset to the first page after sorting
            displayPage(players, page);
            break;
        case '2': {
            BST bst;
            for (Player* player : players) {
                bst.insert(player);
            }
            players.clear();
            bst.inorder(players);
            page = 0; // Reset to the first page after sorting
            displayPage(players, page);
            break;
        }
        case '3':
            cout << "Exiting Leaderboard...\n";
            break;
        case '4':
            if ((page + 1) * 10 < players.size()) {
                page++;
                displayPage(players, page);
            } else {
                cout << "Already on the last page.\n";
            }
            break;
        case '5':
            if (page > 0) {
                page--;
                displayPage(players, page);
            } else {
                cout << "Already on the first page.\n";
            }
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != '3');
}

void showChart() {
    std::cout << "Displaying Chart...\n";
    // Implementation for displaying charts
}

void handleDiscardPileSelection(Stack& discardPile, LinkedList& answeredDeck, string& questionIds, string& cardCategories, int& totalScore, bool& returned) {
    if (discardPile.isEmpty()) {
        std::cout << "The discard pile is empty. Please choose another option.\n";
        returned = false;
        return;
    }

    // Get the top card from the discard pile
    Question selectedQuestion = discardPile.pop();

    std::string answer;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer
    std::cout << "Question: " << selectedQuestion.question << "\nChoices: " << selectedQuestion.choices << "\nEnter your answer (type 'SKIP' to skip or 'exit' to return to menu): ";
    std::getline(std::cin, answer);

    // Convert answer and correct answer to lowercase for comparison
    std::transform(answer.begin(), answer.end(), answer.begin(), ::tolower);
    std::string correctAnswer = selectedQuestion.answer;
    std::transform(correctAnswer.begin(), correctAnswer.end(), correctAnswer.begin(), ::tolower);

    if (answer == "skip" || answer == "exit") {
        // Player chose to skip the question or exit
        if (answer == "skip") {
            std::cout << "You skipped this round.\n";
        } else {
            std::cout << "Returning to the card selection menu.\n";
        }
        discardPile.push(selectedQuestion); // Add the skipped question back to the discard pile
        if (answer == "skip") {
            if (!cardCategories.empty()) {
                cardCategories.append("-");
            }
            cardCategories.append("W"); // Mark it as a wrong card since it was skipped
        }
        returned = (answer == "exit");
    } else {
        if (selectedQuestion.answer.length() > 1) { // Assuming fill-in-the-blank answers have length > 1
            // Calculate partial correctness for fill-in-the-blank questions
            int correctCount = 0;
            int minLength = std::min(answer.length(), correctAnswer.length());
            for (int i = 0; i < minLength; ++i) {
                if (answer[i] == correctAnswer[i]) {
                    ++correctCount;
                }
            }

            int earnedMarks = (0.8 * correctCount * selectedQuestion.marks) / correctAnswer.length();
            std::cout << "You got " << correctCount << " out of " << correctAnswer.length() << " correct. You earned " << earnedMarks << " points.\n";
            totalScore += earnedMarks;
            if (correctCount > 0) {
                answeredDeck.addNodeToFront(selectedQuestion); // Add to answered deck if correct
                if (!cardCategories.empty()) {
                    cardCategories.append("-");
                }
                cardCategories.append("P"); // Mark it as a correct card
            } else {
                discardPile.push(selectedQuestion); // Add to discard pile if incorrect
                if (!cardCategories.empty()) {
                    cardCategories.append("-");
                }
                cardCategories.append("W"); // Mark it as a wrong card
            }
        } else {
            // Handle non-fill-in-the-blank questions
            if (answer == correctAnswer) {
                int earnedMarks = static_cast<int>(selectedQuestion.marks * 0.8); // Earn fewer points for discarded pile
                std::cout << "Correct! You earned " << earnedMarks << " points.\n";
                totalScore += earnedMarks;
                answeredDeck.addNodeToFront(selectedQuestion); // Add to answered deck if correct
                if (!cardCategories.empty()) {
                    cardCategories.append("-");
                }
                cardCategories.append("P"); // Mark it as a correct card
            } else {
                std::cout << "Incorrect. The correct answer is: " << selectedQuestion.answer << "\n";
                discardPile.push(selectedQuestion); // Add to discard pile if incorrect
                if (!cardCategories.empty()) {
                    cardCategories.append("-");
                }
                cardCategories.append("W"); // Mark it as a wrong card
            }
        }

        if (!questionIds.empty()) {
            questionIds.append("-");
        }
        questionIds.append(selectedQuestion.id); // Append the question ID to the player's list
        returned = false;
    }
}

void handleUnansweredPileSelection(Queue& unansweredDeck, LinkedList& answeredDeck, Stack& discardPile, string& questionIds, string& cardCategories, int& totalScore) {
    Question currentQuestion = unansweredDeck.dequeue();
    std::string answer;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer
    std::cout << "Question: " << currentQuestion.question << "\nChoices: " << currentQuestion.choices << "\nEnter your answer (type 'SKIP' to skip): ";
    std::getline(std::cin, answer);

    // Convert answer and correct answer to lowercase for comparison
    std::transform(answer.begin(), answer.end(), answer.begin(), ::tolower);
    std::string correctAnswer = currentQuestion.answer;
    std::transform(correctAnswer.begin(), correctAnswer.end(), correctAnswer.begin(), ::tolower);

    if (answer == "skip") {
        // Player chose to skip the question
        std::cout << "You skipped this round.\n";
        discardPile.push(currentQuestion); // Add the skipped question to the discard pile
        if (!cardCategories.empty()) {
            cardCategories.append("-");
        }
        cardCategories.append("W"); // Mark it as a wrong card since it was skipped
    } else {
        if (currentQuestion.answer.length() > 1) { // Assuming fill-in-the-blank answers have length > 1
            // Calculate partial correctness for fill-in-the-blank questions
            int correctCount = 0;
            int minLength = std::min(answer.length(), correctAnswer.length());
            for (int i = 0; i < minLength; ++i) {
                if (answer[i] == correctAnswer[i]) {
                    ++correctCount;
                }
            }

            int earnedMarks = (correctCount * currentQuestion.marks) / correctAnswer.length();
            std::cout << "You got " << correctCount << " out of " << correctAnswer.length() << " correct. You earned " << earnedMarks << " points.\n";
            totalScore += earnedMarks;
            if (correctCount > 0) {
                answeredDeck.addNodeToFront(currentQuestion); // Add to answered deck if correct
                if (!cardCategories.empty()) {
                    cardCategories.append("-");
                }
                cardCategories.append("F"); // Mark it as a correct card
            } else {
                discardPile.push(currentQuestion); // Add to discard pile if incorrect
                if (!cardCategories.empty()) {
                    cardCategories.append("-");
                }
                cardCategories.append("W"); // Mark it as a wrong card
            }
        } else {
            // Handle non-fill-in-the-blank questions
            if (answer == correctAnswer) {
                std::cout << "Correct! You earned " << currentQuestion.marks << " points.\n";
                totalScore += currentQuestion.marks;
                answeredDeck.addNodeToFront(currentQuestion); // Add to answered deck if correct
                if (!cardCategories.empty()) {
                    cardCategories.append("-");
                }
                cardCategories.append("F"); // Mark it as a correct card
            } else {
                std::cout << "Incorrect. The correct answer is: " << currentQuestion.answer << "\n";
                discardPile.push(currentQuestion); // Add to discard pile if incorrect
                if (!cardCategories.empty()) {
                    cardCategories.append("-");
                }
                cardCategories.append("W"); // Mark it as a wrong card
            }
        }
    }

    if (!questionIds.empty()) {
        questionIds.append("-");
    }
    questionIds.append(currentQuestion.id); // Append the question ID to the player's list
}

bool isNameExists(const LinkedList& playerList, const std::string& name) {
    std::string lowerName = name;
    std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
    Player* temp = playerList.pHead;
    while (temp != nullptr) {
        std::string playerName = temp->name;
        std::transform(playerName.begin(), playerName.end(), playerName.begin(), ::tolower);
        if (playerName == lowerName) {
            return true;
        }
        temp = temp->next;
    }
    return false;
}

void handleUserInteraction(Queue& unansweredDeck, LinkedList& answeredDeck, Stack& discardPile, LinkedList& playerList) {
    std::string name;
    bool nameExists;

    do {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer
        std::cout << "Enter your name: ";
        std::getline(std::cin, name);

        nameExists = isNameExists(playerList, name);
        if (nameExists) {
            std::cout << "Name already exists. Please choose a different name.\n";
        }
    } while (nameExists);

    std::string questionIds, cardCategories;
    int totalScore = 0;

    for (int i = 0; i < 3; ) {
        bool validChoice = false;
        bool returned = false;
        while (!validChoice) {
            char choice;
            std::cout << "Round " << i + 1 << "\nChoose a deck (d: discard pile, u: unanswered pile, s: skip): ";
            std::cin >> choice;

            if (choice == 'd') {
                handleDiscardPileSelection(discardPile, answeredDeck, questionIds, cardCategories, totalScore, returned);
                if (!returned) {
                    validChoice = true;
                }
            }
            else if (choice == 'u') {
                handleUnansweredPileSelection(unansweredDeck, answeredDeck, discardPile, questionIds, cardCategories, totalScore);
                validChoice = true;
            }
            else if (choice == 's') {
                std::cout << "You skipped this round.\n";
                validChoice = true;
            }
            else {
                std::cout << "Invalid choice. Please try again.\n";
            }
        }

        if (validChoice && !returned) {
            i++;
        }
    }

    // Add the player to the player list with question IDs and F/P indicators
    playerList.addPlayer("ID", name, questionIds, cardCategories, totalScore);
}

void startGame(Queue& unansweredDeck, LinkedList& answeredDeck, Stack& discardPile, LinkedList& playerList) {
    std::cout << "Starting Game...\n";
    handleUserInteraction(unansweredDeck, answeredDeck, discardPile, playerList);
}

void mainMenu(Queue& unansweredDeck, LinkedList& answeredDeck, Stack& discardPile, LinkedList& playerList) {
    char choice;
    do {
        std::cout << "Main Menu:\n";
        std::cout << "1. Game\n";
        std::cout << "2. Leaderboard\n";
        std::cout << "3. Additional\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case '1':
            startGame(unansweredDeck, answeredDeck, discardPile, playerList);
            break;
        case '2':
            leaderboardMenu(playerList);
            break;
        case '3':
            // Additional functionalities can be added here
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
    Queue unansweredDeck;
    LinkedList answeredDeck, playerList;
    Stack discardPile;
    std::string questionFilename = "question.txt";
    std::string playerFilename = "player.txt";

    try {
        FileHandler::loadQuestions(questionFilename, unansweredDeck);
        std::cout << "Questions loaded successfully.\n";
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Error loading questions: " << e.what() << std::endl;
        return 1;
    }

    // Shuffle the questions to ensure randomness
    shuffleQuestions(unansweredDeck);

    try {
        FileHandler::loadPlayers(playerFilename, playerList);
        std::cout << "Players loaded successfully.\n";
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Error loading players: " << e.what() << std::endl;
        return 1;
    }

    // Move 12 questions from unansweredDeck to discardPile
    for (int i = 0; i < 12; ++i) {
        discardPile.push(unansweredDeck.dequeue());
    }

    mainMenu(unansweredDeck, answeredDeck, discardPile, playerList);

    return 0;
}
