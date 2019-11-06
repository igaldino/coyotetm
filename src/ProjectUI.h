// ProjectUI.h <<GUI>>
//
// This file contains the ProjectUI class definition.
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

#ifndef _ProjectUI_h_
#define _ProjectUI_h_

class ProjectUI;

#include <FL/fl_ask.H>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>

#include <Observer.h>
#include <ProjectDAO.h>
#include <ImageDB.h>
#include <ProjectListView.h>
#include <ProjectEditUI.h>
#include <PersonUI.h>
#include <TaskUI.h>

class ProjectUI: public Observer
{
   public:
      ProjectUI();
      static void all_cb(Fl_Widget* widget, void* data);

      void show();
      void hide();
      void redraw();

      void notify(const Subject& subject, const std::string& message);

   private:
      // DAO
      ProjectDAO& dao_;

      // Images
      ImageDB& images_;

      // Window
      Fl_Window* window_;

      // Menubar
      Fl_Menu_Bar* menubar_;

      // List
      ProjectListView* list_;

      // Statusbar
      Fl_Box* sb_;
      
      // GUI related actions
      void add();
      void edit();
      void remove();
      void update_statusbar();
};

#endif

