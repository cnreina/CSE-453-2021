
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
*   Application to demonstrate vulnerabilities and how to exploit them:
*
*   1 Array Index
*   2 Pointer Subterfuge (already done for you. See the textbook)
*   3 ARC Injection
*   4 VTable Spraying
*   5 Stack Smashing
*   6 Heap Spraying
*   7 Integer Overflow
*   8 ANSI-Unicode Conversion
*
*********************************************************************/

#include <iostream>
#include <iomanip>
#include <string>
#include <array>
#include <cassert>
using namespace std;

void interact();
void displayResult(string message);
void displayAboutUs();
void displayMenu();
void displayHeader();
void clearScreen();

void arrayVulnerability(int gimmieIndex);
void arcVulnerability(char userInput[]);
void pointerSubterfugeVulnerability(char myArray, char *secretPointer, char *publicPointer);
void VTableSprayingVulnerability(struct myVtable);
void stackVulnerability(char vulnerableText);

void arrayWorking();
void arrayExploit();
void arcWorking();
void arcExploit();
void pointerWorking();
void pointerExploit();
void stackWorking();
void stackExploit();

void testArrayIndex();
void testPointerSubterfuge();
void testArcInjection();
void testVTableSpraying();
void testStackSmashing();
void testHeapSpraying();
void testIntegerOverflow();
void testAnsiUnicode();

void myPointerMethod() {}


/*********************************************************************
 * V-TABLE SPRAYING
 * 1. The vulnerable class must be polymorphic.
 * 2. The class must have a buffer as a member variable.
 * 3. Through some vulnerability, there must be a way for user input
 *    to overwrite parts of the V-Table.
 * 4. After a virtual function pointer is overwritten, the virtual
 *    function must be called.
*********************************************************************/
class Vulnerability
{
    public:
        long buffer[1];
        virtual void vtable();
        virtual void vtableUnsafe();
};

/*********************************************************************
 * V-TABLE SPRAYING
*********************************************************************/
void Vulnerability::vtable()
{
    cout << "Safe " << endl;
};

/*********************************************************************
 * V-TABLE SPRAYING
*********************************************************************/
void Vulnerability::vtableUnsafe()
{
    cout << "Unsafe method call" << endl;
};

/*********************************************************************
 * V-TABLE SPRAYING
 * Invokes class Vulnerability with non-malicious input.
*********************************************************************/
void vTableWorking()
{
    Vulnerability v;
    v.vtable();
};

/*********************************************************************
 * V-TABLE SPRAYING
 * Invokes class Vulnerability with malicious input.
*********************************************************************/
void vTableExploit()
{
    Vulnerability v;

    v.buffer[2] = 1;
    
    v.vtable();

};

/*********************************************************************
 * V-TABLE SPRAYING
*********************************************************************/
void testVTableSpraying()
{
    cout << "V-TABLE SMASHING TEST:\n\n";

    vTableWorking();
    vTableExploit();
};

/*********************************************************************
 * MAIN
*********************************************************************/
int main()
{
    interact ();
    return 0;
};

/*********************************************************************
 * ARRAY INDEX
 *  1. There must be an array and an array index variable
 *  2. The array index variable must be reachable through external input.
 *  3. There must not be bounds checking on the array index variable.
*********************************************************************/
void arrayVulnerability(int gimmieIndex) 
{
    int sheepArray[] = {1,2,3,4,5};
    cout << "\tArray Value: " << sheepArray[gimmieIndex] << "\n";
    sheepArray[gimmieIndex] = -1;
};

/*********************************************************************
 * ARRAY INDEX
 * Calls arrayVulnerability() with non-malicious input.
*********************************************************************/
void arrayWorking()
{
    cout << "\tCalling arrayVulnerability() with non-malicious input.\n\n"
         << "\tResult:\n\n";

    arrayVulnerability(4);
};

/*********************************************************************
 * ARRAY INDEX
 * Calls arrayVulnerability() with malicious input.
*********************************************************************/
void arrayExploit()
{
    cout << "\tCalling arrayVulnerability() with malicious input.\n\n"
         << "\tResult:\n\n";

    arrayVulnerability(7);
    
};

/*********************************************************************
 * ARRAY INDEX
*********************************************************************/
void testArrayIndex() 
{    
    cout << "ARRAY INDEX TEST:\n\n";
    arrayWorking();
    arrayExploit();
};

/*********************************************************************
 * ARC INJECTION
 *  1 There must be a function pointer used in the code.
 *  2 Through some vulnerability, there must be a way for user input
 *    to overwrite the function pointer. This typically happens
 *    through a stack buffer vulnerability.
 *  3 After the memory is overwritten, the function pointer must be
 *    dereferenced.
 * 
 * Overwrites a function pointer so, when it is dereferenced, a
 * different function from the intended one gets executed.
 * 
 * sizeof(userInput) / sizeof(userInput[0]);
*********************************************************************/
void arcVulnerability(char userInput[])
{
    string message;
    int counter;
    int size = 60;
    char buffer[6];

    try
    {
        for (counter = 0; counter < size; counter++) {
            buffer[counter] = userInput[counter];
            message.push_back(userInput[counter]);
        };

        void (*functionPointer)(string) = &displayResult;
        functionPointer(message);
    }
    catch (exception exept)
    {
        cout << "\tOOPS! Program crashed!\n\n";
        return;
    }
};

/*********************************************************************
 * ARC INJECTION
 * calls arcVulnerability() with non-malicious input.
*********************************************************************/
void arcWorking()
{
    cout << "\tCalling arcVulnerability() with non-malicious input.\n\n"
         << "\tResult:\n\n";

    char userInput[] = {'h','e','l','l','o'};
    arcVulnerability(userInput);
};

/*********************************************************************
 * ARC INJECTION
 * calls arcVulnerability() with malicious input.
*********************************************************************/
void arcExploit()
{
    cout << "\tCalling arcVulnerability() with malicious input.\n\n"
         << "\tResult:\n\n";
    
    char userInput[] = {'h','e','l','l','o','a','t','t','a','c','k','t','h','p','o','i','n','t','e','r'};
    int size = 6;
    arcVulnerability(userInput);
};

/*********************************************************************
 * ARC INJECTION
*********************************************************************/
void testArcInjection()
{
    cout << "ARC INJECTION TEST:\n\n";
    arcWorking();
    arcExploit();

};

/*********************************************************************
 * POINTER SUBTERFUGE
 * 1. There must be a pointer used in the code.
 * 2. Through some vulnerability, there must be a way for user input
 *    to overwrite the pointer.
 *    This typically happens through a stack buffer vulnerability.
 * 3. After being overwritten, the pointer must be dereferenced.
*********************************************************************/
void pointerSubterfugeVulnerability(char myArray, char *secretPointer, char *publicPointer)
{
    cout << *publicPointer << "\n";
};

/*********************************************************************
 * POINTER SUBTERFUGE
 * calls pointerSubterfugeVulnerability() with non-malicious input.
*********************************************************************/
void pointerWorking()
{
    cout << "\tCalling pointerSubterfugeVulnerability() with non-malicious input.\n\n"
         << "\tResult:\n\n";

    // char myArray[7];
    // char * secretPointer = "arglefraster";
    // char * publicPointer = "princess cimorene";

    // pointerSubterfugeVulnerability(*myArray, secretPointer, publicPointer);
};

/*********************************************************************
 * POINTER SUBTERFUGE
 * calls pointerSubterfugeVulnerability() with malicious input.
*********************************************************************/
void pointerExploit()
{
    cout << "\tCalling pointerSubterfugeVulnerability() with malicious input.\n\n"
         << "\tResult:\n\n";
    
    // char myArray[7];
    // char * secretPointer = "arglefraster";
    // char * publicPointer = "princess cimorene";

    // pointerSubterfugeVulnerability(*myArray, secretPointer, publicPointer);
};

/*********************************************************************
 * POINTER SUBTERFUGE
*********************************************************************/
void testPointerSubterfuge()
{
    cout << "POINTER SUBTERFUGE TEST:\n\n";

    pointerWorking();
    pointerExploit();
};


/*********************************************************************
 * STACK SMASHING
 * 1. There must be a buffer (such as an array) on the stack.
 * 2. The buffer must be reachable from an external input.
 * 3. The mechanism to fill the buffer from the external input must
 *    not correctly check for the buffer size.
 * 4. A buffer overrun (extend beyond the intended limits of the array)
 *    must extend to the return address on the stack.
*********************************************************************/
void stackVulnerability(char vulnerableText)
{
    cout << "text value : " << vulnerableText << endl;
};

/*********************************************************************
 * STACK SMASHING
 * Calls stackVulnerability() with non-malicious input.
*********************************************************************/
void stackWorking()
{
    cout << "\tCalling stackVulnerability() with non-malicious input.\n\n"
         << "\tResult:\n\n";

    char* buffer1 = new char[5];
    buffer1[0] = 'h';
    buffer1[1] = 'e';
    buffer1[2] = 'l';
    buffer1[3] = 'l';
    buffer1[4] = 'o';
    stackVulnerability(*buffer1);

};

/*********************************************************************
 * STACK SMASHING
 * Calls stackVulnerability() with malicious input.
*********************************************************************/
void stackExploit()
{
    cout << "\tCalling stackVulnerability() with malicious input.\n\n"
         << "\tResult:\n\n";
    
    char* buffer1 = new char[5];
    stackVulnerability(*buffer1);

};

/*********************************************************************
 * STACK SMASHING
*********************************************************************/
void testStackSmashing()
{
    cout << "POINTER SUBTERFUGE TEST:\n\n";
    stackWorking();
    stackExploit();
};

/*********************************************************************
 * 1. There must be two adjacent heap buffers.
 * 2. The first buffer must be reachable through external input.
 * 3. The mechanism to fill the buffer from the external input must
 *    not correctly check for the buffer size.
 * 4. The second buffer must be released before the first.
 * 5. The first buffer must be overrun (extend beyond the intended
 *    limits of the array).
*********************************************************************/
void heapVulnerability(string input)
{
    char* buffer1 = new char[5]; // requires two buffers on the heap
    char* buffer2 = new char[5];

    //assert(buffer1 < buffer2); // buffer 1 must be before buffer 2!
    
    int i = 0;
    try {
        // Copy input to buffer1
        for (string::iterator it = input.begin(); it != input.end(); ++it)
        {
            buffer1[i++] = *it;
        }

        // Delete buffers
        delete[] buffer2; // need to delete second buffer first
        delete[] buffer1;
    }
    
    catch (exception ex) {
        cout << "Congratulations, your heap has crashed. Please have a good one!\n\n";
    }

    return;
};

void heapWorking()
{
    cout << "   Calling heapVulnerability() with non-malicious input.\n\n"
         << "   Result:\n\n";

    heapVulnerability("Test");
    cout << "Test worked.\n\n";
};

void heapExploit()
{
    cout << "   Calling heapVulnerability() with malicious input.\n\n"
         << "   Result:\n\n";

    heapVulnerability("This is too long!");
    cout << "If this prints, the program didn't crash. It's a miracle!\n\n";
};

void testHeapSpraying()
{
    cout << "HEAP SPRAYING TEST:\n\n";
    heapWorking();
    heapExploit();
};

/**************************
 * Integer Overflow
 **************************/
void intVulnerability(int offset) {
    char buffer[256];
    char* sentinel = buffer + 256;

    if (offset + buffer < sentinel)
        cout << "Security check passed! :)";
    else
        cout << "Security check failed. :(" << endl;
    return;

};

void intWorking()
{
    cout << "\tCalling intVulnerability() with non-malicious input.\n\n"
         << "\tResult:\n\n";

    int n = 255;
    cout << "Offset: " << n << endl;
    intVulnerability(n);
};

void intExploit()
{
    cout << "\tCalling intVulnerability() with malicious input.\n\n"
         << "\tResult:\n\n";
    
    long n = 3000000000000000;
    cout << "Offset: " << n << endl;
    intVulnerability((int) n);

};

void testIntegerOverflow()
{
    cout << "INTEGER OVERFLOW TEST:\n\n";
    intWorking();
    intExploit();
};


/**************************
 * ANSI-Unicode Conversion
 **************************/

void ansiVulnerability(short * unicodeText1, int buffSize)
{
    short unicodeText2[20];
    
    // Copy unicodeText1 to unicodeText2
    for (int i = 0; i < buffSize; i++) 
    {
        unicodeText2[i] = unicodeText1[i];
    }

    cout << "Message:" << endl;
    for (int i = 0; i < 20; i++) 
    {
        if (unicodeText2[i] >= ' ' && unicodeText2[i] <= 'z')
            cout << (char) unicodeText2[i];
    }

    cout << endl;
};

void ansiWorking()
{
    short message[] = {'H', 'E', 'L', 'L', 'O'};
    ansiVulnerability(message, sizeof(message));
};

void ansiExploit()
{
    short message[] = {'U',' ','R',' ','H','A','C','K','E','D','!','!'};
    ansiVulnerability(message, sizeof(message));
};

void testAnsiUnicode() 
{
    cout << "ANSI-Unicode Conversion\n"
         << "\tWorking\n\n";

    ansiWorking();

    cout << "\tExploit\n\n";

    ansiExploit();
};

/*********************************************************************
*  clearScreen()
*  Called by interact()
*  Clears the screen.
*********************************************************************/
void clearScreen()
{
    if (system("CLS")) { system("clear"); }
    return;
};

/*********************************************************************
*  displayHeader()
*  Called by interact()
*  Displays app identifier.
*********************************************************************/
void displayHeader()
{
    cout << "\n************ CSE 453 ************\n\n";
    return;
};

/*********************************************************************
*  displayMenu()
*  Called by interact()
*  Displays menu options.
*********************************************************************/
void displayMenu()
{
    cout << "OPTIONS:\n"
        << "   A  Array Index\n"
        << "   B  Pointer Subterfuge\n"
        << "   C  ARC Injection\n"
        << "   D  VTable Spraying\n"
        << "   E  Stack Smashing\n"
        << "   F  Heap Spraying\n"
        << "   G  Integer Overflow\n"
        << "   H  ANSI-Unicode Conversion\n"
        << "   I  About Us\n"
        << "   Q  Quit\n\n";

    return;
};

/*********************************************************************
*  displayAboutUs()
*  Called by interact()
*  Displays authors.
*********************************************************************/
void displayAboutUs()
{
    cout << "AUTHORS:\n"
        << "   Carlos N Reina\n"
        << "   Adrian Whetten\n"
        << "   Jordan Burdett\n"
        << "   Caleb Georgeson\n"
        << "   Orion Christensen\n"
        << "   Russell Roberts\n"
        << "   Stephen Ellis\n\n";

    return;
};

/*********************************************************************
*  displayResult()
*  Called by interact()
*  Displays results.
*********************************************************************/
void displayResult(string message)
{
    cout << message << "\n\n";
    return;
};

/*********************************************************************
*   interact(), until user types "Q".
*   Called by main.
*********************************************************************/
void interact()
{
    clearScreen();
    displayHeader();
    displayMenu();

    char answer[2] = "";
    string returnString;
    do
    {
        if (cin.fail()) // bad input
        {
            cin.clear();
            cin.ignore();
            continue;
        }

        cin.getline(answer, 2);
        if (islower(answer[0])) { answer[0] = toupper(answer[0]); }
        returnString.clear();
        switch (answer[0])
        {
        case '\0': // no input
            clearScreen();
            displayHeader();
            displayMenu();
            break;

        case 'A': // Array Index
            clearScreen();
            displayHeader();
            testArrayIndex();
            displayMenu();
            break;

        case 'B': // Pointer Subterfuge
            clearScreen();
            displayHeader();
            testPointerSubterfuge();
            displayMenu();
            break;

        case 'C': // Arc Injection
            clearScreen();
            displayHeader();
            testArcInjection();
            displayMenu();
            break;

        case 'D': // VTable Spraying
            clearScreen();
            displayHeader();
            testVTableSpraying();
            displayMenu();
            break;

        case 'E': // Stack Smashing
            clearScreen();
            displayHeader();
            testStackSmashing();
            displayMenu();
            break;

        case 'F': // Heap Spraying
            clearScreen();
            displayHeader();
            testHeapSpraying();
            displayMenu();
            break;

        case 'G': // Integer Overflow
            clearScreen();
            displayHeader();
            testIntegerOverflow();
            displayMenu();
            break; 

        case 'H': // ANSI-Unicode Conversion
            clearScreen();
            displayHeader();
            testAnsiUnicode();
            displayMenu();
            break; 

        case 'I': // About Us
            clearScreen();
            displayHeader();
            displayAboutUs();
            displayMenu();
            break;

        case 'Q': // Quit
            clearScreen();
            break;

        default:
            clearScreen();
            displayHeader();
            displayResult("ERROR: Invalid command");
            displayMenu();
            break;
        }
    } while (answer[0] != 'Q' && answer[0] != 'q');

    return;
};
