#include <iostream>
#include <string>
#include <vector>
// #include <conio.h>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <map>
#include <cctype>
#include <sstream>  
#include <iomanip>
#include <set>
#include <queue>
#include <stack> 
#include <utility>
#include <deque>
#include <memory>
 
using namespace std;

void myDelete(int* p) {
    cout << "Deleting " << *p << endl;
    delete p;
}

int main(){
    shared_ptr<int> p1(new int(99), myDelete);
    shared_ptr<int> n=make_shared<int>(42);
    shared_ptr<int> n1=n;
    weak_ptr<int> n2=n;
    *n1=25;
    n.reset(new int(20));
    shared_ptr<int> sp(new int[10], [](int* p){ delete[] p; });

    allocator<string> alloc;
    auto p = alloc.allocate(5);  // память на 5 string, но не инициализированы
    alloc.construct(p, "hello");  // инициализация
    alloc.destroy(p);  // уничтожение объекта
    alloc.deallocate(p, 5);


}