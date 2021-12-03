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

#include <string>
#include <list>
#include <iostream>
#include <fstream>
#include <cassert>
#include "control.h"
#include "user.h"
#include "users.h"

using namespace std;

/***********************************************
 * display the list of users
 ***********************************************/
void Users::display() const
{
   for (list <User> :: const_iterator it = users.begin();
        it != users.end();
        ++it)
      it->displayProperties();
}

/***********************************************
 * show a single user
 **********************************************/
void Users::show(int id, Control subject) const
{
   for (list <User> :: const_iterator it = users.begin();
        it != users.end(); ++it)
      if (it->getID() == id)
         it->displayText(it->getControl());
}

/***********************************************
 * update one single user
 ***********************************************/
void Users::update(int id, const string & text, Control subject)
{
   for (list <User> :: iterator it = users.begin();
        it != users.end();
        ++it)
      if (it->getID() == id)
         it->updateText(it->getControl(), text);
}

/***********************************************
 * remove a single user
 **********************************************/
void Users::remove(int id, Control subject)
{
   for (list <Users> :: iterator it = users.begin();
        it != users.end();
        ++it)
      if (it->getID() == id)
         it->clear(it->getControl());
}

/***********************************************
 * add a new user
 **********************************************/
void Users::add(const string & text,
                   const string & author,
                   const string & date,
                   Control subject)
{
   User user(text, author, date, subject);
   users.push_back(user);
}

/***********************************************
 * read the users from a file
 ***********************************************/
void Users::readUsers(const char * fileName)
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
         User user(text, author, date, control);
         Users.push_back(user);
      }
   }

   // close up shop!
   fin.close();
}
