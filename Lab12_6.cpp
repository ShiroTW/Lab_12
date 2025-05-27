#include <iostream>
#include <ctime>
using namespace std;

struct Person {
    int hatColor;
    int guessedColor;
    bool survived;
    Person* next;
};

void Enqueue(Person*& front, Person*& rear, int hatColor) {
    Person* newPerson = new Person{ hatColor, -1, false, nullptr };
    if (!front)
        front = rear = newPerson;
    else {
        rear->next = newPerson;
        rear = newPerson;
    }
}

void PrintQueue(Person* front) {
    int i = 1;
    while (front) {
        cout << i++ << ") Hat: " << front->hatColor
            << ", Guess: " << (front->guessedColor == -1 ? '?' : '0' + front->guessedColor)
            << ", Survived: " << (front->survived ? "Yes" : "No") << endl;
        front = front->next;
    }
}

int CountQueue(Person* front) {
    int count = 0;
    while (front) {
        count++;
        front = front->next;
    }
    return count;
}

int GetColorSumMod(Person* front, int limit) {
    int sum = 0;
    int count = 0;
    while (front && count < limit) {
        sum += front->hatColor;
        front = front->next;
        count++;
    }
    return sum % 3;
}

void GuessColors(Person* front) {
    const int MOD = 3;
    int total = CountQueue(front);
    Person* people[100];
    int index = 0;
    while (front) {
        people[index++] = front;
        front = front->next;
    }

    int realSum = 0;
    for (int i = 0; i < total - 1; ++i)
        realSum += people[i]->hatColor;

    people[total - 1]->guessedColor = realSum % MOD;
    people[total - 1]->survived = (people[total - 1]->guessedColor == people[total - 1]->hatColor);

    for (int i = total - 2; i >= 0; --i) {
        int seenSum = 0;
        for (int j = 0; j < i; ++j)
            seenSum += people[j]->hatColor;

        int neededSum = people[total - 1]->guessedColor;
        int guessedColor = (neededSum - seenSum % MOD + MOD) % MOD;

        people[i]->guessedColor = guessedColor;
        people[i]->survived = (guessedColor == people[i]->hatColor);
    }
}

void ClearQueue(Person*& front) {
    while (front) {
        Person* temp = front;
        front = front->next;
        delete temp;
    }
}

void GenerateQueue(Person*& front, Person*& rear, int count) {
    srand((unsigned)time(0));
    for (int i = 0; i < count; ++i) {
        int color = rand() % 3;
        Enqueue(front, rear, color);
    }
}

int CountSurvivors(Person* front) {
    int count = 0;
    while (front) {
        if (front->survived)
            count++;
        front = front->next;
    }
    return count;
}

int main() {
    setlocale(LC_ALL, "ukr");

    Person* front = nullptr;
    Person* rear = nullptr;

    GenerateQueue(front, rear, 100);

    GuessColors(front);

    PrintQueue(front);

    int survivors = CountSurvivors(front);
    cout << "\nКiлькiсть тих, хто вгадав свiй колiр (вижив): " << survivors << " з 100\n";
    cout << "Очiкувана кiлькiсть виживших: 99 або бiльше\n";

    ClearQueue(front);
    return 0;
}