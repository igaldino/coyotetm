// TaskDAO.h <<DataAccessObject>> <<Singleton>>
// This file contains the TaskDAO class definition
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

#ifndef _TaskDAO_h_
#define _TaskDAO_h_

class TaskDAO;

#include <vector>
#include <sstream>
#include <Delete.h>
#include <FindID.h>
#include <Database.h>
#include <KeyDAO.h>
#include <PersonDAO.h>
#include <ProjectDAO.h>
#include <OccurrenceDAO.h>
#include <Task.h>

#include <Subject.h>

struct TaskData
{
   long id_;
   long project_id_;
   long person_id_;
   char description_[61];
   char notes_[1025];
   time_t begin_;
   time_t end_;
   time_t due_;
   int status_;
   int priority_;
};

class TaskDAO: public Subject
{
   public:
      // Meyers Singleton
      static TaskDAO& get_dao();

      // Callback function for index database
      static int map_project_idx(Db* db,
                                 const Dbt* key,
                                 const Dbt* data,
                                 Dbt* skey);
      static int map_person_idx(Db* db,
                                const Dbt* key,
                                const Dbt* data,
                                Dbt* skey);

      // persistency methods
      Task* get(const long& id=0,
                const bool& next=true);
      Task* get_by_project(const long& id=0,
                           const bool& next=true);
      Task* get_by_person(const long& id=0,
                          const bool& next=true);
      Task* get_by_project_and_person(const long& id_project=0,
                                      const long& id_person=0,
                                      const bool& next=true);
      std::vector<Task*>& get_all();
      void get_all_by_project(const long& id,
                              std::vector<Task*>& ts);
      void get_all_by_person(const long& id,
                             std::vector<Task*>& ts);
      void get_all_by_project_and_person(const long& id_project,
                                         const long& id_person,
                                         std::vector<Task*>& ts);
      void get_occurrences(Task& task);
      bool save(Task& task, const bool& do_notify_observers=true);
      bool remove(const long& id, const bool& do_notify_observers=true);

      long count(const bool& refresh=true);
      long count_opened(const bool& refresh=false);

      std::vector<std::string> dump();

   private:
      Database db_;
      Database project_idx_;
      Database person_idx_;

      // vector to keep all created task objects
      // in order to remove them by the end of
      // this object life
      std::vector<Task*> tasks_;

      // Meyers Singleton, constructor is hidden
      TaskDAO();

      // destructor is also hidden
      ~TaskDAO();

      // generic get for all task access
      Task* get(Database& db,
                const long& id=0,
                const bool& next=true);
};

#endif
