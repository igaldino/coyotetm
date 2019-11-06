// TaskListView.h <<GUI>>
//
// This file contains the TaskListView class definition.
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

#ifndef _TaskListView_h_
#define _TaskListView_h_

class TaskListView;

#include <vector>
#include <TaskDAO.h>
#include <ListView.h>

class TaskListView: public ListView
{
   public:
      enum
      {
         SORT_BY_DESCRIPTION,
         SORT_BY_STATUS,
         SORT_BY_PRIORITY
      };

      TaskListView(int x, int y, int width, int height, const char* label=0);
      ~TaskListView();

      void show(const bool& all=false);
      void show(Project* project);
      void show(Person* person);
      void callback_sort();
      void sort_by(const int& sort_by, const bool& desc=false);
      Task* get_selected();

      void clean();

   private:
      TaskDAO& dao_;
      Project* project_;
      Person* person_;
      bool show_all_;
      char sort_by_;
      bool desc_;

      std::vector<Task*> list_;

      void load();
      void sort();
      std::string get_cell_text(const int& row, const int& column);
};

#endif

