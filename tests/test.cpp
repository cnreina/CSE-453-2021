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

class Secret
{
public:
   // constructor: set the secret and the pin
   Secret() : secret("Rosebud"), pin(1234) {}
 
   // GET: reveal the secret to those who ask for it
   string get(int pin) { return (authenticate(pin)) ? secret : string(""); }
 
private:
   string secret;            // the secret to be protected at all costs
   int    pin;               // the password required to reveal the secret
 
   // AUTHENTICATE : Determine if the passed PIN is what was expected  
   bool authenticate(int pinUser) { return (pinUser == pin); }
};
 
int main()
{
   // this is the secret, locked deep within
   Secret s;
  
   // prompt for the pin
   int pin;
   cout << "If you want the secret, you need to give me the PIN: ";
   cin  >> pin;
 
   // fetch the secret
   string secret = s.get(pin);
 
   // give the user the answer
   if (secret.size() != 0)
      cout << "The secret is... " << secret << endl;
   else
      cout << "I am not going to tell you!\n";
 
   return 0;
}
