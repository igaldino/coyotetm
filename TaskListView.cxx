// TaskListView.cxx <<GUI>>
//
// This file contains the TaskListView class implementation.
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

#include <TaskListView.h>

static ListViewColumn columns[] =
{
   {"Description", FL_ALIGN_LEFT, 0, 0},
   {"Status", 0, 100, false},
   {"Priority", 0, 70, false},
   {0, 0, 0, 0}
};

TaskListView::TaskListView(int x, int y, int width, int height, const char* label):
   ListView(x, y, width, height, label), dao_(TaskDAO::get_dao()),
   project_(0), person_(0), show_all_(false), sort_by_(SORT_BY_PRIORITY),
   desc_(false), list_()
{
   set_column_info(columns);
   dao_.attach(*this);
}

TaskListView::~TaskListView()
{
   dao_.detach(*this);
}

std::string
TaskListView::get_cell_text(const int& row, const int& column)
{
   if(!column)
   {
      return list_[row]->get_description();
   }
   else if(column == 1)
   {
      return Task::status_to_string(list_[row]->get_status());
   }
   return Task::priority_to_string(list_[row]->get_priority());
}

void
TaskListView::show(Project* project)
{
   project_=project;
   show(true);
}

void
TaskListView::show(Person* person)
{
   person_=person;
   show(true);
}

void
TaskListView::show(const bool& all)
{
   show_all_=all;
   load();
}

void
TaskListView::callback_sort()
{
   if(callback_context() == Fl_Table::CONTEXT_COL_HEADER)
   {
      sort_by(callback_col(), false);
   }
}

void
TaskListView::sort_by(const int& sort_by, const bool& desc)
{
   if(sort_by != sort_by_ || desc != desc_)
   {
      sort_by_=sort_by;
      desc_=desc;
      sort();
   }
}

Task*
TaskListView::get_selected()
{
   int l=get_selected_row();
   if(l >= 0 && l < (int)list_.size())
   {
      return list_[l];
   }
   return 0;
}

void
TaskListView::clean()
{
   list_.clear();
   person_=0;
   project_=0;
   rows(0);
   redraw();
}

void
TaskListView::load()
{
   list_.clear();

   if(project_)
   {
      dao_.get_all_by_project(project_->get_id(), list_);
   }
   else if(person_)
   {
      dao_.get_all_by_person(person_->get_id(), list_);
   }
   else
   {
      std::vector<Task*>& tmp=dao_.get_all();
      list_.resize(tmp.size());
      std::copy(tmp.begin(), tmp.end(), list_.begin());
   }

   for(std::vector<Task*>::iterator it=list_.begin(); it != list_.end(); )
   {
      if(!show_all_ && (*it)->get_status() >= Task::COMPLETED)
      {
         it=list_.erase(it);
      }
      else
      {
         it++;
      }
   }

   int list_size=list_.size();
   rows(list_size);
   if(list_size)
   {
      sort();
   }
}

void
TaskListView::sort()
{
   select_all_rows(0);
   switch(sort_by_)
   {
      case SORT_BY_DESCRIPTION:
         std::sort(list_.begin(), list_.end(), Task::sort_by_description);
         break;
      case SORT_BY_STATUS:
         std::sort(list_.begin(), list_.end(), Task::sort_by_status);
         break;
      case SORT_BY_PRIORITY:
         std::sort(list_.begin(), list_.end(), Task::sort_by_priority);
         break;
   }
   if(desc_)
   {
      std::reverse(list_.begin(), list_.end());
   }
   reselect();
}

