// PersonUI.h <<GUI>>
//
// This file contains the PersonUI class implementation.
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

#include <PersonUI.h>

// default window width, height, field height, field gap and button width
#define DWH 450

// callbacks
enum
{
   NO_CB,
   ABOUT_CB,
   ADD_CB,
   CHANGED_CB,
   CLOSE_CB,
   COLOR_AMBIANCE_CB,
   COLOR_DARK_CB,
   COLOR_DEFAULT_CB,
   COLOR_NUMIX_CB,
   EDIT_CB,
   REMOVE_CB,
   VIEW_PROJECT_CB,
   VIEW_TASK_CB,
   VIEW_HISTORY_CB
};

static Fl_Menu_Item menu[]=
{
   {"&File", 0, 0, 0, FL_SUBMENU, 0, 0, 0, 0},
      {"&New             ", FL_COMMAND+'n', (Fl_Callback*)PersonUI::all_cb, (void*)ADD_CB,   0,               0, 0, 0, 0},
      {"&Open            ", FL_COMMAND+'o', (Fl_Callback*)PersonUI::all_cb, (void*)EDIT_CB,  FL_MENU_DIVIDER, 0, 0, 0, 0},
      {"&Close           ", FL_COMMAND+'w', (Fl_Callback*)PersonUI::all_cb, (void*)CLOSE_CB, 0,               0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
   {"&Edit", 0, 0, 0, FL_SUBMENU, 0, 0, 0, 0},
      {"&Delete          ", FL_COMMAND+'d', (Fl_Callback*)PersonUI::all_cb, (void*)REMOVE_CB, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
   {"&View", 0, 0, 0, FL_SUBMENU, 0, 0, 0, 0},
      {"&Task            ", FL_COMMAND+'1', (Fl_Callback*)PersonUI::all_cb, (void*)VIEW_TASK_CB,    0,               0, 0, 0, 0},
      {"&Project         ", FL_COMMAND+'2', (Fl_Callback*)PersonUI::all_cb, (void*)VIEW_PROJECT_CB, 0,               0, 0, 0, 0},
      {"&History         ", FL_COMMAND+'4', (Fl_Callback*)PersonUI::all_cb, (void*)VIEW_HISTORY_CB, FL_MENU_DIVIDER, 0, 0, 0, 0},
      {"&Colors", 0, 0, 0, FL_SUBMENU, 0, 0, 0, 0},
         {"&Default         ", 0, (Fl_Callback*)PersonUI::all_cb, (void*)COLOR_DEFAULT_CB,  FL_MENU_RADIO | FL_MENU_VALUE, 0, 0, 0, 0},
         {"Dar&k            ", 0, (Fl_Callback*)PersonUI::all_cb, (void*)COLOR_DARK_CB,     FL_MENU_RADIO,                 0, 0, 0, 0},
         {"&Ambiance        ", 0, (Fl_Callback*)PersonUI::all_cb, (void*)COLOR_AMBIANCE_CB, FL_MENU_RADIO,                 0, 0, 0, 0},
         {"&Numix           ", 0, (Fl_Callback*)PersonUI::all_cb, (void*)COLOR_NUMIX_CB,    FL_MENU_RADIO,                 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
   {"&Help", 0, 0, 0, FL_SUBMENU, 0, 0, 0, 0},
      {"&About CoyoteTM  ", 0, (Fl_Callback*)PersonUI::all_cb, (void*)ABOUT_CB, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0}
};

PersonUI::PersonUI():
      dao_(PersonDAO::get_dao()), images_(ImageDB::get_image_db()), window_(0),
      menubar_(0), list_(0), sb_(0)
{
   dao_.attach(*this);

   // Window
   window_=new Fl_Window(UI::default_window_width, DWH, "CoyoteTM - Person List");
   window_->begin();

   // Menubar
   menubar_=new Fl_Menu_Bar(0, 0, UI::default_window_width, UI::default_field_height);
   menubar_->menu(menu);
   menubar_->box(FL_NO_BOX);
   UI::set_color_menu(menubar_);

   // Toolbar
   Fl_Group* toolbar=new Fl_Group(0, menubar_->y()+menubar_->h()+UI::default_field_gap, UI::default_window_width, UI::default_field_height);
   toolbar->begin();

   // Toolbar / New Person
   Fl_Button* button=0;
   button=new Fl_Button(0, toolbar->y(), UI::default_field_height, UI::default_field_height);
   button->tooltip("New Person");
   button->image(images_.get(ImageDB::ADD));
   button->align(FL_ALIGN_IMAGE_NEXT_TO_TEXT);
   button->callback(all_cb, (void*)ADD_CB);
   button->box(FL_NO_BOX);
   button->down_box(FL_UP_BOX);

   // Toolbar / Edit Person
   button=new Fl_Button(button->x()+button->w()+UI::default_field_gap, toolbar->y(), UI::default_field_height, UI::default_field_height);
   button->tooltip("Edit Person");
   button->image(images_.get(ImageDB::EDIT));
   button->align(FL_ALIGN_IMAGE_NEXT_TO_TEXT);
   button->callback(all_cb, (void*)EDIT_CB);
   button->box(FL_NO_BOX);
   button->down_box(FL_UP_BOX);

   // Toolbar / Delete
   button=new Fl_Button(button->x()+button->w()+UI::default_field_gap, toolbar->y(), UI::default_field_height, UI::default_field_height);
   button->tooltip("Delete");
   button->image(images_.get(ImageDB::DELETE));
   button->align(FL_ALIGN_IMAGE_NEXT_TO_TEXT);
   button->callback(all_cb, (void*)REMOVE_CB);
   button->box(FL_NO_BOX);
   button->down_box(FL_UP_BOX);

   // Toolbar / Separator & Task
   button=new Fl_Button(button->x()+button->w()+UI::default_field_height/3, toolbar->y(), UI::default_field_height, UI::default_field_height);
   button->tooltip("Task");
   button->image(images_.get(ImageDB::TASK));
   button->align(FL_ALIGN_IMAGE_NEXT_TO_TEXT);
   button->callback(all_cb, (void*)VIEW_TASK_CB);
   button->box(FL_NO_BOX);
   button->down_box(FL_UP_BOX);

   // Toolbar / Project
   button=new Fl_Button(button->x()+button->w()+UI::default_field_gap, toolbar->y(), UI::default_field_height, UI::default_field_height);
   button->tooltip("Project");
   button->image(images_.get(ImageDB::PROJECT));
   button->align(FL_ALIGN_IMAGE_NEXT_TO_TEXT);
   button->callback(all_cb, (void*)VIEW_PROJECT_CB);
   button->box(FL_NO_BOX);
   button->down_box(FL_UP_BOX);

   // Toolbar / History
   button=new Fl_Button(button->x()+button->w()+UI::default_field_gap, toolbar->y(), UI::default_field_height, UI::default_field_height);
   button->tooltip("History");
   button->image(images_.get(ImageDB::CLOCK));
   button->align(FL_ALIGN_IMAGE_NEXT_TO_TEXT);
   button->callback(all_cb, (void*)VIEW_HISTORY_CB);
   button->box(FL_NO_BOX);
   button->down_box(FL_UP_BOX);

   // Tollbar / Statusbar
   sb_=new Fl_Box(button->x()+button->w()+UI::default_field_height/3, toolbar->y(), UI::default_window_width-(button->x()+button->w()+UI::default_field_height/3), UI::default_field_height);
   sb_->align(FL_ALIGN_RIGHT|FL_ALIGN_INSIDE);

   // Toolbar final setup
   toolbar->resizable(sb_);
   toolbar->end();

   // Content - Person list
   list_=new PersonListView(0, toolbar->y()+toolbar->h()+UI::default_field_gap, UI::default_window_width, DWH-(toolbar->y()+toolbar->h()+UI::default_field_gap));
   list_->callback(all_cb, (void*)CHANGED_CB);
   list_->show();

   // Window final setup
   UI::get_ui().set_icon(window_);
   window_->resizable(list_);
   window_->end();
   window_->show();
   window_->callback(all_cb, this);
   window_->size_range(350, 150);

   update_statusbar();
}

void
PersonUI::all_cb(Fl_Widget* widget, void* data)
{
   PersonUI* ui=0;
   int option=(long)data;

   if(!widget->window())
   {
      ui=(PersonUI*)widget->user_data();
      option=CLOSE_CB;
   }
   else
   {
      ui=(PersonUI*)widget->window()->user_data();
   }

   switch(option)
   {
      case ABOUT_CB:
         UI::get_ui().about();
         break;
      case ADD_CB:
         ui->add();
         break;
      case CHANGED_CB:
         if(Fl::event_clicks() || Fl::event_key() == FL_Enter)
         {
            Fl::event_clicks(0);
            ui->edit();
         }
         break;
      case CLOSE_CB:
         ui->hide();
         break;
      case COLOR_AMBIANCE_CB:
         UI::get_ui().set_theme(UI::AMBIANCE_THEME);
         break;
      case COLOR_DARK_CB:
         UI::get_ui().set_theme(UI::DARK_THEME);
         break;
      case COLOR_DEFAULT_CB:
         UI::get_ui().set_theme(UI::DEFAULT_THEME);
         break;
      case COLOR_NUMIX_CB:
         UI::get_ui().set_theme(UI::NUMIX_THEME);
         break;
      case EDIT_CB:
         ui->edit();
         break;
      case REMOVE_CB:
         ui->remove();
         break;
      case VIEW_PROJECT_CB:
         UI::get_ui().view_project();
         break;
      case VIEW_TASK_CB:
         UI::get_ui().view_task();
         break;
      case VIEW_HISTORY_CB:
         UI::get_ui().view_occurrence();
         break;
   }
}

void
PersonUI::show()
{
   list_->take_focus();
   window_->show();
}

void
PersonUI::hide()
{
   window_->hide();
}

void
PersonUI::redraw()
{
   if(window_->shown())
   {
      window_->redraw();
      UI::set_color_menu(menubar_);
   }
}

void
PersonUI::add()
{
   UI::get_ui().edit_person();
}

void
PersonUI::remove()
{
   UI::get_ui().remove_person(list_->get_selected());
}

void
PersonUI::edit()
{
   Person* person=list_->get_selected();
   if(person != 0)
   {
      UI::get_ui().edit_person(person);
   }
}

void
PersonUI::update_statusbar()
{
   std::stringstream ss;
   ss << dao_.count() << " total";
   sb_->copy_label(ss.str().c_str());
}

void
PersonUI::notify(const Subject& subject __attribute__((unused)),
      const std::string& message __attribute__((unused)))
{
   if(window_->visible_r())
   {
      update_statusbar();
   }
}
