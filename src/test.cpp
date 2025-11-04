#include "include/int2048.h"
#include <iostream>

using namespace std;
using namespace sjtu;

int main() {
    // Test basic operations
    int2048 a("123");
    int2048 b("456");
    
    cout << "a = " << a << endl;
    cout << "b = " << b << endl;
    cout << "a + b = " << (a + b) << endl;
    cout << "a - b = " << (a - b) << endl;
    cout << "a * b = " << (a * b) << endl;
    cout << "b / a = " << (b / a) << endl;
    cout << "b % a = " << (b % a) << endl;
    
    // Test negative division (floor division)
    int2048 x("10");
    int2048 y("-3");
    cout << "10 / -3 = " << (x / y) << endl;  // Should be -4
    cout << "10 % -3 = " << (x % y) << endl;
    
    int2048 p("-10");
    int2048 q("3");
    cout << "-10 / 3 = " << (p / q) << endl;  // Should be -4
    cout << "-10 % 3 = " << (p % q) << endl;
    
    return 0;
}
