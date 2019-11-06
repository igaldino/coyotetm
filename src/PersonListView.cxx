// PersonListView.cxx <<GUI>>
//
// This file contains the PersonListView class implementation.
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

#include <PersonListView.h>

static ListViewColumn columns[] =
{
   {"Name", FL_ALIGN_LEFT, 0, 0},
   {0, 0, 0, 0}
};

PersonListView::PersonListView(int x, int y, int width, int height, const char* label):
   ListView(x, y, width, height, label), dao_(PersonDAO::get_dao()), list_()
{
   set_column_info(columns);
   dao_.attach(*this);
}

PersonListView::~PersonListView()
{
   dao_.detach(*this);
}

std::string
PersonListView::get_cell_text(const int& row,
      const int& column __attribute__((unused)))
{
   return list_[row]->get_name();
}

void
PersonListView::show()
{
   load();
}

Person*
PersonListView::get_selected()
{
   int l=get_selected_row();
   if(l >= 0 && l < (int)list_.size())
   {
      return list_[l];
   }
   return 0;
}

void
PersonListView::clean()
{
   list_.clear();
   rows(0);
   redraw();
}

void
PersonListView::load()
{
   list_.clear();

   std::vector<Person*>& tmp=dao_.get_all();
   list_.resize(tmp.size());
   std::copy(tmp.begin(), tmp.end(), list_.begin());

   int list_size=list_.size();
   rows(list_size);
   if(list_size)
   {
      select_all_rows(0);
      std::sort(list_.begin(), list_.end(), Person::sort_by_name);
      reselect();
   }
}

