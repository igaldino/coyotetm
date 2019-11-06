// Task.h <<BusinessObject>>
// This file contains the Task class definition.
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

#ifndef _Task_h_
#define _Task_h_

class Task;

#include <Project.h>
#include <Person.h>
#include <Occurrence.h>
#include <Date.h>

#include <string>
#include <vector>
#include <algorithm>

class Task
{
   public:
      Task(const std::string& description="");

      enum
      {
         NOT_STARTED=1,
         IN_PROGRESS,
         COMPLETED,
         CANCELED,
         ON_HOLD
      };

      enum
      {
         CRITICAL=1,
         HIGH,
         MEDIUM,
         LOW,
         NONE
      };

      static const int default_status;
      static const int default_priority;

      const long& get_id();
      void set_id(const long& id);

      const std::string& get_description();
      void set_description(const std::string& description);

      const std::string& get_notes();
      void set_notes(const std::string& notes);

      const time_t& get_begin();
      void set_begin(const time_t& begin);

      const time_t& get_end();
      void set_end(const time_t& end);

      const time_t& get_due();
      void set_due(const time_t& due);

      const int& get_status();
      void set_status(const int& status);

      const int& get_priority();
      void set_priority(const int& priority);

      Project* get_project();
      void set_project(Project* project);

      Person* get_person();
      void set_person(Person* person);

      std::vector<Occurrence*>& get_occurrences();
      void set_occurrences(const std::vector<Occurrence*>& occs);

      double calc_total_hours();

      void cancel();
      void mark_completed();

      static bool sort_by_description(const Task* first, const Task* second);
      static bool sort_by_status(const Task* first, const Task* second);
      static bool sort_by_priority(const Task* first, const Task* second);

      static std::string status_to_string(const int& status);
      static std::string priority_to_string(const int& priority);

   private:
      long id_;
      std::string description_;
      std::string notes_;
      time_t begin_;
      time_t end_;
      time_t due_;
      int status_;
      int priority_;
      Project* project_;
      Person* person_;
      std::vector<Occurrence*> occs_;
};

#endif

