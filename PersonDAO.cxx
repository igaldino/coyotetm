// PersonDAO.cxx <<DataAccessObject>> <<Singleton>>
// This file contains the routines to manipulate data.
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

#include <PersonDAO.h>

// Meyers singleton
PersonDAO&
PersonDAO::get_dao()
{
   static PersonDAO dao;
   return dao;
}

// Hidden constructor
PersonDAO::PersonDAO():
   Subject("person"),
   db_("person.db"),
   people_()
{
}

// hidden destructor
PersonDAO::~PersonDAO()
{
   for_each(people_.begin(), people_.end(), Delete<Person>());
}

// Public methods
Person*
PersonDAO::get(const long& id,
               const bool& next)
{
   Person* person=0;

   PersonData* person_data=(PersonData*)db_.get(id, next);
   if(person_data != 0)
   {
      // first try to find the person by its id in the vector
      std::vector<Person*>::iterator it=
         std::find_if(people_.begin(),
                      people_.end(),
                      FindID<Person>(person_data->id_));

      if(it != people_.end())
      {
         person=*it;
         person->set_name(person_data->name_);
         person->get_tasks().clear();
      }
      // if not found, create a new one
      else
      {
         person=new Person(person_data->id_, person_data->name_);
         people_.push_back(person);
      }
   }

   return person;
}

std::vector<Person*>&
PersonDAO::get_all()
{
   for(Person* p=get(0, false); p != 0; p=get())
   {
   }
   return people_;
}

void
PersonDAO::get_tasks(Person& person)
{
   std::vector<Task*> tasks;
   TaskDAO::get_dao().get_all_by_person(person.get_id(), tasks);
   person.set_tasks(tasks);
}

bool
PersonDAO::save(Person& person,
                const bool& do_notify_observers)
{
   // get key value
   if(person.get_id() == 0)
   {
      person.set_id(KeyDAO::get_dao().get(KeyDAO::PERSON_KEY));
   }

   // fulfill data object
   PersonData person_data;
   person_data.id_=person.get_id();
   strncpy(person_data.name_, person.get_name().c_str(), 60);
   person_data.name_[60]=0;

   // save it
   bool ok=db_.save(person_data.id_, (void*)&person_data, sizeof(PersonData));
   if(ok && do_notify_observers)
   {
      notify_observers();
   }
   return ok;
}

bool
PersonDAO::remove(const long& id,
                  const bool& do_notify_observers)
{
   // first get the object
   Person* p=get(id);
   if(p != 0)
   {
      // load its dependents
      get_tasks(*p);
      std::vector<Task*>::const_iterator it=p->get_tasks().begin();
      std::vector<Task*>::const_iterator end=p->get_tasks().end();

      // remove the children first
      for(; it != end; it++)
      {
         TaskDAO::get_dao().remove((*it)->get_id(), false);
      }

      // remove it
      bool ok=db_.remove(id);
      if(ok)
      {
         // try to find the person by its id in the vector
         std::vector<Person*>::iterator it=
            std::find_if(people_.begin(),
                         people_.end(),
                         FindID<Person>(id));
         // if found, remove it
         if(it != people_.end())
         {
            delete (*it);
            people_.erase(it);
         }
         if(do_notify_observers)
         {
            notify_observers();
            TaskDAO::get_dao().notify_observers();
            OccurrenceDAO::get_dao().notify_observers();
         }
      }
      return ok;
   }

   return false;
}

long
PersonDAO::count(const bool& refresh)
{
   if(refresh)
   {
      get_all();
   }
   return people_.size();
}

std::vector<std::string>
PersonDAO::dump()
{
   std::vector<std::string> tmp;
   std::ostringstream ss;
   Person* p;
   get_all();
   for(std::vector<Person*>::const_iterator it=people_.begin(), end=people_.end(); it != end; it++)
   {
      p=*it;
      ss.str("");
      ss << "\"PERSON\",\"";
      ss << p->get_id();
      ss << "\",\"";
      ss << Database::encode(p->get_name());
      ss << "\"";
      tmp.push_back(ss.str());
   }
   return tmp;
}

