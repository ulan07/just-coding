#include <iostream>
#include <string>

// 1. БАЗОВЫЙ КЛАСС (Обычный товар)
class Quote {
public:
    Quote() = default;
    Quote(const std::string &book, double sales_price) : bookNo(book), price(sales_price) { }
    std::string isbn() const { return bookNo; }
    
    // Обычная виртуальная функция (имеет тело по умолчанию)
    virtual double net_price(std::size_t n) const { return n * price; }
    virtual ~Quote() = default;
private:
    std::string bookNo;
protected:
    double price = 0.0;
};

// 2. АБСТРАКТНЫЙ БАЗОВЫЙ КЛАСС (Общие данные для всех скидок)
class Disc_quote : public Quote {
public:
    Disc_quote() = default;
    Disc_quote(const std::string& book, double price, std::size_t qty, double disc) :
        Quote(book, price), quantity(qty), discount(disc) { }

    // ЧИСТАЯ ВИРТУАЛЬНАЯ ФУНКЦИЯ (Заменяем тело функции на = 0)
    double net_price(std::size_t) const override = 0;

protected:
    std::size_t quantity = 0; 
    double discount = 0.0;   
};

// 3. КОНКРЕТНЫЙ КЛАСС (Оптовая скидка)
class Bulk_quote : public Disc_quote {
public:
    Bulk_quote() = default;
    
    // Конструктор инициализирует ТОЛЬКО своего ПРЯМОГО родителя
    Bulk_quote(const std::string& book, double price, std::size_t qty, double disc) :
        Disc_quote(book, price, qty, disc) { }

    // Обязаны переопределить чистую виртуальную функцию, иначе этот класс тоже станет абстрактным
    double net_price(std::size_t cnt) const override {
        if (cnt >= quantity) { // Переменная quantity унаследована от Disc_quote
            return cnt * price * (1.0 - discount); 
        }
        return cnt * price;
    }
};

int main() {
    // --- ПРОВЕРКА РАБОТЫ В МЕМУАРАХ КОМПИЛЯТОРА ---

    // ОШИБКА КОМПИЛЯЦИИ! Disc_quote — абстрактный класс. Создавать его объекты нельзя.
    // Disc_quote abstract_obj; 

    // ОК: Создаем конкретный класс, который переопределил net_price
    Bulk_quote bulk("Effective C++", 100.0, 10, 0.2);

    Quote* ptr = &bulk;
    std::cout << "Price for 15: " << ptr->net_price(15) << std::endl; // Выведет 1200

    return 0;
}
