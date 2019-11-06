// PersonDAO.h <<DataAccessObject>> <<Singleton>>
// This file contains the PersonDAO class definition
//
// Copyright (C) 2000-2015 Isaque Galdino de Araujo.
// Written by Isaque Galdino de Araujo <igaldino@gmail.com>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
// 02111-1307, USA.

#ifndef _PersonDAO_h_
#define _PersonDAO_h_

class PersonDAO;

#include <vector>
#include <cstring>
#include <Delete.h>
#include <FindID.h>
#include <Database.h>
#include <KeyDAO.h>
#include <TaskDAO.h>
#include <Person.h>

#include <Subject.h>

struct PersonData
{
   long id_;
   char name_[61];
};

class PersonDAO: public Subject
{
   public:
      // Meyers Singleton
      static PersonDAO& get_dao();

      // persistency methods
      Person* get(const long& id=0,
                  const bool& next=true);
      std::vector<Person*>& get_all();
      void get_tasks(Person& person);
      bool save(Person& person, const bool& do_notify_observers=true);
      bool remove(const long& id, const bool& do_notify_observers=true);

      long count(const bool& refresh=true);

      std::vector<std::string> dump();

   private:
      Database db_;

      // vector to keep all created person objects
      // in order to remove them by the end of
      // this object life
      std::vector<Person*> people_;

      // Meyers Singleton, constructor is hidden
      PersonDAO();

      // destructor is also hidden
      ~PersonDAO();
};

#endif

