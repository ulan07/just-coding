#include <iostream>
#include <string>

// --- БАЗОВЫЙ КЛАСС (Обычный товар) ---
class Quote {
public:
    Quote() = default; 
    
    Quote(const std::string &book, double sales_price) : 
        bookNo(book), price(sales_price) { }
        
    std::string isbn() const { 
        return bookNo; 
    }
    
    virtual double net_price(std::size_t n) const { 
        return n * price; 
    }
    
    virtual ~Quote() = default; 

private:
    std::string bookNo; 
protected:
    double price = 0.0; 
};

// --- ПРОИЗВОДНЫЙ КЛАСС (Оптовый товар со скидкой) ---
class Bulk_quote : public Quote {
public:
    Bulk_quote() = default;
    
    Bulk_quote(const std::string& book, double p, std::size_t qty, double disc) : 
        Quote(book, p), min_qty(qty), discount(disc) { }

    double net_price(std::size_t cnt) const override {
        if (cnt >= min_qty) {
            return cnt * price * (1.0 - discount); 
        }
        return cnt * price;
    }

private:
    std::size_t min_qty = 0; 
    double discount = 0.0;   
};

// --- ТОЧКА ВХОДА (Исполнение) ---
int main() {
    Quote standardBook("C++ Primer", 100.0);
    Bulk_quote bulkBook("Effective C++", 100.0, 10, 0.2);

    Quote* ptr1 = &standardBook;
    Quote* ptr2 = &bulkBook;

    std::cout << ptr1->isbn() << " price: " << ptr1->net_price(15) << std::endl;
    std::cout << ptr2->isbn() << " price: " << ptr2->net_price(15) << std::endl;

    return 0;
}
