// TaskChoiceView.cxx <<GUI>>
//
// This file contains the TaskChoiceView class implementation.
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

#include <TaskChoiceView.h>

TaskChoiceView::TaskChoiceView(int x, int y, int width, int height, const char* label):
   Fl_Choice(x, y, width, height, label), dao_(TaskDAO::get_dao()), show_all_(false)
{
   dao_.attach(*this);
   load();
}

TaskChoiceView::~TaskChoiceView()
{
   dao_.detach(*this);
}

Task*
TaskChoiceView::get_selected()
{
   const Fl_Menu_Item* i=mvalue();
   if(i)
   {
      return (Task*)i->user_data_;
   }
   return 0;
}

void
TaskChoiceView::set_selected(Task* task)
{
   if(!task)
   {
      deselect();
      return;
   }

   // first clean selection or load() will be in a loop situation
   value((Fl_Menu_Item*)NULL);

   // rebuild task list if needed
   int status=task->get_status();
   if(!show_all_ &&  status >= Task::COMPLETED)
   {
      show_all_=true;
      load();
   }
   else if(show_all_ && status < Task::COMPLETED)
   {
      show_all_=false;
      load();
   }

   // now just sets the value
   value(find_item(task->get_description().c_str()));
}

void
TaskChoiceView::deselect()
{
   value((Fl_Menu_Item*)NULL);
   show_all_=false;
   load();
}

void
TaskChoiceView::notify(const Subject& subject __attribute__((unused)),
      const std::string& message __attribute__((unused)))
{
   if(this->visible_r())
   {
      load();
   }
}

void
TaskChoiceView::load()
{
   Task *p=get_selected();
   Task *aux=0; // this will make sure that task still exists
   std::string text;

   clear();

   std::vector<Task*>& tmp=dao_.get_all();

   std::vector<Task*>::const_iterator it=tmp.begin();
   std::vector<Task*>::const_iterator end=tmp.end();

   while(it != end)
   {
      if(show_all_ || (*it)->get_status() < Task::COMPLETED)
      {
         text=UI::clean_choice_text((*it)->get_description());
         add(text.c_str(), 0, 0, (*it));
         if(p == (*it))
         {
            aux=(*it); // previous selected task still exists
         }
      }
      it++;
   }

   if(aux)
   {
      set_selected(aux);
   }
}

