// UI.h <<GUI>> <<Singleton>>
//
// This file contains the UI class definition.
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

#ifndef _UI_h_
#define _UI_h_

class UI;

#include <config.h>
#include <string>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Tooltip.H>
#include <FL/fl_ask.H>
#include <KeyDAO.h>
#include <PersonUI.h>
#include <ProjectUI.h>
#include <TaskUI.h>
#include <OccurrenceUI.h>
#include <PersonEditUI.h>
#include <ProjectEditUI.h>
#include <TaskEditUI.h>
#include <OccurrenceEditUI.h>

class UI
{
   public:
      enum
      {
         DEFAULT_THEME,
         DARK_THEME,
         AMBIANCE_THEME,
         NUMIX_THEME
      };

      // Meyers Singleton
      static UI& get_ui();

      int run(int argc, char* argv[]);

      // Set application theme and should be called just after Fl_Window::show(argc, argv)
      void init();

      void view_person();
      void view_project();
      void view_task(int argc=0, char* argv[]=0);
      void view_occurrence();
      bool remove_person(Person* person);
      bool remove_project(Project* project);
      bool mark_task_canceled(Task* task);
      bool mark_task_completed(Task* task);
      bool remove_occurrence(Occurrence* occurrence);
      void about();
      void quit();
      void edit_person(Person* person=0);
      void edit_project(Project* project=0);
      void edit_task(Task* task=0, Project* project=0, Person* person=0);
      void edit_task(Project* project, Task* task=0);
      void edit_task(Person* person, Task* task=0);
      void edit_occurrence(Occurrence* occurrence=0);
      void edit_occurrence(Task* task, Occurrence* occurrence=0);

      bool ask_cancel_pending_work();

      const Fl_Font& get_font() const;
      void set_font(const Fl_Font& font);
      const Fl_Fontsize& get_fontsize() const;
      void set_fontsize(const Fl_Fontsize& fontsize);
      void apply_font();
      void apply_font(Fl_Input* input);
      void apply_font(Fl_Text_Display* text);

      void set_read_only(Fl_Input* input, const bool& read_only=true);
      void set_read_only(Fl_Text_Display* input, const bool& read_only=true);

      void set_icon(Fl_Window* window);

      const int& get_theme() const;
      void set_theme(const int& theme);

      static void draw_label(const Fl_Label* label, int x, int y, int width, int height, Fl_Align align);
      static void measure_label(const Fl_Label* label, int& width, int& height);

      static std::string clean_choice_text(const std::string& text);

      static void set_color_menu(Fl_Menu_Bar* menubar);

      static const int default_window_width;
      static const int default_field_height;
      static const int default_field_indentation;
      static const int default_field_gap;
      static const int default_button_width;

   private:
      // Windows
      PersonUI* person_ui_;
      ProjectUI* project_ui_;
      TaskUI* task_ui_;
      OccurrenceUI* occurrence_ui_;
      PersonEditUI* person_edit_ui_;
      ProjectEditUI* project_edit_ui_;
      TaskEditUI* task_edit_ui_;
      OccurrenceEditUI* occurrence_edit_ui_;

      int theme_;

      // Meyers Singleton, constructor is hidden
      UI();

      // destructor is also hidden
      ~UI();

      // Default fonts
      Fl_Font font_;
      Fl_Fontsize fontsize_;
};

#endif
