// OccurrenceEditUI.cxx <<GUI>>
//
// This file contains the OccurrenceEditUI class implementation.
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

#include <OccurrenceEditUI.h>

// default window width, height, field height, field indentation, field gap and button width
#define DWH 153

// callbacks
enum
{
   NO_CB,
   CANCEL_CB,
   OK_CB,
   REMOVE_CB,
   WHEN_CB
};

OccurrenceEditUI::OccurrenceEditUI():
   occurrence_(0), is_new_(true), ui_(UI::get_ui()), stop_save_(0)
{
   // Window
   window_=new Fl_Window(UI::default_window_width, DWH, "New History");
   window_->begin();

   // Task
   task_=new TaskChoiceView(UI::default_field_indentation, 0, UI::default_window_width-UI::default_field_indentation, UI::default_field_height, "Task:");

   // When
   when_=new Fl_Input(UI::default_field_indentation, task_->y()+task_->h()+UI::default_field_gap, UI::default_window_width/2-UI::default_field_indentation, UI::default_field_height, "When:");
   when_->tooltip("Use MM/DD/YYYY format.");
   when_->callback(all_cb, (void*)WHEN_CB);
   ui_.apply_font(when_);

   // Time
   time_=new Fl_Float_Input(UI::default_window_width/2+UI::default_field_indentation, when_->y(), UI::default_window_width/2-UI::default_field_indentation, UI::default_field_height, "Hours:");
   time_->tooltip("Use decimal format:\n1:00 = 1\n1:30 = 1.5\n1:45 = 1.75\n1:20 = 1.33");
   ui_.apply_font(time_);

   // Notes
   notes_=new Fl_Text_Editor(UI::default_field_indentation, time_->y()+time_->h()+UI::default_field_gap, UI::default_window_width-UI::default_field_indentation, UI::default_field_height*3, "Notes:");
   bnotes_=new Fl_Text_Buffer();
   notes_->buffer(bnotes_);
   notes_->align(FL_ALIGN_LEFT_TOP);
   notes_->wrap_mode(Fl_Text_Display::WRAP_AT_BOUNDS, 1);
   notes_->box(FL_DOWN_BOX);
   ui_.apply_font(notes_);

   // Notes read only
   notes_read_only_=new Fl_Text_Display(notes_->x(), notes_->y(), notes_->w(), notes_->h(), "Notes:");
   notes_read_only_->buffer(bnotes_);
   notes_read_only_->align(FL_ALIGN_LEFT_TOP);
   notes_read_only_->wrap_mode(Fl_Text_Display::WRAP_AT_BOUNDS, 1);
   notes_read_only_->box(FL_DOWN_BOX);
   ui_.apply_font(notes_read_only_);
   notes_read_only_->hide();

   // Actionbar
   Fl_Group* actionbar=new Fl_Group(0, notes_->y()+notes_->h()+UI::default_field_gap, UI::default_window_width, UI::default_field_height);
   actionbar->begin();

   // Actionbar / Delete
   delete_=new Fl_Button(0, actionbar->y(), UI::default_button_width, UI::default_field_height, "Delete");
   delete_->tooltip("Delete History");
   delete_->callback(all_cb, (void*)REMOVE_CB);
   delete_->shortcut(FL_COMMAND+'d');

   // Actionbar / OK
   ok_=new Fl_Button(UI::default_window_width-UI::default_button_width, actionbar->y(), UI::default_button_width, UI::default_field_height, "&OK");
   ok_->callback(all_cb, (void*)OK_CB);

   // Actionbar / Cancel
   Fl_Button* button=new Fl_Button(UI::default_window_width-(UI::default_button_width*2+UI::default_field_gap), actionbar->y(), UI::default_button_width, UI::default_field_height, "&Cancel");
   button->callback(all_cb, (void*)CANCEL_CB);

   // Actionbar final setup
   actionbar->resizable(new Fl_Box(button->x()-UI::default_field_gap, actionbar->y(), UI::default_field_gap, UI::default_field_height));
   actionbar->end();

   // Window final setup
   ui_.set_icon(window_);
   window_->resizable(notes_);
   window_->end();
   window_->show();
   window_->callback(all_cb, this);
   window_->position(Fl::event_x_root()-10, Fl::event_y_root()-10);
   window_->size_range(UI::default_button_width * 4 + UI::default_field_gap * 3, DWH);
}

OccurrenceEditUI::~OccurrenceEditUI()
{
   delete_occurrence();
}

void
OccurrenceEditUI::edit(Occurrence* occurrence)
{
   // check if we are still editing anything
   fl_message_title("Question");
   if(occurrence_ && !ui_.ask_cancel_pending_work())
   {
      window_->show();
      return;
   }

   // init
   clean();
   occurrence_=occurrence;
   set_read_only(false);
   task_->deselect();

   if(occurrence)
   {
      std::ostringstream buffer;

      is_new_=false;
      std::string new_title="History: " + Date::time_to_string(occurrence_->get_when());
      window_->copy_label(new_title.c_str());

      Task* task=occurrence_->get_task();
      if(task)
      {
         task_->set_selected(task);
      }

      when_->value(Date::time_to_string(occurrence_->get_when()).c_str());
      buffer << std::fixed << std::setprecision(2) << occurrence_->get_time();
      time_->value(buffer.str().c_str());
      bnotes_->text(occurrence_->get_notes().c_str());

      if(task && task->get_status() >= Task::COMPLETED)
      {
         set_read_only();
      }
   }
   else
   {
      is_new_=true;
      // Choice size is size of the list + 1
      if(task_->size() == 2)
      {
         task_->value(0);
      }
      when_->value(Date::get_today().get_string().c_str());
      when_changed();
   }
   if(task_->get_selected())
   {
      time_->take_focus();
   }
   else
   {
      task_->take_focus();
   }
   window_->show();
}

void
OccurrenceEditUI::hide()
{
   delete_occurrence();
   window_->hide();
}

void
OccurrenceEditUI::redraw()
{
   if(window_->shown())
   {
      window_->redraw();
   }
}

bool
OccurrenceEditUI::save()
{
   if(stop_save_)
   {
      stop_save_->take_focus();
      return false;
   }

   std::stringstream buffer;
   float time_float;

   if(!is_new_ && !occurrence_)
   {
      return false;
   }

   fl_message_title("Information");
   Task* task=task_->get_selected();
   if(task == 0)
   {
      fl_alert("Please choose a task");
      task_->take_focus();
      return false;
   }
   else if(when_->size() == 0)
   {
      fl_alert("Please enter date");
      when_->take_focus();
      return false;
   }
   else if(time_->size() == 0)
   {
      fl_alert("Please enter hours");
      time_->take_focus();
      return false;
   }

   new_occurrence();

   occurrence_->set_task(task);
   occurrence_->set_when(Date::string_to_time(when_->value()));
   buffer << time_->value();
   buffer >> time_float;
   occurrence_->set_time(time_float);
   // Text for notes is kept in a buffer object and text()
   // method returns a new allocated copy of that text,
   // so we need to free that memory after using it.
   char* aux=bnotes_->text();
   occurrence_->set_notes(aux);
   if(aux)
   {
      free(aux);
   }

   // save occurrence
   if(OccurrenceDAO::get_dao().save(*occurrence_))
   {
      // delete local object and use the global one
      if(is_new_)
      {
         Occurrence* aux=OccurrenceDAO::get_dao().get(occurrence_->get_id());
         delete_occurrence();
         occurrence_=aux;
         is_new_=false;
      }
      return true;
   }
   return false;
}

void
OccurrenceEditUI::when_changed()
{
   stop_save_=when_;
   time_t when_in_time=Date::string_to_time(when_->value());
   fl_message_title("Information");
   if(when_in_time == -1)
   {
      fl_alert("Invalid when date [%s]", when_->value());
      when_->take_focus();
      return;
   }
   Task* task=task_->get_selected();
   if(task && when_in_time < task->get_begin())
   {
      fl_alert("When date [%s] can't be earlier than\ntask start date [%s]",
            when_->value(), Date::time_to_string(task->get_begin()).c_str());
      when_->take_focus();
      return;
   }
   std::string new_title=when_->value();
   new_title="History: " + new_title;
   window_->copy_label(new_title.c_str());
   stop_save_=0;
}

void
OccurrenceEditUI::ok()
{
   if(save())
   {
      hide();
   }
}

void
OccurrenceEditUI::remove()
{
   if(save() && ui_.remove_occurrence(occurrence_))
   {
      hide();
   }
}

void
OccurrenceEditUI::all_cb(Fl_Widget* widget, void* data)
{
   OccurrenceEditUI* ui=0;
   int option=(long)data;

   if(!widget->window())
   {
      ui=(OccurrenceEditUI*)widget->user_data();
      option=CANCEL_CB;
   }
   else
   {
      ui=(OccurrenceEditUI*)widget->window()->user_data();
   }

   switch(option)
   {
      case CANCEL_CB:
         ui->hide();
         break;
      case OK_CB:
         ui->ok();
         break;
      case REMOVE_CB:
         ui->remove();
         break;
      case WHEN_CB:
         ui->when_changed();
         break;
   }
}

void
OccurrenceEditUI::new_occurrence()
{
   if(is_new_ && !occurrence_)
   {
      occurrence_=new Occurrence;
   }
}

void
OccurrenceEditUI::delete_occurrence()
{
   if(is_new_ && occurrence_)
   {
      delete occurrence_;
   }
   occurrence_=0;
}

void
OccurrenceEditUI::clean()
{
   delete_occurrence();
   stop_save_=0;
   when_->value(Date::get_today().get_string().c_str());
   time_->value("");
   bnotes_->text("");
}

void
OccurrenceEditUI::set_task(Task* task)
{
   if(task)
   {
      task_->set_selected(task);
      when_->take_focus();
   }
}

void
OccurrenceEditUI::set_read_only(const bool& read_only)
{
   ui_.set_read_only(when_, read_only);
   ui_.set_read_only(time_, read_only);
   ui_.set_read_only(notes_read_only_, read_only);

   if(read_only)
   {
      task_->deactivate();
      notes_->hide();
      notes_read_only_->show();
      ok_->deactivate();
      delete_->deactivate();
   }
   else
   {
      task_->activate();
      notes_->show();
      notes_read_only_->hide();
      ok_->activate();
      delete_->activate();
   }
}

