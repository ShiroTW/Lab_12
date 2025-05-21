#include <iostream>
#include <cassert>  // Для unit-тестів
using namespace std;

typedef int Info;

struct Elem {
    Elem* next;
    Elem* prev;
    Info info;
};

void enqueue(Elem*& first, Elem*& last, Info value) {
    Elem* tmp = new Elem;
    tmp->info = value;
    tmp->next = NULL;

    if (last != NULL)
        last->next = tmp;
    tmp->prev = last;
    last = tmp;

    if (first == NULL)
        first = tmp;
}

void printList(Elem* L) {
    while (L != NULL) {
        cout << L->info << " ";
        L = L->next;
    }
    cout << endl;
}

int countNegatives(Elem* L) {
    int count = 0;
    while (L != NULL) {
        if (L->info < 0)
            count++;
        L = L->next;
    }
    return count;
}

void deleteList(Elem*& first) {
    while (first != NULL) {
        Elem* tmp = first->next;
        delete first;
        first = tmp;
    }
}

int main() {
    Elem* first = NULL;
    Elem* last = NULL;

    // Введення даних
    enqueue(first, last, 4);
    enqueue(first, last, -2);
    enqueue(first, last, 0);
    enqueue(first, last, -9);
    enqueue(first, last, 7);
    enqueue(first, last, -1);

    cout << "List: ";
    printList(first);

    int negativeCount = countNegatives(first);
    cout << "Number of negative elements: " << negativeCount << endl;

    deleteList(first);
    return 0;
}
