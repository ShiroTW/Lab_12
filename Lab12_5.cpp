#include <iostream>
using namespace std;

struct Point {
    double x;
    double y;
    Point* next;
};

void Push(Point*& top, double x, double y) {
    Point* newPoint = new Point{ x, y, top };
    top = newPoint;
}

void PrintStack(Point* top) {
    cout << "Вміст стеку (координати точок):\n";
    while (top) {
        cout << "(" << top->x << ", " << top->y << ")\n";
        top = top->next;
    }
}

void ReadPointsFromInput(Point*& top) {
    int n;
    cout << "Введіть кількість точок (n > 3): ";
    cin >> n;

    if (n <= 3) {
        cout << "Помилка: кількість точок має бути більшою за 3.\n";
        return;
    }

    cout << "Введіть " << n << " пар координат (x y):\n";
    for (int i = 0; i < n; ++i) {
        double x, y;
        cout << i + 1 << ": ";
        cin >> x >> y;
        Push(top, x, y);
    }
}

void CalculateCenterOfMass(Point* top, double& centerX, double& centerY) {
    int count = 0;
    double sumX = 0.0, sumY = 0.0;

    for (Point* current = top; current; current = current->next) {
        sumX += current->x;
        sumY += current->y;
        count++;
    }

    if (count > 0) {
        centerX = sumX / count;
        centerY = sumY / count;
    }
    else {
        centerX = centerY = 0.0;
    }
}

void ClearStack(Point*& top) {
    while (top) {
        Point* temp = top;
        top = top->next;
        delete temp;
    }
}

int main() {
    setlocale(LC_CTYPE, "ukr");
    Point* stack = nullptr;

    ReadPointsFromInput(stack);

    PrintStack(stack);


    double centerX, centerY;
    CalculateCenterOfMass(stack, centerX, centerY);

    cout << "\nЦентр мас: (" << centerX << ", " << centerY << ")\n";

    ClearStack(stack);

    return 0;
}
