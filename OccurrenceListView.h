// OccurrenceListView.h <<GUI>>
//
// This file contains the OccurrenceListView class definition.
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

#ifndef _OccurrenceListView_h_
#define _OccurrenceListView_h_

class OccurrenceListView;

#include <sstream>
#include <vector>
#include <OccurrenceDAO.h>
#include <ListView.h>

class OccurrenceListView: public ListView
{
   public:
      enum { TASK_MODE, HISTORY_MODE };

      OccurrenceListView(int x, int y, int width, int height,
            const int& mode=TASK_MODE, const char* label=0);
      ~OccurrenceListView();

      void show(Task* task=0);
      Occurrence* get_selected();

      void clean();

   private:
      OccurrenceDAO& dao_;
      Task* task_;
      int mode_;

      std::vector<Occurrence*> list_;

      void load();
      std::string get_cell_text(const int& row, const int& column);
};

#endif

