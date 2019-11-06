// OccurrenceDAO.h <<DataAccessObject>> <<Singleton>>
// This file contains the OccurrenceDAO class definition
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

#ifndef _OccurrenceDAO_h_
#define _OccurrenceDAO_h_

class OccurrenceDAO;

#include <vector>
#include <Delete.h>
#include <FindID.h>
#include <Database.h>
#include <KeyDAO.h>
#include <TaskDAO.h>
#include <Occurrence.h>

#include <Subject.h>

struct OccurrenceData
{
   long id_;
   long task_id_;
   time_t when_;
   float time_;
   char notes_[1025];
};

class OccurrenceDAO: public Subject
{
   public:
      // Meyers Singleton
      static OccurrenceDAO& get_dao();

      // Callback function for index database
      static int map_task_idx(Db* db,
                              const Dbt* key,
                              const Dbt* data,
                              Dbt* skey);

      // persistency methods
      Occurrence* get(const long& id=0,
                      const bool& next=true);
      Occurrence* get_by_task(const long& id=0,
                              const bool& next=true);
      std::vector<Occurrence*>& get_all();
      void get_all_by_task(const long& id,
                           std::vector<Occurrence*>& occs);
      bool save(Occurrence& occurrence, const bool& do_notify_observers=true);
      bool remove(const long& id, const bool& do_notify_observers=true);

      long count(const bool& refresh=true);

      std::vector<std::string> dump();

   private:
      Database db_;
      Database task_idx_;

      // vector to keep all created task objects
      // in order to remove them by the end of
      // this object life
      std::vector<Occurrence*> occurrences_;

      // Meyers Singleton, constructor is hidden
      OccurrenceDAO();

      // destructor is also hidden
      ~OccurrenceDAO();

      // generic get for all task access
      Occurrence* get(Database& db,
                      const long& id=0,
                      const bool& next=true);
};

#endif

