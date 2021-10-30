#include <iostream>

using namespace std;

void doNothing() {};

int main() 
{
    int one = 1;
    int * two = new int(2);
    void (*pointerFunction)() = doNothing;

    cout << "Address on stack: " << &one << endl
         << "Address on heap:  " << two << endl
         << "Address on code:  " << (void *) pointerFunction << endl;

    delete two;
}