#include <iostream>
#include <windows.h>


using namespace std;
int main(){  
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    // you can loop k higher to see more color choices
    for(int k = 1; k < 511; k++){
        cout << k << ' ' << (char)k << endl;
    }
}