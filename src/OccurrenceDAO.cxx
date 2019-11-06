// OccurrenceDAO.cxx <<DataAccessObject>> <<Singleton>>
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

#include <OccurrenceDAO.h>

// Meyers singleton
OccurrenceDAO&
OccurrenceDAO::get_dao()
{
   static OccurrenceDAO dao;
      return (dao);
}

// Hidden constructor
OccurrenceDAO::OccurrenceDAO():
   Subject("occurrence"),
   db_("occ.db"),
   task_idx_("occ_task.idx", true),
   occurrences_()
{
   task_idx_.associate(db_, map_task_idx);
}

// hidden destructor
OccurrenceDAO::~OccurrenceDAO()
{
   for_each(occurrences_.begin(), occurrences_.end(), Delete<Occurrence>());
}

// Callback function for index database
int
OccurrenceDAO::map_task_idx(Db* db __attribute__((unused)),
                            const Dbt* key __attribute__((unused)),
                            const Dbt* data,
                            Dbt* skey)
{
   OccurrenceData* occ=(OccurrenceData*)data->get_data();

   skey->set_data(&occ->task_id_);
   skey->set_size(sizeof(occ->task_id_));

   return 0;
}

Occurrence*
OccurrenceDAO::get(Database& db,
                   const long& id,
                   const bool& next)
{
   Occurrence* occ=0;

   OccurrenceData* occ_data=(OccurrenceData*)db.get(id, next);
   if(occ_data != 0)
   {
      // first try to find the occurrence by its id in the vector
      std::vector<Occurrence*>::iterator it=
         std::find_if(occurrences_.begin(),
                      occurrences_.end(),
                      FindID<Occurrence>(occ_data->id_));
      if(it != occurrences_.end())
      {
         occ=*it;
         occ->set_when(occ_data->when_);
         occ->set_time(occ_data->time_);
      }
      // if not found, create a new one
      else
      {
         occ=new Occurrence(occ_data->when_, occ_data->time_);
         occurrences_.push_back(occ);
      }
      occ->set_id(occ_data->id_);
      occ->set_notes(occ_data->notes_);
      if(occ_data->task_id_ != 0)
      {
         occ->set_task(TaskDAO::get_dao().get(occ_data->task_id_));
      }
   }

   return occ;
}

// Public methods
Occurrence*
OccurrenceDAO::get(const long& id,
                   const bool& next)
{
   return get(db_, id, next);
}

Occurrence*
OccurrenceDAO::get_by_task(const long& id,
                           const bool& next)
{
   return get(task_idx_, id, next);
}

std::vector<Occurrence*>&
OccurrenceDAO::get_all()
{
   for(Occurrence* o=get(0, false); o !=0; o=get())
   {
   }
   return occurrences_;
}

void
OccurrenceDAO::get_all_by_task(const long& id,
                               std::vector<Occurrence*>& occs)
{
   for(Occurrence* o=get_by_task(id, false); o !=0; o=get_by_task())
   {
      occs.push_back(o);
   }
}

bool
OccurrenceDAO::save(Occurrence& occ,
                    const bool& do_notify_observers)
{
   // get key value
   if(occ.get_id() == 0)
   {
      occ.set_id(KeyDAO::get_dao().get(KeyDAO::OCCURRENCE_KEY));
   }

   // fulfill data object
   OccurrenceData occ_data;
   occ_data.id_=occ.get_id();
   occ_data.when_=occ.get_when();
   occ_data.time_=occ.get_time();
   strncpy(occ_data.notes_, occ.get_notes().c_str(), 1024);
   occ_data.notes_[1024]=0;
   occ_data.task_id_=occ.get_task() ? occ.get_task()->get_id() : 0;

   // save it
   bool ok=db_.save(occ_data.id_, (void*)&occ_data, sizeof(OccurrenceData));
   if(ok && do_notify_observers)
   {
      notify_observers();
   }
   return ok;
}

bool
OccurrenceDAO::remove(const long& id,
                      const bool& do_notify_observers)
{
   // remove it
   bool ok=db_.remove(id);
   if(ok)
   {
      // try to find the occurrence by its id in the vector
      std::vector<Occurrence*>::iterator it=
         std::find_if(occurrences_.begin(),
                      occurrences_.end(),
                      FindID<Occurrence>(id));
      // if found, then remove it from vector and free memory
      if(it != occurrences_.end())
      {
         delete (*it);
         occurrences_.erase(it);
      }
      if(do_notify_observers)
      {
         notify_observers();
      }
   }
   return ok;
}

long
OccurrenceDAO::count(const bool& refresh)
{
   if(refresh)
   {
      get_all();
   }
   return occurrences_.size();
}

std::vector<std::string>
OccurrenceDAO::dump()
{
   std::vector<std::string> tmp;
   std::ostringstream ss;
   Occurrence* o;
   get_all();
   for(std::vector<Occurrence*>::const_iterator it=occurrences_.begin(), end=occurrences_.end(); it != end; it++)
   {
      o=*it;
      ss.str("");
      ss << "\"OCCURRENCE\",\"";
      ss << o->get_id();
      ss << "\",\"";
      if(o->get_task())
      {
         ss << o->get_task()->get_id();
      }
      ss << "\",\"";
      ss << Date::time_to_string(o->get_when());
      ss << "\",\"";
      ss << std::fixed << std::setprecision(2) << o->get_time();
      ss << "\",\"";
      ss << Database::encode(o->get_notes());
      ss << "\"";
      tmp.push_back(ss.str());
   }
   return tmp;
}

