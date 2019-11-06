// OccurrenceEditUI.h <<GUI>>
//
// This file contains the OccurrenceEditUI class definition.
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

#ifndef _OccurrenceEditUI_h_
#define _OccurrenceEditUI_h_

class OccurrenceEditUI;

#include <FL/fl_ask.H>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Float_Input.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>

#include <UI.h>
#include <Date.h>
#include <OccurrenceDAO.h>
#include <TaskChoiceView.h>

class OccurrenceEditUI
{
   public:
      OccurrenceEditUI();
      ~OccurrenceEditUI();
      static void all_cb(Fl_Widget* widget, void* data);

      void set_task(Task* task);

      void edit(Occurrence* occurrence=0);
      void hide();
      void redraw();

   private:
      // Model
      Occurrence* occurrence_;
      bool is_new_;

      // UI
      UI& ui_;

      // Window
      Fl_Window* window_;

      // Fields
      TaskChoiceView* task_;
      Fl_Input* when_;
      Fl_Float_Input* time_;
      Fl_Text_Buffer* bnotes_;
      Fl_Text_Editor* notes_;
      Fl_Text_Display* notes_read_only_;

      Fl_Button* ok_;
      Fl_Button* delete_;

      Fl_Widget* stop_save_;

      // GUI related actions
      void when_changed();
      void ok();
      void remove();
      void set_read_only(const bool& read_only=true);

      bool save();
      void new_occurrence();
      void delete_occurrence();
      void clean();
};

#endif

