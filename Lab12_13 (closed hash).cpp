#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
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
    bool isEmpty;

    Record() : destination(""), type(REGULAR), hour(0), minute(0), isEmpty(true) {}

    Record(const string& dest, ComfortType t, int h, int m, bool empty = false)
        : destination(dest), type(t), hour(h), minute(m), isEmpty(empty) {}
};

const int TABLE_SIZE = 100;
Record table[TABLE_SIZE];

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
    for (int i = 0; i < TABLE_SIZE; ++i) {
        int idx = (hash + i) % TABLE_SIZE;
        if (table[idx].isEmpty) {
            table[idx] = rec;
            table[idx].isEmpty = false;
            return true;
        }
    }
    return false;
}

int FindIndex(const string& dest, int h, int m) {
    int hash = Hash(dest, h, m);
    for (int i = 0; i < TABLE_SIZE; ++i) {
        int idx = (hash + i) % TABLE_SIZE;
        if (table[idx].isEmpty) continue;
        if (table[idx].destination == dest && table[idx].hour == h && table[idx].minute == m) {
            return idx;
        }
    }
    return -1;
}

bool DeleteRecord(const string& dest, int h, int m) {
    int idx = FindIndex(dest, h, m);
    if (idx != -1) {
        table[idx].isEmpty = true;
        return true;
    }
    return false;
}

bool EditRecord(const string& dest, int h, int m, const Record& newRec) {
    int idx = FindIndex(dest, h, m);
    if (idx != -1) {
        table[idx] = newRec;
        table[idx].isEmpty = false;
        return true;
    }
    return false;
}

// Виведення всіх записів після заданого часу
void DisplayAfterTime(int h, int m) {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        if (!table[i].isEmpty) {
            if (table[i].hour > h || (table[i].hour == h && table[i].minute > m)) {
                cout << table[i].destination << " | "
                    << ComfortTypeToString(table[i].type) << " | "
                    << setfill('0') << setw(2) << table[i].hour << ":"
                    << setfill('0') << setw(2) << table[i].minute << "\n";
            }
        }
    }
}

// Збереження у файл
bool SaveToFile(const string& filename) {
    ofstream ofs(filename);
    if (!ofs.is_open()) return false;

    for (int i = 0; i < TABLE_SIZE; ++i) {
        if (!table[i].isEmpty) {
            ofs << table[i].destination << ";"
                << table[i].type << ";"
                << table[i].hour << ";"
                << table[i].minute << "\n";
        }
    }

    ofs.close();
    return true;
}

// Завантаження з файлу
bool LoadFromFile(const string& filename) {
    ifstream ifs(filename);
    if (!ifs.is_open()) return false;

    string line;
    while (getline(ifs, line)) {
        string dest;
        int type, h, m;

        size_t pos1 = line.find(';');
        size_t pos2 = line.find(';', pos1 + 1);
        size_t pos3 = line.find(';', pos2 + 1);

        if (pos1 == string::npos || pos2 == string::npos || pos3 == string::npos) continue;

        dest = line.substr(0, pos1);
        type = stoi(line.substr(pos1 + 1, pos2 - pos1 - 1));
        h = stoi(line.substr(pos2 + 1, pos3 - pos2 - 1));
        m = stoi(line.substr(pos3 + 1));

        Record rec{ dest, static_cast<ComfortType>(type), h, m, false };
        InsertRecord(rec);
    }

    ifs.close();
    return true;
}

// Меню
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
            r.isEmpty = false;
            if (!InsertRecord(r)) cout << "Таблиця заповнена. Неможливо додати.\n";
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
            nr.destination = dest;
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
            nr.isEmpty = false;
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
            // Очистити таблицю
            for (int i = 0; i < TABLE_SIZE; ++i)
                table[i] = Record();
            if (!LoadFromFile(fname)) cout << "Помилка завантаження\n";
            break;
        }
        default:
            cout << "Невірний вибір\n";
        }
    }

    return 0;
}
