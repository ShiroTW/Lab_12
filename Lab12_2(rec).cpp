#include <iostream>
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
    if (L == NULL) {
        cout << endl;
        return;
    }
    cout << L->info << " ";
    printList(L->next);
}

int countNegatives(Elem* L) {
    if (L == NULL)
        return 0;
    return (L->info < 0 ? 1 : 0) + countNegatives(L->next);
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

    enqueue(first, last, 10);
    enqueue(first, last, -3);
    enqueue(first, last, 5);
    enqueue(first, last, -8);
    enqueue(first, last, -1);

    cout << "List: ";
    printList(first);

    cout << "Number of negative elements: " << countNegatives(first) << endl;

    deleteList(first);
    return 0;
}