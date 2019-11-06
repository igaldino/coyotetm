// ListView.cxx <<GUI>>
//
// This file contains the ListView class implementation.
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

#include <ListView.h>
#include <UI.h>

int ListViewColumn::size() const
{
   const ListViewColumn* m = this;
   while(m->text)
   {
      m++;
   }
   return (int)(m-this);
}

ListView::ListView(int x, int y, int width, int height, const char* label):
      Fl_Table_Row(x, y, width, height, label), column_info_(0)
{
   end();
}

void
ListView::resize(int x, int y, int width, int height)
{
   Fl_Table_Row::resize(x, y, width, height);
   resize_columns(width);
}

void
ListView::resize_columns(int width)
{
   int available_width=(vscrollbar->visible() ? width-18 : width-2);
   const ListViewColumn* m=column_info_;
   int expand_col=0;

   for(int col=0; m->text && available_width; m++, col++)
   {
      if(!m->width)
      {
         expand_col=col;
      }
      else if(m->is_percent)
      {
         col_width(col, available_width * m->width / 100);
         available_width-=col_width(col);
      }
      else if(m->width > available_width)
      {
         col_width(col, available_width);
         available_width=0;
      }
      else
      {
         col_width(col, m->width);
         available_width-=m->width;
      }
   }
   if(available_width)
   {
      col_width(expand_col, available_width);
   }
}

void
ListView::rows(const int& value)
{
   if(Fl_Table_Row::rows() != value)
   {
      Fl_Table_Row::rows(value);
      resize_columns(w());
   }
}

void
ListView::draw_cell(TableContext context, int row, int col,
      int x, int y, int width, int height)
{
   switch(context)
   {
      case CONTEXT_STARTPAGE:
         UI::get_ui().apply_font();
         break;
      case CONTEXT_COL_HEADER:
         fl_push_clip(x, y, width, height);
         // background
         fl_color(FL_BACKGROUND_COLOR);
         fl_rectf(x, y, width, height);
         // text
         fl_color(FL_FOREGROUND_COLOR);
         fl_draw((column_info_+col)->text, x, y, width, height, FL_ALIGN_CENTER);
         // border
         fl_color(FL_BLACK);
         if(col > 0)
         {
            fl_rect(x-1, y, width+1, height);
         }
         else
         {
            fl_rect(x, y, width, height);
         }
         fl_pop_clip();
         break;
      case CONTEXT_CELL:
         fl_push_clip(x, y, width, height);
         // background
         if(row_selected(row))
         {
            fl_color(FL_SELECTION_COLOR);
         }
         else
         {
            fl_color(FL_BACKGROUND2_COLOR);
         }
         fl_rectf(x, y, width, height);
         // text
         fl_color(FL_FOREGROUND_COLOR);
         fl_draw(get_cell_text(row, col).c_str(), x, y, width, height, (column_info_+col)->flags);
         // border
         fl_color(FL_BACKGROUND_COLOR);
         fl_rect((col > 0 ? x-1 : x), (row > 0 ? y-1 : y), (col > 0 ? width+1 : width), (row > 0 ? height+1 : height));
         fl_pop_clip();
         break;
      case CONTEXT_NONE:
      case CONTEXT_ENDPAGE:
      case CONTEXT_ROW_HEADER:
      case CONTEXT_TABLE:
      case CONTEXT_RC_RESIZE:
         break;
   }
}

void
ListView::notify(const Subject& subject __attribute__((unused)),
      const std::string& message __attribute__((unused)))
{
   if(this->visible_r())
   {
      load();
   }
}

int
ListView::get_selected_row()
{
   for(int i=0, maxi=Fl_Table_Row::rows(); i < maxi; i++)
   {
      if(row_selected(i))
      {
         return i;
      }
   }
   return -1;
}

int
ListView::handle(int event)
{
   int ret=0;
   int curr=current_row < 0 ? 0 : current_row;
   switch(event)
   {
      case FL_DRAG:
         ret = 1;
         break;
      case FL_KEYBOARD:
         switch(Fl::event_key())
         {
            case FL_Enter:
               Fl_Widget::do_callback();
               ret = 1;
               break;
            case FL_Up:
            case FL_Down:
            case FL_Page_Up:
            case FL_Page_Down:
               select_row(curr, 0);
               break;
            case FL_Left:
            case FL_Right:
               ret = 1;
               break;
         }
         redraw();
         break;
      case FL_KEYUP:
         switch(Fl::event_key())
         {
            case FL_Up:
            case FL_Down:
            case FL_Page_Up:
            case FL_Page_Down:
               select_row(curr);
               break;
            case FL_Left:
            case FL_Right:
               ret = 1;
               break;
         }
         redraw();
         break;
      case FL_FOCUS:
      case FL_UNFOCUS:
         ret = 1;
         break;
      case FL_PUSH:
         take_focus();
         break;
   }
   return ret ? 1 : Fl_Table_Row::handle(event);
}

void
ListView::set_column_info(const ListViewColumn* info)
{
   column_info_ = const_cast<ListViewColumn*>(info);
   col_header(1);
   cols(column_info_->size());
   resize_columns(w());
}

void
ListView::reselect()
{
   int l=callback_row();
   if(l < 0 || l > Fl_Table_Row::rows())
   {
      l=0;
   }
   select_row(l);
}

