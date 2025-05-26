#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <cctype>
using namespace std;

enum ComfortType {
    REGULAR,
    COMFORT,
    EXPRESS
};

struct Record {
    string destination;
    ComfortType type;
    int hour;
    int minute;
};

struct TrieNode {
    unordered_map<char, TrieNode*> children;
    vector<Record> records;
    bool isEnd;
    TrieNode() : isEnd(false) {}
};

// Create a new trie node
TrieNode* CreateNode() {
    return new TrieNode();
}

string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\n\r");
    size_t end = s.find_last_not_of(" \t\n\r");
    return (start == string::npos) ? "" : s.substr(start, end - start + 1);
}

// Переведення рядка в нижній регістр
string toLower(const string& s) {
    string result = s;
    for (char& c : result) c = tolower(static_cast<unsigned char>(c));
    return result;
}

bool ParseComfortTypeFromNumber(int num, ComfortType& out) {
    switch (num) {
    case 1: out = REGULAR; return true;
    case 2: out = COMFORT; return true;
    case 3: out = EXPRESS; return true;
    default: return false;
    }
}


string ComfortTypeToString(ComfortType type) {
    switch (type) {
    case REGULAR: return "звичайний";
    case COMFORT: return "підвищеного комфорту";
    case EXPRESS: return "швидкісний експрес";
    }
    return "";
}

// Insert a record into the trie
void InsertRecord(TrieNode* root, const Record& rec) {
    TrieNode* node = root;
    for (char c : rec.destination) {
        if (!node->children[c]) {
            node->children[c] = CreateNode();
        }
        node = node->children[c];
    }
    node->isEnd = true;
    node->records.push_back(rec);
}

// Find node for a given destination
TrieNode* FindNode(TrieNode* root, const string& dest) {
    TrieNode* node = root;
    for (char c : dest) {
        if (!node->children.count(c)) return nullptr;
        node = node->children[c];
    }
    return node->isEnd ? node : nullptr;
}

// Delete a record by destination and time
bool DeleteRecord(TrieNode* root, const string& dest, int h, int m) {
    TrieNode* node = FindNode(root, dest);
    if (!node) return false;
    auto& v = node->records;
    for (auto it = v.begin(); it != v.end(); ++it) {
        if (it->hour == h && it->minute == m) {
            v.erase(it);
            return true;
        }
    }
    return false;
}

// Edit a record: locate and replace
bool EditRecord(TrieNode* root, const string& dest, int h, int m, const Record& newRec) {
    TrieNode* node = FindNode(root, dest);
    if (!node) return false;
    for (auto& r : node->records) {
        if (r.hour == h && r.minute == m) {
            r = newRec;
            return true;
        }
    }
    return false;
}

// Display records departing after given time
void DisplayAfterTime(TrieNode* node, int h, int m) {
    if (node->isEnd) {
        for (auto& r : node->records) {
            if (r.hour > h || (r.hour == h && r.minute > m)) {
                printf("%s | %s | %02d:%02d\n", r.destination.c_str(), ComfortTypeToString(r.type).c_str(), r.hour, r.minute);
            }
        }
    }
    for (auto& p : node->children) {
        DisplayAfterTime(p.second, h, m);
    }
}

// Recursive save helper
void SaveTrieHelper(TrieNode* node, const string& prefix, ofstream& ofs) {
    if (node->isEnd) {
        for (auto& r : node->records) {
            ofs << r.destination << ";" << r.type << ";" << r.hour << ";" << r.minute << "\n";
        }
    }
    for (auto& p : node->children) {
        SaveTrieHelper(p.second, prefix + p.first, ofs);
    }
}

// Save entire trie to file
bool SaveTrieToFile(TrieNode* root, const string& filename) {
    ofstream ofs(filename);
    if (!ofs.is_open()) return false;
    SaveTrieHelper(root, "", ofs);
    ofs.close();
    return true;
}

// Load records from file into trie
bool LoadTrieFromFile(TrieNode* root, const string& filename) {
    ifstream ifs(filename);
    if (!ifs.is_open()) return false;
    string line;
    while (getline(ifs, line)) {
        stringstream ss(line);
        string dest, typeStr;
        int t, h, m;
        if (!getline(ss, dest, ';')) return false;
        if (!(ss >> t)) return false;
        ss.ignore(1);
        if (!(ss >> h)) return false;
        ss.ignore(1);
        if (!(ss >> m)) return false;
        Record r{ dest, static_cast<ComfortType>(t), h, m };
        InsertRecord(root, r);
    }
    ifs.close();
    return true;
}

// Validate time input
bool ValidateTime(int h, int m) {
    return (h >= 0 && h < 24 && m >= 0 && m < 60);
}

void DeleteTrie(TrieNode* node) {
    for (auto& p : node->children) {
        DeleteTrie(p.second);
    }
    delete node;
}

// Top-down menu-driven program
int main() {
    setlocale(LC_CTYPE, "ukr");
    TrieNode* root = CreateNode();
    while (true) {
        cout << "1. Додати запис\n2. Видалити запис\n3. Редагувати запис\n4. Показати поїзди після часу\n5. Зберегти в файл\n6. Завантажити з файлу\n7. Вихід\n";
        cout << "Ваш вибір: ";
        int choice;
        if (!(cin >> choice)) { cin.clear(); cin.ignore(10000, '\n'); cout << "Невірне введення\n"; continue; }
        if (choice == 7) break;
        switch (choice) {
        case 1: {
            Record r;
            cout << "Введіть пункт призначення: "; cin >> ws; getline(cin, r.destination);
            int tnum;
            cout << "Введіть тип (1 - звичайний, 2 - підвищеного комфорту, 3 - швидкісний експрес): ";
            if (!(cin >> tnum) || !ParseComfortTypeFromNumber(tnum, r.type)) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Невірний тип\n";
                break;
            }
            cin.ignore(10000, '\n');
            cout << "Введіть час відправлення (години хвилини): "; cin >> r.hour >> r.minute;
            if (!ValidateTime(r.hour, r.minute)) { cout << "Невірний час\n"; break; }
            InsertRecord(root, r);
            break;
        }
        case 2: {
            string dest;
            int h, m;
            cout << "Введіть пункт призначення для видалення: "; cin >> ws; getline(cin, dest);
            cout << "Введіть час відправлення (години хвилини): "; cin >> h >> m;
            if (!DeleteRecord(root, dest, h, m)) cout << "Запис не знайдено\n";
            break;
        }
        case 3: {
            string dest;
            int h, m;
            cout << "Введіть пункт призначення для редагування: "; cin >> ws; getline(cin, dest);
            cout << "Введіть час поточного відправлення (години хвилини): "; cin >> h >> m;
            Record nr;
            nr.destination = dest;
            int tnum;
            cout << "Введіть новий тип (1 - звичайний, 2 - підвищеного комфорту, 3 - швидкісний експрес): ";
            if (!(cin >> tnum) || !ParseComfortTypeFromNumber(tnum, nr.type)) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Невірний тип\n";
                break;
            }
            cin.ignore(10000, '\n');
            cout << "Введіть новий час (години хвилини): "; cin >> nr.hour >> nr.minute;
            if (!ValidateTime(nr.hour, nr.minute)) { cout << "Невірний час\n"; break; }
            if (!EditRecord(root, dest, h, m, nr)) cout << "Запис для редагування не знайдено\n";
            break;
        }
        case 4: {
            int h, m;
            cout << "Показати поїзди після часу (години хвилини): "; cin >> h >> m;
            if (!ValidateTime(h, m)) { cout << "Невірний час\n"; break; }
            DisplayAfterTime(root, h, m);
            break;
        }
        case 5: {
            string fname;
            cout << "Введіть ім'я файлу для збереження: "; cin >> fname;
            if (!SaveTrieToFile(root, fname)) cout << "Не вдалося зберегти в файл\n";
            break;
        }
        case 6: {
            string fname;
            cout << "Введіть ім'я файлу для завантаження: "; cin >> fname;
            DeleteTrie(root);

            root = CreateNode();

            if (!LoadTrieFromFile(root, fname)) {
                cout << "Не вдалося завантажити з файлу\n";
            }
            break;
        }
        default:
            cout << "Невірний вибір\n";
        }
    }
    return 0;
}
