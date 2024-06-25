#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <random>
#include <algorithm>
#include <limits>

using namespace std;

struct Question {
    string question;
    string choices;
    string answer;
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
            throw runtime_error("Queue is empty");
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
    static void loadQuestions(const string& filename, Queue& queue) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error opening file: " << filename << endl;
            throw runtime_error("Failed to open file.");
        }

        string line;
        while (getline(file, line)) {
            istringstream ss(line);
            Question q;
            getline(ss, q.question, ',');
            getline(ss, q.choices, ',');
            getline(ss, q.answer, ',');

            queue.enqueue(q);
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

    random_device rd;
    mt19937 g(rd());
    shuffle(questions, questions + count, g);

    for (int i = 0; i < count; ++i) {
        queue.enqueue(questions[i]);
    }

    delete[] questions;
}

void startQuiz(Queue& unansweredDeck) {
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer

    while (!unansweredDeck.isEmpty()) {
        Question currentQuestion = unansweredDeck.dequeue();

        cout << "Question: " << currentQuestion.question << "\nChoices: " << currentQuestion.choices << "\nEnter your answer: ";
        string answer;
        getline(cin, answer);

        if (answer == currentQuestion.answer) {
            cout << "Correct!\n";
        } else {
            cout << "Incorrect. The correct answer is: " << currentQuestion.answer << "\n";
        }
    }
}

int main() {
    Queue unansweredDeck;
    string questionFilename = "question2.txt";

    try {
        FileHandler::loadQuestions(questionFilename, unansweredDeck);
        cout << "Questions loaded successfully.\n";
    }
    catch (const runtime_error& e) {
        cerr << "Error loading questions: " << e.what() << endl;
        return 1;
    }

    // Shuffle the questions to ensure randomness
    shuffleQuestions(unansweredDeck);

    startQuiz(unansweredDeck);

    return 0;
}
