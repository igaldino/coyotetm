// TaskUI.h <<GUI>>
//
// This file contains the TaskUI class implementation.
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

#include <TaskUI.h>

// default window width, height, field height, field gap and button width
#define DWH 450

// callbacks
enum
{
   NO_CB,
   ABOUT_CB,
   ADD_CB,
   COLOR_AMBIANCE_CB,
   COLOR_DARK_CB,
   COLOR_DEFAULT_CB,
   COLOR_NUMIX_CB,
   EDIT_CB,
   LIST_CB,
   MARK_CANCELED_CB,
   MARK_COMPLETED_CB,
   QUIT_CB,
   SHOW_ALL_CB,
   VIEW_HISTORY_CB,
   VIEW_PERSON_CB,
   VIEW_PROJECT_CB
};

// menu items
static Fl_Menu_Item menu[]=
{
   {"&File", 0, 0, 0, FL_SUBMENU, 0, 0, 0, 0},
      {"&New             ", FL_COMMAND+'n', (Fl_Callback*)TaskUI::all_cb, (void*)ADD_CB,  0,               0, 0, 0, 0},
      {"&Open            ", FL_COMMAND+'o', (Fl_Callback*)TaskUI::all_cb, (void*)EDIT_CB, FL_MENU_DIVIDER, 0, 0, 0, 0},
      {"&Quit            ", FL_COMMAND+'q', (Fl_Callback*)TaskUI::all_cb, (void*)QUIT_CB, 0,               0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
   {"&Edit", 0, 0, 0, FL_SUBMENU, 0, 0, 0, 0},
      {"Mark Completed   ", FL_COMMAND+'k', (Fl_Callback*)TaskUI::all_cb, (void*)MARK_COMPLETED_CB, 0, 0, 0, 0, 0},
      {"Mark Canceled    ", FL_COMMAND+'x', (Fl_Callback*)TaskUI::all_cb, (void*)MARK_CANCELED_CB,  0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
   {"&View", 0, 0, 0, FL_SUBMENU, 0, 0, 0, 0},
      {"&Project         ", FL_COMMAND+'2', (Fl_Callback*)TaskUI::all_cb, (void*)VIEW_PROJECT_CB, 0,                                 0, 0, 0, 0},
      {"Per&son          ", FL_COMMAND+'3', (Fl_Callback*)TaskUI::all_cb, (void*)VIEW_PERSON_CB,  0,                                 0, 0, 0, 0},
      {"&History         ", FL_COMMAND+'4', (Fl_Callback*)TaskUI::all_cb, (void*)VIEW_HISTORY_CB, FL_MENU_DIVIDER,                   0, 0, 0, 0},
      {"Show &All        ", FL_COMMAND+'a', (Fl_Callback*)TaskUI::all_cb, (void*)SHOW_ALL_CB,     FL_MENU_TOGGLE | FL_MENU_DIVIDER,  0, 0, 0, 0},
      {"&Colors", 0, 0, 0, FL_SUBMENU, 0, 0, 0, 0},
         {"&Default         ", 0, (Fl_Callback*)TaskUI::all_cb, (void*)COLOR_DEFAULT_CB,  FL_MENU_RADIO | FL_MENU_VALUE, 0, 0, 0, 0},
         {"Dar&k            ", 0, (Fl_Callback*)TaskUI::all_cb, (void*)COLOR_DARK_CB,     FL_MENU_RADIO,                 0, 0, 0, 0},
         {"&Ambiance        ", 0, (Fl_Callback*)TaskUI::all_cb, (void*)COLOR_AMBIANCE_CB, FL_MENU_RADIO,                 0, 0, 0, 0},
         {"&Numix           ", 0, (Fl_Callback*)TaskUI::all_cb, (void*)COLOR_NUMIX_CB,    FL_MENU_RADIO,                 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
   {"&Help", 0, 0, 0, FL_SUBMENU, 0, 0, 0, 0},
      {"&About CoyoteTM  ", 0, (Fl_Callback*)TaskUI::all_cb, (void*)ABOUT_CB, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0}
};

TaskUI::TaskUI(int argc, char* argv[]):
      dao_(TaskDAO::get_dao()), ui_(UI::get_ui()),
      images_(ImageDB::get_image_db()), window_(0), menubar_(0), list_(0), sb_(0)
{
   dao_.attach(*this);

   // Window
   window_=new Fl_Window(UI::default_window_width, DWH, "CoyoteTM");
   window_->begin();

   // Menubar
   menubar_=new Fl_Menu_Bar(0, 0, UI::default_window_width, UI::default_field_height);
   menubar_->menu(menu);
   menubar_->box(FL_NO_BOX);

   // Toolbar
   Fl_Group* toolbar=new Fl_Group(0, menubar_->y()+menubar_->h()+UI::default_field_gap, UI::default_window_width, UI::default_field_height);
   toolbar->begin();

   // Toolbar / New Task
   Fl_Button* button=0;
   button=new Fl_Button(0, toolbar->y(), UI::default_field_height, UI::default_field_height);
   button->tooltip("New Task");
   button->image(images_.get(ImageDB::ADD));
   button->align(FL_ALIGN_IMAGE_NEXT_TO_TEXT);
   button->callback(all_cb, (void*)ADD_CB);
   button->box(FL_NO_BOX);
   button->down_box(FL_UP_BOX);

   // Toolbar / Edit Task
   button=new Fl_Button(button->x()+button->w()+UI::default_field_gap, toolbar->y(), UI::default_field_height, UI::default_field_height);
   button->tooltip("Edit Task");
   button->image(images_.get(ImageDB::EDIT));
   button->align(FL_ALIGN_IMAGE_NEXT_TO_TEXT);
   button->callback(all_cb, (void*)EDIT_CB);
   button->box(FL_NO_BOX);
   button->down_box(FL_UP_BOX);

   // Toolbar / Mark Completed
   button=new Fl_Button(button->x()+button->w()+UI::default_field_gap, toolbar->y(), UI::default_field_height, UI::default_field_height);
   button->tooltip("Mark Completed");
   button->image(images_.get(ImageDB::COMPLETE));
   button->align(FL_ALIGN_IMAGE_NEXT_TO_TEXT);
   button->callback(all_cb, (void*)MARK_COMPLETED_CB);
   button->box(FL_NO_BOX);
   button->down_box(FL_UP_BOX);

   // Toolbar / Mark Canceled
   button=new Fl_Button(button->x()+button->w()+UI::default_field_gap, toolbar->y(), UI::default_field_height, UI::default_field_height);
   button->tooltip("Mark Canceled");
   button->image(images_.get(ImageDB::CANCEL));
   button->align(FL_ALIGN_IMAGE_NEXT_TO_TEXT);
   button->callback(all_cb, (void*)MARK_CANCELED_CB);
   button->box(FL_NO_BOX);
   button->down_box(FL_UP_BOX);

   // Toolbar / Separator & Project
   button=new Fl_Button(button->x()+button->w()+UI::default_field_height/3, toolbar->y(), UI::default_field_height, UI::default_field_height);
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

   // Content - Task list
   list_=new TaskListView(0, toolbar->y()+toolbar->h()+UI::default_field_gap, UI::default_window_width, DWH-(toolbar->y()+toolbar->h()+UI::default_field_gap));
   list_->callback(all_cb, (void*)LIST_CB);
   list_->show();

   // Window final setup
   ui_.set_icon(window_);
   window_->resizable(list_);
   window_->end();
   window_->show(argc, argv);
   ui_.init();
   window_->callback(all_cb, this);
   window_->size_range(350, 150);

   set_show_all(KeyDAO::get_dao().current(KeyDAO::SHOW_ALL_KEY));
   show_all();
   ui_.set_color_menu(menubar_); // this must be called after ui_.init();
}

void
TaskUI::all_cb(Fl_Widget* widget, void* data)
{
   TaskUI* ui=0;
   int option=(long)data;

   if(!widget->window())
   {
      ui=(TaskUI*)widget->user_data();
      option=QUIT_CB;
   }
   else
   {
      ui=(TaskUI*)widget->window()->user_data();
   }

   switch(option)
   {
      case ABOUT_CB:
         ui->ui_.about();
         break;
      case ADD_CB:
         ui->add();
         break;
      case COLOR_AMBIANCE_CB:
         ui->ui_.set_theme(UI::AMBIANCE_THEME);
         break;
      case COLOR_DARK_CB:
         ui->ui_.set_theme(UI::DARK_THEME);
         break;
      case COLOR_DEFAULT_CB:
         ui->ui_.set_theme(UI::DEFAULT_THEME);
         break;
      case COLOR_NUMIX_CB:
         ui->ui_.set_theme(UI::NUMIX_THEME);
         break;
      case EDIT_CB:
         ui->edit();
         break;
      case LIST_CB:
         if(Fl::event_clicks() || Fl::event_key() == FL_Enter)
         {
            Fl::event_clicks(0);
            ui->edit();
         }
         else
         {
            ui->list_->callback_sort();
         }
         break;
      case MARK_CANCELED_CB:
         ui->mark_canceled();
         break;
      case MARK_COMPLETED_CB:
         ui->mark_completed();
         break;
      case QUIT_CB:
         ui->ui_.quit();
         break;
      case SHOW_ALL_CB:
         ui->show_all();
         break;
      case VIEW_HISTORY_CB:
         ui->ui_.view_occurrence();
         break;
      case VIEW_PERSON_CB:
         ui->ui_.view_person();
         break;
      case VIEW_PROJECT_CB:
         ui->ui_.view_project();
         break;
   }
}

void
TaskUI::show()
{
   list_->take_focus();
   window_->show();
}

void
TaskUI::hide()
{
   window_->hide();
}

void
TaskUI::redraw()
{
   if(window_->shown())
   {
      window_->redraw();
      UI::set_color_menu(menubar_);
   }
}

const Fl_Window*
TaskUI::get_window() const
{
   return window_;
}

void
TaskUI::add()
{
   ui_.edit_task();
}

void
TaskUI::edit()
{
   Task* task=list_->get_selected();
   if(task != 0)
   {
      ui_.edit_task(task);
   }
}

void
TaskUI::mark_canceled()
{
   ui_.mark_task_canceled(list_->get_selected());
}

void
TaskUI::mark_completed()
{
   ui_.mark_task_completed(list_->get_selected());
}

Fl_Menu_Item*
TaskUI::get_show_all_menu_item()
{
   return (Fl_Menu_Item*)menubar_->find_item("&View/Show &All        ");
}

bool
TaskUI::get_show_all()
{
   Fl_Menu_Item* item=get_show_all_menu_item();
   if(item)
   {
      return item->value();
   }
   return 0;
}

void
TaskUI::set_show_all(const bool& value)
{
   Fl_Menu_Item* item=get_show_all_menu_item();
   if(item)
   {
      if(value)
      {
         item->set();
      }
      else
      {
         item->clear();
      }
   }
}

void
TaskUI::show_all()
{
   bool selected=get_show_all();
   KeyDAO::get_dao().set(KeyDAO::SHOW_ALL_KEY, selected);
   list_->show(selected);
   update_statusbar();
}

void
TaskUI::update_statusbar()
{
   bool show_all=get_show_all();
   std::stringstream ss;
   if(show_all)
   {
      ss << dao_.count() << " total";
   }
   else
   {
      ss << dao_.count_opened() << " opened";
   }
   sb_->copy_label(ss.str().c_str());
}

void
TaskUI::notify(const Subject& subject __attribute__((unused)),
      const std::string& message __attribute__((unused)))
{
   if(window_->visible_r())
   {
      update_statusbar();
   }
}
