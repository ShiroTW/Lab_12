#include <iostream>
using namespace std;

struct Node {
    int data;
    Node* prev;
    Node* next;
};

Node* CreateNode(int value) {
    Node* node = new Node;
    node->data = value;
    node->prev = nullptr;
    node->next = nullptr;
    return node;
}

void PushBack(Node*& head, Node*& tail, int value) {
    Node* newNode = CreateNode(value);
    if (!head) {
        head = tail = newNode;
    }
    else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
}

void PrintDeque(Node* head) {
    while (head) {
        cout << head->data << " ";
        head = head->next;
    }
    cout << endl;
}

int CountElements(Node* head) {
    int count = 0;
    while (head) {
        count++;
        head = head->next;
    }
    return count;
}

//(x1 - xn) + (x2 - x_{n-1}) + ...
int CalculateEvenExpression(Node* head, Node* tail) {
    int result = 0;
    while (head && tail && head->next != tail && head != tail) {
        result += (head->data - tail->data);
        head = head->next;
        tail = tail->prev;
    }
    if (head && tail && head->next == tail) {
        result += (head->data - tail->data);
    }
    return result;
}

int CalculateSum(Node* head) {
    int sum = 0;
    while (head) {
        sum += head->data;
        head = head->next;
    }
    return sum;
}

void ClearDeque(Node*& head) {
    while (head) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

void InputFromKeyboard(Node*& head, Node*& tail) {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    cout << "Enter " << n << " integer values:\n";
    for (int i = 0; i < n; ++i) {
        int val;
        cin >> val;
        PushBack(head, tail, val);
    }
}

int main() {
    Node* head = nullptr;
    Node* tail = nullptr;

    InputFromKeyboard(head, tail);

    cout << "Deque elements:\n";
    PrintDeque(head);

    int count = CountElements(head);
    cout << "Number of elements: " << count << endl;

    if (count % 2 == 0) {
        int result = CalculateEvenExpression(head, tail);
        cout << "Even expression result: " << result << endl;
    }
    else {
        int sum = CalculateSum(head);
        cout << "Sum of elements: " << sum << endl;
    }

    ClearDeque(head);
    return 0;
}
