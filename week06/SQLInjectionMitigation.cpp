
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
#include <vector>
#include <sstream>
#include <string>
#include <array>

using namespace std;

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
        << "   A  Test Valid\n"
        << "   B  Test Tautology\n"
        << "   C  Test Union\n"
        << "   D  Test Additional Statement\n"
        << "   E  Test Comment\n"
        << "   F  About Us\n"
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
*  genQuery()
*  Provides no mitigation against all four attacks.
*  Returns non-sanitized sql that epresents the query used to
*  determine if a user is authenticated on a given system.
*********************************************************************/
string genQuery(string username, string password)
{
    string query = "SELECT * FROM USERS WHERE username = \'" + username + "\' AND password = \'" + password + "\';";

    return query;
}

/*********************************************************************
*  genQueryWeak()
*  Provides weak mitigation against all four attacks.
*  Returns sanitized sql that epresents the query used to
*  determine if a user is authenticated on a given system.
* 
*  NOTES:
*  Last resort used when unable to perform complete or strong mitigation.
*  Looks for input known to be dangerous.
*  Prevents the attack by invalidating any user input containing
*  a semicolon.
*  This technique is called a blocklist, blacklist, or unapprovedlist.
*  The list contains elements known to be unsafe.
*  Effective if all unsafe elements reside on this list and no user input
*  conforms to the list.
*********************************************************************/
string genQueryWeak(string username, string password)
{
    // filter illegal input
    string blacklist[] = {" ","'","OR","AND","SELECT","UNION","INSERT","JOIN","ALTER","DELETE","FROM",";"};
 
    for (size_t i = 0; i < size(blacklist); i++)
    {
        int position;
        position = username.find(blacklist[i]);
        if (position >= 0)
        {
            username = username.erase(position, username.length());
        }

        position = password.find(blacklist[i]);
        if (position >= 0)
        {
            password = password.erase(position, password.length());
        }
    }

    // process query
    if (username.empty() || password.empty()){ return ""; }
    string query = "SELECT * FROM USERS WHERE username = \'" + username + "\' AND password = \'" + password + "\';";
    
    return query;
}

/*********************************************************************
*  genQueryStrong()
*  Will return username and password if they are found in the 
*  whitelist. Otherwise, it will return the entire string as empty.
* 
*  NOTES:
*  Next preferred option for when it is not possible to achieve
*  complete mitigation.
*  Restricts input to only valid commands.
*  This technique is called an allowlist, whitelist, or approvelist.
*  Effective as long as no unsafe elements reside on the list and
*  all user input conforms to the list.
*********************************************************************/
string genQueryStrong(string username, string password)
{
    // filter illegal input
    string blacklist[] = { " ","'","OR","AND","SELECT","UNION","INSERT","JOIN","ALTER","DELETE","FROM",";" };
    string whiteListUserName[] = { "jordanburdett", "Orionchristensen", "adrianWhetten", "calebgeorgeson", "carlosnreina", "stephenellis", "russellroberts"};
    string whiteListPassword[] = { "P@ss_word1$$", "MJU&nhy6bgt5vfr4", "ALPHAMOONDAWG", "totally_secure_password", "securePass_123", "TwoCool_4_YoU", "Rustyspoon7171"};

    string correctUsername = "";
    string correctPassword = "";
    
    for (size_t i = 0; i <= size(whiteListUserName); i++)
    {
        if (username == whiteListUserName[i] && username.length() == whiteListUserName[i].length()) {
            correctUsername = whiteListUserName[i];
        }
    }

    for (size_t i = 0; i <= size(whiteListPassword); i++)
    {
        if (password == whiteListPassword[i] && password.length() == whiteListPassword[i].length()) {
            correctPassword = whiteListPassword[i];
        }
    }

    // process query
    if (correctUsername.empty() || correctPassword.empty()) { return ""; }

    string query = "SELECT * FROM USERS WHERE username = \'" + correctUsername + "\' AND password = \'" + correctPassword + "\';";

    return query;
}

/*********************************************************************
*  testValid()
*  Represents valid input where the username and the password consist 
*  of letters, numbers, and underscores.
*  Displays the resulting query.
*********************************************************************/
void testValid()
{
    string username, password, queryResults;
    cout << "---------- Running Test Valid\n\n";

    // Jordan's test case
    username = "jordanburdett";
    password = "P@ss_word1$$";
    cout << "\tUsername:\t" << username << "\n";
    cout << "\tPassword:\t" << password << "\n";
    cout << "\tgenQuery:\t" << genQuery(username, password) << "\n";
    cout << "\tgenQueryWeak:\t" << genQueryWeak(username, password) << "\n";
    cout << "\tgenQueryStrong:\t" << genQueryStrong(username, password) << "\n\n";
    
    // Orion's test case.
    username = "Orionchristensen";
    password = "MJU&nhy6bgt5vfr4";
    cout << "\tUsername:\t" << username << "\n";
    cout << "\tPassword:\t" << password << "\n";
    cout << "\tgenQuery:\t" << genQuery(username, password) << "\n";
    cout << "\tgenQueryWeak:\t" << genQueryWeak(username, password) << "\n";
    cout << "\tgenQueryStrong:\t" << genQueryStrong(username, password) << "\n\n";

    // Adrian's Test Case
    username = "adrianWhetten";
    password = "ALPHAMOONDAWG";
    cout << "\tUsername:\t" << username << "\n";
    cout << "\tPassword:\t" << password << "\n";
    cout << "\tgenQuery:\t" << genQuery(username, password) << "\n";
    cout << "\tgenQueryWeak:\t" << genQueryWeak(username, password) << "\n";
    cout << "\tgenQueryStrong:\t" << genQueryStrong(username, password) << "\n\n";

    // Caleb's test case
    username = "calebgeorgeson";
    password = "totally_secure_password";
    cout << "\tUsername:\t" << username << "\n";
    cout << "\tPassword:\t" << password << "\n";
    cout << "\tgenQuery:\t" << genQuery(username, password) << "\n";
    cout << "\tgenQueryWeak:\t" << genQueryWeak(username, password) << "\n";
    cout << "\tgenQueryStrong:\t" << genQueryStrong(username, password) << "\n\n";

    // Carlos' test case
    username = "carlosnreina";
    password = "securePass_123";
    cout << "\tUsername:\t" << username << "\n";
    cout << "\tPassword:\t" << password << "\n";
    cout << "\tgenQuery:\t" << genQuery(username, password) << "\n";
    cout << "\tgenQueryWeak:\t" << genQueryWeak(username, password) << "\n";
    cout << "\tgenQueryStrong:\t" << genQueryStrong(username, password) << "\n\n";

    // Stephen's test case
    username = "stephenellis";
    password = "TwoCool_4_YoU";
    cout << "\tUsername:\t" << username << "\n";
    cout << "\tPassword:\t" << password << "\n";
    cout << "\tgenQuery:\t" << genQuery(username, password) << "\n";
    cout << "\tgenQueryWeak:\t" << genQueryWeak(username, password) << "\n";
    cout << "\tgenQueryStrong:\t" << genQueryStrong(username, password) << "\n\n";
    
    // Russell's test case
    username = "russellroberts";
    password = "Rustyspoon7171";
    cout << "\tUsername:\t" << username << "\n";
    cout << "\tPassword:\t" << password << "\n";
    cout << "\tgenQuery:\t" << genQuery(username, password) << "\n";
    cout << "\tgenQueryWeak:\t" << genQueryWeak(username, password) << "\n";
    cout << "\tgenQueryStrong:\t" << genQueryStrong(username, password) << "\n\n";

    cout << "---------- End of Test\n\n";

}

/*********************************************************************
*  testTautology()
*  Injects a boolean expression that always  evaluates  to  true.
* 
*  NOTES:
*  Must contain an 'OR' command.
*  It works when user input is fed directly into an SQL statement.
*********************************************************************/
void testTautology()
{
    string username, password, queryResults;
    cout << "---------- Running Test Tautology\n\n";

    // Jordan's test case
    username = "jordanburdett;;;;;;; AND SELECT asdf' OR 'abc' = 'abc'";
    password = "password' OR 'test' = 'test'";
    cout << "\tUsername:\t" << username << "\n";
    cout << "\tPassword:\t" << password << "\n";
    cout << "\tgenQuery:\t" << genQuery(username, password) << "\n";
    cout << "\tgenQueryWeak:\t" << genQueryWeak(username, password) << "\n";
    cout << "\tgenQueryStrong:\t" << genQueryStrong(username, password) << "\n\n";

    // Orion's test case.
    username = "Orionchristensen";
    password = "'gomdrop' OR 'test' = 'test'";
    cout << "\tUsername:\t" << username << "\n";
    cout << "\tPassword:\t" << password << "\n";
    cout << "\tgenQuery:\t" << genQuery(username, password) << "\n";
    cout << "\tgenQueryWeak:\t" << genQueryWeak(username, password) << "\n";
    cout << "\tgenQueryStrong:\t" << genQueryStrong(username, password) << "\n\n";

    // Adrian's Test Case
    username = "adrianWhetten OR 'd'='d'";
    password = "'junkPassword' OR 'kl' = 'kl'";
    cout << "\tUsername:\t" << username << "\n";
    cout << "\tPassword:\t" << password << "\n";
    cout << "\tgenQuery:\t" << genQuery(username, password) << "\n";
    cout << "\tgenQueryWeak:\t" << genQueryWeak(username, password) << "\n";
    cout << "\tgenQueryStrong:\t" << genQueryStrong(username, password) << "\n\n";

    // Caleb's test case
    username = "calebgeorgeson";
    password = "password' OR 'a' = 'a";
    cout << "\tUsername:\t" << username << "\n";
    cout << "\tPassword:\t" << password << "\n";
    cout << "\tgenQuery:\t" << genQuery(username, password) << "\n";
    cout << "\tgenQueryWeak:\t" << genQueryWeak(username, password) << "\n";
    cout << "\tgenQueryStrong:\t" << genQueryStrong(username, password) << "\n\n";

    // Carlos' test case
    username = "root/carlos";
    password = "test' OR 'x' = 'x";
    cout << "\tUsername:\t" << username << "\n";
    cout << "\tPassword:\t" << password << "\n";
    cout << "\tgenQuery:\t" << genQuery(username, password) << "\n";
    cout << "\tgenQueryWeak:\t" << genQueryWeak(username, password) << "\n";
    cout << "\tgenQueryStrong:\t" << genQueryStrong(username, password) << "\n\n";

    // Stephen's test case
    username = "stephen_Ellis";
    password = "stephenellis' OR 'True' = 'True' OR 'getIn' = 'getIn";
    cout << "\tUsername:\t" << username << "\n";
    cout << "\tPassword:\t" << password << "\n";
    cout << "\tgenQuery:\t" << genQuery(username, password) << "\n";
    cout << "\tgenQueryWeak:\t" << genQueryWeak(username, password) << "\n";
    cout << "\tgenQueryStrong:\t" << genQueryStrong(username, password) << "\n\n";

    // Russell's test case
    username = "russellroberts";
    password = "russellroberts OR True = True";
    cout << "\tUsername:\t" << username << "\n";
    cout << "\tPassword:\t" << password << "\n";
    cout << "\tgenQuery:\t" << genQuery(username, password) << "\n";
    cout << "\tgenQueryWeak:\t" << genQueryWeak(username, password) << "\n";
    cout << "\tgenQueryStrong:\t" << genQueryStrong(username, password) << "\n\n";

    cout << "---------- End of Test\n\n";

}

/*********************************************************************
*  testUnion()
*  Inserts a UNION clause into the end of an SQL statement.
*
*  NOTES:
*  Allows multiple statements to be joined into a single result to
*  combine queries or make a single statement return a richer set
*  of results.
*********************************************************************/
void testUnion()
{
    string username, password, queryResults;
    cout << "---------- Running Test Union\n\n";

    // Jordan's test case
    username = "jordanburdett";
    password = "password'; UNION SELECT ";
    cout << "\tUsername:\t" << username << "\n";
    cout << "\tPassword:\t" << password << "\n";
    cout << "\tgenQuery:\t" << genQuery(username, password) << "\n";
    cout << "\tgenQueryWeak:\t" << genQueryWeak(username, password) << "\n";
    cout << "\tgenQueryStrong:\t" << genQueryStrong(username, password) << "\n\n";

    // Orion's test case.
    username = "Orionchristensen";
    password = "starunion'; UNION SELECT * FROM USERS";
    cout << "\tUsername:\t" << username << "\n";
    cout << "\tPassword:\t" << password << "\n";
    cout << "\tgenQuery:\t" << genQuery(username, password) << "\n";
    cout << "\tgenQueryWeak:\t" << genQueryWeak(username, password) << "\n";
    cout << "\tgenQueryStrong:\t" << genQueryStrong(username, password) << "\n\n";

    // Adrian's Test Case
    username = "adrianWhetten";
    password = "doubleDipper'; UNION SELECT * FROM USERS";
    cout << "\tUsername:\t" << username << "\n";
    cout << "\tPassword:\t" << password << "\n";
    cout << "\tgenQuery:\t" << genQuery(username, password) << "\n";
    cout << "\tgenQueryWeak:\t" << genQueryWeak(username, password) << "\n";
    cout << "\tgenQueryStrong:\t" << genQueryStrong(username, password) << "\n\n";

    // Caleb's test case
    username = "calebgeorgeson";
    password = "totally_secure_password' UNION SELECT name, ssn FROM employee_data";
    cout << "\tUsername:\t" << username << "\n";
    cout << "\tPassword:\t" << password << "\n";
    cout << "\tgenQuery:\t" << genQuery(username, password) << "\n";
    cout << "\tgenQueryWeak:\t" << genQueryWeak(username, password) << "\n";
    cout << "\tgenQueryStrong:\t" << genQueryStrong(username, password) << "\n\n";

    // Carlos' test case
    username = "root/carlos";
    password = "test' UNION SELECT authenticate FROM existentTable";
    cout << "\tUsername:\t" << username << "\n";
    cout << "\tPassword:\t" << password << "\n";
    cout << "\tgenQuery:\t" << genQuery(username, password) << "\n";
    cout << "\tgenQueryWeak:\t" << genQueryWeak(username, password) << "\n";
    cout << "\tgenQueryStrong:\t" << genQueryStrong(username, password) << "\n\n";

    // Stephen's test case
    username = "stephen_Ellis";
    password = "newAndImproved_Password' UNION SELECT * FROM password";
    cout << "\tUsername:\t" << username << "\n";
    cout << "\tPassword:\t" << password << "\n";
    cout << "\tgenQuery:\t" << genQuery(username, password) << "\n";
    cout << "\tgenQueryWeak:\t" << genQueryWeak(username, password) << "\n";
    cout << "\tgenQueryStrong:\t" << genQueryStrong(username, password) << "\n\n";

    // Russell's test case
    username = "russellroberts";
    password = "bank_account_password UNION SELECT acct_number FROM bank_information";
    cout << "\tUsername:\t" << username << "\n";
    cout << "\tPassword:\t" << password << "\n";
    cout << "\tgenQuery:\t" << genQuery(username, password) << "\n";
    cout << "\tgenQueryWeak:\t" << genQueryWeak(username, password) << "\n";
    cout << "\tgenQueryStrong:\t" << genQueryStrong(username, password) << "\n\n";

    cout << "---------- End of Test\n\n";

}

/*********************************************************************
*  testAddState()
*  Additional Statement attack.
*  Appends a semi-colon onto an SQL command stream.
* 
*  NOTES:
*  Executes a second command  where the author intended only one.
*  As with C++ and a variety of other languages, a semi-colon
*  indicates the end of one statement and the beginning of a second.
*  It can retrieve any information contained in the database, alter
*  information, remove information, and physically destroy the server.
*********************************************************************/
void testAddState()
{
    string username, password, queryResults;
    cout << "---------- Running Test Additional Statement\n\n";

    // Jordan's test case
    username = "jordanburdett;;;;;;; AND SELECT asdf' OR 'abc' = 'abc'";
    password = "password' OR 'test' = 'test'";
    cout << "\tUsername:\t" << username << "\n";
    cout << "\tPassword:\t" << password << "\n";
    cout << "\tgenQuery:\t" << genQuery(username, password) << "\n";
    cout << "\tgenQueryWeak:\t" << genQueryWeak(username, password) << "\n";
    cout << "\tgenQueryStrong:\t" << genQueryStrong(username, password) << "\n\n";

    // Orion's test case.
    username = "Orionchristensen";
    password = "excelcior'; DELETE * from employee_data";
    cout << "\tUsername:\t" << username << "\n";
    cout << "\tPassword:\t" << password << "\n";
    cout << "\tgenQuery:\t" << genQuery(username, password) << "\n";
    cout << "\tgenQueryWeak:\t" << genQueryWeak(username, password) << "\n";
    cout << "\tgenQueryStrong:\t" << genQueryStrong(username, password) << "\n\n";

    // Adrian's Test Case
    username = "'adrianWhetten'";
    password = "'oldAndRundown_password'; DELETE * FROM USERS";
    cout << "\tUsername:\t" << username << "\n";
    cout << "\tPassword:\t" << password << "\n";
    cout << "\tgenQuery:\t" << genQuery(username, password) << "\n";
    cout << "\tgenQueryWeak:\t" << genQueryWeak(username, password) << "\n";
    cout << "\tgenQueryStrong:\t" << genQueryStrong(username, password) << "\n\n";

    // Caleb's test case
    username = "calebgeorgeson";
    password = "totally_secure_password'; DELETE FROM employee_data";
    cout << "\tUsername:\t" << username << "\n";
    cout << "\tPassword:\t" << password << "\n";
    cout << "\tgenQuery:\t" << genQuery(username, password) << "\n";
    cout << "\tgenQueryWeak:\t" << genQueryWeak(username, password) << "\n";
    cout << "\tgenQueryStrong:\t" << genQueryStrong(username, password) << "\n\n";

    // Carlos' test case
    username = "carlosnreina";
    password = "test'; INSERT INTO passwordList (newName, newPasswd) VALUES 'Carlos', 'test";
    cout << "\tUsername:\t" << username << "\n";
    cout << "\tPassword:\t" << password << "\n";
    cout << "\tgenQuery:\t" << genQuery(username, password) << "\n";
    cout << "\tgenQueryWeak:\t" << genQueryWeak(username, password) << "\n";
    cout << "\tgenQueryStrong:\t" << genQueryStrong(username, password) << "\n\n";

    // Stephen's test case
    username = "stephen_Ellis";
    password = "newAndImproved_Password'; UNION SELECT * FROM password; UNION DELETE * FROM password";
    cout << "\tUsername:\t" << username << "\n";
    cout << "\tPassword:\t" << password << "\n";
    cout << "\tgenQuery:\t" << genQuery(username, password) << "\n";
    cout << "\tgenQueryWeak:\t" << genQueryWeak(username, password) << "\n";
    cout << "\tgenQueryStrong:\t" << genQueryStrong(username, password) << "\n\n";

    // Russell's test case
    username = "russellroberts";
    password = "bank_account_password UNION SELECT acct_number FROM bank_information";
    cout << "\tUsername:\t" << username << "\n";
    cout << "\tPassword:\t" << password << "\n";
    cout << "\tgenQuery:\t" << genQuery(username, password) << "\n";
    cout << "\tgenQueryWeak:\t" << genQueryWeak(username, password) << "\n";
    cout << "\tgenQueryStrong:\t" << genQueryStrong(username, password) << "\n\n";

    cout << "---------- End of Test\n\n";

}

/*********************************************************************
*  testComment()
*  Specifies text that is ignored by the interpreter.
* 
*  NOTES:
*  If a comment is inserted into part of an SQL statement, the
*  remainder of the query will is ignored by the interpreter.
*********************************************************************/
void testComment()
{

    string username, password, queryResults;
    cout << "---------- Running Test Comment\n\n";

    // Jordan's test case
    username = "jordanburdett'; --";
    password = "you got hacked";
    cout << "\tUsername:\t" << username << "\n";
    cout << "\tPassword:\t" << password << "\n";
    cout << "\tgenQuery:\t" << genQuery(username, password) << "\n";
    cout << "\tgenQueryWeak:\t" << genQueryWeak(username, password) << "\n";
    cout << "\tgenQueryStrong:\t" << genQueryStrong(username, password) << "\n\n";

    // Orion's test case.
    username = "orion'; --";
    password = "harhar!";
    cout << "\tUsername:\t" << username << "\n";
    cout << "\tPassword:\t" << password << "\n";
    cout << "\tgenQuery:\t" << genQuery(username, password) << "\n";
    cout << "\tgenQueryWeak:\t" << genQueryWeak(username, password) << "\n";
    cout << "\tgenQueryStrong:\t" << genQueryStrong(username, password) << "\n\n";

    // Adrian's Test Case
    username = "adrianWhetten; --";
    password = "peek-a-boo";
    cout << "\tUsername:\t" << username << "\n";
    cout << "\tPassword:\t" << password << "\n";
    cout << "\tgenQuery:\t" << genQuery(username, password) << "\n";
    cout << "\tgenQueryWeak:\t" << genQueryWeak(username, password) << "\n";
    cout << "\tgenQueryStrong:\t" << genQueryStrong(username, password) << "\n\n";

    // Caleb's test case
    username = "calebgeorgeson' -- The rest is a comment ->";
    password = "Whatever I want!";
    cout << "\tUsername:\t" << username << "\n";
    cout << "\tPassword:\t" << password << "\n";
    cout << "\tgenQuery:\t" << genQuery(username, password) << "\n";
    cout << "\tgenQueryWeak:\t" << genQueryWeak(username, password) << "\n";
    cout << "\tgenQueryStrong:\t" << genQueryStrong(username, password) << "\n\n";

    // Carlos' test case
    username = "root/carlos'; --";
    password = "test";
    cout << "\tUsername:\t" << username << "\n";
    cout << "\tPassword:\t" << password << "\n";
    cout << "\tgenQuery:\t" << genQuery(username, password) << "\n";
    cout << "\tgenQueryWeak:\t" << genQueryWeak(username, password) << "\n";
    cout << "\tgenQueryStrong:\t" << genQueryStrong(username, password) << "\n\n";

    // Stephen's test case
    username = "stephen_Ellis'; -- newEntry for comment ";
    password = "I got access!";
    cout << "\tUsername:\t" << username << "\n";
    cout << "\tPassword:\t" << password << "\n";
    cout << "\tgenQuery:\t" << genQuery(username, password) << "\n";
    cout << "\tgenQueryWeak:\t" << genQueryWeak(username, password) << "\n";
    cout << "\tgenQueryStrong:\t" << genQueryStrong(username, password) << "\n\n";

    // Russell's test case
    username = "russellroberts; --";
    password = "It actually commented!";
    cout << "\tUsername:\t" << username << "\n";
    cout << "\tPassword:\t" << password << "\n";
    cout << "\tgenQuery:\t" << genQuery(username, password) << "\n";
    cout << "\tgenQueryWeak:\t" << genQueryWeak(username, password) << "\n";
    cout << "\tgenQueryStrong:\t" << genQueryStrong(username, password) << "\n\n";

    cout << "---------- End of Test\n\n";

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

        case 'A': // Test Valid
            clearScreen();
            displayHeader();
            testValid();
            displayMenu();
            break;

        case 'B': // Test Tautology
            clearScreen();
            displayHeader();
            testTautology();
            displayMenu();
            break;

        case 'C': // Test Union
            clearScreen();
            displayHeader();
            testUnion();
            displayMenu();
            break;

        case 'D': // Test Additional Statement
            clearScreen();
            displayHeader();
            testAddState();
            displayMenu();
            break;

        case 'E': // Test Comment
            clearScreen();
            displayHeader();
            testComment();
            displayMenu();
            break;

        case 'F': // About Us
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

/*********************************************************************
*   Main driver of the program
*********************************************************************/
int main()
{
    interact();

    return 0;
}
