#include <iostream>
#include <queue>
using namespace std;

// Узел бинарного дерева
struct Node {
    int data;
    Node* left;
    Node* right;

    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Класс Binary Search Tree
class BST {
private:
    Node* root;

    // Рекурсивная вставка
    Node* insertRec(Node* node, int val) {
        if (node == nullptr) {
            return new Node(val);
        }

        if (val < node->data) {
            node->left = insertRec(node->left, val);
        } else if (val > node->data) {
            node->right = insertRec(node->right, val);
        }

        return node;
    }

    // Рекурсивный поиск
    bool searchRec(Node* node, int val) {
        if (node == nullptr) return false;
        if (node->data == val) return true;

        if (val < node->data) {
            return searchRec(node->left, val);
        } else {
            return searchRec(node->right, val);
        }
    }

    // Найти минимальный узел (самый левый)
    Node* findMin(Node* node) {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    // Рекурсивное удаление
    Node* removeRec(Node* node, int val) {
        if (node == nullptr) return nullptr;

        if (val < node->data) {
            node->left = removeRec(node->left, val);
        } else if (val > node->data) {
            node->right = removeRec(node->right, val);
        } else {
            // Найден узел для удаления

            // Случай 1: нет детей (лист)
            if (node->left == nullptr && node->right == nullptr) {
                delete node;
                return nullptr;
            }

            // Случай 2: один ребенок
            if (node->left == nullptr) {
                Node* temp = node->right;
                delete node;
                return temp;
            }
            if (node->right == nullptr) {
                Node* temp = node->left;
                delete node;
                return temp;
            }

            // Случай 3: два ребенка
            // Находим минимальный узел справа (inorder successor)
            Node* minRight = findMin(node->right);
            node->data = minRight->data;
            node->right = removeRec(node->right, minRight->data);
        }

        return node;
    }

    // Inorder обход (левый -> корень -> правый) - выводит отсортированно
    void inorderRec(Node* node) {
        if (node == nullptr) return;

        inorderRec(node->left);
        cout << node->data << " ";
        inorderRec(node->right);
    }

    // Preorder обход (корень -> левый -> правый)
    void preorderRec(Node* node) {
        if (node == nullptr) return;

        cout << node->data << " ";
        preorderRec(node->left);
        preorderRec(node->right);
    }

    // Postorder обход (левый -> правый -> корень)
    void postorderRec(Node* node) {
        if (node == nullptr) return;

        postorderRec(node->left);
        postorderRec(node->right);
        cout << node->data << " ";
    }

    // Высота дерева
    int heightRec(Node* node) {
        if (node == nullptr) return 0;

        int leftHeight = heightRec(node->left);
        int rightHeight = heightRec(node->right);

        return max(leftHeight, rightHeight) + 1;
    }

    // Подсчет узлов
    int countRec(Node* node) {
        if (node == nullptr) return 0;
        return 1 + countRec(node->left) + countRec(node->right);
    }

    // Освобождение памяти
    void destroyRec(Node* node) {
        if (node == nullptr) return;

        destroyRec(node->left);
        destroyRec(node->right);
        delete node;
    }

public:
    BST() : root(nullptr) {}

    // Вставка
    void insert(int val) {
        root = insertRec(root, val);
        cout << "Вставлен " << val << "\n";
    }

    // Поиск
    bool search(int val) {
        return searchRec(root, val);
    }

    // Удаление
    void remove(int val) {
        if (!search(val)) {
            cout << "Значение " << val << " не найдено\n";
            return;
        }
        root = removeRec(root, val);
        cout << "Удален " << val << "\n";
    }

    // Inorder (отсортированный вывод)
    void printInorder() {
        cout << "Inorder: ";
        inorderRec(root);
        cout << "\n";
    }

    // Preorder
    void printPreorder() {
        cout << "Preorder: ";
        preorderRec(root);
        cout << "\n";
    }

    // Postorder
    void printPostorder() {
        cout << "Postorder: ";
        postorderRec(root);
        cout << "\n";
    }

    // Level-order обход (BFS)
    void printLevelOrder() {
        if (root == nullptr) {
            cout << "Дерево пусто\n";
            return;
        }

        cout << "Level-order: ";
        queue<Node*> q;
        q.push(root);

        while (!q.empty()) {
            Node* current = q.front();
            q.pop();

            cout << current->data << " ";

            if (current->left != nullptr) q.push(current->left);
            if (current->right != nullptr) q.push(current->right);
        }
        cout << "\n";
    }

    // Высота дерева
    int height() {
        return heightRec(root);
    }

    // Количество узлов
    int size() {
        return countRec(root);
    }

    // Проверка на пустоту
    bool isEmpty() {
        return root == nullptr;
    }

    // Деструктор
    ~BST() {
        destroyRec(root);
    }
};

int main() {
    BST tree;

    cout << "=== BINARY SEARCH TREE DEMO ===\n\n";

    // Тест 1: Вставка
    cout << "--- Тест 1: Вставка ---\n";
    tree.insert(50);
    tree.insert(30);
    tree.insert(70);
    tree.insert(20);
    tree.insert(40);
    tree.insert(60);
    tree.insert(80);
    cout << "Размер: " << tree.size() << "\n";
    cout << "Высота: " << tree.height() << "\n\n";

    // Тест 2: Обходы
    cout << "--- Тест 2: Обходы ---\n";
    tree.printInorder();     // 20 30 40 50 60 70 80
    tree.printPreorder();    // 50 30 20 40 70 60 80
    tree.printPostorder();   // 20 40 30 60 80 70 50
    tree.printLevelOrder();  // 50 30 70 20 40 60 80
    cout << "\n";

    // Тест 3: Поиск
    cout << "--- Тест 3: Поиск ---\n";
    cout << "Поиск 40: " << (tree.search(40) ? "найден" : "не найден") << "\n";
    cout << "Поиск 100: " << (tree.search(100) ? "найден" : "не найден") << "\n\n";

    // Тест 4: Удаление (лист)
    cout << "--- Тест 4: Удаление листа (20) ---\n";
    tree.remove(20);
    tree.printInorder();
    cout << "\n";

    // Тест 5: Удаление (один ребенок)
    cout << "--- Тест 5: Удаление с одним ребенком (30) ---\n";
    tree.remove(30);
    tree.printInorder();
    cout << "\n";

    // Тест 6: Удаление (два ребенка)
    cout << "--- Тест 6: Удаление с двумя детьми (50 - корень) ---\n";
    tree.remove(50);
    tree.printInorder();
    cout << "\n";

    // Тест 7: Статистика
    cout << "--- Тест 7: Статистика ---\n";
    cout << "Размер: " << tree.size() << "\n";
    cout << "Высота: " << tree.height() << "\n";
    cout << "Пусто: " << (tree.isEmpty() ? "да" : "нет") << "\n";

    return 0;
}
