/***********************************************************************
 * COMPONENT:
 *    MESSAGES
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
 *    This class stores the notion of a collection of messages
 ************************************************************************/

#include <string>     // for convenience
#include <list>       // to store the messages
#include <iostream>   // standard input and output
#include <fstream>    // the messages are read from a file
#include <cassert>    // because I am paraniod
#include "control.h"  // all the Bell-LaPadula code
#include "message.h"  // all the code about a single message
#include "messages.h" // a collection messages

using namespace std;

/***********************************************
 * MESSAGES :: DISPLAY
 * display the list of messages
 ***********************************************/
void Messages::display() const
{
   for (list <Message> :: const_iterator it = messages.begin();
        it != messages.end();
        ++it)
      it->displayProperties();
}

/***********************************************
 * MESSAGES :: SHOW
 * show a single message
 **********************************************/
void Messages::show(int id, Control subject) const
{
   for (list <Message> :: const_iterator it = messages.begin();
        it != messages.end(); ++it)
      if (it->getID() == id){
         if (!securityConditionRead(it->getControl(), subject)){
            cout << "\nRead access denied\n";
            return;
         };

         it->displayText(it->getControl());
      };
}

/***********************************************
 * MESSAGES :: UPDATE
 * update one single message
 ***********************************************/
void Messages::update(int id, const string & text, Control subject)
{
   for (list <Message> :: iterator it = messages.begin();
        it != messages.end();
        ++it)
      if (it->getID() == id)
         it->updateText(it->getControl(), text);
}

/***********************************************
 * MESSAGES :: REMOVE
 * remove a single message
 **********************************************/
void Messages::remove(int id, Control subject)
{
   for (list <Message> :: iterator it = messages.begin();
        it != messages.end();
        ++it)
      if (it->getID() == id)
         it->clear(it->getControl());
}

/***********************************************
 * MESSAGES :: ADD
 * add a new message
 **********************************************/
void Messages::add(const string & text,
                   const string & author,
                   const string & date,
                   Control subject)
{
   Message message(text, author, date, subject);
   messages.push_back(message);
}

/***********************************************
 * MESSAGES :: READ MESSAGES
 * read the messages from a file
 ***********************************************/
void Messages::readMessages(const char * fileName)
{
   // open the file
   ifstream fin(fileName);
   if (fin.fail())
   {
      cout << "ERROR! Unable to open file "
           << fileName
           << endl;
      return;
   }

   // continue reading until we fail
   while (!fin.fail() && !fin.eof())
   {
      string author;
      string date;
      string text;
      string textControl;
      getline(fin, textControl, '|');
      getline(fin, author, '|');
      getline(fin, date, '|');
      getline(fin, text);

      Control control;
      if(textControl == "Public"){
         control = Control::PUBLIC;
      };
      if(textControl == "Confidential"){
         control = Control::CONFIDENTIAL;
      };
      if(textControl == "Privileged"){
         control = Control::PRIVILEGED;
      };
      if(textControl == "Secret"){
         control = Control::SECRET;
      };
      
      if (!fin.fail())
      {
         Message message(text, author, date, control);
         messages.push_back(message);
      }
   }

   // close up shop!
   fin.close();
}
