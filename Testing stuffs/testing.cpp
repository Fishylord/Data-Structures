#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <random>
#include <limits>

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
    string marks;
    int totalScore;
    Player* next;

    Player(const string& pid, const string& nm, const string& qIds, const string& mks, int score)
        : id(pid), name(nm), questionIds(qIds), marks(mks), totalScore(score), next(nullptr) {}
};

class LinkedList {
public:
    Player* pHead;
    int manualPlayCount;

    LinkedList() : pHead(nullptr), manualPlayCount(0) {}

    ~LinkedList() {
        while (pHead) {
            Player* temp = pHead;
            pHead = pHead->next;
            delete temp;
        }
    }

    void addPlayer(const string& id, const string& name, const string& questionIds, const string& marks, int totalScore) {
        Player* newPlayer = new Player(id, name, questionIds, marks, totalScore);
        newPlayer->next = pHead;
        pHead = newPlayer;
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

    string generatePlayerID() {
        if (manualPlayCount >= 10) {
            return "";
        }
        return "P0" + to_string(81 + manualPlayCount++);
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

class PlayerQueue {
public:
    struct PlayerQueueNode {
        Player* data;
        PlayerQueueNode* next;
    };

    PlayerQueueNode* front;
    PlayerQueueNode* rear;

    PlayerQueue() : front(nullptr), rear(nullptr) {}

    ~PlayerQueue() {
        while (front) {
            PlayerQueueNode* temp = front;
            front = front->next;
            delete temp;
        }
    }

    void enqueue(Player* p) {
        PlayerQueueNode* newNode = new PlayerQueueNode{ p, nullptr };
        if (rear == nullptr) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
    }

    Player* dequeue() {
        if (front == nullptr) {
            throw std::runtime_error("Queue is empty");
        }
        PlayerQueueNode* temp = front;
        Player* p = temp->data;
        front = front->next;
        if (front == nullptr) {
            rear = nullptr;
        }
        delete temp;
        return p;
    }

    bool isEmpty() const {
        return front == nullptr;
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

    static void loadPlayers(const string& filename, PlayerQueue& queue) {
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

            Player* newPlayer = new Player(id, name, "", "", 0);
            queue.enqueue(newPlayer);
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

void displayPage(Player* players, int page, bool sorted) {
    const int pageSize = 10;
    int start = page * pageSize;
    int end = start + pageSize;

    Player* temp = players;
    int index = 0;

    if (sorted) {
        cout << left << setw(10) << "Rank" << setw(20) << "Student"
             << setw(15) << "Round 1" << setw(15) << "Round 2" << setw(15) << "Round 3" << setw(15) << "Overall Score" << endl;
    } else {
        cout << left << setw(15) << "Player ID" << setw(20) << "Student"
             << setw(15) << "Round 1" << setw(15) << "Round 2" << setw(15) << "Round 3" << setw(15) << "Overall Score" << endl;
    }
    cout << string(100, '-') << endl;

    while (temp != nullptr && index < end) {
        if (index >= start) {
            stringstream ss(temp->questionIds);
            string qid1, qid2, qid3;
            getline(ss, qid1, '-');
            getline(ss, qid2, '-');
            getline(ss, qid3, '-');

            stringstream mks(temp->marks);
            string mk1, mk2, mk3;
            getline(mks, mk1, '-');
            getline(mks, mk2, '-');
            getline(mks, mk3, '-');

            if (sorted) {
                cout << setw(10) << (index + 1) << setw(20) << temp->name
                     << setw(15) << qid1 << setw(15) << qid2 << setw(15) << qid3 << setw(15) << temp->totalScore << endl;
            } else {
                cout << setw(15) << temp->id << setw(20) << temp->name
                     << setw(15) << qid1 << setw(15) << qid2 << setw(15) << qid3 << setw(15) << temp->totalScore << endl;
            }

            if (sorted) {
            cout << setw(10) << "" << setw(20) << ""
                 << setw(15) << mk1 << setw(15) << mk2 << setw(15) << mk3 << endl;
            } else {     
            cout << setw(15) << "" << setw(20) << ""
                 << setw(15) << mk1 << setw(15) << mk2 << setw(15) << mk3 << endl;
            }
        }
        index++;
        temp = temp->next;
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

    void inorder(Player*& head) {
        inorder(root, head);
    }

    void inorder(BSTNode* node, Player*& head) {
        if (node) {
            inorder(node->left, head);
            node->player->next = head;
            head = node->player;
            inorder(node->right, head);
        }
    }

    void reverseInorder(Player*& head) {
        reverseInorder(root, head);
    }

    void reverseInorder(BSTNode* node, Player*& head) {
        if (node) {
            reverseInorder(node->right, head);
            node->player->next = head;
            head = node->player;
            reverseInorder(node->left, head);
        }
    }
};

void leaderboardMenu(LinkedList& playerList) {
    int page = 0;
    Player* players = playerList.pHead;
    bool sorted = false;

    // Initially display the first 10 players unsorted
    displayPage(players, page, sorted);

    char choice;
    do {
        cout << "Leaderboard Menu:\n";
        cout << "1. Sort by Highest\n";
        cout << "2. Sort by Lowest\n";
        cout << "3. Exit\n";
        cout << "4. Page Down\n";
        cout << "5. Page Up\n";
        cout << "6. Search by Name\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case '1': {
            BST bst;
            Player* temp = players;
            while (temp) {
                bst.insert(temp);
                temp = temp->next;
            }
            players = nullptr;
            bst.inorder(players);
            page = 0; // Reset to the first page after sorting
            sorted = true;
            displayPage(players, page, sorted);
            break;
        }
        case '2': {
            BST bst;
            Player* temp = players;
            while (temp) {
                bst.insert(temp);
                temp = temp->next;
            }
            players = nullptr;
            bst.reverseInorder(players);
            page = 0; // Reset to the first page after sorting
            sorted = true;
            displayPage(players, page, sorted);
            break;
        }
        case '3':
            cout << "Exiting Leaderboard...\n";
            break;
        case '4':
            page++;
            displayPage(players, page, sorted);
            break;
        case '5':
            if (page > 0) {
                page--;
            }
            displayPage(players, page, sorted);
            break;
        case '6': {
            string searchName;
            cout << "Enter the name to search: ";
            cin.ignore();
            getline(cin, searchName);

            // Convert search name to lowercase for case-insensitive comparison
            transform(searchName.begin(), searchName.end(), searchName.begin(), ::tolower);

            int currentIndex = 0;
            bool found = false;
            Player* temp = players;
            while (temp != nullptr) {
                string playerName = temp->name;
                transform(playerName.begin(), playerName.end(), playerName.begin(), ::tolower);
                if (playerName == searchName) {
                    page = currentIndex / 10; // Calculate the page number
                    found = true;
                    break;
                }
                currentIndex++;
                temp = temp->next;
            }

            if (found) {
                displayPage(players, page, sorted);
            } else {
                cout << "Player not found.\n";
            }
            break;
        }
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != '3');
}

string generateRandomAnswer(const string& answer) {
    std::random_device rd;
    std::mt19937 gen(rd());

    if (answer.length() == 1) {
        if (answer == "T" || answer == "F") {
            std::uniform_int_distribution<> dist(0, 1);
            return dist(gen) ? "T" : "F";
        } else {
            const char options[] = { 'A', 'B', 'C', 'D' };
            std::uniform_int_distribution<> dist(0, 3);
            return string(1, options[dist(gen)]);
        }
    } else {
        const char letters[] = "ABCD";
        std::uniform_int_distribution<> dist(0, 3);
        string generatedAnswer;
        for (int i = 0; i < 4; ++i) {
            generatedAnswer += letters[dist(gen)];
        }
        return generatedAnswer;
    }
}

void handleDiscardPileSelection(Queue& discardPile, Stack& answeredDeck, string& questionIds, string& marks, int& totalScore, bool& returned, const string& autoAnswer = "") {
    if (discardPile.isEmpty()) {
        returned = false;
        return;
    }

    // Get the top card from the discard pile
    Question selectedQuestion = discardPile.dequeue();
    string answer = autoAnswer;

    if (answer.empty()) {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
        std::cout << "Question: " << selectedQuestion.question << "\nChoices: " << selectedQuestion.choices << "\nEnter your answer (type 'SKIP' to skip or 'exit' to return to menu): ";
        std::getline(std::cin, answer);
    }

    // Upper and lower case validation
    std::transform(answer.begin(), answer.end(), answer.begin(), ::tolower);
    std::string correctAnswer = selectedQuestion.answer;
    std::transform(correctAnswer.begin(), correctAnswer.end(), correctAnswer.begin(), ::tolower);

    if (answer == "skip" || answer == "exit") {
        if (answer == "skip") {
            discardPile.enqueue(selectedQuestion); 
            if (!marks.empty()) {
                marks.append("-");
            }
            marks.append("0"); 
        }
        returned = (answer == "exit");
    } else {
        if (selectedQuestion.answer.length() > 1) { 
            int correctCount = 0;
            int minLength = std::min(answer.length(), correctAnswer.length());
            for (int i = 0; i < minLength; ++i) {
                if (answer[i] == correctAnswer[i]) {
                    ++correctCount;
                }
            }

            int earnedMarks = (0.8 * correctCount * selectedQuestion.marks) / correctAnswer.length();
            totalScore += earnedMarks;
            answeredDeck.push(selectedQuestion); 
            std::cout << "You got " << correctCount << " out of " << correctAnswer.length() << " correct. You earned " << earnedMarks << " points.\n";
            if (!marks.empty()) {
                marks.append("-");
            }
            marks.append(to_string(earnedMarks));
        } else {
            if (answer == correctAnswer) {
                int earnedMarks = static_cast<int>(selectedQuestion.marks * 0.8); 
                totalScore += earnedMarks;
                answeredDeck.push(selectedQuestion); 
                std::cout << "Correct! You earned " << earnedMarks << " points.\n";
                if (!marks.empty()) {
                    marks.append("-");
                }
                marks.append(to_string(earnedMarks));
            } else {
                std::cout << "Incorrect. The correct answer is: " << selectedQuestion.answer << "\n";
                discardPile.enqueue(selectedQuestion); 
                if (!marks.empty()) {
                    marks.append("-");
                }
                marks.append("0"); 
            }
        }

        if (!questionIds.empty()) {
            questionIds.append("-");
        }
        questionIds.append(selectedQuestion.id); 
        returned = false;
    }
}

void handleUnansweredPileSelection(Queue& unansweredDeck, Stack& answeredDeck, Queue& discardPile, string& questionIds, string& marks, int& totalScore, const string& autoAnswer = "") {
    Question currentQuestion = unansweredDeck.dequeue();
    string answer = autoAnswer;

    if (answer.empty()) {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
        std::cout << "Question: " << currentQuestion.question << "\nChoices: " << currentQuestion.choices << "\nEnter your answer (type 'SKIP' to skip): ";
        std::getline(std::cin, answer);
    }

    // Convert answer and correct answer to lowercase for comparison
    std::transform(answer.begin(), answer.end(), answer.begin(), ::tolower);
    std::string correctAnswer = currentQuestion.answer;
    std::transform(correctAnswer.begin(), correctAnswer.end(), correctAnswer.begin(), ::tolower);

    if (answer == "skip") {
        discardPile.enqueue(currentQuestion); 
        if (!marks.empty()) {
            marks.append("-");
        }
        marks.append("0"); 
    } else {
        if (currentQuestion.answer.length() > 1) { 
            int correctCount = 0;
            int minLength = std::min(answer.length(), correctAnswer.length());
            for (int i = 0; i < minLength; ++i) {
                if (answer[i] == correctAnswer[i]) {
                    ++correctCount;
                }
            }

            int earnedMarks = (correctCount * currentQuestion.marks) / correctAnswer.length();
            totalScore += earnedMarks;
            answeredDeck.push(currentQuestion); 
            std::cout << "You got " << correctCount << " out of " << correctAnswer.length() << " correct. You earned " << earnedMarks << " points.\n";
            if (!marks.empty()) {
                marks.append("-");
            }
            marks.append(to_string(earnedMarks));
        } else {
            if (answer == correctAnswer) {
                totalScore += currentQuestion.marks;
                answeredDeck.push(currentQuestion); 
                std::cout << "Correct! You earned " << currentQuestion.marks << " points.\n";
                if (!marks.empty()) {
                    marks.append("-");
                }
                marks.append(to_string(currentQuestion.marks));
            } else {
                std::cout << "Incorrect. The correct answer is: " << currentQuestion.answer << "\n";
                discardPile.enqueue(currentQuestion); 
                if (!marks.empty()) {
                    marks.append("-");
                }
                marks.append("0"); 
            }
        }
    }

    if (!questionIds.empty()) {
        questionIds.append("-");
    }
    questionIds.append(currentQuestion.id); 
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

void handleUserInteraction(Queue& unansweredDeck, Stack& answeredDeck, Queue& discardPile, LinkedList& playerList) {
    if (playerList.manualPlayCount >= 10) {
        std::cout << "Max Manual Play, generate Autoplay to Complete simulation\n";
        return;
    }

    std::string name;
    bool nameExists;

    do {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
        std::cout << "Enter your name: ";
        std::getline(std::cin, name);

        nameExists = isNameExists(playerList, name);
        if (nameExists) {
            std::cout << "Name already exists. Please choose a different name.\n";
        }
    } while (nameExists);

    std::string playerID = playerList.generatePlayerID();
    if (playerID.empty()) {
        std::cout << "Max Manual Play, generate Autoplay to Complete simulation\n";
        return;
    }

    std::string questionIds, marks;
    int totalScore = 0;

    for (int i = 0; i < 3; ) {
        bool validChoice = false;
        bool returned = false;
        while (!validChoice) {
            char choice;
            std::cout << "Round " << i + 1 << "\nChoose a deck (d: discard pile, u: unanswered pile, s: skip): ";
            std::cin >> choice;

            if (choice == 'd') {
                handleDiscardPileSelection(discardPile, answeredDeck, questionIds, marks, totalScore, returned);
                if (!returned) {
                    validChoice = true;
                }
            }
            else if (choice == 'u') {
                handleUnansweredPileSelection(unansweredDeck, answeredDeck, discardPile, questionIds, marks, totalScore);
                validChoice = true;
            }
            else if (choice == 's') {
                std::cout << "You skipped this round.\n";
                if (!marks.empty()) {
                    marks.append("-");
                }
                marks.append("0"); 
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

    playerList.addPlayer(playerID, name, questionIds, marks, totalScore);
}

void autoplay(Queue& unansweredDeck, Stack& answeredDeck, Queue& discardPile, LinkedList& playerList, Player* autoPlayer) {
    std::string questionIds, marks;
    int totalScore = 0;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 1);

    for (int i = 0; i < 3; ++i) {
        bool validChoice = false;
        bool returned = false;
        while (!validChoice) {
            bool pickDiscard = dist(gen);
            if (pickDiscard && !discardPile.isEmpty()) {
                string autoAnswer = generateRandomAnswer(discardPile.peek().answer);
                handleDiscardPileSelection(discardPile, answeredDeck, questionIds, marks, totalScore, returned, autoAnswer);
                validChoice = true;
            } else if (!unansweredDeck.isEmpty()) {
                string autoAnswer = generateRandomAnswer(unansweredDeck.peek().answer);
                handleUnansweredPileSelection(unansweredDeck, answeredDeck, discardPile, questionIds, marks, totalScore, autoAnswer);
                validChoice = true;
            }
        }
    }

    playerList.addPlayer(autoPlayer->id, autoPlayer->name, questionIds, marks, totalScore);
}

void startGame(Queue& unansweredDeck, Stack& answeredDeck, Queue& discardPile, LinkedList& playerList) {
    std::cout << "Starting Game...\n";
    handleUserInteraction(unansweredDeck, answeredDeck, discardPile, playerList);
}

void mainMenu(Queue& unansweredDeck, Stack& answeredDeck, Queue& discardPile, LinkedList& playerList, PlayerQueue& autoplayPlayers) {
    char choice;
    do {
        std::cout << "Main Menu:\n";
        std::cout << "1. Game\n";
        std::cout << "2. Leaderboard\n";
        std::cout << "3. Autoplay\n";
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
            while (!autoplayPlayers.isEmpty()) {
                Player* autoPlayer = autoplayPlayers.dequeue();
                autoplay(unansweredDeck, answeredDeck, discardPile, playerList, autoPlayer);
                delete autoPlayer; // Clean up autoPlayer
            }
            std::cout << "Autoplay simulation completed.\n";
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
    Queue unansweredDeck, discardPile;
    Stack answeredDeck;
    LinkedList playerList;
    std::string questionFilename = "question.txt";
    std::string playerFilename = "player.txt";
    PlayerQueue autoplayPlayers;

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
        FileHandler::loadPlayers(playerFilename, autoplayPlayers);
        std::cout << "Player names loaded successfully for autoplay.\n";
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Error loading player names: " << e.what() << std::endl;
        return 1;
    }

    // Move 12 questions from unansweredDeck to discardPile for starting
    for (int i = 0; i < 12; ++i) {
        discardPile.enqueue(unansweredDeck.dequeue());
    }

    mainMenu(unansweredDeck, answeredDeck, discardPile, playerList, autoplayPlayers);

    return 0;
}
