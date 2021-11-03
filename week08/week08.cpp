
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
using namespace std;

void interact();
void displayResult(std::string message);
void displayAboutUs();
void displayMenu();
void displayHeader();
void clearScreen();

void testArrayIndex();
void testPointerSubterfuge();
void testArcInjection();
void testVTableSpraying();
void testStackSmashing();
void testHeapSpraying();
void testIntegerOverflow();
void testAnsiUnicode();

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
    {

        cin >> myText;

    }

    virtual void malignant();

};

void VulnerableVTable::naive()
{
}

void VulnerableVTable::malignant()
{

}

/**********************************************
 * MAIN : 
 **********************************************/
int main()
{
    interact ();
    return 0;
}



/*********************************************************************
*  ArrayIndex hack  from the book, page 137
*  Called by interact()
*********************************************************************/
void arrayVulnerability() 
{
    int sheepArray[5];  // a vulnerable soft meek array like a sheep
    bool crappyAuthenticationThatMakesThingsWorse = false;  // I guess to do array index, we have to have a check that actually makes the array vulnerable rather than safe?

    int gimmieIndex;
    cin >> gimmieIndex;
    sheepArray[gimmieIndex] = -1;  //book says "if index == 4, problem! therefore for us, if index == 5 problem?"

}

/*********************************************************************
*  pointer subterfuge hack from the quiz question 8
*  Called by interact()
*********************************************************************/
void testPointerSubterfuge()
{

    char myArray[7];  // quiz question 8 has array[8] i did 7 so we aren't "copying"
    char * secret = "arglefraster";
    char * public = "princess cimorene";
    cin.getline(input, 19);      

    /*for above, the quiz question 8 makes the number i have "19" bigger than the size of "Citizen Kane",
    which is 12 charand getline is 17, so my "princess cimorene" is 17 so i made the 17 in getline into a 19*/

    cout << public << endl;

}

/*********************************************************************
*  ARC injection hack zoh my gato! we blow up iron man's cold fusion reactor! from the book, page 141
*  Called by interact()
*********************************************************************/
void testArcInjection()
{

    long myVulnerableBuffer[8];
    void (*myPointerMethod)() = naive;

    cin >> myVulnerableBuffer[8];  
    
    /*pretty sure above we expose the buffer, 
    and therefore the pointer and method it executes, naked, cold, and afraid
    to the depraved wiles of every nefarious, skulldugerous, and malcisously nasty hacker with their pointy teeth and hairy hands with claws scraped with grit in their fingernails!*/

    myPointerMethod();

}

/*********************************************************************
*  vtable smash  
*  Called by interact()
*********************************************************************/
void testVTableSpraying() 
{

    VulnerableVTable myVtable;

    myVtable.naive();  
    
    /*above, my understanding is, once we give access to the pointer 
    or buffer of a vtable, we have let the crazies control the 
    madhouse*/

}

/*********************************************************************
*  stack smash from the sounds of it, this is the bread and butter
*  of all hackers, seems pretty simple to actually build 
*  vulnerable code for this too, just let a string, array, or buffer 
*  be exposed naked to input.
*  Called by interact()
*********************************************************************/
void testStackSmashing()
{

    char vulnerableText[256];
    cin >> vulnerableText;


}


void testHeapSpraying(){};
void testIntegerOverflow(){};
void testAnsiUnicode(){};





























/*********************************************************************
*  ArrayIndex hack  from the book, page 137
*  Called by interact()
*********************************************************************/
void testArrayIndex() 
{

}


/*********************************************************************
*  clearScreen()
*  Called by interact()
*  Clears the screen.
*********************************************************************/
void clearScreen()
{
    if (system("CLS")) { system("clear"); }
    return;
}

/*********************************************************************
*  displayHeader()
*  Called by interact()
*  Displays app identifier.
*********************************************************************/
void displayHeader()
{
    cout << "\n************ CSE 453 ************\n\n";
    return;
}

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
}

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
}

/*********************************************************************
*  displayResult()
*  Called by interact()
*  Displays results.
*********************************************************************/
void displayResult(std::string message)
{
    std::cout << message << "\n\n";
    return;
}

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
}




