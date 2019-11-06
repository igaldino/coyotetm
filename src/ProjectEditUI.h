// ProjectEditUI.h <<GUI>>
//
// This file contains the ProjectEditUI class definition.
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

#ifndef _ProjectEditUI_h_
#define _ProjectEditUI_h_

class ProjectEditUI;

#include <FL/fl_ask.H>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>

#include <ProjectDAO.h>
#include <ImageDB.h>
#include <TaskListView.h>
#include <TaskEditUI.h>

class ProjectEditUI
{
   public:
      ProjectEditUI();
      ~ProjectEditUI();
      static void all_cb(Fl_Widget* widget, void* data);

      void edit(Project* project=0);
      void hide();
      void redraw();

   private:
      // Model
      Project* project_;
      bool is_new_;

      // UI
      UI& ui_;

      // Images
      ImageDB& images_;

      // Window
      Fl_Window* window_;

      // Fields
      Fl_Input* name_;
      Fl_Input* description_;
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
      void new_project();
      void delete_project();
      void clean();
};

#endif

