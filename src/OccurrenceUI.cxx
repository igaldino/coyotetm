// OccurrenceUI.h <<GUI>>
//
// This file contains the OccurrenceUI class implementation.
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

#include <OccurrenceUI.h>

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
   VIEW_PERSON_CB
};

static Fl_Menu_Item menu[]=
{
   {"&File", 0, 0, 0, FL_SUBMENU, 0, 0, 0, 0},
      {"&New             ", FL_COMMAND+'n', (Fl_Callback*)OccurrenceUI::all_cb, (void*)ADD_CB,   0,               0, 0, 0, 0},
      {"&Open            ", FL_COMMAND+'o', (Fl_Callback*)OccurrenceUI::all_cb, (void*)EDIT_CB,  FL_MENU_DIVIDER, 0, 0, 0, 0},
      {"&Close           ", FL_COMMAND+'w', (Fl_Callback*)OccurrenceUI::all_cb, (void*)CLOSE_CB, 0,               0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
   {"&Edit", 0, 0, 0, FL_SUBMENU, 0, 0, 0, 0},
      {"&Delete          ", FL_COMMAND+'d', (Fl_Callback*)OccurrenceUI::all_cb, (void*)REMOVE_CB, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
   {"&View", 0, 0, 0, FL_SUBMENU, 0, 0, 0, 0},
      {"&Task            ", FL_COMMAND+'1', (Fl_Callback*)OccurrenceUI::all_cb, (void*)VIEW_TASK_CB,    0,               0, 0, 0, 0},
      {"&Project         ", FL_COMMAND+'2', (Fl_Callback*)OccurrenceUI::all_cb, (void*)VIEW_PROJECT_CB, 0,               0, 0, 0, 0},
      {"Per&son          ", FL_COMMAND+'3', (Fl_Callback*)OccurrenceUI::all_cb, (void*)VIEW_PERSON_CB,  FL_MENU_DIVIDER, 0, 0, 0, 0},
      {"&Colors", 0, 0, 0, FL_SUBMENU, 0, 0, 0, 0},
         {"&Default         ", 0, (Fl_Callback*)OccurrenceUI::all_cb, (void*)COLOR_DEFAULT_CB,  FL_MENU_RADIO | FL_MENU_VALUE, 0, 0, 0, 0},
         {"Dar&k            ", 0, (Fl_Callback*)OccurrenceUI::all_cb, (void*)COLOR_DARK_CB,     FL_MENU_RADIO,                 0, 0, 0, 0},
         {"&Ambiance        ", 0, (Fl_Callback*)OccurrenceUI::all_cb, (void*)COLOR_AMBIANCE_CB, FL_MENU_RADIO,                 0, 0, 0, 0},
         {"&Numix           ", 0, (Fl_Callback*)OccurrenceUI::all_cb, (void*)COLOR_NUMIX_CB,    FL_MENU_RADIO,                 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
   {"&Help", 0, 0, 0, FL_SUBMENU, 0, 0, 0, 0},
      {"&About CoyoteTM  ", 0, (Fl_Callback*)OccurrenceUI::all_cb, (void*)ABOUT_CB, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0}
};

OccurrenceUI::OccurrenceUI():
      reload_(false), dao_(OccurrenceDAO::get_dao()),
      images_(ImageDB::get_image_db()), window_(0), menubar_(0), list_(0),
      sb_(0)
{
   dao_.attach(*this);

   // Window
   window_=new Fl_Window(UI::default_window_width, DWH, "CoyoteTM - History");
   window_->begin();

   // Menubar
   menubar_=new Fl_Menu_Bar(0, 0, UI::default_window_width, UI::default_field_height);
   menubar_->menu(menu);
   menubar_->box(FL_NO_BOX);
   UI::set_color_menu(menubar_);

   // Toolbar
   Fl_Group* toolbar=new Fl_Group(0, menubar_->y()+menubar_->h()+UI::default_field_gap, UI::default_window_width, UI::default_field_height);
   toolbar->begin();

   // Toolbar / New Occurrence
   Fl_Button* button=0;
   button=new Fl_Button(0, toolbar->y(), UI::default_field_height, UI::default_field_height);
   button->tooltip("New History");
   button->image(images_.get(ImageDB::ADD));
   button->align(FL_ALIGN_IMAGE_NEXT_TO_TEXT);
   button->callback(all_cb, (void*)ADD_CB);
   button->box(FL_NO_BOX);
   button->down_box(FL_UP_BOX);

   // Toolbar / Edit Occurrence
   button=new Fl_Button(button->x()+button->w()+UI::default_field_gap, toolbar->y(), UI::default_field_height, UI::default_field_height);
   button->tooltip("Edit History");
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

   // Toolbar / Person
   button=new Fl_Button(button->x()+button->w()+UI::default_field_gap, toolbar->y(), UI::default_field_height, UI::default_field_height);
   button->tooltip("Person");
   button->image(images_.get(ImageDB::PERSON));
   button->align(FL_ALIGN_IMAGE_NEXT_TO_TEXT);
   button->callback(all_cb, (void*)VIEW_PERSON_CB);
   button->box(FL_NO_BOX);
   button->down_box(FL_UP_BOX);

   // Tollbar / Statusbar
   sb_=new Fl_Box(button->x()+button->w()+UI::default_field_height/3, toolbar->y(), UI::default_window_width-(button->x()+button->w()+UI::default_field_height/3), UI::default_field_height);
   sb_->align(FL_ALIGN_RIGHT|FL_ALIGN_INSIDE);

   // Toolbar final setup
   toolbar->resizable(sb_);
   toolbar->end();

   // Content - Occurrence list
   list_=new OccurrenceListView(0, toolbar->y()+toolbar->h()+UI::default_field_gap, UI::default_window_width, DWH-(toolbar->y()+toolbar->h()+UI::default_field_gap), OccurrenceListView::HISTORY_MODE);
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
OccurrenceUI::all_cb(Fl_Widget* widget, void* data)
{
   OccurrenceUI* ui=0;
   int option=(long)data;

   if(!widget->window())
   {
      ui=(OccurrenceUI*)widget->user_data();
      option=CLOSE_CB;
   }
   else
   {
      ui=(OccurrenceUI*)widget->window()->user_data();
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
      case VIEW_TASK_CB:
         UI::get_ui().view_task();
         break;
      case VIEW_PROJECT_CB:
         UI::get_ui().view_project();
         break;
      case VIEW_PERSON_CB:
         UI::get_ui().view_person();
         break;
   }
}

void
OccurrenceUI::show()
{
   if(reload_)
   {
      reload_=false;
      list_->show();
      update_statusbar();
   }
   list_->take_focus();
   window_->show();
}

void
OccurrenceUI::hide()
{
   window_->hide();
}

void
OccurrenceUI::redraw()
{
   if(window_->shown())
   {
      window_->redraw();
      UI::set_color_menu(menubar_);
   }
}

void
OccurrenceUI::add()
{
   UI::get_ui().edit_occurrence();
}

void
OccurrenceUI::remove()
{
   Occurrence* occurrence=list_->get_selected();
   if(!occurrence)
   {
      return;
   }

   Task* task=occurrence->get_task();
   if(task)
   {
      int status=task->get_status();
      if(status != Task::IN_PROGRESS)
      {
         fl_message_title("Question");
         if(fl_choice("Parent task is in %s status, so I can't remove it!\n"
               "Do you want to open the task for review?",
               "Don't Open It",
               "Open Task",
               0,
               Task::status_to_string(status).c_str()) == 1)
         {
            UI::get_ui().edit_task(task);
         }
         return;
      }
   }

   UI::get_ui().remove_occurrence(occurrence);
}

void
OccurrenceUI::edit()
{
   Occurrence* occurrence=list_->get_selected();
   if(occurrence != 0)
   {
      UI::get_ui().edit_occurrence(occurrence);
   }
}

void
OccurrenceUI::update_statusbar()
{
   std::stringstream ss;
   ss << dao_.count() << " total";
   sb_->copy_label(ss.str().c_str());
}

void
OccurrenceUI::notify(const Subject& subject __attribute__((unused)),
      const std::string& message __attribute__((unused)))
{
   if(window_->visible_r())
   {
      update_statusbar();
   }
   else
   {
      reload_=true;
   }
}

