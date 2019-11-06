// PersonEditUI.cxx <<GUI>>
//
// This file contains the PersonEditUI class implementation.
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

#include <PersonEditUI.h>

// default window width, height, field height, field indentation, field gap and button width
#define DWH 450

// callbacks
enum
{
   NO_CB,
   ADD_TASK_CB,
   CANCEL_CB,
   EDIT_TASK_CB,
   LIST_CB,
   MARK_TASK_CANCELED_CB,
   MARK_TASK_COMPLETED_CB,
   NAME_CB,
   OK_CB,
   REMOVE_CB
};

PersonEditUI::PersonEditUI():
   person_(0), is_new_(true), ui_(UI::get_ui()), images_(ImageDB::get_image_db())
{
   // Window
   window_=new Fl_Window(UI::default_window_width, DWH, "New Person");
   window_->begin();

   // Name
   name_=new Fl_Input(UI::default_field_indentation, 0, UI::default_window_width-UI::default_field_indentation, UI::default_field_height, "Name:");
   name_->callback(all_cb, (void*)NAME_CB);
   ui_.apply_font(name_);

   // Task bar
   Fl_Group* taskbar=new Fl_Group(0, name_->y()+name_->h()+UI::default_field_gap, UI::default_window_width, UI::default_field_height);
   taskbar->begin();

   // Task bar / Label
   Fl_Box* task_label=new Fl_Box(0, taskbar->y(), UI::default_window_width-(UI::default_field_height*4+UI::default_field_gap*3), UI::default_field_height, "Task");
   task_label->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);

   // Task bar / Add Task
   Fl_Button* button=new Fl_Button(task_label->x()+task_label->w(), taskbar->y(), UI::default_field_height, UI::default_field_height);
   button->image(images_.get(ImageDB::ADD));
   button->tooltip("Add Task");
   button->callback(all_cb, (void*)ADD_TASK_CB);
   button->shortcut(FL_COMMAND+'n');
   button->box(FL_NO_BOX);
   button->down_box(FL_UP_BOX);

   // Task bar / Edit Task
   button=new Fl_Button(button->x()+button->w()+UI::default_field_gap, taskbar->y(), UI::default_field_height, UI::default_field_height);
   button->image(images_.get(ImageDB::EDIT));
   button->tooltip("Edit Task");
   button->callback(all_cb, (void*)EDIT_TASK_CB);
   button->shortcut(FL_COMMAND+'o');
   button->box(FL_NO_BOX);
   button->down_box(FL_UP_BOX);

   // Task bar / Mark Task Completed
   button=new Fl_Button(button->x()+button->w()+UI::default_field_gap, taskbar->y(), UI::default_field_height, UI::default_field_height);
   button->image(images_.get(ImageDB::COMPLETE));
   button->tooltip("Mark Task Completed");
   button->callback(all_cb, (void*)MARK_TASK_COMPLETED_CB);
   button->shortcut(FL_COMMAND+'k');
   button->box(FL_NO_BOX);
   button->down_box(FL_UP_BOX);

   // Task bar / Mark Task Canceled
   button=new Fl_Button(button->x()+button->w()+UI::default_field_gap, taskbar->y(), UI::default_field_height, UI::default_field_height);
   button->image(images_.get(ImageDB::CANCEL));
   button->tooltip("Mark Task Canceled");
   button->callback(all_cb, (void*)MARK_TASK_CANCELED_CB);
   button->shortcut(FL_COMMAND+'x');
   button->box(FL_NO_BOX);
   button->down_box(FL_UP_BOX);

   taskbar->resizable(task_label);
   taskbar->end();

   // Task List
   list_=new TaskListView(0, taskbar->y()+taskbar->h()+UI::default_field_gap, UI::default_window_width, DWH-(taskbar->y()+taskbar->h()+UI::default_field_gap*2+UI::default_field_height));
   list_->callback(all_cb, (void*)LIST_CB);

   // Actionbar
   Fl_Group* actionbar=new Fl_Group(0, list_->y()+list_->h()+UI::default_field_gap, UI::default_window_width, UI::default_field_height);
   actionbar->begin();

   // Actionbar / Delete
   button=new Fl_Button(0, actionbar->y(), UI::default_button_width, UI::default_field_height, "Delete");
   button->tooltip("Delete Person");
   button->callback(all_cb, (void*)REMOVE_CB);
   button->shortcut(FL_COMMAND+'d');

   // Actionbar / OK
   button=new Fl_Button(UI::default_window_width-UI::default_button_width, actionbar->y(), UI::default_button_width, UI::default_field_height, "&OK");
   button->callback(all_cb, (void*)OK_CB);

   // Actionbar / Cancel
   button=new Fl_Button(UI::default_window_width-(UI::default_button_width*2+UI::default_field_gap), actionbar->y(), UI::default_button_width, UI::default_field_height, "&Cancel");
   button->callback(all_cb, (void*)CANCEL_CB);

   // Actionbar final setup
   actionbar->resizable(new Fl_Box(button->x()-UI::default_field_gap, actionbar->y(), UI::default_field_gap, UI::default_field_height));
   actionbar->end();

   // Window final setup
   ui_.set_icon(window_);
   window_->resizable(list_);
   window_->end();
   window_->show();
   window_->callback(all_cb, this);
   window_->position(Fl::event_x_root()-10, Fl::event_y_root()-10);
   window_->size_range(UI::default_button_width * 4 + UI::default_field_gap * 3, 150);
}

PersonEditUI::~PersonEditUI()
{
   delete_person();
}

void
PersonEditUI::all_cb(Fl_Widget* widget, void* data)
{
   PersonEditUI* ui=0;
   int option=(long)data;

   if(!widget->window())
   {
      ui=(PersonEditUI*)widget->user_data();
      option=CANCEL_CB;
   }
   else
   {
      ui=(PersonEditUI*)widget->window()->user_data();
   }

   switch(option)
   {
      case ADD_TASK_CB:
         ui->add_task();
         break;
      case CANCEL_CB:
         ui->hide();
         break;
      case EDIT_TASK_CB:
         ui->edit_task();
         break;
      case LIST_CB:
         if(Fl::event_clicks() || Fl::event_key() == FL_Enter)
         {
            Fl::event_clicks(0);
            ui->edit_task();
         }
         else
         {
            ui->list_->callback_sort();
         }
         break;
      case MARK_TASK_CANCELED_CB:
         ui->mark_task_canceled();
         break;
      case MARK_TASK_COMPLETED_CB:
         ui->mark_task_completed();
         break;
      case NAME_CB:
         ui->name_changed();
         break;
      case OK_CB:
         ui->ok();
         break;
      case REMOVE_CB:
         ui->remove();
         break;
   }
}

void
PersonEditUI::edit(Person* person)
{
   // check if we are still editing anything
   fl_message_title("Question");
   if(person_ && !ui_.ask_cancel_pending_work())
   {
      window_->show();
      return;
   }
   clean();
   delete_person();
   person_=person;
   if(person)
   {
      is_new_=false;

      std::string new_title="Person: " + person->get_name();
      window_->copy_label(new_title.c_str());
 
      name_->value(person_->get_name().c_str());
      list_->show(person_);
   }
   else
   {
      is_new_=true;
      window_->copy_label("Person: <NEW>");
   }
   name_->take_focus();
   window_->show();
}

void
PersonEditUI::add_task()
{
   if(save())
   {
      ui_.edit_task(person_);
   }
}

void
PersonEditUI::hide()
{
   delete_person();
   window_->hide();
}

void
PersonEditUI::redraw()
{
   if(window_->shown())
   {
      window_->redraw();
   }
}

void
PersonEditUI::edit_task()
{
   Task* task=list_->get_selected();
   if(task != 0 && save())
   {
      ui_.edit_task(person_, task);
   }
}

void
PersonEditUI::mark_task_canceled()
{
   if(save())
   {
      ui_.mark_task_canceled(list_->get_selected());
   }
}

void
PersonEditUI::mark_task_completed()
{
   if(save())
   {
      ui_.mark_task_completed(list_->get_selected());
   }
}

bool
PersonEditUI::save()
{
   if(!is_new_ && !person_)
   {
      return false;
   }

   if(name_->size() == 0)
   {
      fl_message_title("Information");
      fl_alert("Please enter name");
      name_->take_focus();
      return false;
   }

   new_person();

   person_->set_name(name_->value());
   
   // save person
   if(PersonDAO::get_dao().save(*person_))
   {
      // delete local object and use the global one
      if(is_new_)
      {
         Person* aux=PersonDAO::get_dao().get(person_->get_id());
         delete_person();
         person_=aux;
         is_new_=false;
         // update task list
         list_->show(person_);
      }
      return true;
   }
   return false;
}

void
PersonEditUI::ok()
{
   if(save())
   {
      hide();
   }
}

void
PersonEditUI::remove()
{
   if(save() && ui_.remove_person(person_))
   {
      hide();
   }
}

void
PersonEditUI::name_changed()
{
      std::string new_title=name_->value();
      new_title="Person: " + new_title;
      window_->copy_label(new_title.c_str());
}

void
PersonEditUI::new_person()
{
   if(is_new_ && !person_)
   {
      person_=new Person;
   }
}

void
PersonEditUI::delete_person()
{
   if(is_new_ && person_)
   {
      delete person_;
   }
   person_=0;
}

void
PersonEditUI::clean()
{
   name_->value("");
   list_->clean();
}
