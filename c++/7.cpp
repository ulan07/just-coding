#include <iostream>

// --- БАЗОВЫЙ КЛАСС ---
class Base {
    friend class Pal; // Класс Pal — друг для Base
protected:
    int prot_mem = 10; // Доступен только наследникам и друзьям
private:
    char priv_mem = 'A'; // Доступен ТОЛЬКО внутри самого Base
public:
    void pub_mem() { std::cout << "Вызов Base::pub_mem\n"; }
};

// --- СИТУАЦИЯ 1: Хитрый доступ к protected ---
class Sneaky : public Base {
    friend void clobber(Sneaky&); 
    friend void clobber(Base&);   
    int j = 0;
};

// ОК: Друг класса Sneaky может менять protected-члены Base, но ТОЛЬКО через объект Sneaky!
void clobber(Sneaky &s) { 
    s.j = 0; 
    s.prot_mem = 0; 
}

// ОШИБКА: Тот же самый друг Sneaky НЕ имеет права трогать prot_mem у чистого объекта Base.
// void clobber(Base &b) { 
//     b.prot_mem = 0; 
// }


// --- СИТУАЦИЯ 2: Виды наследования (Public против Private) ---

// Публичное наследование: всё сохраняет свои права
struct Pub_Derv : public Base {
    int f() { return prot_mem; } // ОК: Потомку доступен protected родителя
    // char g() { return priv_mem; } // ОШИБКА: private родителя закрыт навсегда!
};

// Приватное наследование: делает ВСЕ унаследованные члены приватными для внешнего мира
struct Priv_Derv : private Base {
    int f1() const { return prot_mem; } // ОК: Внутри самого класса доступ к protected все еще ЕСТЬ
};


// --- СИТУАЦИЯ 3: Наследование от наследников (Цепочка) ---

struct Derived_from_Public : public Pub_Derv {
    int use_base() { return prot_mem; } // ОК: prot_mem остался protected в Pub_Derv
};

struct Derived_from_Private : public Priv_Derv {
    // int use_base() { return prot_mem; } 
    // ОШИБКА! Внутри Priv_Derv все члены дедушки превратились в private. 
    // Поэтому внук Derived_from_Private к ним доступ потерял.
};


// --- СИТУАЦИЯ 4: Дружба и наследование (Friendship) ---
class Pal {
public:
    int f(Base b) { return b.prot_mem; } // ОК: Pal — друг для Base, видит его protected
    // int f2(Sneaky s) { return s.j; }   // ОШИБКА: Дружба не наследуется. Pal не друг для Sneaky!
    
    // ОК: Pal может заглянуть в "базовую часть", которая сидит внутри Sneaky
    int f3(Sneaky s) { return s.prot_mem; } 
};

int main() {
    Pub_Derv d1; 
    Priv_Derv d2; 

    d1.pub_mem(); // ОК: При public-наследовании pub_mem остался публичным
    // d2.pub_mem(); // ОШИБКА! При private-наследовании pub_mem стал приватным методом d2

    // --- Преобразование потомка в родителя (Derived-to-Base) ---
    Base* p1 = &d1; // ОК: Пользовательский код может неявно приводить к Base, так как наследование public
    // Base* p2 = &d2; // ОШИБКА! Пользовательский код НЕ может привести d2 к Base, так как наследование private

    return 0;
}
