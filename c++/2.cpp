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

int main(){
    int n;
    cin>>n;
    
    int *p=new int(42);
    int *s=new int[n];
    for(int i=0;i<n;i++){
        s[i]=i+1;
    }
    delete p;
    delete[] s;
    
    
}