// Occurrence.h <<BusinessObject>>
// This file contains the Occurrence class definition.
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

#ifndef _Occurrence_h_
#define _Occurrence_h_

class Occurrence;

#include <Task.h>

#include <string>
#include <ctime>

class Occurrence
{
   public:
      Occurrence(const time_t& date=0, const float& hours=0);

      const long& get_id();
      void set_id(const long& id);

      const time_t& get_when();
      void set_when(const time_t& when);

      const float& get_time();
      void set_time(const float& time);

      const std::string& get_notes();
      void set_notes(const std::string& notes);

      Task* get_task();
      void set_task(Task* task);

      bool is_empty();

      static bool sort(Occurrence* o1, Occurrence* o2);

   private:
      long id_;
      time_t when_;
      float time_;
      std::string notes_;
      Task* task_;
};

#endif

