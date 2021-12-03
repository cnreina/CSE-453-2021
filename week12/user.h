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

#pragma once

#include <string>     // for convenience
#include "control.h"  // for all the control stuff

/**************************************************
 * one user to be displayed to the user or not
 **************************************************/
class User
{
public:
   // default constructor for an empty user
   User();

   // the most commonly used constructor: create a user
   User(const std::string & text,
           const std::string & author,
           const std::string & date,
           Control control);

   // determine the unique ID of this user
   int getID() const { return id; }

   // determine the unique ID of this user
   Control getControl() const { return control; }

   // display the properties but not content of the user
   void displayProperties() const;

   // display the content of the user
   void displayText(Control subject) const;

   // update the text componnet of the user
   void updateText(Control subject, const std::string & newText);

   // clear out a message
   void clear(Control subject);
   
private:
   int id;                   // the unique ID of this message
   static int idNext;        // the id of the next message created
   bool empty;               // is this message empty / cleared?
   std::string text;         // the textual content of this message
   std::string author;       // the author of this message
   std::string date;         // the date this message was created

   Control control;
   bool securityConditionRead(Control control, Control subject) const;
   bool securityConditionWrite(Control control, Control subject) const;
};
