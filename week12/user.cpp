/***********************************************************************
 * Author:
 *    Br. Helfrich
 *    Carlos N Reina
 *    Adrian Whetten
 *    Jordan Burdett
 *    Caleb Georgeson
 *    Russell Roberts
 *    Stephen Ellis
 *    Orion Christensen
 ************************************************************************/

#include <iostream>
#include <cassert>
#include "user.h"
using namespace std;

/**************************************************
 * Set a user to empty
 **************************************************/
User ::  User()
{
   empty = true;
   text = "Empty";
   id = idNext++;
   control = Control::PUBLIC;
}

/**************************************************
 * Create a user and fill it
 **************************************************/
User::User(const string & text,
                 const string & author,
                 const string & date,
                 Control control)
{
   this->text = text;
   this->author = author;
   this->date = date;
   this->id = idNext++;
   empty = false;
   this->control = control;
}

bool User::securityConditionRead(Control control, Control subject)const{
   return subject >= control;
};

bool User::securityConditionWrite(Control control, Control subject)const{
   return subject <= control;
};

/**************************************************
 * Display the attributes/properties but not the
 * content of this user
 **************************************************/
void User::displayProperties() const
{
   // skip this one if there is nothing to see
   if (empty)
      return;
   
   // display the user
   cout << "\t[" << id << "] ";
      cout << "User from " << author << " at " << date;
   cout << endl;
}

/**************************************************
 * Display the contents or the text of the user
 **************************************************/
void User::displayText(Control subject) const
{
   if (!securityConditionRead(control, subject)){
      cout << "\nRead access denied\n";
      return;
   };
   
   cout << "\tUser: " << text << "\n";
}

/**************************************************
 * Update the contents or text of the user
 **************************************************/
void User::updateText(Control subject, const string & newText)
{
   if (!securityConditionWrite(control, subject)){
      cout << "\nWrite access denied\n";
      return;
   };

   text = newText;
}

/**************************************************
 * Delete the contents of a user and mark it as empty
 *************************************************/
void User::clear(Control subject)
{
   if (!securityConditionWrite(control, subject)){
      cout << "\nWrite access denied\n";
      return;
   };

   text = "Empty";
   author.clear();
   date.clear();
   empty = true;
}

int User::idNext = 100;
