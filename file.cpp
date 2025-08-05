#include <iostream>
using namespace std;

int main() {
    int a = 5;
    int b = 0;
    int result = a / b; // This line will cause a division by zero error
    cout << "Result: " << result << endl;
    return 0;
}
