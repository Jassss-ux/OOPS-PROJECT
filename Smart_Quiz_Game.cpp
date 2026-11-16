#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <cctype>

using namespace std;

// ============================================================
// SMART QUIZ GAME WITH AUTOMATED SCORING
// Single-file OOP project for a 3-member team
// Compile: g++ -std=c++17 main.cpp -o quiz
// ============================================================

// ------------------------- CLASS 1 ---------------------------
// Abstract base class: common structure for every question.
class Question {
protected:
    int questionId;
    string category;
    string questionText;

public:
    Question(int id, const string& cat, const string& text)
        : questionId(id), category(cat), questionText(text) {}

    virtual void display(int number) const = 0;
    virtual bool isValidAnswer(char answer) const = 0;
    virtual bool checkAnswer(char answer) const = 0;
    virtual char getCorrectAnswer() const = 0;

    string getCategory() const {
        return category;
    }

    virtual ~Question() = default;
};

// ------------------------- CLASS 2 ---------------------------
// Derived class for multiple-choice questions.
class MCQQuestion : public Question {
private:
    vector<string> options;
    char correctOption;

public:
    MCQQuestion(int id, const string& cat, const string& text,
                const vector<string>& choices, char answer)
        : Question(id, cat, text), options(choices),
          correctOption(static_cast<char>(toupper(static_cast<unsigned char>(answer)))) {}

    void display(int number) const override {
        cout << "\nQ" << number << ". " << questionText << "\n";
        for (size_t i = 0; i < options.size(); ++i) {
            cout << static_cast<char>('A' + i) << ". " << options[i] << "\n";
        }
    }

    bool isValidAnswer(char answer) const override {
        answer = static_cast<char>(toupper(static_cast<unsigned char>(answer)));
        return answer >= 'A' && answer < ('A' + static_cast<int>(options.size()));
    }

    bool checkAnswer(char answer) const override {
        answer = static_cast<char>(toupper(static_cast<unsigned char>(answer)));
        return answer == correctOption;
    }

    char getCorrectAnswer() const override {
        return correctOption;
    }
};

// ------------------------- CLASS 3 ---------------------------
// Another derived class to demonstrate runtime polymorphism.
class TrueFalseQuestion : public Question {
private:
    char correctAnswer; // T or F

public:
    TrueFalseQuestion(int id, const string& cat, const string& text,
                      char answer)
        : Question(id, cat, text),
          correctAnswer(static_cast<char>(toupper(static_cast<unsigned char>(answer)))) {}

    void display(int number) const override {
        cout << "\nQ" << number << ". " << questionText << "\n";
        cout << "T. True\nF. False\n";
    }

    bool isValidAnswer(char answer) const override {
        answer = static_cast<char>(toupper(static_cast<unsigned char>(answer)));
        return answer == 'T' || answer == 'F';
    }

    bool checkAnswer(char answer) const override {
        answer = static_cast<char>(toupper(static_cast<unsigned char>(answer)));
        return answer == correctAnswer;
    }

    char getCorrectAnswer() const override {
        return correctAnswer;
    }
};

// ------------------------- CLASS 4 ---------------------------
// Stores a player's name and score.
class Player {
private:
    string name;
    int score;

public:
    Player() : name("Guest"), score(0) {}

    explicit Player(const string& playerName)
        : name(playerName), score(0) {}

    string getName() const {
        return name;
    }

    int getScore() const {
        return score;
    }

    void addPoint() {
        ++score;
    }

    // Operator overloading: allows a Player object to be printed with cout.
    friend ostream& operator<<(ostream& out, const Player& player) {
        out << "Player: " << player.name << " | Score: " << player.score;
        return out;
    }
};

// ------------------------- CLASS 5 ---------------------------
// Creates and stores the fixed question bank.
class QuestionBank {
private:
    vector<unique_ptr<Question>> questions;

    void addMCQ(int id, const string& category, const string& text,
                const vector<string>& options, char answer) {
        questions.push_back(make_unique<MCQQuestion>(
            id, category, text, options, answer));
    }

    void addTrueFalse(int id, const string& category,
                      const string& text, char answer) {
        questions.push_back(make_unique<TrueFalseQuestion>(
            id, category, text, answer));
    }

public:
    QuestionBank() {
        // C++ category
        addMCQ(1, "C++", "Which keyword is used to define a class in C++?",
               {"class", "object", "define", "function"}, 'A');
        addMCQ(2, "C++", "Which access specifier hides members from outside the class?",
               {"public", "private", "static", "virtual"}, 'B');
        addMCQ(3, "C++", "Which special member function is called when an object is created?",
               {"Destructor", "Friend function", "Constructor", "Operator"}, 'C');
        addMCQ(4, "C++", "Which OOP concept allows one class to acquire another class's features?",
               {"Encapsulation", "Inheritance", "Abstraction", "Compilation"}, 'B');
        addTrueFalse(5, "C++",
                     "Function overloading allows functions to have the same name with different parameter lists.",
                     'T');

        // AI/ML category
        addMCQ(6, "AI/ML", "What is the full form of AI?",
               {"Automated Internet", "Artificial Intelligence", "Advanced Input", "Applied Information"}, 'B');
        addMCQ(7, "AI/ML", "Supervised learning generally uses which type of data?",
               {"Labelled data", "Only images", "Unlabelled data", "No training data"}, 'A');
        addMCQ(8, "AI/ML", "Which technique groups similar data points without given labels?",
               {"Regression", "Classification", "Clustering", "Sorting"}, 'C');
        addMCQ(9, "AI/ML", "What is the full form of ML?",
               {"Machine Learning", "Multiple Logic", "Manual Language", "Model Listing"}, 'A');
        addTrueFalse(10, "AI/ML",
                     "Deep learning is a branch of machine learning.", 'T');

        // General Knowledge category
        addMCQ(11, "General Knowledge", "Which is the largest planet in our solar system?",
               {"Earth", "Mars", "Jupiter", "Venus"}, 'C');
        addMCQ(12, "General Knowledge", "What is the capital city of Australia?",
               {"Sydney", "Canberra", "Melbourne", "Perth"}, 'B');
        addMCQ(13, "General Knowledge", "What is the chemical formula of water?",
               {"CO2", "O2", "NaCl", "H2O"}, 'D');
        addMCQ(14, "General Knowledge", "What is the currency of Japan?",
               {"Yen", "Won", "Dollar", "Euro"}, 'A');
        addTrueFalse(15, "General Knowledge",
                     "Earth is the third planet from the Sun.", 'T');
    }

    vector<Question*> getQuestions(const string& selectedCategory) const {
        vector<Question*> selected;
        for (const auto& question : questions) {
            if (question->getCategory() == selectedCategory) {
                selected.push_back(question.get());
            }
        }
        return selected;
    }
};

// ------------------------- STRUCT ----------------------------
// One saved quiz attempt read from results.txt.
struct ResultRecord {
    string playerName;
    string category;
    int score;
    int totalQuestions;

    double percentage() const {
        if (totalQuestions == 0) return 0.0;
        return (static_cast<double>(score) / totalQuestions) * 100.0;
    }
};

// ------------------------- CLASS 6 ---------------------------
// Saves and displays previous results and the leaderboard.
class ResultManager {
private:
    const string fileName = "results.txt";

    vector<ResultRecord> loadResults() const {
        vector<ResultRecord> records;
        ifstream file(fileName);
        if (!file.is_open()) {
            return records; // No result file exists yet.
        }

        string name, category, scoreText, totalText;
        while (getline(file, name, '|')) {
            if (!getline(file, category, '|') ||
                !getline(file, scoreText, '|') ||
                !getline(file, totalText)) {
                break; // Ignore an incomplete final record.
            }

            try {
                int score = stoi(scoreText);
                int total = stoi(totalText);
                if (total > 0 && score >= 0 && score <= total) {
                    records.push_back({name, category, score, total});
                }
            } catch (const exception&) {
                // Skip a malformed record and continue reading.
            }
        }
        return records;
    }

public:
    bool saveResult(const Player& player, const string& category,
                    int totalQuestions) const {
        ofstream file(fileName, ios::app);
        if (!file.is_open()) {
            cerr << "Could not open results.txt for saving.\n";
            return false;
        }

        file << player.getName() << '|'
             << category << '|'
             << player.getScore() << '|'
             << totalQuestions << '\n';
        return static_cast<bool>(file);
    }

    void showHistory() const {
        vector<ResultRecord> records = loadResults();
        if (records.empty()) {
            cout << "\nNo previous quiz results found. Play a quiz first.\n";
            return;
        }

        cout << "\n================ QUIZ HISTORY ================\n";
        cout << left << setw(22) << "Player"
             << setw(22) << "Category"
             << setw(12) << "Score"
             << "Percentage\n";
        cout << string(68, '-') << '\n';

        for (const auto& record : records) {
            cout << left << setw(22) << record.playerName.substr(0, 20)
                 << setw(22) << record.category.substr(0, 20)
                 << setw(12) << (to_string(record.score) + "/" + to_string(record.totalQuestions))
                 << fixed << setprecision(1) << record.percentage() << "%\n";
        }
    }

    void showLeaderboard() const {
        vector<ResultRecord> records = loadResults();
        if (records.empty()) {
            cout << "\nLeaderboard is empty. Play a quiz first.\n";
            return;
        }

        sort(records.begin(), records.end(),
             [](const ResultRecord& a, const ResultRecord& b) {
                 if (a.percentage() != b.percentage()) {
                     return a.percentage() > b.percentage();
                 }
                 return a.score > b.score;
             });

        cout << "\n=============== TOP QUIZ ATTEMPTS ===============\n";
        cout << left << setw(8) << "Rank"
             << setw(22) << "Player"
             << setw(22) << "Category"
             << setw(12) << "Score"
             << "Percentage\n";
        cout << string(76, '-') << '\n';

        int limit = min(10, static_cast<int>(records.size()));
        for (int i = 0; i < limit; ++i) {
            cout << left << setw(8) << (i + 1)
                 << setw(22) << records[i].playerName.substr(0, 20)
                 << setw(22) << records[i].category.substr(0, 20)
                 << setw(12) << (to_string(records[i].score) + "/" + to_string(records[i].totalQuestions))
                 << fixed << setprecision(1) << records[i].percentage() << "%\n";
        }
        cout << "Note: Leaderboard shows the best individual quiz attempts.\n";
    }
};

// ------------------------- CLASS 7 ---------------------------
// Controls the game menu and quiz flow.
class QuizGame {
private:
    QuestionBank questionBank;
    ResultManager resultManager;

    int readMenuChoice(const string& prompt, int minimum, int maximum) const {
        int choice;
        while (true) {
            cout << prompt;
            if (cin >> choice && choice >= minimum && choice <= maximum) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return choice;
            }

            cout << "Invalid choice. Please enter a number from "
                 << minimum << " to " << maximum << ".\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    string chooseCategory() const {
        cout << "\n------------- SELECT QUIZ CATEGORY -------------\n";
        cout << "1. C++\n";
        cout << "2. AI/ML\n";
        cout << "3. General Knowledge\n";

        int choice = readMenuChoice("Enter category: ", 1, 3);
        if (choice == 1) return "C++";
        if (choice == 2) return "AI/ML";
        return "General Knowledge";
    }

    void playQuiz() {
        string playerName;
        cout << "\nEnter player name: ";
        getline(cin, playerName);

        if (playerName.empty()) {
            cout << "Player name cannot be empty. Returning to menu.\n";
            return;
        }
        // Prevent the file separator from breaking the saved record format.
        replace(playerName.begin(), playerName.end(), '|', '/');

        string category = chooseCategory();
        vector<Question*> quizQuestions = questionBank.getQuestions(category);
        if (quizQuestions.empty()) {
            cout << "No questions are available in this category.\n";
            return;
        }

        Player player(playerName);
        cout << "\n========== QUIZ STARTED ==========\n";
        cout << "Player: " << player.getName() << "\n";
        cout << "Category: " << category << "\n";
        cout << "Total questions: " << quizQuestions.size() << "\n";
        cout << "Scoring: +1 for each correct answer, no negative marking.\n";

        for (size_t i = 0; i < quizQuestions.size(); ++i) {
            // Runtime polymorphism: the correct derived display/check method runs.
            Question* question = quizQuestions[i];
            question->display(static_cast<int>(i + 1));

            char answer;
            while (true) {
                cout << "Your answer: ";
                if (!(cin >> answer)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Please enter a valid option.\n";
                    continue;
                }
                answer = static_cast<char>(toupper(static_cast<unsigned char>(answer)));
                if (question->isValidAnswer(answer)) {
                    break;
                }
                cout << "Invalid option. Please try again.\n";
            }

            if (question->checkAnswer(answer)) {
                cout << "Correct answer!\n";
                player.addPoint();
            } else {
                cout << "Wrong answer. Correct option: "
                     << question->getCorrectAnswer() << "\n";
            }
        }

        int total = static_cast<int>(quizQuestions.size());
        double percentage = (static_cast<double>(player.getScore()) / total) * 100.0;

        cout << "\n============== QUIZ RESULT ==============\n";
        cout << player << '\n';
        cout << "Category: " << category << '\n';
        cout << "Total questions: " << total << '\n';
        cout << "Correct answers: " << player.getScore() << '\n';
        cout << "Wrong answers: " << total - player.getScore() << '\n';
        cout << "Percentage: " << fixed << setprecision(1) << percentage << "%\n";

        if (percentage >= 80.0) {
            cout << "Performance: Excellent!\n";
        } else if (percentage >= 60.0) {
            cout << "Performance: Good job!\n";
        } else if (percentage >= 40.0) {
            cout << "Performance: Keep practising!\n";
        } else {
            cout << "Performance: More practice needed.\n";
        }

        if (resultManager.saveResult(player, category, total)) {
            cout << "Your result has been saved in results.txt.\n";
        }
        cout << "=========================================\n";
    }

    void showInstructions() const {
        cout << "\n=============== HOW TO PLAY ===============\n";
        cout << "1. Enter your name and select a category.\n";
        cout << "2. Each category contains five questions.\n";
        cout << "3. For MCQs, enter A, B, C or D.\n";
        cout << "4. For True/False, enter T or F.\n";
        cout << "5. Each correct answer gives one point.\n";
        cout << "6. There is no negative marking.\n";
        cout << "7. Your result is saved automatically.\n";
        cout << "===========================================\n";
    }

public:
    void run() {
        bool running = true;
        while (running) {
            cout << "\n=============================================\n";
            cout << "       SMART QUIZ GAME WITH AUTOMATED SCORING\n";
            cout << "=============================================\n";
            cout << "1. Play Quiz\n";
            cout << "2. View Quiz History\n";
            cout << "3. View Leaderboard\n";
            cout << "4. How to Play\n";
            cout << "5. Exit\n";

            int choice = readMenuChoice("Enter your choice: ", 1, 5);
            switch (choice) {
                case 1:
                    playQuiz();
                    break;
                case 2:
                    resultManager.showHistory();
                    break;
                case 3:
                    resultManager.showLeaderboard();
                    break;
                case 4:
                    showInstructions();
                    break;
                case 5:
                    running = false;
                    cout << "Thank you for playing Smart Quiz Game!\n";
                    break;
            }
        }
    }
};

// ------------------------- MAIN ------------------------------
int main() {
    QuizGame game;
    game.run();
    return 0;
}
