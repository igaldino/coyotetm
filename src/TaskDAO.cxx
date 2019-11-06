// TaskDAO.cxx <<DataAccessObject>> <<Singleton>>
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

#include <TaskDAO.h>

// Meyers singleton
TaskDAO&
TaskDAO::get_dao()
{
   static TaskDAO dao;
   return (dao);
}

// Hidden constructor
TaskDAO::TaskDAO():
   Subject("task"),
   db_("task.db"),
   project_idx_("task_prj.idx", true),
   person_idx_("task_per.idx", true),
   tasks_()
{
   project_idx_.associate(db_, map_project_idx);
   person_idx_.associate(db_, map_person_idx);
}

// hidden destructor
TaskDAO::~TaskDAO()
{
   for_each(tasks_.begin(), tasks_.end(), Delete<Task>());
}

// Callback function for index database
int
TaskDAO::map_project_idx(Db* db __attribute__((unused)), const Dbt* key __attribute__((unused)), const Dbt* data, Dbt* skey)
{
   TaskData* task=(TaskData*)data->get_data();

   skey->set_data(&task->project_id_);
   skey->set_size(sizeof(task->project_id_));

   return 0;
}

int
TaskDAO::map_person_idx(Db* db __attribute__((unused)), const Dbt* key __attribute__((unused)), const Dbt* data, Dbt* skey)
{
   TaskData* task=(TaskData*)data->get_data();

   skey->set_data(&task->person_id_);
   skey->set_size(sizeof(task->person_id_));

   return 0;
}

Task*
TaskDAO::get(Database& db, const long& id, const bool& next)
{
   Task* task=0;

   TaskData* task_data=(TaskData*)db.get(id, next);
   if(task_data != 0)
   {
      // first try to find the task by its id in the vector
      std::vector<Task*>::iterator it=
         std::find_if(tasks_.begin(),
                      tasks_.end(),
                      FindID<Task>(task_data->id_));
      if(it != tasks_.end())
      {
         task=*it;
         task->set_description(task_data->description_);
         task->get_occurrences().clear();
      }
      // if not found, create a new one
      else
      {
         task=new Task(task_data->description_);
         tasks_.push_back(task);
      }
      task->set_id(task_data->id_);
      task->set_notes(task_data->notes_);
      task->set_begin(task_data->begin_);
      task->set_end(task_data->end_);
      task->set_due(task_data->due_);
      task->set_status(task_data->status_);
      task->set_priority(task_data->priority_);
      if(task_data->person_id_ != 0)
      {
         task->set_person(PersonDAO::get_dao().get(task_data->person_id_));
      }
      if(task_data->project_id_ != 0)
      {
         task->set_project(ProjectDAO::get_dao().get(task_data->project_id_));
      }
   }

   return task;
}


// Public methods
Task*
TaskDAO::get(const long& id,
             const bool& next)
{
   return get(db_, id, next);
}

Task*
TaskDAO::get_by_project(const long& id,
                        const bool& next)
{
   return get(project_idx_, id, next);
}

Task*
TaskDAO::get_by_person(const long& id,
                       const bool& next)
{
   return get(person_idx_, id, next);
}

Task*
TaskDAO::get_by_project_and_person(const long& id_project,
                                   const long& id_person,
                                   const bool& next)
{
   if(next == false)
   {
      if(id_project == 0 || id_person == 0)
      {
         return 0;
      }

      get(project_idx_, id_project, next);
      get(person_idx_, id_person, next);

      std::vector<Database*> indexes;
      indexes.push_back(&project_idx_);
      indexes.push_back(&person_idx_);

      if(db_.join(indexes) == false)
      {
         return 0;
      }
   }

   Task* task=get(0, true);
   if(task == 0)
   {
      db_.unjoin();
   }
   return task;
}

std::vector<Task*>&
TaskDAO::get_all()
{
   for(Task* t=get(0, false); t != 0; t=get())
   {
   }
   return tasks_;
}

void
TaskDAO::get_all_by_project(const long& id,
                            std::vector<Task*>& ts)
{
   for(Task* t=get_by_project(id, false); t != 0; t=get_by_project())
   {
      ts.push_back(t);
   }
}

void
TaskDAO::get_all_by_person(const long& id,
                           std::vector<Task*>& ts)
{
   for(Task* t=get_by_person(id, false); t != 0; t=get_by_person())
   {
      ts.push_back(t);
   }
}

void
TaskDAO::get_all_by_project_and_person(const long& id_project,
                                       const long& id_person,
                                       std::vector<Task*>& ts)
{
   for(Task* t=get_by_project_and_person(id_project, id_person, false);
       t != 0;
       t=get_by_project_and_person())
   {
      ts.push_back(t);
   }
}

void
TaskDAO::get_occurrences(Task& task)
{
   std::vector<Occurrence*> occs;
   OccurrenceDAO::get_dao().get_all_by_task(task.get_id(), occs);
   task.set_occurrences(occs);
}

bool
TaskDAO::save(Task& task,
              const bool& do_notify_observers)
{
   // get key value
   if(task.get_id() == 0)
   {
      task.set_id(KeyDAO::get_dao().get(KeyDAO::TASK_KEY));
   }

   // fulfill data object
   TaskData task_data;
   task_data.id_=task.get_id();
   strncpy(task_data.description_, task.get_description().c_str(), 60);
   task_data.description_[60]=0;
   strncpy(task_data.notes_, task.get_notes().c_str(), 1024);
   task_data.notes_[1024]=0;
   task_data.begin_=task.get_begin();
   task_data.end_=task.get_end();
   task_data.due_=task.get_due();
   task_data.status_=task.get_status();
   task_data.priority_=task.get_priority();
   task_data.project_id_=task.get_project() ? task.get_project()->get_id() : 0;
   task_data.person_id_=task.get_person() ? task.get_person()->get_id() : 0;

   // save it
   bool ok=db_.save(task_data.id_, (void*)&task_data, sizeof(TaskData));
   if(ok && do_notify_observers)
   {
      notify_observers();
   }
   return ok;
}

bool
TaskDAO::remove(const long& id,
                const bool& do_notify_observers)
{
   // first get the object
   Task* t=get(id);
   if(t != 0)
   {
      // load its dependents
      get_occurrences(*t);

      // remove the children first
      for(std::vector<Occurrence*>::const_iterator it=t->get_occurrences().begin(), end=t->get_occurrences().end(); it != end; it++)
      {
         OccurrenceDAO::get_dao().remove((*it)->get_id(), false);
      }

      // remove it
      bool ok=db_.remove(id);
      if(ok)
      {
         // try to find the task by its id in the vector
         std::vector<Task*>::iterator it=
            std::find_if(tasks_.begin(),
                         tasks_.end(),
                         FindID<Task>(id));
         // if found remove it
         if(it != tasks_.end())
         {
            delete (*it);
            tasks_.erase(it);
         }
         if(do_notify_observers)
         {
            notify_observers();
            OccurrenceDAO::get_dao().notify_observers();
         }
      }
      return ok;
   }

   return false;
}

long
TaskDAO::count(const bool& refresh)
{
   if(refresh)
   {
      get_all();
   }
   return tasks_.size();
}

long
TaskDAO::count_opened(const bool& refresh)
{
   if(refresh)
   {
      get_all();
   }
   long cnt=0;
   for(std::vector<Task*>::const_iterator it=tasks_.begin(), end=tasks_.end(); it != end; it++)
   {
      if(((*it)->get_status() != Task::COMPLETED) &&
          (*it)->get_status() != Task::CANCELED)
      {
         cnt++;
      }
   }
   return cnt++;
}

std::vector<std::string>
TaskDAO::dump()
{
   std::vector<std::string> tmp;
   std::ostringstream ss;
   Task* t;
   get_all();
   for(std::vector<Task*>::const_iterator it=tasks_.begin(), end=tasks_.end(); it != end; it++)
   {
      t=*it;
      ss.str("");
      ss << "\"TASK\",\"";
      ss << t->get_id();
      ss << "\",\"";
      ss << Database::encode(t->get_description());
      ss << "\",\"";
      ss << Database::encode(t->get_notes());
      ss << "\",\"";
      ss << Date::time_to_string(t->get_begin());
      ss << "\",\"";
      ss << Date::time_to_string(t->get_end());
      ss << "\",\"";
      ss << Date::time_to_string(t->get_due());
      ss << "\",\"";
      ss << Task::status_to_string(t->get_status());
      ss << "\",\"";
      ss << Task::priority_to_string(t->get_priority());
      ss << "\",\"";
      if(t->get_project())
      {
         ss << t->get_project()->get_id();
      }
      ss << "\",\"";
      if(t->get_person())
      {
         ss << t->get_person()->get_id();
      }
      ss << "\"";
      tmp.push_back(ss.str());
   }
   return tmp;
}

