#include <iostream>
#include <iomanip>
using namespace std;

typedef int Info;

struct Elem {
    Elem* link;
    Info info;
};

void insert(Elem*& L, Info value) {
    Elem* tmp = new Elem;
    tmp->info = value;

    if (L != NULL) {
        Elem* T = L;
        while (T->link != L)
            T = T->link;
        T->link = tmp;
    }
    else {
        L = tmp;
    }
    tmp->link = L;
}

void printList(Elem* L) {
    if (L == NULL) {
        cout << "Список порожній" << endl;
        return;
    }

    Elem* first = L;
    cout << setw(4) << L->info;
    while (L->link != first) {
        L = L->link;
        cout << setw(4) << L->info;
    }
    cout << endl;
}

bool isSubsequence(Elem* L1, Elem* L2) {
    if (L1 == NULL) return true;
    if (L2 == NULL) return false;

    Elem* startL2 = L2;
    do {
        Elem* p1 = L1;
        Elem* p2 = L2;
        bool match = true;

        do {
            if (p1->info != p2->info) {
                match = false;
                break;
            }
            p1 = p1->link;
            p2 = p2->link;
        } while (p1 != L1);

        if (match)
            return true;

        L2 = L2->link;
    } while (L2 != startL2);

    return false;
}

void deleteList(Elem*& L) {
    if (L == NULL)
        return;

    Elem* first = L;
    Elem* tmp;

    do {
        tmp = L;
        L = L->link;
        delete tmp;
    } while (L != first);

    L = NULL;
}

int main() {
    setlocale(LC_CTYPE, "ukr");
    Elem* L1 = NULL;
    Elem* L2 = NULL;

    insert(L1, 2);
    insert(L1, 3);

    insert(L2, 1);
    insert(L2, 2);
    insert(L2, 3);
    insert(L2, 4);
    insert(L2, 5);

    cout << "L1: ";
    printList(L1);
    cout << "L2: ";
    printList(L2);

    if (isSubsequence(L1, L2))
        cout << "L1 входить як підпослідовність у L2" << endl;
    else
        cout << "L1 не входить у L2" << endl;

    deleteList(L1);
    deleteList(L2);

    return 0;
}
