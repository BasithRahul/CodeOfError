#include <iostream>
using namespace std;

cout<<"I am a C++ program. I am pulling the code"<<endl;
int main() {
    int a = 5;
    int b = 0;
    int result = 0;
    if (b != 0) {
        result = a / b;
        cout << "Result: " << result << endl;
    } else {
        cout << "Error: Division by zero is not allowed." << endl;
        
    }
    return 0;
}
