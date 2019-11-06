// OccurrenceListView.cxx <<GUI>>
//
// This file contains the OccurrenceListView class implementation.
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

#include <OccurrenceListView.h>

static ListViewColumn columns_task_mode[] =
{
   {"Date", FL_ALIGN_LEFT, 80, false},
   {"Hours", FL_ALIGN_RIGHT, 50, false},
   {"Notes", FL_ALIGN_LEFT, 0, 0},
   {0, 0, 0, 0}
};

static ListViewColumn columns_history_mode[] =
{
   {"Date", FL_ALIGN_LEFT, 80, false},
   {"Task", FL_ALIGN_LEFT, 30, true},
   {"Notes", FL_ALIGN_LEFT, 0, 0},
   {0, 0, 0, 0}
};

OccurrenceListView::OccurrenceListView(int x, int y, int width, int height, const int& mode, const char* label):
   ListView(x, y, width, height, label), dao_(OccurrenceDAO::get_dao()),
   task_(0), mode_(mode), list_()
{
   dao_.attach(*this);
   if(mode_ == TASK_MODE)
   {
      set_column_info(columns_task_mode);
   }
   else
   {
      TaskDAO::get_dao().attach(*this);
      set_column_info(columns_history_mode);
   }
}

OccurrenceListView::~OccurrenceListView()
{
   dao_.detach(*this);
   if(mode_ == HISTORY_MODE)
   {
      TaskDAO::get_dao().detach(*this);
   }
}

std::string
OccurrenceListView::get_cell_text(const int& row, const int& column)
{
   if(!column)
   {
      return Date::time_to_string(list_[row]->get_when());
   }
   else if(column == 1 && mode_ == TASK_MODE)
   {
      std::ostringstream buffer;
      buffer << std::fixed << std::setprecision(2) << list_[row]->get_time();
      return buffer.str();
   }
   else if(column == 1)
   {
      Task *task=list_[row]->get_task();
      if(task)
      {
         return task->get_description();
      }
      else
      {
         return "Task is empty";
      }
   }
   return list_[row]->get_notes();
}

void
OccurrenceListView::show(Task* task)
{
   task_=task;
   load();
}

Occurrence*
OccurrenceListView::get_selected()
{
   int l=get_selected_row();
   if(l >= 0 && l < (int)list_.size())
   {
      return list_[l];
   }
   return 0;
}

void
OccurrenceListView::clean()
{
   list_.clear();
   task_=0;
   rows(0);
   redraw();
}

void
OccurrenceListView::load()
{
   list_.clear();
   if(task_)
   {
      dao_.get_all_by_task(task_->get_id(), list_);
   }
   else
   {
      std::vector<Occurrence*>& tmp=dao_.get_all();
      list_.resize(tmp.size());
      std::copy(tmp.begin(), tmp.end(), list_.begin());
   }

   int list_size=list_.size();
   rows(list_size);
   if(list_size)
   {
      select_all_rows(0);
      std::sort(list_.begin(), list_.end(), Occurrence::sort);
      reselect();
   }
}

