/***********************************************************************
 * COMPONENT:
 *    MESSAGE
 * Author:
 *    Br. Helfrich
 *    Carlos N Reina
 *    Adrian Whetten
 *    Jordan Burdett
 *    Caleb Georgeson
 *    Russell Roberts
 *    Stephen Ellis
 *    Orion Christensen
 * 
 * Summary:
 *    This class stores the notion of a message
 ************************************************************************/

#include <iostream>   // standard input and output
#include <cassert>    // because I am paraniod
#include "message.h"  // for the header file
using namespace std;

/**************************************************
 * MESSAGE DEFAULT CONSTRUCTOR
 * Set a message to empty
 **************************************************/
Message ::  Message()
{
   empty = true;
   text = "Empty";
   id = idNext++;
   control = Control::PUBLIC;
}


/**************************************************
 * MESSAGE NON-DEFAULT CONSTRUCTOR
 * Create a message and fill it
 **************************************************/
Message::Message(const string & text,
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

bool Message::securityConditionRead(Control control, Control subject)const{
   return subject >= control;
};

bool Message::securityConditionWrite(Control control, Control subject)const{
   return subject <= control;
};

/**************************************************
 * MESSAGE :: DISPLAY PROPERTIES
 * Display the attributes/properties but not the
 * content of this message
 **************************************************/
void Message::displayProperties() const
{
   // skip this one if there is nothing to see
   if (empty)
      return;
   
   // display the message
   cout << "\t[" << id << "] ";
      cout << "Message from " << author << " at " << date;
   cout << endl;
}

/**************************************************
 * MESSAGE :: DISPLAY TEXT
 * Display the contents or the text of the message
 **************************************************/
void Message::displayText(Control subject) const
{
   if (!securityConditionRead(control, subject)){
      cout << "\nRead access denied\n";
      return;
   };
   
   cout << "\tMessage: " << text << "\n";
}

/**************************************************
 * MESSAGE :: UPDATE TEXT
 * Update the contents or text of the message
 **************************************************/
void Message::updateText(Control subject, const string & newText)
{
   if (!securityConditionWrite(control, subject)){
      cout << "\nWrite access denied\n";
      return;
   };

   text = newText;
}

/**************************************************
 * MESSAGE :: CLEAR
 * Delete the contents of a message and mark it as empty
 *************************************************/
void Message::clear(Control subject)
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

int Message::idNext = 100;
