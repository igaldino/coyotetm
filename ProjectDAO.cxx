// ProjectDAO.cxx <<DataAccessObject>> <<Singleton>>
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

#include <ProjectDAO.h>
// Meyers singleton
ProjectDAO&
ProjectDAO::get_dao()
{
   static ProjectDAO dao;
   return dao;
}

// Hidden constructor
ProjectDAO::ProjectDAO():
   Subject("project"),
   db_("project.db"),
   projects_()
{
}

// hidden destructor
ProjectDAO::~ProjectDAO()
{
   for_each(projects_.begin(), projects_.end(), Delete<Project>());
}

// Public methods
Project*
ProjectDAO::get(const long& id,
                const bool& next)
{
   Project* project=0;

   ProjectData* project_data=(ProjectData*)db_.get(id, next);
   if(project_data != 0)
   {
      // first try to find the project by its id in the vector
      std::vector<Project*>::iterator it=
         std::find_if(projects_.begin(),
                      projects_.end(),
                      FindID<Project>(project_data->id_));
      if(it != projects_.end())
      {
         project=*it;
         project->set_name(project_data->name_);
         project->set_description(project_data->description_);
         project->get_tasks().clear();
      }
      // if not found, create a new one
      else
      {
         project=
            new Project(project_data->id_,
                        project_data->name_,
                        project_data->description_);
         projects_.push_back(project);
      }
   }

   return project;
}

std::vector<Project*>&
ProjectDAO::get_all()
{
   for(Project* p=get(0, false); p != 0; p=get())
   {
   }
   return projects_;
}

void
ProjectDAO::get_tasks(Project& project)
{
   std::vector<Task*> tasks;
   TaskDAO::get_dao().get_all_by_project(project.get_id(), tasks);
   project.set_tasks(tasks);
}

bool
ProjectDAO::save(Project& project,
                 const bool& do_notify_observers)
{
   // get key value
   if(project.get_id() == 0)
   {
      project.set_id(KeyDAO::get_dao().get(KeyDAO::PROJECT_KEY));
   }

   // fulfill data object
   ProjectData project_data;
   project_data.id_=project.get_id();
   strncpy(project_data.name_, project.get_name().c_str(), 20);
   project_data.name_[20]=0;
   strncpy(project_data.description_,
           project.get_description().c_str(),
           60); 
   project_data.description_[60]=0; 
 
   // save it
   bool ok=
      db_.save(project_data.id_, (void*)&project_data, sizeof(ProjectData));
   if(ok && do_notify_observers)
   {
      notify_observers();
   }
   return ok;
}

bool
ProjectDAO::remove(const long& id,
                   const bool& do_notify_observers)
{
   // first get the object
   Project* p=get(id);
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
         // try to find the project by its id in the vector
         std::vector<Project*>::iterator it=
            std::find_if(projects_.begin(),
                         projects_.end(),
                         FindID<Project>(id));
         if(it != projects_.end())
         {
            delete (*it);
            projects_.erase(it);
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
ProjectDAO::count(const bool& refresh)
{
   if(refresh)
   {
      get_all();
   }
   return projects_.size();
}

std::vector<std::string>
ProjectDAO::dump()
{
   std::vector<std::string> tmp;
   std::ostringstream ss;
   Project* p;
   get_all();
   for(std::vector<Project*>::const_iterator it=projects_.begin(), end=projects_.end(); it != end; it++)
   {
      p=*it;
      ss.str("");
      ss << "\"PROJECT\",\"";
      ss << p->get_id();
      ss << "\",\"";
      ss << Database::encode(p->get_name());
      ss << "\",\"";
      ss << Database::encode(p->get_description());
      ss << "\"";
      tmp.push_back(ss.str());
   }
   return tmp;
}

