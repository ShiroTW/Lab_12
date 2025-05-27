#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cctype>
#include <sstream>

using namespace std;

struct TreeNode {
    string word;
    int level;
    TreeNode* left;
    TreeNode* right;
};

TreeNode* CreateNode(const string& word, int level) {
    TreeNode* node = new TreeNode{ word, level, nullptr, nullptr };
    return node;
}

void DeleteTree(TreeNode* node) {
    if (!node) return;
    DeleteTree(node->left);
    DeleteTree(node->right);
    delete node;
}

vector<string> ReadWordsFromFile(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cout << "Помилка: не вдалося вiдкрити файл \"" << filename << "\"\n";
        return {};
    }
    vector<string> words;
    string line;
    while (getline(file, line)) {
        string word;
        for (char c : line) {
            if (isalpha(static_cast<unsigned char>(c))) {
                word += static_cast<char>(tolower(c));
            }
            else if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        }
        if (!word.empty()) words.push_back(word);
    }
    return words;
}

vector<string> FilterWordsByLength(const vector<string>& words, int length) {
    vector<string> result;
    for (const auto& w : words) {
        if ((int)w.length() == length) {
            result.push_back(w);
        }
    }
    return result;
}

TreeNode* InsertNodeIter(TreeNode* root, TreeNode* newNode) {
    if (!root) return newNode;
    TreeNode* current = root;
    TreeNode* parent = nullptr;
    while (current) {
        parent = current;
        if (newNode->word < current->word)
            current = current->left;
        else if (newNode->word > current->word)
            current = current->right;
        else {
            delete newNode;
            return root;
        }
    }
    if (newNode->word < parent->word)
        parent->left = newNode;
    else
        parent->right = newNode;
    return root;
}

void PrintTree(TreeNode* node) {
    if (!node) return;
    PrintTree(node->left);
    cout << node->word << " (довжина: " << node->level << ")\n";
    PrintTree(node->right);
}

bool SearchWord(TreeNode* root, const string& word) {
    TreeNode* current = root;
    while (current) {
        if (word == current->word)
            return true;
        else if (word < current->word)
            current = current->left;
        else
            current = current->right;
    }
    return false;
}

int InputNumberInRange(int min, int max) {
    int choice;
    while (true) {
        if (cin >> choice && choice >= min && choice <= max) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return choice;
        }
        else {
            cout << "Будь ласка, введiть число вiд " << min << " до " << max << ": ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

vector<string> ChooseWordsToInclude(const vector<string>& words, int maxCount) {
    vector<string> chosen;
    cout << "Вiдмiтьте iндекси слів, якi хочете включити до дерева (макс. " << maxCount << "):\n";
    for (size_t i = 0; i < words.size(); ++i) {
        cout << i << ": " << words[i] << "\n";
    }
    cout << "Введiть iндекси через пробіл, завершивши рядок Enter:\n";

    while (chosen.size() < (size_t)maxCount) {
        string line;
        getline(cin, line);
        if (line.empty()) break;

        istringstream iss(line);
        int idx;
        while (iss >> idx) {
            if (idx >= 0 && idx < (int)words.size()) {
                if (find(chosen.begin(), chosen.end(), words[idx]) == chosen.end()) {
                    chosen.push_back(words[idx]);
                    if (chosen.size() == (size_t)maxCount) break;
                }
            }
            else {
                cout << "Iндекс " << idx << " недійсний.\n";
            }
        }
        if (chosen.size() < (size_t)maxCount)
            cout << "Вибрано " << chosen.size() << " слів. Можна вибрати ще: " << maxCount - chosen.size() << "\n";
        else
            break;
    }
    return chosen;
}

TreeNode* RemoveNode(TreeNode* root, const string& word) {
    if (!root) return nullptr;
    if (word < root->word)
        root->left = RemoveNode(root->left, word);
    else if (word > root->word)
        root->right = RemoveNode(root->right, word);
    else {
        if (!root->left) {
            TreeNode* rightChild = root->right;
            delete root;
            return rightChild;
        }
        else if (!root->right) {
            TreeNode* leftChild = root->left;
            delete root;
            return leftChild;
        }
        else {
            TreeNode* minNode = root->right;
            while (minNode->left)
                minNode = minNode->left;
            root->word = minNode->word;
            root->level = minNode->level;
            root->right = RemoveNode(root->right, minNode->word);
        }
    }
    return root;
}

int main() {
    setlocale(LC_CTYPE, "ukr");
    srand(static_cast<unsigned int>(time(nullptr)));

    TreeNode* root = nullptr;
    vector<string> allWords;
    vector<string> excludedWords;
    string filename;

    while (true) {
        cout << "\nМеню:\n"
            << "1. Зчитати слова з файлу\n"
            << "2. Побудувати дерево\n"
            << "3. Додати слово\n"
            << "4. Видалити слово\n"
            << "5. Знайти слово\n"
            << "6. Вивести дерево\n"
            << "7. Вивести слова поза деревом\n"
            << "0. Вихiд\n"
            << "Ваш вибiр: ";

        int choice = InputNumberInRange(0, 7);

        if (choice == 0) break;

        if (choice == 1) {
            cout << "Введiть iм'я файлу: ";
            getline(cin, filename);
            allWords = ReadWordsFromFile(filename);
            if (allWords.empty()) {
                cout << "Слова не знайденi або файл пустий.\n";
            }
            else {
                cout << "Зчитано " << allWords.size() << " слiв.\n";
            }
            DeleteTree(root);
            root = nullptr;
            excludedWords.clear();
        }
        else if (choice == 2) {
            if (allWords.empty()) {
                cout << "Спочатку зчитайте слова з файлу.\n";
                continue;
            }

            vector<string> oneLetterWords = FilterWordsByLength(allWords, 1);
            if (oneLetterWords.empty()) {
                cout << "Немає однолiтерних слiв для кореня дерева.\n";
                continue;
            }

            string rootWord = oneLetterWords[rand() % oneLetterWords.size()];
            DeleteTree(root);
            root = CreateNode(rootWord, 1);
            excludedWords.clear();

            int maxLevel = 0;
            for (const auto& w : allWords)
                if ((int)w.length() > maxLevel)
                    maxLevel = (int)w.length();

            for (int lvl = 2; lvl <= maxLevel; ++lvl) {
                vector<string> wordsAtLevel = FilterWordsByLength(allWords, lvl);
                sort(wordsAtLevel.begin(), wordsAtLevel.end());

                int maxCount = 1 << (lvl - 1);
                vector<string> wordsToInsert;

                if ((int)wordsAtLevel.size() > maxCount) {
                    cout << "На рiвнi " << lvl << " бiльше слiв, ніж можна додати (" << wordsAtLevel.size() << " > " << maxCount << ").\n";
                    wordsToInsert = ChooseWordsToInclude(wordsAtLevel, maxCount);
                    for (const auto& w : wordsAtLevel) {
                        if (find(wordsToInsert.begin(), wordsToInsert.end(), w) == wordsToInsert.end())
                            excludedWords.push_back(w);
                    }
                }
                else {
                    wordsToInsert = wordsAtLevel;
                }

                for (const auto& w : wordsToInsert) {
                    root = InsertNodeIter(root, CreateNode(w, lvl));
                }
            }
            cout << "Дерево побудовано з коренем \"" << rootWord << "\".\n";
        }
        else if (choice == 3) {
            cout << "Введiть слово для додавання: ";
            string w; getline(cin, w);
            string cleanWord;
            for (char c : w) {
                if (isalpha(static_cast<unsigned char>(c)))
                    cleanWord += static_cast<char>(tolower(c));
            }
            if (cleanWord.empty()) {
                cout << "Невiрне слово.\n";
                continue;
            }
            if (!root && cleanWord.length() != 1) {
                cout << "Спочатку побудуйте дерево з коренем (однолiтерним словом).\n";
                continue;
            }
            if (cleanWord.length() == 1 && !root) {
                root = CreateNode(cleanWord, 1);
                cout << "Корiнь додано.\n";
            }
            else {
                root = InsertNodeIter(root, CreateNode(cleanWord, (int)cleanWord.length()));
                cout << "Слово додано.\n";
            }
        }
        else if (choice == 4) {
            if (!root) {
                cout << "Дерево пусте.\n";
                continue;
            }
            cout << "Введiть слово для видалення: ";
            string w; getline(cin, w);
            string cleanWord;
            for (char c : w) {
                if (isalpha(static_cast<unsigned char>(c)))
                    cleanWord += static_cast<char>(tolower(c));
            }
            if (cleanWord.empty()) {
                cout << "Невiрне слово.\n";
                continue;
            }
            if (!SearchWord(root, cleanWord)) {
                cout << "Слово не знайдено в деревi.\n";
                continue;
            }
            root = RemoveNode(root, cleanWord);
            cout << "Слово видалено.\n";
        }
        else if (choice == 5) {
            if (!root) {
                cout << "Дерево пусте.\n";
                continue;
            }
            cout << "Введіть слово для пошуку: ";
            string w; getline(cin, w);
            string cleanWord;
            for (char c : w) {
                if (isalpha(static_cast<unsigned char>(c)))
                    cleanWord += static_cast<char>(tolower(c));
            }
            if (cleanWord.empty()) {
                cout << "Невірне слово.\n";
                continue;
            }
            if (SearchWord(root, cleanWord))
                cout << "Слово знайдено в деревi.\n";
            else
                cout << "Слово не знайдено в деревi.\n";
        }
        else if (choice == 6) {
            if (!root) {
                cout << "Дерево пусте.\n";
                continue;
            }
            cout << "Вмiст дерева (iнфiксний обхід):\n";
            PrintTree(root);
        }
        else if (choice == 7) {
            if (excludedWords.empty()) {
                cout << "Немає слiв поза деревом.\n";
            }
            else {
                cout << "Слова, що не увiйшли до дерева:\n";
                for (const auto& w : excludedWords) cout << w << "\n";
            }
        }
    }

    DeleteTree(root);
    return 0;
}
