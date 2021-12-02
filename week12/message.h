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

#pragma once

#include <string>     // for convenience
#include "control.h"  // for all the control stuff


/**************************************************
 * MESSAGE
 * one message to be displayed to the user or not
 **************************************************/
class Message
{
public:
   // default constructor for an empty message
   Message();

   // the most commonly used constructor: create a message
   Message(const std::string & text,
           const std::string & author,
           const std::string & date,
           Control control);

   // determine the unique ID of this message
   int getID() const { return id; }

   // determine the unique ID of this message
   Control getControl() const { return control; }

   // display the properties but not content of the message
   void displayProperties() const;

   // display the content of the message
   void displayText(Control subject) const;

   // update the text componnet of the message
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
