// Project.h <<BusinessObject>>
// This file contains the Project class definition.
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

#ifndef _Project_h_
#define _Project_h_

class Project;

#include <string>
#include <vector>
#include <Task.h>

class Project
{
   public:
      Project(const long& id=0,
              const std::string& name="",
              const std::string& description="");

      const long& get_id();
      void set_id(const long& id);

      const std::string& get_name();
      void set_name(const std::string& name);

      const std::string& get_description();
      void set_description(const std::string& description);

      std::vector<Task*>& get_tasks();
      void set_tasks(const std::vector<Task*>& tasks);

      static bool sort_by_name(const Project* first, const Project* second);
      static bool sort_by_description(const Project* first, const Project* second);

   private:
      long id_;
      std::string name_;
      std::string description_;
      std::vector<Task*> tasks_;
};

#endif

