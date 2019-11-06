// StatusChoiceView.cxx <<GUI>>
//
// This file contains the StatusChoiceView class implementation.
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

#include <StatusChoiceView.h>

StatusChoiceView::StatusChoiceView(int x, int y, int width, int height, const char* label):
   Fl_Choice(x, y, width, height, label)
{
   add(Task::status_to_string(Task::NOT_STARTED).c_str(), 0, 0, (void*)Task::NOT_STARTED);
   add(Task::status_to_string(Task::IN_PROGRESS).c_str(), 0, 0, (void*)Task::IN_PROGRESS);
   add(Task::status_to_string(Task::COMPLETED).c_str(), 0, 0, (void*)Task::COMPLETED);
   add(Task::status_to_string(Task::CANCELED).c_str(), 0, 0, (void*)Task::CANCELED);
   add(Task::status_to_string(Task::ON_HOLD).c_str(), 0, 0, (void*)Task::ON_HOLD);
}

int
StatusChoiceView::get_selected()
{
   const Fl_Menu_Item* i=mvalue();
   if(i)
   {
      return (size_t)i->user_data_;
   }
   return 0;
}

void
StatusChoiceView::set_selected(int status)
{
   if(status >= Task::NOT_STARTED && status <= Task::ON_HOLD)
   {
      value(find_item(Task::status_to_string(status).c_str()));
   }
}

