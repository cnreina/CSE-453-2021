
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
using namespace std;


void testArrayIndex() {};
void testPointerSubterfuge() {};
void testArcInjection() {};
void testVTableSpraying() {};
void testStackSmashing() {};
void testHeapSpraying() {};
void testIntegerOverflow() {};
void testAnsiUnicode() {};


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

/**********************************************
 * MAIN : 
 **********************************************/
int main()
{

    interact ();

    return 0;
}
