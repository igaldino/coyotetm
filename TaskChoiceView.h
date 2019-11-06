// TaskChoiceView.h <<GUI>>
//
// This file contains the TaskChoiceView class definition.
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

#ifndef _TaskChoiceView_h_
#define _TaskChoiceView_h_

class TaskChoiceView;

#include <sstream>
#include <vector>

#include <FL/Fl_Choice.H>

#include <Observer.h>
#include <TaskDAO.h>
#include <UI.h>

class TaskChoiceView: public Fl_Choice, public Observer
{
   public:
      TaskChoiceView(int x, int y, int width, int height, const char* label=0);
      ~TaskChoiceView();

      Task* get_selected();
      void set_selected(Task* task);
      void deselect();

      void notify(const Subject& subject, const std::string& message);

   private:
      TaskDAO& dao_;
      bool show_all_;

      void load();
};

#endif

