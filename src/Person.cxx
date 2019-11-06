// Person.cxx <<BusinessObject>>
// This file contains the Person class methods.
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

#include <Person.h>
#include <iostream>

Person::Person(const long& id, const std::string& name):
   id_(id), name_(name), tasks_()
{
}

const long&
Person::get_id()
{
   return id_;
}

void
Person::set_id(const long& id)
{
   id_=id;
}

const std::string&
Person::get_name()
{
   return name_;
}

void
Person::set_name(const std::string& name)
{
   name_=name;
}

std::vector<Task*>&
Person::get_tasks()
{
   return tasks_;
}

void
Person::set_tasks(const std::vector<Task*>& tasks)
{
   tasks_.clear();
   std::copy(tasks.begin(), tasks.end(), std::back_inserter(tasks_));
}

bool
Person::sort_by_name(const Person* first, const Person* second)
{
   // compare using lowercase
   std::string n1=first->name_;
   std::string n2=second->name_;

   std::transform(n1.begin(), n1.end(), n1.begin(), ::tolower);
   std::transform(n2.begin(), n2.end(), n2.begin(), ::tolower);

   return n1 < n2;
}

