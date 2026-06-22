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

unique_ptr<int[]> createNumber(int n){
    unique_ptr<int[]> arr(new int[n]);
    return arr;
}

int main(){
    unique_ptr<string> doc1(new string("Report 2026"));
    
    // Передаем владение из doc1 в новый указатель doc2:
    // release() забирает адрес у doc1 и делает doc1 равным nullptr
    unique_ptr<string> doc2(doc1.release());
    int n;
    cin>>n;
    auto s=createNumber(n);
    for(int i=0;i<n;i++){
        s[i]=i;
    }
    unique_ptr<int> p1(new int(10));
    unique_ptr<int> p2 = move(p1);
    
}