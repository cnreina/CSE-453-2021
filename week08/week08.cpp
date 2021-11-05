
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
void displayResult(std::string message);
void displayAboutUs();
void displayMenu();
void displayHeader();
void clearScreen();

void arrayVulnerability(int gimmieIndex);
void arcVulnerability(long myVulnerableBuffer);
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
*  Hint: For extra credit, can you demonstrate VTable Smashing?
*
*********************************************************************/
class Vulnerability
{
    /*
    instantiates a Vulnerability object and calls the vulnerable method.
    this method will behave normally because malicious input is not passed.
    Provides output from this function. 
    */
    void vtableWorking(){};
    
    /*
    demonstrates vtable spraying.
    In other words, it is not necessary to demonstrate vtable smashing, but 
    rather just spraying.
    Provides output from this function. 
    */
    void vtableExploit(){};
    
};

/*********************************************************************
*  VTable Spraying
*  Derived class
*********************************************************************/
class VulnerabilityDerived : public Vulnerability
{
    
};

/*********************************************************************
*  vtable for my vtable smash, thank me boys, 
* the original name for the struct was: 
* "VerilyEvincedVisibleAndEvidentVulnerableVtable"
*********************************************************************/
struct VulnerableVTable
{
    /*in the quiz question 5 there is a public:
    word above the single variable and 2 virtual functions*/
    char myText[256];
    virtual void naive();
    virtual void malignant();

};

void VulnerableVTable::naive()
{
};

void VulnerableVTable::malignant()
{ 
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
*  ARRAY INDEX
*********************************************************************/

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
    int sheepArray[] = {1,2,3,4,5};  // a vulnerable soft meek array like a sheep
    cout << "Array Value: " << sheepArray[gimmieIndex] << endl;  // I guess to do array index, we have to have a check that actually makes the array vulnerable rather than safe?

    //int gimmieIndex;
    //cin >> gimmieIndex;
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
    // Pass in valid value to the array vulnerability 
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
    // Pass in an invalid value to exploit the array.
    arrayVulnerability(7);
    
};

/*********************************************************************
*  ARC INJECTION
*********************************************************************/

/*********************************************************************
*  1 There must be a function pointer used in the code.
*  2 Through some vulnerability, there must be a way for user input to
*    overwrite the function pointer. This typically happens through a 
*    stack buffer vulnerability.
*  3 After the memory is overwritten, the function pointer must be dereferenced.
*********************************************************************/
void arcVulnerability(long myVulnerableBuffer)
{
    //long myVulnerableBuffer[8];
    //void (*myPointerMethod)() = &VulnerableVTable::naive();

    //cin >> myVulnerableBuffer[8];  
    
    /*pretty sure above we expose the buffer, 
    and therefore the pointer and method it executes, naked, cold, and afraid
    to the depraved wiles of every nefarious, skulldugerous, and malcisously 
    nasty hacker with their pointy teeth and hairy hands with claws scraped 
    with grit in their fingernails!*/

    myPointerMethod();
};

/*********************************************************************
*  calls arcVulnerability() with non-malicious input.
*  the vulnerability function will behave normally. 
*  Provides output from this function.
*********************************************************************/
void arcWorking()
{

    // needs guts

};

/*********************************************************************
*  calls arcVulnerability().
*  exploits ARC injection. 
*  Provide output from this function.
*********************************************************************/
void arcExploit()
{

    // needs guts

};


/*********************************************************************
*  pointer subterfuge hack from the quiz question 8
*  Called by interact()
*********************************************************************/
void pointerSubterfugeVulnerability(char myArray, char *secretPointer, char *publicPointer)
{

    // quiz question 8 has array[8] i did 7 so we aren't "copying"     

    /*for above, the quiz question 8 makes the number i have "19" bigger than the size of "Citizen Kane",
    which is 12 charand getline is 17, so my "princess cimorene" is 17 so i made the 17 in getline into a 19*/

    cout << *publicPointer << endl;

};

void pointerWorking()
{

};

void pointerExploit()
{

};

/*********************************************************************
*  vtable smash  
*  Called by interact()
*********************************************************************/
void VTableSprayingVulnerability(struct VulnerableVTable) 
{
    
    //VulnerableVTable::naive();  

    /*above, my understanding is, once we give access to the pointer 
    or buffer of a vtable, we have let the crazies control the 
    madhouse*/

};

/*********************************************************************
*  stack smash from the sounds of it, this is the bread and butter
*  of all hackers, seems pretty simple to actually build 
*  vulnerable code for this too, just let a string, array, or buffer 
*  be exposed naked to input.
*  Called by interact()
*********************************************************************/
void stackVulnerability(char vulnerableText)
{
    
    cout << "text value : " << vulnerableText << endl;
    
    // I don't know what to write here, we overwrite the buffer when we do a Cin and Cin is in "testStackSmashing"

};

void stackWorking()
{
    // Valid input that will output correctly
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
    // Invalid input that will exploit the buffer
    char* buffer1 = new char[5];
    stackVulnerability(*buffer1);

};


/**************************
 * HeapSpraying 
 **************************/
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
    cout << "Testing heap with string 'Test'.";
    heapVulnerability("Test");
    cout << "Test worked.";
};

void heapExploit()
{
    cout << "Testing heap with string 'This is too long!'\n";
    heapVulnerability("This is too long!");
    cout << "If this prints, the program didn't crash. It's a miracle!\n";
};

void testHeapSpraying()
{
    cout << "HeapSpraying\n"
        << "\tWorking\n\n";

    heapWorking();

    cout << "\tExploit\n\n";

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
    int n = 255;
    cout << "Offset: " << n << endl;
    intVulnerability(n);
};

void intExploit()
{
    long n = 3000000000000000;
    cout << "Offset: " << n << endl;
    intVulnerability((int) n);

};

void testIntegerOverflow()
{
    cout << "Integer Overflow\n"
        << "\tWorking\n\n";

    intWorking();

    cout << "\n\n\tExploit\n\n";

    intExploit();
    cout << "\n\n";
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
*  ArrayIndex hack  from the book, page 137
*  Called by interact()
*********************************************************************/
void testArrayIndex() 
{    
    // Test the array index.
    arrayWorking();
    arrayExploit();
};

void testArcInjection()
{
    
    long myVulnerableBuffer;
    
    arcVulnerability(myVulnerableBuffer);

};

void testStackSmashing()
{
    // Test the Stack.
    stackWorking();
    stackExploit();
};

void testPointerSubterfuge()
{

    char myArray[7];  // quiz question 8 has array[8] i did 7 so we aren't "copying"
    char * secretPointer = "arglefraster";
    char * publicPointer = "princess cimorene";

    pointerSubterfugeVulnerability(*myArray, secretPointer, publicPointer);
    
};

void testVTableSpraying()
{

    VulnerableVTable myVtable;

    /*above, my understanding is, once we give access to the pointer 
    or buffer of a vtable, we have let the crazies control the 
    madhouse*/

    VTableSprayingVulnerability(myVtable);

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
void displayResult(std::string message)
{
    std::cout << message << "\n\n";
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
