// ProjectDAO.h <<DataAccessObject>> <<Singleton>>
// This file contains the ProjectDAO class definition
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

#ifndef _ProjectDAO_h_
#define _ProjectDAO_h_

class ProjectDAO;

#include <vector>
#include <Delete.h>
#include <FindID.h>
#include <Database.h>
#include <KeyDAO.h>
#include <TaskDAO.h>
#include <Project.h>

#include <Subject.h>

struct ProjectData
{
   long id_;
   char name_[21];
   char description_[61];
};

class ProjectDAO: public Subject
{
   public:
      // Meyers Singleton
      static ProjectDAO& get_dao();
      //

      // persistency methods
      Project* get(const long& id=0,
                   const bool& next=true);
      std::vector<Project*>& get_all();
      void get_tasks(Project& project);
      bool save(Project& project, const bool& do_notify_observers=true);
      bool remove(const long& id, const bool& do_notify_observers=true);

      long count(const bool& refresh=true);

      std::vector<std::string> dump();

   private:
      Database db_;

      // vector to keep all created project objects
      // in order to remove them by the end of
      // this object life
      std::vector<Project*> projects_;

      // Meyers Singleton, constructor is hidden
      ProjectDAO();

      // destructor is also hidden
      ~ProjectDAO();
};

#endif

