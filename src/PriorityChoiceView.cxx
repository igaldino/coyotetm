// PriorityChoiceView.cxx <<GUI>>
//
// This file contains the PriorityChoiceView class implementation.
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

#include <PriorityChoiceView.h>

PriorityChoiceView::PriorityChoiceView(int x, int y, int width, int height, const char* label):
   Fl_Choice(x, y, width, height, label)
{
   add(Task::priority_to_string(Task::CRITICAL).c_str(), 0, 0, (void*)Task::CRITICAL);
   add(Task::priority_to_string(Task::HIGH).c_str(), 0, 0, (void*)Task::HIGH);
   add(Task::priority_to_string(Task::MEDIUM).c_str(), 0, 0, (void*)Task::MEDIUM);
   add(Task::priority_to_string(Task::LOW).c_str(), 0, 0, (void*)Task::LOW);
   add(Task::priority_to_string(Task::NONE).c_str(), 0, 0, (void*)Task::NONE);
}

int
PriorityChoiceView::get_selected()
{
   const Fl_Menu_Item* i=mvalue();
   if(i)
   {
      return (size_t)i->user_data_;
   }
   return 0;
}

void
PriorityChoiceView::set_selected(int priority)
{
   if(priority >= Task::CRITICAL && priority <= Task::NONE)
   {
      value(find_item(Task::priority_to_string(priority).c_str()));
   }
   else
   {
      value(find_item(Task::priority_to_string(Task::NONE).c_str()));
   }
}

