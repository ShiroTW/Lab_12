#include <iostream>
#include <string>
using namespace std;

struct Patient {
    string surname;
    string diagnosis;
    int waitTime;
    Patient* next;
};

void Enqueue(Patient*& front, Patient*& rear, const string& surname, const string& diagnosis, int waitTime) {
    Patient* newPatient = new Patient{ surname, diagnosis, waitTime, nullptr };
    if (!front)
        front = rear = newPatient;
    else {
        rear->next = newPatient;
        rear = newPatient;
    }
}

void PrintQueue(Patient* front) {
    int i = 1;
    while (front) {
        cout << i++ << ") " << front->surname << ", " << front->diagnosis
            << ", Час очкування: " << front->waitTime << " хв\n";
        front = front->next;
    }
}

void ClearQueue(Patient*& front) {
    while (front) {
        Patient* temp = front;
        front = front->next;
        delete temp;
    }
}

int GetMaxWaitTime(Patient* front) {
    int maxWait = 0;
    while (front) {
        if (front->waitTime > maxWait)
            maxWait = front->waitTime;
        front = front->next;
    }
    return maxWait;
}

int CalculateMaxQueueSize(int workMinutes, int arrivalInterval, int serviceTime) {
    int maxInQueue = 0;
    int currentInQueue = 0;
    int doctorAvailableAt = 0;

    for (int t = 0; t < workMinutes; t += arrivalInterval) {
        if (t >= doctorAvailableAt) {
            doctorAvailableAt = t + serviceTime;
        }
        else {
            currentInQueue++;
            if (currentInQueue > maxInQueue)
                maxInQueue = currentInQueue;
        }
    }

    return maxInQueue;
}

int main() {
    setlocale(LC_ALL, "ukr");

    Patient* front = nullptr;
    Patient* rear = nullptr;

    const int serviceTime = 15;
    const int arrivalInterval = 5;
    const int totalPatients = 4;

    string surnames[] = { "Iваненко", "Петренко", "Сидорчук", "Мельник" };
    string diagnoses[] = { "Грип", "Кашель", "Головний бiль", "Алергiя" };

    int waitTime = 0;
    int doctorAvailableAt = 0;

    for (int i = 0; i < totalPatients; ++i) {
        int arrivalTime = i * arrivalInterval;
        if (arrivalTime < doctorAvailableAt)
            waitTime = doctorAvailableAt - arrivalTime;
        else
            waitTime = 0;

        Enqueue(front, rear, surnames[i], diagnoses[i], waitTime);

        if (arrivalTime >= doctorAvailableAt)
            doctorAvailableAt = arrivalTime + serviceTime;
        else
            doctorAvailableAt += serviceTime;
    }

    cout << "Черга пацiєнтiв:\n";
    PrintQueue(front);

    int maxWait = GetMaxWaitTime(front);
    cout << "\nМаксимальний час очiкування: " << maxWait << " хв\n";

    int maxQueue = CalculateMaxQueueSize(480, arrivalInterval, serviceTime); // 8 год = 480 хв
    cout << "Максимальна кiлькiсть людей у черзi протягом робочого дня: " << maxQueue << endl;

    ClearQueue(front);

    return 0;
}
