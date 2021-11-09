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

/*********************************************************************
 * MAIN : 
 ********************************************************************/
int main()
{
    // get the secret
   string secret;
   cout << "What is the secret?   ";
   cin  >> secret;
   // get the password
   string password;
   cout << "What is the password? ";
   cin  >> password;
   // now go in a holding pattern until the user provides the correct password
   string prompt;
   do
   {
      cout << "You can get the secret only with the password: ";
      cin  >> prompt;
   }
   while (prompt != password);
   // reveal the secret
   cout << "The secret is " << secret << endl;      
   return 0;
}
