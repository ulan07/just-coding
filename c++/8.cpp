#include <iostream>
#include <string>
#include <utility>

// --- 1. ПРАВИЛЬНЫЙ БАЗОВЫЙ КЛАСС (Вернули все операции) ---
class Quote {
public:
    Quote() { std::cout << "1. Конструктор Quote\n"; }
    
    // ВАЖНО: Если мы хотим быстрых потомков, мы обязаны ЯВНО вернуть 
    // все 5 функций управления копированием через = default.
    Quote(const Quote&) = default;
    Quote(Quote&&) = default;
    Quote& operator=(const Quote&) = default;
    Quote& operator=(Quote&&) = default;
    
    virtual ~Quote() { std::cout << "3. Деструктор Quote\n"; }
};

// --- 2. ПРОМЕЖУТОЧНЫЙ КЛАСС ---
class Disc_quote : public Quote {
public:
    Disc_quote() { std::cout << "2. Конструктор Disc_quote\n"; }
    virtual ~Disc_quote() { std::cout << "2. Деструктор Disc_quote\n"; }
};

// --- 3. КЛАСС-ПОТОМОК ---
class Bulk_quote : public Disc_quote {
public:
    Bulk_quote() { std::cout << "3. Конструктор Bulk_quote\n"; }
    ~Bulk_quote() override { std::cout << "1. Деструктор Bulk_quote\n"; }
};

// --- СИТУАЦИЯ С БЛОКИРОВКОЙ (Пример B и D из книги) ---
class B {
public:
    B() = default;
    B(const B&) = delete; // ЯВНО ЗАПРЕТИЛИ КОПИРОВАНИЕ
};

class D : public B {
    // Ничего не пишем, надеемся на компилятор
};

int main() {
    std::cout << "--- Создание объекта ---\n";
    {
        Bulk_quote bulk; 
        // В консоли мы увидим строгий порядок создания:
        // Сначала Quote -> потом Disc_quote -> в самом конце Bulk_quote
    } 
    // При выходе из блока } деструкторы сработают в ОБРАТНОМ порядке:
    // Сначала Bulk_quote -> потом Disc_quote -> в самом конце деструктор корня Quote
    std::cout << "--- Объект уничтожен ---\n\n";

    // --- Проверка заблокированного класса D ---
    D d1; // ОК: Дефолтный конструктор родителя B открыт
    
    // D d2(d1); 
    // ОШИБКА КОМПИЛЯЦИИ! Компилятор автоматически удалил конструктор копирования у класса D,
    // потому что родитель B запретил копировать свою часть объекта (B(const B&) = delete).

    // D d3(std::move(d1)); 
    // ОШИБКА КОМПИЛЯЦИИ! Так как у родителя B запрещено перемещение, 
    // потомок D тоже не может быть перемещен.

    return 0;
}
