#include <iostream>
#include <vector>
#include <memory>
#include <string>

// Базовый класс (из прошлых глав)
class Quote {
public:
    Quote(const std::string &book, double sales_price) : bookNo(book), price(sales_price) { }
    virtual double net_price(std::size_t n) const { return n * price; }
    virtual ~Quote() = default;
private:
    std::string bookNo;
protected:
    double price = 0.0;
};

// Класс-потомок со скидкой
class Bulk_quote : public Quote {
public:
    Bulk_quote(const std::string& book, double price, std::size_t qty, double disc) :
        Quote(book, price), min_qty(qty), discount(disc) { }
    
    double net_price(std::size_t cnt) const override {
        if (cnt >= min_qty) return cnt * price * (1.0 - discount);
        return cnt * price;
    }
private:
    std::size_t min_qty = 0;
    double discount = 0.0;
};

int main() {
    // --- EXERCISE 15.28: Контейнер ОБЪЕКТОВ (Ловушка срезки) ---
    std::vector<Quote> object_basket;
    
    // Кладем оптовую книгу (100 руб, скидка 20% при покупке от 10 шт)
    object_basket.push_back(Bulk_quote("123-45", 100.0, 10, 0.2));
    
    double total_objects = 0.0;
    for (const auto& item : object_basket) {
        total_objects += item.net_price(15); // Покупаем 15 штук
    }
    std::cout << "Сумма в корзине объектов: " << total_objects << " руб.\n";
    // РЕЗУЛЬТАТ: 1500 руб. (Скидка ИГНОРИРУЕТСЯ, произошла срезка!)


    // --- EXERCISE 15.29: Контейнер УМНЫХ УКАЗАТЕЛЕЙ (Правильный полиморфизм) ---
    std::vector<std::shared_ptr<Quote>> pointer_basket;
    
    // Кладем точно такую же книгу, но через умный указатель
    pointer_basket.push_back(std::make_shared<Bulk_quote>("123-45", 100.0, 10, 0.2));
    
    double total_pointers = 0.0;
    for (const auto& item : pointer_basket) {
        total_pointers += item->net_price(15); // Используем -> для вызова через указатель
    }
    std::cout << "Сумма в корзине указателей: " << total_pointers << " руб.\n";
    // РЕЗУЛЬТАТ: 1200 руб. (Скидка СРАБОТАЛА! Полиморфизм успешен)

    return 0;
}
