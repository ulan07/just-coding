#include <iostream>
using namespace std;

// Узел связного списка
struct Node {
    int data;
    Node* next;

    Node(int val) : data(val), next(nullptr) {}
};

// Класс односвязного списка
class LinkedList {
private:
    Node* head;

public:
    LinkedList() : head(nullptr) {}

    // Вставка в начало - O(1)
    void pushFront(int val) {
        Node* newNode = new Node(val);
        newNode->next = head;
        head = newNode;
        cout << "Добавлен " << val << " в начало\n";
    }

    // Вставка в конец - O(n)
    void pushBack(int val) {
        Node* newNode = new Node(val);

        if (head == nullptr) {
            head = newNode;
            cout << "Добавлен " << val << " (первый элемент)\n";
            return;
        }

        Node* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
        cout << "Добавлен " << val << " в конец\n";
    }

    // Вставка после определенного значения
    void insertAfter(int afterVal, int val) {
        Node* current = head;

        while (current != nullptr && current->data != afterVal) {
            current = current->next;
        }

        if (current == nullptr) {
            cout << "Значение " << afterVal << " не найдено\n";
            return;
        }

        Node* newNode = new Node(val);
        newNode->next = current->next;
        current->next = newNode;
        cout << "Добавлен " << val << " после " << afterVal << "\n";
    }

    // Удаление первого элемента - O(1)
    void popFront() {
        if (head == nullptr) {
            cout << "Список пуст\n";
            return;
        }

        Node* temp = head;
        head = head->next;
        cout << "Удален " << temp->data << " из начала\n";
        delete temp;
    }

    // Удаление последнего элемента - O(n)
    void popBack() {
        if (head == nullptr) {
            cout << "Список пуст\n";
            return;
        }

        if (head->next == nullptr) {
            cout << "Удален " << head->data << "\n";
            delete head;
            head = nullptr;
            return;
        }

        Node* current = head;
        while (current->next->next != nullptr) {
            current = current->next;
        }

        cout << "Удален " << current->next->data << " из конца\n";
        delete current->next;
        current->next = nullptr;
    }

    // Удаление по значению
    void remove(int val) {
        if (head == nullptr) {
            cout << "Список пуст\n";
            return;
        }

        if (head->data == val) {
            popFront();
            return;
        }

        Node* current = head;
        while (current->next != nullptr && current->next->data != val) {
            current = current->next;
        }

        if (current->next == nullptr) {
            cout << "Значение " << val << " не найдено\n";
            return;
        }

        Node* temp = current->next;
        current->next = current->next->next;
        cout << "Удален " << val << "\n";
        delete temp;
    }

    // Поиск элемента - O(n)
    bool search(int val) {
        Node* current = head;
        while (current != nullptr) {
            if (current->data == val) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    // Получить размер - O(n)
    int size() {
        int count = 0;
        Node* current = head;
        while (current != nullptr) {
            count++;
            current = current->next;
        }
        return count;
    }

    // Реверс списка - O(n)
    void reverse() {
        Node* prev = nullptr;
        Node* current = head;
        Node* next = nullptr;

        while (current != nullptr) {
            next = current->next;
            current->next = prev;
            prev = current;
            current = next;
        }

        head = prev;
        cout << "Список реверсирован\n";
    }

    // Печать списка
    void print() {
        if (head == nullptr) {
            cout << "Список пуст\n";
            return;
        }

        Node* current = head;
        cout << "Список: ";
        while (current != nullptr) {
            cout << current->data;
            if (current->next != nullptr) cout << " -> ";
            current = current->next;
        }
        cout << "\n";
    }

    // Деструктор - освобождаем память
    ~LinkedList() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

int main() {
    LinkedList list;

    cout << "=== LINKED LIST DEMO ===\n\n";

    // Тест 1: Добавление элементов
    cout << "--- Тест 1: Добавление ---\n";
    list.pushBack(10);
    list.pushBack(20);
    list.pushBack(30);
    list.pushFront(5);
    list.print();
    cout << "Размер: " << list.size() << "\n\n";

    // Тест 2: Вставка после элемента
    cout << "--- Тест 2: Вставка после ---\n";
    list.insertAfter(20, 25);
    list.print();
    cout << "\n";

    // Тест 3: Поиск
    cout << "--- Тест 3: Поиск ---\n";
    cout << "Поиск 25: " << (list.search(25) ? "найден" : "не найден") << "\n";
    cout << "Поиск 100: " << (list.search(100) ? "найден" : "не найден") << "\n\n";

    // Тест 4: Удаление
    cout << "--- Тест 4: Удаление ---\n";
    list.popFront();
    list.print();
    list.popBack();
    list.print();
    list.remove(20);
    list.print();
    cout << "\n";

    // Тест 5: Реверс
    cout << "--- Тест 5: Реверс ---\n";
    list.reverse();
    list.print();
    cout << "\n";

    // Тест 6: Операции с пустым списком
    cout << "--- Тест 6: Пустой список ---\n";
    LinkedList empty;
    empty.popFront();
    empty.print();

    return 0;
}
