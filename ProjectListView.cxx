// ProjectListView.cxx <<GUI>>
//
// This file contains the ProjectListView class implementation.
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

#include <ProjectListView.h>

static ListViewColumn columns[] =
{
   {"Name", FL_ALIGN_LEFT, 30, true},
   {"Description", FL_ALIGN_LEFT, 0, 0},
   {0, 0, 0, 0}
};

ProjectListView::ProjectListView(int x, int y, int width, int height, const char* label):
   ListView(x, y, width, height, label), dao_(ProjectDAO::get_dao()),
   sort_by_(SORT_BY_NAME), list_()
{
   set_column_info(columns);
   dao_.attach(*this);
}

ProjectListView::~ProjectListView()
{
   dao_.detach(*this);
}

std::string
ProjectListView::get_cell_text(const int& row, const int& column)
{
   if(!column)
   {
      return list_[row]->get_name();
   }
   return list_[row]->get_description();
}

void
ProjectListView::show()
{
   load();
}

void
ProjectListView::callback_sort()
{
   if(callback_context() == Fl_Table::CONTEXT_COL_HEADER)
   {
      sort_by(callback_col());
   }
}

void
ProjectListView::sort_by(const int& sort_by)
{
   if(sort_by != sort_by_)
   {
      sort_by_=sort_by;
      sort();
   }
}

Project*
ProjectListView::get_selected()
{
   int l=get_selected_row();
   if(l >= 0 && l < (int)list_.size())
   {
      return list_[l];
   }
   return 0;
}

void
ProjectListView::clean()
{
   list_.clear();
   rows(0);
   redraw();
}

void
ProjectListView::load()
{
   list_.clear();

   std::vector<Project*>& tmp=dao_.get_all();
   list_.resize(tmp.size());
   std::copy(tmp.begin(), tmp.end(), list_.begin());

   int list_size=list_.size();
   rows(list_size);
   if(list_size)
   {
      sort();
   }
}

void
ProjectListView::sort()
{
   select_all_rows(0);
   switch(sort_by_)
   {
      case SORT_BY_NAME:
         std::sort(list_.begin(), list_.end(), Project::sort_by_name);
         break;
      case SORT_BY_DESCRIPTION:
         std::sort(list_.begin(), list_.end(), Project::sort_by_description);
         break;
   }
   reselect();
}

