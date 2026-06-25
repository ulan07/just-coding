#include <iostream>
#include <string>
#include <utility>
#include <memory>
#include <algorithm>

class StrVec {
public:
    // Конструктор по умолчанию: инициализирует всё в nullptr
    StrVec() : elements(nullptr), first_free(nullptr), cap(nullptr) { }

    // Конструктор копирования
    StrVec(const StrVec& s) {
        // Выделяем память и копируем элементы из диапазона [s.begin(), s.end())
        auto newdata = alloc_n_copy(s.begin(), s.end());
        elements = newdata.first;
        first_free = newdata.second;
        cap = newdata.second; // В копии капасити равен размеру данных
    }

    // Оператор копирующего присваивания
    StrVec& operator=(const StrVec& rhs) {
        // Сначала копируем данные во временные указатели (безопасность при самоприсваивании)
        auto data = alloc_n_copy(rhs.begin(), rhs.end());
        
        free(); // Освобождаем старую память текущего объекта
        
        elements = data.first;
        first_free = data.second;
        cap = data.second;
        return *this;
    }

    // Деструктор
    ~StrVec() { 
        free(); 
    }

    // Добавление элемента в конец
    void push_back(const std::string& s) {
        chk_n_alloc(); // Проверяем, есть ли свободное место
        alloc.construct(first_free++, s); // Конструируем строку в сырой памяти
    }

    // Интерфейсные методы
    size_t size() const { return first_free - elements; }
    size_t capacity() const { return cap - elements; }
    std::string* begin() const { return elements; }
    std::string* end() const { return first_free; }

private:
    // Статический аллокатор (один на весь класс)
    static std::allocator<std::string> alloc;

    // Проверка места: если массив полон, запускаем reallocate
    void chk_n_alloc() { 
        if (size() == capacity()) reallocate(); 
    }

    // Вспомогательные утилиты управления памятью
    std::pair<std::string*, std::string*> alloc_n_copy(const std::string* b, const std::string* e) {
        auto data = alloc.allocate(e - b); // Выделяем сырую память
        return {data, std::uninitialized_copy(b, e, data)}; // Копируем объекты
    }

    // Полная реализация функции free()
    void free() {
        if (elements) { // Если массив не пустой
            // 1. Уничтожаем созданные объекты в обратном порядке
            for (auto p = first_free; p != elements; /* пусто */) {
                alloc.destroy(--p); 
            }
            // 2. Освобождаем саму сырую память массива
            alloc.deallocate(elements, cap - elements);
        }
    }

    // Переаллокация (расширение) памяти
    void reallocate() {
        // Если массив пустой, выделяем 1 ячейку, иначе — удваиваем текущую емкость
        auto newcapacity = size() ? 2 * capacity() : 1;
        
        // 1. Выделяем новый кусок сырой памяти
        auto newdata = alloc.allocate(newcapacity);
        
        // 2. Перемещаем (или копируем) старые элементы в новую память
        auto dest = newdata;
        auto elem = elements;
        for (size_t i = 0; i != size(); ++i) {
            alloc.construct(dest++, std::move(*elem++)); // std::move избегает лишнего копирования строк
        }
        
        free(); // 3. Уничтожаем старый массив
        
        // 4. Обновляем указатели на новые адреса
        elements = newdata;
        first_free = dest;
        cap = elements + newcapacity;
    }

    // Указатели управления структурой данных
    std::string* elements;    // Начало массива
    std::string* first_free;  // Первый свободный слот
    std::string* cap;         // Конец выделенной памяти
};

// Определение статического члена за пределами класса (обязательно в C++)
std::allocator<std::string> StrVec::alloc;

int main() {
    StrVec v;
    v.push_back("Hello");
    v.push_back("World");

    std::cout << "Size: " << v.size() << std::endl;

    return 0;
}