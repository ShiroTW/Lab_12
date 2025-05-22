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
    tmp->prev = last;

    if (last != NULL)
        last->next = tmp;
    else
        first = tmp;

    last = tmp;
}

void printList(Elem* L) {
    if (L == NULL) {
        cout << endl;
        return;
    }
    cout << L->info << " ";
    printList(L->next);
}

void insertBeforeValueRecursive(Elem*& first, Elem* current, Info V1, Info V2) {
    if (current == NULL) return;

    if (current->info == V1) {
        Elem* newElem = new Elem;
        newElem->info = V2;
        newElem->next = current;
        newElem->prev = current->prev;

        if (current->prev != NULL)
            current->prev->next = newElem;
        else
            first = newElem;

        current->prev = newElem;

        insertBeforeValueRecursive(first, current->next, V1, V2);
    }
    else {
        insertBeforeValueRecursive(first, current->next, V1, V2);
    }
}

void deleteList(Elem*& first) {
    if (first == NULL) return;
    Elem* tmp = first->next;
    delete first;
    first = tmp;
    deleteList(first);
}

int main() {
    Elem* first = NULL;
    Elem* last = NULL;

    // Список: 1 2 3 2 4, V1 = 2, V2 = 9 → 1 9 2 3 9 2 4
    enqueue(first, last, 1);
    enqueue(first, last, 2);
    enqueue(first, last, 3);
    enqueue(first, last, 2);
    enqueue(first, last, 4);

    cout << "Original list: ";
    printList(first);

    Info V1 = 2;
    Info V2 = 9;

    insertBeforeValueRecursive(first, first, V1, V2);

    cout << "Modified list: ";
    printList(first);

    deleteList(first);
    return 0;
}
