
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
#include <string>

/*********************************************************************
*  displayMenu()
*  Called by interact()
*  Displays menu options.
*********************************************************************/
void displayMenu()
{
    std::cout << "OPTIONS:\n"
        << "   A  Option 1\n"
        << "   B  Option 2\n"
        << "   Q  Quit\n";

    return;
}

/*********************************************************************
*  displayPrompt()
*  Displays passed message.
*********************************************************************/
void displayPrompt(std::string message)
{
    std::cout << message << "\n";
    return;
}

/*********************************************************************
*  displayResult()
*  Called by interact()
*  Displays results.
*********************************************************************/
void displayResult(std::string message)
{
    std::cout << "TEST RESULT: " << message << "\n\n";
    return;
}

/*********************************************************************
*  displayError()
*  Called by interact()
*  Displays error message.
*********************************************************************/
void displayError(std::string message)
{
    std::cout << message << "\n\n";
    return;
}

/*********************************************************************
*  displayHeader()
*  Called by interact()
*  Displays app identifier.
*********************************************************************/
void displayHeader()
{
    std::cout << "\n************ CSE 453 ************\n\n";
    return;
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
*   interact(), until user types "Q".
*   Called by main.
*********************************************************************/
void interact()
{
    clearScreen();
    displayHeader();
    displayMenu();

    char answer[2] = "";
    std::string returnString;
    do
    {
        if (std::cin.fail()) // bad input
        {
            std::cin.clear();
            std::cin.ignore();
            continue;
        }

        std::cin.getline(answer, 2);
        if (islower(answer[0])) { answer[0] = toupper(answer[0]); }
        returnString.clear();
        switch (answer[0])
        {
        case '\0': // no input
            clearScreen();
            displayHeader();
            displayError("ERROR: Invalid command");
            displayMenu();
            break;

        case 'A':
            clearScreen();
            displayHeader();
            displayError("ERROR: Not implemented");
            displayMenu();
            break;

        case 'B':
            clearScreen();
            displayHeader();
            displayError("ERROR: Not implemented");
            displayMenu();
            break;

        case 'Q': // Quit
            clearScreen();
            break;

        default:
            clearScreen();
            displayHeader();
            displayError("ERROR: Invalid command");
            displayMenu();
            break;
        }
    } while (answer[0] != 'Q' && answer[0] != 'q');

    return;
}

/*********************************************************************
*   Main driver of the program
*********************************************************************/
int main()
{
    //Russells_commit
    //Calebs_commit
    interact();

    return 0;
}

/*************************************
 * ARRAY VULNERABILTY
 * 1. There must be an array and an array index variable
 * 2. The array index variable must be reachable through external input.
 * 3. There must not be bounds checking on the array index variable.
 ****************************************/
void arrayVulnerability(/* feel free to add parameters */)
{
}

/**************************************
 * ARRAY WORKING
 * Call arrayVulnerability() in a way that does
 * not yield unexpected behavior
 *************************************/
void arrayWorking()
{
    arrayVulnerability(/* parameters with non-malicious data */);
}

/**************************************
 * ARRAY EXPLOIT
 * 1. The attacker provides an array index value outside the expected range
 * 2. The attacker must be able to provide input or redirect
 *    existing input into the array at the index he provided
 * 3. The injected value must alter program state in a way
 *    that is desirable to the attacker
 *************************************/
void arrayExploit()
{
    arrayVulnerability(/* parameters with malicious data */);
}