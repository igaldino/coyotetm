// ProjectChoiceView.cxx <<GUI>>
//
// This file contains the ProjectChoiceView class implementation.
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

#include <ProjectChoiceView.h>

ProjectChoiceView::ProjectChoiceView(int x, int y, int width, int height, const char* label):
   Fl_Choice(x, y, width, height, label), dao_(ProjectDAO::get_dao())
{
   dao_.attach(*this);
   load();
}

ProjectChoiceView::~ProjectChoiceView()
{
   dao_.detach(*this);
}

Project*
ProjectChoiceView::get_selected()
{
   const Fl_Menu_Item* i=mvalue();
   if(i)
   {
      return (Project*)i->user_data_;
   }
   return 0;
}

void
ProjectChoiceView::set_selected(Project* project)
{
   value(find_item(project->get_name().c_str()));
}

void
ProjectChoiceView::deselect()
{
   value((Fl_Menu_Item*)NULL);
   load();
}

void
ProjectChoiceView::notify(const Subject& subject __attribute__((unused)),
      const std::string& message __attribute__((unused)))
{
   if(this->visible_r())
   {
      load();
   }
}

void
ProjectChoiceView::load()
{
   Project *p=get_selected();
   Project *aux=0; // this will make sure that project still exists
   std::string text;

   clear();

   std::vector<Project*> tmp=dao_.get_all();
   std::sort(tmp.begin(), tmp.end(), Project::sort_by_name);

   std::vector<Project*>::const_iterator it=tmp.begin();
   std::vector<Project*>::const_iterator end=tmp.end();

   while(it != end)
   {
      text=UI::clean_choice_text((*it)->get_name());
      add(text.c_str(), 0, 0, (*it));
      if(p == (*it))
      {
         aux=(*it); // previous selected project still exists
      }
      it++;
   }

   if(aux)
   {
      set_selected(aux);
   }
}

