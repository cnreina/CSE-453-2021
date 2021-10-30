
/*********************************************************************
*   CSE 453
*
*   Carlos N Reina
*   Adrian Whetten
*   Jordan Burdett
*   Caleb Georgeson
*   Russell Roberts
*   Stephen Ellis
*   Orion Christensen
*
*********************************************************************/

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cassert>
using namespace std;

void one(long number);
void two(long number);
void pass() { cout << "You pass :)\n"; }
void fail() { cout << "You've failed :(\n"; }
const char * passMessage = ":)";
const char * failMessage = ":(";

/**********************************************
 * MAIN : The top of the callstack.
 **********************************************/
int main()
{
    char text[8] = "*MAIN**";
    // long number = 123456;
    long number = 654321;
    void (*pointerFunction)() = fail;
    const char* message = failMessage;

    // display the initial values of the local variables
    cout << "main() : " << (void*)main << endl;
    cout << "\ttext:             " << text << endl;
    cout << "\tnumber:           " << number << endl;
    cout << "\tmessage:          " << message << endl;
    cout << "\tfunction pointer: ";
    pointerFunction();

    // call the other functions
    one(number + 111111);     // 234567

    // display the new values of the local variables
    cout << "main() - after\n";
    cout << "\ttext:             " << text << endl;
    cout << "\tnumber:           " << number << endl;
    cout << "\tmessage:          " << message << endl;
    cout << "\tfunction pointer: ";
    pointerFunction();

    return 0;
}

/************************************************
 * CONVERT TO STRING
 * Convert the data from p into a human-readable string
 * by removing all the unprintable characters and replacing
 * them with a dot
 ***********************************************/
string displayCharArray(const char* p)
{
    string output;
    for (int i = 0; i < 8; i++)
        output += string(" ") + (p[i] >= ' ' && p[i] <= 'z' ? p[i] : '.');
    return output;
}

string longToHexString(long number)
{
    stringstream stream;
    stream << hex << number;

    return "0x" + stream.str();
}

/**********************************************
 * ONE : The next item on the call stack
 **********************************************/
void one(long number)               // 234567
{
    char text[8] = "**ONE**";

    cout << "one() : " << (void*)one << endl;
    cout << "\tmessage: " << (void*)failMessage << endl;
    cout << "\tfail():  " << (void*)fail << endl;

    two(number + 111111);    // 345678
}

/**********************************************
 * TWO : The bottom of the call stack
 **********************************************/
void two(long number)              // 345678
{
    // start your display of the stack from this point
    long bow = number + 111111;     // 456789
    char text[8] = "**TWO**";
    long* pLong = NULL;
    char* pChar = NULL;

    // header for our table. Use these setw() offsets in your table
    cout << '[' << setw(2) << 'i' << ']'
        << setw(15) << "address"
        << setw(20) << "hexadecimal"
        << setw(20) << "decimal"
        << setw(18) << "characters"
        << endl;
    cout << "----+"
        << "---------------+"
        << "-------------------+"
        << "-------------------+"
        << "-----------------+\n";
   for (long i = 30; i >= -4; i--)   // You may need to change 24 to another number
   {
      ////////////////////////////////////////////////
      // Insert code here to display the callstack
        // Point pLong to something 'i' steps above bow
        pLong = &bow + i;

        // Point pChar to the same place, but cast it as a char *
        pChar = (char *) pLong;

        cout << '[' << setw(2) << i << ']'
             << setw(15) << pLong                   // Print where pLong is pointing to
             << setw(20) << longToHexString(*pLong) // Print the contents in hex
             << setw(20) << *pLong                  // Print the contents in decimal
             << setw(18) << displayCharArray(pChar) // Treat the contents as a char array and print the result
             << endl;
      //
      ////////////////////////////////////////////////
   }

   ////////////////////////////////////////////////
   // Insert code here to change the variables in main()
                                                                                
   // change text in main() to "*main**"
   // The table shows "*MAIN**" is 28 steps above bow (THIS MAY CHANGE ON A DIFFERENT MACHINE)
    pLong = &bow + 28;        // Point pLong 28 steps above bow
    pChar = (char *) pLong;   // Treat it as a char array
    pChar[1] = 'm';           // Modify indices 1-4
    pChar[2] = 'a';
    pChar[3] = 'i';
    pChar[4] = 'n';           // text in main is now "*main**"
    
   // change number in main() to 654321
   // The table shows 123456 is 25 steps above bow (again, this could be different for a separate machine)
   pLong = &bow + 25;         // Point pLong 25 steps above bow
   *pLong = 654321;           // Change the value to 654321
                              // number in main is now 654321

   // change pointerFunction in main() to point to pass
   // The table shows that pointerFunction is 26 steps above bow (same as the rest, this might be different)
   pLong = &bow + 26;         // Point pLong 26 steps above bow
   *pLong = (long) &pass;     // Change the value of pLong to the address of pass
                              // pointerFunction in main is now pointed to pass

   // change message in main() to point to passMessage
   // The table shows that message is 27 steps above bow
   pLong = &bow + 27;          // Point pLong 27 steps above bow
   *pLong = (long) passMessage;// Change the value of pLong to the address of passMessage
                               // message in main is now pointed to passMessage

   //cout << pPointAtMessage << endl;
   //
   ////////////////////////////////////////////////
}
