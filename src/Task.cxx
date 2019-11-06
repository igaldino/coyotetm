// Task.cxx <<BusinessObject>>
// This file contains the Task class methods.
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

#include <Task.h>

const int Task::default_status=Task::NOT_STARTED;
const int Task::default_priority=Task::NONE;

Task::Task(const std::string& description): id_(0), description_(description),
   notes_(""), begin_(0), end_(0), due_(0), status_(Task::NOT_STARTED),
   priority_(Task::NONE), project_(0), person_(0), occs_()
{
}

const long&
Task::get_id()
{
   return id_;
}

void
Task::set_id(const long& id)
{
   id_=id;
}

const std::string&
Task::get_description()
{
   return description_;
}

void
Task::set_description(const std::string& description)
{
   description_=description;
}

const std::string&
Task::get_notes()
{
   return notes_;
}

void
Task::set_notes(const std::string& notes)
{
   notes_=notes;
}

const time_t&
Task::get_begin()
{
   return begin_;
}

void
Task::set_begin(const time_t& begin)
{
   begin_=begin;
}

const time_t&
Task::get_end()
{
   return end_;
}

void
Task::set_end(const time_t& end)
{
   end_=end;
}

const time_t&
Task::get_due()
{
   return due_;
}

void
Task::set_due(const time_t& due)
{
   due_=due;
}

const int&
Task::get_status()
{
   return status_;
}

void
Task::set_status(const int& status)
{
   status_=status;

   if(status_ == COMPLETED ||
      status_ == CANCELED)
   {
      Date d=Date::get_today();
      if(begin_ == 0)
      {
         begin_=d.get_time();
      }
      if(end_ == 0)
      {
         end_=d.get_time();
      }
   }
}

const int&
Task::get_priority()
{
   return priority_;
}

void
Task::set_priority(const int& priority)
{
   priority_=priority;
}

Project*
Task::get_project()
{
   return project_;
}

void
Task::set_project(Project* project)
{
   if(project_ != 0)
   {
      std::vector<Task*>& tasks=project_->get_tasks();
      std::vector<Task*>::iterator it=std::find(tasks.begin(), tasks.end(), this);

      if(it != tasks.end())
      {
         tasks.erase(it);
      }
   }
   project_=project;
   if(project_ != 0)
   {
      project_->get_tasks().push_back(this);
   }
}

Person*
Task::get_person()
{
   return person_;
}

void
Task::set_person(Person* person)
{
   if(person_ != 0)
   {
      std::vector<Task*>& tasks=person_->get_tasks();
      std::vector<Task*>::iterator it= std::find(tasks.begin(), tasks.end(), this);

      if(it != tasks.end())
      {
         tasks.erase(it);
      }
   }
   person_=person;
   if(person_ != 0)
   {
      person_->get_tasks().push_back(this);
   }
}

std::vector<Occurrence*>&
Task::get_occurrences()
{
   return occs_;
}

void
Task::set_occurrences(const std::vector<Occurrence*>& occs)
{
   occs_.clear();
   std::copy(occs.begin(), occs.end(), std::back_inserter(occs_));
}

double
Task::calc_total_hours()
{
   double total_hours=0.0f;
   std::vector<Occurrence*>::const_iterator it=occs_.begin();
   std::vector<Occurrence*>::const_iterator end=occs_.end();

   for(; it != end; it++)
   {
      total_hours+=(*it)->get_time();
   }

   return total_hours;
}

void
Task::cancel()
{
   set_status(CANCELED);
}

void
Task::mark_completed()
{
   set_status(COMPLETED);
}

bool
Task::sort_by_description(const Task* first, const Task* second)
{
   // compare using lowercase
   std::string d1=first->description_;
   std::string d2=second->description_;

   std::transform(d1.begin(), d1.end(), d1.begin(), ::tolower);
   std::transform(d2.begin(), d2.end(), d2.begin(), ::tolower);

   return d1 < d2;
}

bool
Task::sort_by_status(const Task* first, const Task* second)
{
   int p1 = first->priority_;
   int p2 = second->priority_;
   int s1 = first->status_ * 10 + p1;
   int s2 = second->status_ * 10 + p2;
   return s1 < s2;
}

bool
Task::sort_by_priority(const Task* first, const Task* second)
{
   int s1 = first->status_;
   int s2 = second->status_;
   int p1 = first->priority_ * 10 + s1;
   int p2 = second->priority_ * 10 + s2;
   p1 *= s1 >= COMPLETED ? 10 : 1;
   p2 *= s2 >= COMPLETED ? 10 : 1;
   return p1 < p2;
}

std::string
Task::status_to_string(const int& status)
{
   switch(status)
   {
      case Task::NOT_STARTED:
         return "NOT STARTED";
      case Task::IN_PROGRESS:
         return "IN PROGRESS";
      case Task::COMPLETED:
         return "COMPLETED";
      case Task::CANCELED:
         return "CANCELED";
      case Task::ON_HOLD:
         return "ON HOLD";
   }
   return "NA";
}

std::string
Task::priority_to_string(const int& priority)
{
   switch(priority)
   {
      case Task::CRITICAL:
         return "CRITICAL";
      case Task::HIGH:
         return "HIGH";
      case Task::MEDIUM:
         return "MEDIUM";
      case Task::LOW:
         return "LOW";
      case Task::NONE:
         return "NONE";
   }
   return "NA";
}

