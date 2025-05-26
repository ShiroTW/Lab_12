#include <iostream>
#include <climits>
using namespace std;

typedef int Info;

struct Node {
    Node* left;
    Node* right;
    Info info;
};

Node* CreateTree(int nodeCount);
void PrintTree(Node* root, int level, Node* highlight = NULL);
Node* BinarySearchInsert(Node*& root, Info value, bool& found);
void PostfixOrder(Node* root, Info& minVal, Node*& minNode);
int CountNodes(Node* root);

int Height(Node* node);
int BFactor(Node* node);
Node* RotateRight(Node* p);
Node* RotateLeft(Node* q);
Node* Balance(Node* node);

Node* FindMin(Node* p);
Node* RemoveMin(Node* p);
Node* Remove(Node* p, Info value);
void DeleteSubtree(Node*& root);
void DeleteBranch(Node*& root, Info value);

int main() {
    Node* root = NULL;
    int choice, N;
    Info value;
    bool found;

    do {
        cout << "\nMenu:\n";
        cout << "1. Create binary tree\n";
        cout << "2. Create binary search tree\n";
        cout << "3. Add element to BST\n";
        cout << "4. Print tree\n";
        cout << "5. Find first min element (postfix order)\n";
        cout << "6. Count nodes in tree\n";
        cout << "7. Balance BST\n";
        cout << "8. Delete node by value\n";
        cout << "9. Delete whole branch by root value\n";
        cout << "0. Exit\n";
        cout << "Your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Enter number of nodes: ";
            cin >> N;
            root = CreateTree(N);
            break;
        case 2:
            cout << "Enter number of nodes: ";
            cin >> N;
            root = NULL;
            for (int i = 0; i < N; i++) {
                cout << "Enter value " << i + 1 << ": ";
                cin >> value;
                BinarySearchInsert(root, value, found);
            }
            break;
        case 3:
            cout << "Enter value to add: ";
            cin >> value;
            BinarySearchInsert(root, value, found);
            break;
        case 4:
            cout << "Tree:\n";
            PrintTree(root, 0, NULL);
            break;
        case 5: {
            Info minVal = INT_MAX;
            Node* minNode = NULL;
            PostfixOrder(root, minVal, minNode);
            if (minNode != NULL) {
                cout << "First min element in postfix order: " << minVal << endl;
                cout << "Tree with highlighted element:\n";
                PrintTree(root, 0, minNode);
            }
            else {
                cout << "Tree is empty!\n";
            }
            break;
        }
        case 6:
            cout << "Total number of nodes in tree: " << CountNodes(root) << endl;
            break;
        case 7:
            root = Balance(root);
            cout << "Tree balanced.\n";
            break;
        case 8:
            cout << "Enter value to delete: ";
            cin >> value;
            root = Remove(root, value);
            break;
        case 9:
            cout << "Enter value of branch root to delete: ";
            cin >> value;
            DeleteBranch(root, value);
            break;
        case 0:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice!\n";
            break;
        }
    } while (choice != 0);

    return 0;
}

Node* CreateTree(int nodeCount) {
    if (nodeCount == 0)
        return NULL;
    Node* newNode = new Node;
    cout << "Enter node value: ";
    cin >> newNode->info;
    int leftCount = nodeCount / 2;
    int rightCount = nodeCount - leftCount - 1;
    newNode->left = CreateTree(leftCount);
    newNode->right = CreateTree(rightCount);
    return newNode;
}

void PrintTree(Node* root, int level, Node* highlight) {
    if (root != NULL) {
        PrintTree(root->right, level + 1, highlight);
        for (int i = 1; i <= level; i++) cout << " ";
        if (root == highlight) cout << "[" << root->info << "]" << endl;
        else cout << root->info << endl;
        PrintTree(root->left, level + 1, highlight);
    }
}

Node* BinarySearchInsert(Node*& root, Info value, bool& found) {
    if (root == NULL) {
        root = new Node{ NULL, NULL, value };
        found = false;
        return root;
    }
    else if (value == root->info) {
        found = true;
        return root;
    }
    else if (value < root->info) {
        return BinarySearchInsert(root->left, value, found);
    }
    else {
        return BinarySearchInsert(root->right, value, found);
    }
}

void PostfixOrder(Node* root, Info& minVal, Node*& minNode) {
    if (root != NULL) {
        PostfixOrder(root->left, minVal, minNode);
        PostfixOrder(root->right, minVal, minNode);
        if (root->info < minVal) {
            minVal = root->info;
            minNode = root;
        }
    }
}

int CountNodes(Node* root) {
    if (!root) return 0;
    return 1 + CountNodes(root->left) + CountNodes(root->right);
}

int Height(Node* node) {
    if (!node) return 0;
    return 1 + max(Height(node->left), Height(node->right));
}

int BFactor(Node* node) {
    return Height(node->right) - Height(node->left);
}

Node* RotateRight(Node* p) {
    Node* q = p->left;
    p->left = q->right;
    q->right = p;
    return q;
}

Node* RotateLeft(Node* q) {
    Node* p = q->right;
    q->right = p->left;
    p->left = q;
    return p;
}

Node* Balance(Node* node) {
    if (!node) return NULL;
    node->left = Balance(node->left);
    node->right = Balance(node->right);
    int bf = BFactor(node);
    if (bf == 2) {
        if (BFactor(node->right) < 0)
            node->right = RotateRight(node->right);
        return RotateLeft(node);
    }
    if (bf == -2) {
        if (BFactor(node->left) > 0)
            node->left = RotateLeft(node->left);
        return RotateRight(node);
    }
    return node;
}

Node* FindMin(Node* p) {
    return p->left ? FindMin(p->left) : p;
}

Node* RemoveMin(Node* p) {
    if (p->left == 0) return p->right;
    p->left = RemoveMin(p->left);
    return p;
}

Node* Remove(Node* p, Info value) {
    if (!p) return 0;
    if (value < p->info)
        p->left = Remove(p->left, value);
    else if (value > p->info)
        p->right = Remove(p->right, value);
    else {
        Node* q = p->left;
        Node* r = p->right;
        delete p;
        if (!r) return q;
        Node* min = FindMin(r);
        min->right = RemoveMin(r);
        min->left = q;
        return min;
    }
    return p;
}

void DeleteSubtree(Node*& root) {
    if (!root) return;
    DeleteSubtree(root->left);
    DeleteSubtree(root->right);
    delete root;
    root = NULL;
}

void DeleteBranch(Node*& root, Info value) {
    if (!root) return;
    if (root->info == value) {
        DeleteSubtree(root);
        root = NULL;
    }
    else {
        DeleteBranch(root->left, value);
        DeleteBranch(root->right, value);
    }
}
