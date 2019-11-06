// ListView.h <<GUI>>
//
// This file contains the ListView class definition.
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

#ifndef _ListView_h_
#define _ListView_h_

#include <FL/Fl_Table_Row.H>
#include <Observer.h>

struct ListViewColumn
{
   const char* text;
   int flags;
   int width;
   bool is_percent;
   int size() const;
};

class ListView: public Fl_Table_Row, public Observer
{
   public:
      ListView(int x, int y, int width, int height, const char* label=0);

      void resize(int x, int y, int width, int height);
      void resize_columns(int width);

      void rows(const int& value);

      void draw_cell(TableContext context, int row, int col,
            int x, int y, int width, int height);

      void notify(const Subject& subject, const std::string& message);

      int get_selected_row();

   protected:
      int handle(int event);
      void set_column_info(const ListViewColumn* info);
      void reselect();

   private:
      ListViewColumn* column_info_;

      virtual void load() = 0;
      virtual std::string get_cell_text(const int& row, const int& column) = 0;
};

#endif

