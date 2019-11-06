// PersonEditUI.h <<GUI>>
//
// This file contains the PersonEditUI class definition.
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

#ifndef _PersonEditUI_h_
#define _PersonEditUI_h_

class PersonEditUI;

#include <FL/fl_ask.H>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>

#include <PersonDAO.h>
#include <ImageDB.h>
#include <TaskListView.h>
#include <TaskEditUI.h>

class PersonEditUI
{
   public:
      PersonEditUI();
      ~PersonEditUI();
      static void all_cb(Fl_Widget* widget, void* data);

      void edit(Person* person=0);
      void hide();
      void redraw();

   private:
      // Model
      Person* person_;
      bool is_new_;

      // UI
      UI& ui_;

      // Images
      ImageDB& images_;

      // Window
      Fl_Window* window_;

      // Fields
      Fl_Input* name_;
      TaskListView* list_;

      // GUI related actions
      void add_task();
      void edit_task();
      void mark_task_canceled();
      void mark_task_completed();
      void name_changed();
      void ok();
      void remove();

      bool save();
      void new_person();
      void delete_person();
      void clean();
};

#endif

