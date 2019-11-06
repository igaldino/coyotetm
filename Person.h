// Person.h <<BusinessObject>>
// This file contains the Person class definition.
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
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
// 02111-1307, USA.

#ifndef _Person_h_
#define _Person_h_

class Person;

#include <string>
#include <vector>
#include <Task.h>

class Person
{
   public:
      Person(const long& id=0, const std::string& name="");

      const long& get_id();
      void set_id(const long& id);

      const std::string& get_name();
      void set_name(const std::string& name);

      std::vector<Task*>& get_tasks();
      void set_tasks(const std::vector<Task*>& tasks);

      static bool sort_by_name(const Person* first, const Person* second);

   private:
      long id_;
      std::string name_;

      std::vector<Task*> tasks_;
};

#endif
