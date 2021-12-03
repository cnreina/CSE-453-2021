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

#include <string>
#include <list>
#include "control.h"
#include "user.h"

/***********************************************
 * The collection of users
 ***********************************************/
class Users
{
public:
   // constructor: read a file to fill the users
   Users(const char * fileName) { readUsers(fileName); }

   // display the list of users
   void display() const;

   // show a single user
   void show(int id, Control subject) const;

   // update one single user
   void update(int id,
               const std::string & text,
               Control subject);

   // remove a single user
   void remove(int id, Control subject);

   // add a new user
   void add(const std::string & text,
            const std::string & author,
            const std::string & date,
            Control subject);

private:
   // the list of users
   std::list <Users> users;

   // read the users from a file
   void readUsers(const char * fileName);
};
