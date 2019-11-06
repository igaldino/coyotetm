// TaskEditUI.h <<GUI>>
//
// This file contains the TaskEditUI class definition.
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

#ifndef _TaskEditUI_h_
#define _TaskEditUI_h_

class TaskEditUI;

#include <FL/fl_ask.H>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>

#include <Date.h>
#include <TaskDAO.h>
#include <ImageDB.h>
#include <StatusChoiceView.h>
#include <PriorityChoiceView.h>
#include <ProjectChoiceView.h>
#include <PersonChoiceView.h>
#include <OccurrenceListView.h>
#include <OccurrenceEditUI.h>

class TaskEditUI
{
   public:
      TaskEditUI();
      ~TaskEditUI();
      static void all_cb(Fl_Widget* widget, void* data);

      void edit(Task* task=0);
      void set_project(Project* project);
      void set_person(Person* person);
      void hide();
      void redraw();

   private:
      // Model
      Task* task_;
      bool is_new_;

      // UI
      UI& ui_;

      // Images
      ImageDB& images_;

      // Window
      Fl_Window* window_;

      // Fields
      Fl_Input* description_;
      StatusChoiceView* status_;
      Fl_Input* due_;
      PriorityChoiceView* priority_;
      ProjectChoiceView* project_;
      PersonChoiceView* person_;
      Fl_Input* begin_;
      Fl_Input* end_;
      Fl_Text_Buffer* bnotes_;
      Fl_Text_Editor* notes_;
      Fl_Text_Display* notes_read_only_;
      OccurrenceListView* list_;

      Fl_Button* ok_;
      std::vector<Fl_Button*> buttons_;

      Fl_Widget* stop_save_;

      // GUI related actions
      void add_occurrence();
      void begin_changed();
      void delete_occurrence();
      void description_changed();
      void due_changed();
      void edit_occurrence();
      void end_changed();
      void mark_canceled();
      void mark_completed();
      void ok();
      void set_read_only(const bool& read_only=true);
      void status_changed(int status=0);

      bool start_task();
      bool save();
      void new_task();
      void delete_task();
      void clean();
};

#endif

