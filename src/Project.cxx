// Project.cxx <<BusinessObject>>
// This file contains the Project class methods.
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

#include <Project.h>

Project::Project(const long& id,
                 const std::string& name,
                 const std::string& description)
: id_(id),
  name_(name),
  description_(description),
  tasks_()
{
}

const long&
Project::get_id()
{
   return id_;
}

void
Project::set_id(const long& id)
{
   id_=id;
}

const std::string&
Project::get_name()
{
   return name_;
}

void
Project::set_name(const std::string& name)
{
   name_=name;
}

const std::string&
Project::get_description()
{
   return description_;
}

void
Project::set_description(const std::string& description)
{
   description_=description;
}

std::vector<Task*>&
Project::get_tasks()
{
   return tasks_;
}

void
Project::set_tasks(const std::vector<Task*>& tasks)
{
   tasks_.clear();
   std::copy(tasks.begin(), tasks.end(), std::back_inserter(tasks_));
}

bool
Project::sort_by_name(const Project* first, const Project* second)
{
   // compare using lowercase
   std::string n1=first->name_;
   std::string n2=second->name_;

   std::transform(n1.begin(), n1.end(), n1.begin(), ::tolower);
   std::transform(n2.begin(), n2.end(), n2.begin(), ::tolower);

   return n1 < n2;
}

bool
Project::sort_by_description(const Project* first, const Project* second)
{
   // compare using lowercase
   std::string d1=first->description_;
   std::string d2=second->description_;

   std::transform(d1.begin(), d1.end(), d1.begin(), ::tolower);
   std::transform(d2.begin(), d2.end(), d2.begin(), ::tolower);

   return d1 < d2;
}

