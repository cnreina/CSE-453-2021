
/*********************************************************************
 * CSE 453
 * Carlos N Reina
 *
 * Week 11 Lab (Individual)
 * App to analyze the strength of a given user password.
 * Textbook Page 229.
 * 
 * Program Flow:
 * 1. prompt the user for his password. 
 * 2. determine how many combinations of passwords exist in this set.
 * 3. determine the bit equivalence based on the number of combinations.
 * 4. compute the bit strength of the password.
 * 
*******************************************************************/

#include <iostream>
#include <string>
#include <cassert>
#include <cmath>

void                interact();
void                clearScreen();
void                displayHeader();
void                displayMenu();
void                displayResult(std::string message);
void                displayAboutUs();

std::string         testPassword();
std::string         getInput();
unsigned long long  getAlphabetSize(std::string password);


/*********************************************************************
 * /////////////////////////// MAIN
*********************************************************************/
int main()
{
    interact();
    return 0;
};

/*********************************************************************
 * Analyzes the strength of a given user password.
 * Returns the following data:
 *  Password
 *  Alphabet Size
 *  Combinations
 *  Bits
*********************************************************************/
std::string testPassword()
{
    std::string returnString;

    // get user input
    std::string password = getInput();
    if (password.empty())
    {
        clearScreen();
        displayHeader();
        return "Invalid Input";  
    };
    returnString.append("Password:\t");
    returnString.append(password);
    returnString.append("\n");

    // compute alphabet size
    unsigned long long alphabetSize = getAlphabetSize(password);
    if (alphabetSize <= 0)
    {
        clearScreen();
        displayHeader();
        return "Error: alphabetSize <= 0";
    };
    returnString.append("Alphabet Size:\t");
    returnString.append(std::to_string(alphabetSize));
    returnString.append("\n");

    // compute combinations
    int                 passwordLength = password.length();
    unsigned long long  combinations = std::pow(alphabetSize, passwordLength);
    if (combinations <= 0)
    {
        clearScreen();
        displayHeader();
        return "Error: combinations <= 0";  
    };
    returnString.append("Combinations:\t");
    returnString.append(std::to_string(combinations));
    returnString.append("\n");

    // compute bit strength
    unsigned long long bitEquivalence = std::log2(combinations);
    if (bitEquivalence <= 0)
    {
        clearScreen();
        displayHeader();
        return "Error: bitEquivalence <= 0";  
    };
    returnString.append("Bits:\t\t");
    returnString.append(std::to_string(bitEquivalence));
    returnString.append("\n");

    clearScreen();
    displayHeader();
    return returnString;
};

/*********************************************************************
 * Prompts for, validates, and returns a pasword.
*********************************************************************/
std::string getInput()
{
    std::string userInput = "";
    char answer[255] = "";
    bool done = false;
    do
    {
        if (std::cin.fail()) // bad input
        {
            std::cin.clear();
            std::cin.ignore();
            continue;
        }

        clearScreen();
        displayHeader();
        std::cout << "\nType a pasword: ";
        std::cin.getline(answer, 255);
        if (answer[0] == '\0')
        {
            return userInput;
        };
        userInput.assign(answer);
        done = true;

    } while (!done);

    return userInput;

};

/*********************************************************************
 * Computes the alphabet size for the passed password.
*********************************************************************/
unsigned long long getAlphabetSize(std::string password)
{
    // identify alphabet
    int passwordLength = password.length();
    bool isNumber = false;
    bool isLowerAlpha = false;
    bool isUpperAlpha = false;
    bool isSymbol = false;
    int counter;
    for (counter = 0; counter < passwordLength; counter++)
    {
        if (!isNumber && std::isdigit(password[counter]))
        {
            isNumber = true;
            continue;
        };
        if (!isLowerAlpha && std::isalpha(password[counter]) && std::islower(password[counter]))
        {
            isLowerAlpha = true;
            continue;
        };
        if (!isUpperAlpha && std::isalpha(password[counter]) && std::isupper(password[counter]))
        {
            isUpperAlpha = true;
            continue;
        };
        if (!isSymbol && !(std::isdigit(password[counter]) || 
            std::isalpha(password[counter]) || 
            std::islower(password[counter] || 
            std::isalpha(password[counter]) || 
            std::isupper(password[counter]))))
        {
            isSymbol = true;
            continue;
        };
    };

    // compute alphabet size
    unsigned long long alphabetSize = 0;
    if (isNumber)
    {
        alphabetSize += 10;
    };
    if (isLowerAlpha)
    {
        alphabetSize += 26;
    };
    if (isUpperAlpha)
    {
        alphabetSize += 26;
    };
    if (isSymbol)
    {
        alphabetSize += 32;
    };
    
    return alphabetSize;
};


/*********************************************************************
 * /////////////////////////// MENU SYSTEM
*********************************************************************/

/*********************************************************************
 * Main program loop.
 * Runs until user types "Q".
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
            displayMenu();
            break;

        case 'A': // Compute password combinations
            clearScreen();
            displayHeader();
            displayResult(testPassword());
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

/*********************************************************************
 * Clears the screen.
*********************************************************************/
void clearScreen()
{
    if (system("CLS")) { system("clear"); }
    return;
};

/*********************************************************************
 * Displays app identifier.
*********************************************************************/
void displayHeader()
{
    std::cout << "\n************ CSE 453 ************\n\n";
    return;
};

/*********************************************************************
 * Displays menu options.
*********************************************************************/
void displayMenu()
{
    std::cout << "OPTIONS:\n"
        << "   A  Menu Item\n"
        << "   I  About Us\n"
        << "   Q  Quit\n\n";

    return;
};

/*********************************************************************
 * Formats and displays results.
*********************************************************************/
void displayResult(std::string message)
{
    std::cout << message << "\n\n";

    return;
};

/*********************************************************************
 * Formats and displays authors.
*********************************************************************/
void displayAboutUs()
{
    std::cout << "AUTHORS:\n"
        << "   Carlos N Reina\n\n";

    return;
};
