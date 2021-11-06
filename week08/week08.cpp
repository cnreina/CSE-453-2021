
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
*  VTable Spraying
*  class vulnerable to vtable smashing attack.
*  there must be a method or two in this class that is vulnerable.
*  Hint: You will need two classes to do this: a base class and a derived class.
*********************************************************************/
class Vulnerability
{
    /*
    instantiates a Vulnerability object and calls the vulnerable method.
    this method will behave normally because malicious input is not passed.
    Provides output from this function. 
    */
    public:
        long buffer[1];
        virtual void vtable();
        virtual void vtableUnsafe();
    /*
    demonstrates vtable spraying.
    In other words, it is not necessary to demonstrate vtable smashing, but 
    rather just spraying.
    Provides output from this function. 
    */
};
void Vulnerability::vtable()
{
 cout << "Safe " << endl;
}

void Vulnerability::vtableUnsafe()
{
 cout << "Unsafe method call" << endl;
}

void vTableWorking()
{
    Vulnerability v;
    v.vtable();
};

void vTableExploit()
{
    Vulnerability v;

    v.buffer[2] = 1;
    
    v.vtable();

};

/**********************************************
 * MAIN : 
 **********************************************/
int main()
{
    interact ();
    return 0;
};

/*********************************************************************
*  ARRAY VULNERABILTY
*   1. There must be an array and an array index variable
*   2. The array index variable must be reachable through external input.
*   3. There must not be bounds checking on the array index variable.
*
*  See page 137
*********************************************************************/
void arrayVulnerability(int gimmieIndex) 
{
    int sheepArray[] = {1,2,3,4,5};  // Array with all the values.
    cout << "    Array Value: " << sheepArray[gimmieIndex] << endl;  // Print out the array value.

    sheepArray[gimmieIndex] = -1;  //book says "if index == 4, problem! therefore for us, if index == 5 problem?"

};

/*********************************************************************
* ARRAY WORKING
* Call arrayVulnerability() in a way that does
* not yield unexpected behavior
* 
*********************************************************************/
void arrayWorking()
{
    cout << "   Calling arrayVulnerability() with non-malicious input.\n\n"
         << "   Result:\n\n";

    arrayVulnerability(4);
};

/*********************************************************************
* ARRAY EXPLOIT
 * 1. The attacker provides an array index value outside the expected range
 * 2. The attacker must be able to provide input or redirect
 *    existing input into the array at the index he provided
 * 3. The injected value must alter program state in a way
 *    that is desirable to the attacker
*********************************************************************/
void arrayExploit()
{
    cout << "   Calling arrayVulnerability() with malicious input.\n\n"
         << "   Result:\n\n";

    arrayVulnerability(7);
    
};

/*********************************************************************
*  ArrayIndex hack  from the book, page 137
*  Called by interact()
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
 *  2 Through some vulnerability, there must be a way for user input to
 *    overwrite the function pointer. This typically happens through a 
 *    stack buffer vulnerability.
 *  3 After the memory is overwritten, the function pointer must be dereferenced.
 * 
 * Overwriting a function pointer so, when it is dereferenced, a
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
    for (counter = 0; counter < size; counter++) {
        buffer[counter] = userInput[counter];
        message.push_back(userInput[counter]);
    };

    void (*functionPointer)(string) = &displayResult;
    functionPointer(message);
};

/*********************************************************************
 *  calls arcVulnerability() with non-malicious input.
 *  the vulnerability function will behave normally. 
 *  Provides output from this function.
*********************************************************************/
void arcWorking()
{
    cout << "   Calling arcVulnerability() with non-malicious input.\n\n"
         << "   Result:\n\n";

    char userInput[] = {'h','e','l','l','o'};
    arcVulnerability(userInput);
};

/*********************************************************************
 * EXPLOIT ARC INJECTION
 *  calls arcVulnerability().
 *  exploits ARC injection. 
 *  Provide output from this function.
*********************************************************************/
void arcExploit()
{
    cout << "   Calling arcVulnerability() with malicious input.\n\n"
         << "   Result:\n\n";
    
    char userInput[] = {'h','e','l','l','o','a','t','t','a','c','k','t','h','p','o','i','n','t','e','r'};
    int size = 6;
    arcVulnerability(userInput);
};

/*********************************************************************
 * TEST ARC INJECTION
 *  1 Calls arcWorking()
 *  2 Calls arcExploit()
*********************************************************************/
void testArcInjection()
{
    cout << "ARC INJECTION TEST:\n\n";
    arcWorking();
    arcExploit();

};

/*********************************************************************
 * 1. There must be a pointer used in the code.
 * 2. Through some vulnerability, there must be a way for user input
 *    to overwrite the pointer.
 *    This typically happens through a stack buffer vulnerability.
 * 3. After the pointer is overwritten, the pointer must be dereferenced.
*********************************************************************/
void pointerSubterfugeVulnerability(char myArray, char *secretPointer, char *publicPointer)
{
    cout << *publicPointer << "\n";
};

/*********************************************************************
 * 
*********************************************************************/
void pointerWorking()
{
    // cout << "   Calling pointerSubterfugeVulnerability() with non-malicious input.\n\n"
    //      << "   Result:\n\n";

    // char myArray[7];
    // char * secretPointer = "arglefraster";
    // char * publicPointer = "princess cimorene";

    // pointerSubterfugeVulnerability(*myArray, secretPointer, publicPointer);
};

/*********************************************************************
*  pointer subterfuge hack from the quiz question 8
*  Called by interact()
*********************************************************************/
void pointerExploit()
{
    // cout << "   Calling pointerSubterfugeVulnerability() with malicious input.\n\n"
    //      << "   Result:\n\n";
    
    // char myArray[7];
    // char * secretPointer = "arglefraster";
    // char * publicPointer = "princess cimorene";

    // pointerSubterfugeVulnerability(*myArray, secretPointer, publicPointer);
};

void testPointerSubterfuge()
{
    cout << "POINTER SUBTERFUGE TEST:\n\n";

    pointerWorking();
    pointerExploit();
};


/*********************************************************************
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

void stackWorking()
{
    cout << "   Calling stackVulnerability() with non-malicious input.\n\n"
         << "   Result:\n\n";

    char* buffer1 = new char[5];
    buffer1[0] = 'h';
    buffer1[1] = 'e';
    buffer1[2] = 'l';
    buffer1[3] = 'l';
    buffer1[4] = 'o';
    stackVulnerability(*buffer1);

};

void stackExploit()
{
    cout << "   Calling stackVulnerability() with malicious input.\n\n"
         << "   Result:\n\n";
    
    char* buffer1 = new char[5];
    stackVulnerability(*buffer1);

};

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
        cout << "Congratulations, your heap has crashed. Please have a good one!" << endl;
    }

    return;
};

void heapWorking()
{
    cout << "   Calling heapVulnerability() with non-malicious input.\n\n"
         << "   Result:\n\n";

    heapVulnerability("Test");
    cout << "Test worked.";
};

void heapExploit()
{
    cout << "   Calling heapVulnerability() with malicious input.\n\n"
         << "   Result:\n\n";

    heapVulnerability("This is too long!");
    cout << "If this prints, the program didn't crash. It's a miracle!\n";
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
    cout << "   Calling intVulnerability() with non-malicious input.\n\n"
         << "   Result:\n\n";

    int n = 255;
    cout << "Offset: " << n << endl;
    intVulnerability(n);
};

void intExploit()
{
    cout << "   Calling intVulnerability() with malicious input.\n\n"
         << "   Result:\n\n";
    
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

void testVTableSpraying()
{
    cout << "V-TABLE SMASHING TEST:\n\n";

    vTableWorking();
    vTableExploit();
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
