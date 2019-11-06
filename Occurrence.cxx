// Occurrence.cxx <<BusinessObject>>
// This file contains the Occurrence class methods.
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

#include <Occurrence.h>

Occurrence::Occurrence(const time_t& when, const float& time)
: id_(0),
  when_(when),
  time_(time),
  notes_(""),
  task_(0)
{
}

const long&
Occurrence::get_id()
{
   return id_;
}

void
Occurrence::set_id(const long& id)
{
   id_=id;
}

const time_t&
Occurrence::get_when()
{
   return when_;
}

void
Occurrence::set_when(const time_t& when)
{
   when_=when;
   if(task_ != 0)
   {
      if(task_->get_begin() == 0)
      {
         task_->set_begin(when_);
      }
   }
}

const float&
Occurrence::get_time()
{
   return time_;
}

void
Occurrence::set_time(const float& time)
{
   time_=time;
}

const std::string&
Occurrence::get_notes()
{
   return notes_;
}

void
Occurrence::set_notes(const std::string& notes)
{
   notes_=notes;
}

Task*
Occurrence::get_task()
{
   return task_;
}

void
Occurrence::set_task(Task* task)
{
   if(task_ != 0)
   {
      std::vector<Occurrence*>& occs=task_->get_occurrences();
      std::vector<Occurrence*>::iterator it=std::find(occs.begin(), occs.end(), this);

      if(it != occs.end())
      {
         occs.erase(it);
      }
   }
   task_=task;
   if(task_ != 0)
   {
      if(task_->get_status() == Task::NOT_STARTED)
      {
         task_->set_status(Task::IN_PROGRESS);
      }
      if(task_->get_begin() == 0)
      {
         task_->set_begin(when_);
      }
      task_->get_occurrences().push_back(this);
   }
}

bool
Occurrence::is_empty()
{
   if(id_ == 0 && when_ == 0 && time_ == 0 &&
      notes_.empty() && task_ == 0)
   {
      return true;
   }
   else
   {
      return false;
   }
}

bool
Occurrence::sort(Occurrence* o1, Occurrence* o2)
{
   return o1->get_when() > o2->get_when();
}

