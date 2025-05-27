#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <vector>
#include <algorithm>
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

    Record() : destination(""), type(REGULAR), hour(0), minute(0) {}

    Record(const string& dest, ComfortType t, int h, int m)
        : destination(dest), type(t), hour(h), minute(m) {}
};

const int TABLE_SIZE = 100;
vector<Record> table[TABLE_SIZE];

int Hash(const string& dest, int hour, int minute) {
    int hash = 0;
    for (char c : dest) hash += c;
    hash += hour * 60 + minute;
    return hash % TABLE_SIZE;
}

bool ValidateTime(int h, int m) {
    return h >= 0 && h < 24 && m >= 0 && m < 60;
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

bool InsertRecord(const Record& rec) {
    int hash = Hash(rec.destination, rec.hour, rec.minute);
    for (const Record& r : table[hash]) {
        if (r.destination == rec.destination && r.hour == rec.hour && r.minute == rec.minute)
            return false; // дубль
    }
    table[hash].push_back(rec);
    return true;
}

Record* FindRecord(const string& dest, int h, int m) {
    int hash = Hash(dest, h, m);
    for (Record& r : table[hash]) {
        if (r.destination == dest && r.hour == h && r.minute == m)
            return &r;
    }
    return nullptr;
}

bool DeleteRecord(const string& dest, int h, int m) {
    int hash = Hash(dest, h, m);
    auto& bucket = table[hash];
    auto it = remove_if(bucket.begin(), bucket.end(), [&](const Record& r) {
        return r.destination == dest && r.hour == h && r.minute == m;
        });
    if (it != bucket.end()) {
        bucket.erase(it, bucket.end());
        return true;
    }
    return false;
}

bool EditRecord(const string& dest, int h, int m, const Record& newRec) {
    Record* r = FindRecord(dest, h, m);
    if (!r) return false;
    *r = newRec;
    return true;
}

void DisplayAfterTime(int h, int m) {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        for (const Record& r : table[i]) {
            if (r.hour > h || (r.hour == h && r.minute > m)) {
                cout << r.destination << " | "
                    << ComfortTypeToString(r.type) << " | "
                    << setfill('0') << setw(2) << r.hour << ":"
                    << setfill('0') << setw(2) << r.minute << "\n";
            }
        }
    }
}

bool SaveToFile(const string& filename) {
    ofstream ofs(filename);
    if (!ofs.is_open()) return false;

    for (int i = 0; i < TABLE_SIZE; ++i) {
        for (const Record& r : table[i]) {
            ofs << r.destination << ";" << r.type << ";" << r.hour << ";" << r.minute << "\n";
        }
    }

    return true;
}

bool LoadFromFile(const string& filename) {
    ifstream ifs(filename);
    if (!ifs.is_open()) return false;

    string line;
    while (getline(ifs, line)) {
        size_t p1 = line.find(';');
        size_t p2 = line.find(';', p1 + 1);
        size_t p3 = line.find(';', p2 + 1);
        if (p1 == string::npos || p2 == string::npos || p3 == string::npos) continue;

        string dest = line.substr(0, p1);
        int type = stoi(line.substr(p1 + 1, p2 - p1 - 1));
        int h = stoi(line.substr(p2 + 1, p3 - p2 - 1));
        int m = stoi(line.substr(p3 + 1));

        InsertRecord({ dest, static_cast<ComfortType>(type), h, m });
    }

    return true;
}

int main() {
    setlocale(LC_CTYPE, "ukr");

    while (true) {
        cout << "\n1. Додати запис\n2. Видалити запис\n3. Редагувати запис\n"
            << "4. Показати поїзди після часу\n5. Зберегти в файл\n"
            << "6. Завантажити з файлу\n7. Вихід\n";
        cout << "Ваш вибір: ";
        int choice;
        if (!(cin >> choice)) {
            cin.clear(); cin.ignore(10000, '\n');
            cout << "Невірне введення\n"; continue;
        }

        if (choice == 7) break;

        switch (choice) {
        case 1: {
            Record r;
            cout << "Введіть пункт призначення: "; cin >> ws; getline(cin, r.destination);
            int tnum;
            cout << "Тип (1 - звичайний, 2 - підвищеного комфорту, 3 - швидкісний експрес): ";
            if (!(cin >> tnum) || !ParseComfortTypeFromNumber(tnum, r.type)) {
                cin.clear(); cin.ignore(10000, '\n');
                cout << "Невірний тип\n"; break;
            }
            cout << "Час (години хвилини): "; cin >> r.hour >> r.minute;
            if (!ValidateTime(r.hour, r.minute)) {
                cout << "Невірний час\n"; break;
            }
            if (!InsertRecord(r)) cout << "Запис уже існує\n";
            break;
        }
        case 2: {
            string dest;
            int h, m;
            cout << "Пункт призначення: "; cin >> ws; getline(cin, dest);
            cout << "Час (години хвилини): "; cin >> h >> m;
            if (!DeleteRecord(dest, h, m)) cout << "Запис не знайдено\n";
            break;
        }
        case 3: {
            string dest;
            int h, m;
            cout << "Пункт призначення для редагування: "; cin >> ws; getline(cin, dest);
            cout << "Поточний час (години хвилини): "; cin >> h >> m;
            Record nr;
            cout << "Новий пункт призначення: "; cin >> ws; getline(cin, nr.destination);
            int tnum;
            cout << "Новий тип (1 - звичайний, 2 - підвищеного комфорту, 3 - швидкісний експрес): ";
            if (!(cin >> tnum) || !ParseComfortTypeFromNumber(tnum, nr.type)) {
                cin.clear(); cin.ignore(10000, '\n');
                cout << "Невірний тип\n"; break;
            }
            cout << "Новий час (години хвилини): "; cin >> nr.hour >> nr.minute;
            if (!ValidateTime(nr.hour, nr.minute)) {
                cout << "Невірний час\n"; break;
            }
            if (!EditRecord(dest, h, m, nr)) cout << "Запис для редагування не знайдено\n";
            break;
        }
        case 4: {
            int h, m;
            cout << "Час (години хвилини): "; cin >> h >> m;
            if (!ValidateTime(h, m)) { cout << "Невірний час\n"; break; }
            DisplayAfterTime(h, m);
            break;
        }
        case 5: {
            string fname;
            cout << "Ім'я файлу: "; cin >> fname;
            if (!SaveToFile(fname)) cout << "Помилка збереження\n";
            break;
        }
        case 6: {
            string fname;
            cout << "Ім'я файлу: "; cin >> fname;
            for (int i = 0; i < TABLE_SIZE; ++i) table[i].clear();
            if (!LoadFromFile(fname)) cout << "Помилка завантаження\n";
            break;
        }
        default:
            cout << "Невірний вибір\n";
        }
    }

    return 0;
}
