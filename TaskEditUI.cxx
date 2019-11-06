// TaskEditUI.cxx <<GUI>>
//
// This file contains the TaskEditUI class implementation.
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

#include <TaskEditUI.h>

// default window width, height, field height, field indentation, field gap and button width
#define DWH 458

// callbacks
enum
{
   NO_OK,
   ADD_OCCURRENCE_CB,
   BEGIN_CB,
   CANCEL_CB,
   DELETE_OCCURRENCE_CB,
   DESCRIPTION_CB,
   DUE_CB,
   EDIT_OCCURRENCE_CB,
   END_CB,
   LIST_CB,
   MARK_COMPLETED_CB,
   MARK_CANCELED_CB,
   OK_CB,
   STATUS_CB
};

TaskEditUI::TaskEditUI():
   task_(0), is_new_(true), ui_(UI::get_ui()), images_(ImageDB::get_image_db()),
   stop_save_(0)
{
   // Window
   window_=new Fl_Window(UI::default_window_width, DWH, "New Task");
   window_->begin();

   // Description
   description_=new Fl_Input(UI::default_field_indentation, 0, UI::default_window_width-UI::default_field_indentation, UI::default_field_height, "Description:");
   description_->callback(all_cb, (void*)DESCRIPTION_CB);
   ui_.apply_font(description_);

   // Status
   status_=new StatusChoiceView(UI::default_field_indentation, description_->y()+description_->h()+UI::default_field_gap, UI::default_window_width-UI::default_field_indentation, UI::default_field_height);
   status_->callback(all_cb, (void*)STATUS_CB);
   
   // Due
   due_=new Fl_Input(UI::default_field_indentation, status_->y()+status_->h()+UI::default_field_gap, UI::default_window_width/2-UI::default_field_indentation, UI::default_field_height, "Due:");
   due_->tooltip("Use MM/DD/YYYY format.");
   due_->callback(all_cb, (void*)DUE_CB);
   ui_.apply_font(due_);

   // Priority
   priority_=new PriorityChoiceView(UI::default_window_width/2+UI::default_field_indentation, due_->y(), UI::default_window_width/2-UI::default_field_indentation, UI::default_field_height);

   // Project
   project_=new ProjectChoiceView(UI::default_field_indentation, priority_->y()+priority_->h()+UI::default_field_gap, UI::default_window_width/2-UI::default_field_indentation, UI::default_field_height, "Project:");

   // Person
   person_=new PersonChoiceView(UI::default_window_width/2+UI::default_field_indentation, project_->y(), UI::default_window_width/2-UI::default_field_indentation, UI::default_field_height, "Person:");

   // Begin
   begin_=new Fl_Input(UI::default_field_indentation, person_->y()+person_->h()+UI::default_field_gap, UI::default_window_width/2-UI::default_field_indentation, UI::default_field_height, "Start:");
   begin_->tooltip("Use MM/DD/YYYY format.");
   begin_->callback(all_cb, (void*)BEGIN_CB);
   ui_.apply_font(begin_);

   // End
   end_=new Fl_Input(UI::default_window_width/2+UI::default_field_indentation, begin_->y(), UI::default_window_width/2-UI::default_field_indentation, UI::default_field_height, "End:");
   end_->tooltip("Use MM/DD/YYYY format.");
   end_->callback(all_cb, (void*)END_CB);
   ui_.apply_font(end_);

   // Notes
   notes_=new Fl_Text_Editor(UI::default_field_indentation, end_->y()+end_->h()+UI::default_field_gap, UI::default_window_width-UI::default_field_indentation, UI::default_field_height*3, "Notes:");
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

   // FUP bar
   Fl_Group* fupbar=new Fl_Group(0, notes_->y()+notes_->h()+UI::default_field_gap, UI::default_window_width, UI::default_field_height);
   fupbar->begin();

   // FUP bar / Label
   Fl_Box* fup_label=new Fl_Box(0, fupbar->y(), UI::default_window_width-(UI::default_field_height*3+UI::default_field_gap*2), UI::default_field_height, "History");
   fup_label->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);

   // FUP bar / Add FUP
   Fl_Button* button=new Fl_Button(fup_label->x()+fup_label->w(), fupbar->y(), UI::default_field_height, UI::default_field_height);
   button->image(images_.get(ImageDB::ADD));
   button->tooltip("Add History");
   button->callback(all_cb, (void*)ADD_OCCURRENCE_CB);
   button->shortcut(FL_COMMAND+'n');
   button->box(FL_NO_BOX);
   button->down_box(FL_UP_BOX);
   //buttons_.push_back(button);

   // FUP bar / Edit FUP
   button=new Fl_Button(button->x()+button->w()+UI::default_field_gap, fupbar->y(), UI::default_field_height, UI::default_field_height);
   button->image(images_.get(ImageDB::EDIT));
   button->tooltip("Edit History");
   button->callback(all_cb, (void*)EDIT_OCCURRENCE_CB);
   button->shortcut(FL_COMMAND+'o');
   button->box(FL_NO_BOX);
   button->down_box(FL_UP_BOX);
   //buttons_.push_back(button);

   // FUP bar / Remove FUP
   button=new Fl_Button(button->x()+button->w()+UI::default_field_gap, fupbar->y(), UI::default_field_height, UI::default_field_height);
   button->image(images_.get(ImageDB::DELETE));
   button->tooltip("Remove History");
   button->callback(all_cb, (void*)DELETE_OCCURRENCE_CB);
   button->shortcut(FL_COMMAND+'d');
   button->box(FL_NO_BOX);
   button->down_box(FL_UP_BOX);
   //buttons_.push_back(button);

   fupbar->resizable(fup_label);
   fupbar->end();

   // FUP List
   list_=new OccurrenceListView(0, fupbar->y()+fupbar->h()+UI::default_field_gap, UI::default_window_width, UI::default_field_height*8);
   list_->callback(all_cb, (void*)LIST_CB);

   // Actionbar
   Fl_Group* actionbar=new Fl_Group(0, list_->y()+list_->h()+UI::default_field_gap, UI::default_window_width, UI::default_field_height);
   actionbar->begin();

   // Actionbar / Mark Completed
   button=new Fl_Button(0, actionbar->y(), UI::default_button_width * 1.5, UI::default_field_height, "Mark Completed");
   button->tooltip("Mark Task Completed");
   button->callback(all_cb, (void*)MARK_COMPLETED_CB);
   button->shortcut(FL_COMMAND+'k');
   buttons_.push_back(button);

   // Actionbar / Mark Canceled
   button=new Fl_Button(button->w()+UI::default_field_gap, actionbar->y(), UI::default_button_width * 1.5, UI::default_field_height, "Mark Canceled");
   button->tooltip("Mark Task Canceled");
   button->callback(all_cb, (void*)MARK_CANCELED_CB);
   button->shortcut(FL_COMMAND+'x');
   buttons_.push_back(button);

   // Actionbar / OK
   ok_=new Fl_Button(UI::default_window_width-UI::default_button_width, actionbar->y(), UI::default_button_width, UI::default_field_height, "&OK");
   ok_->callback(all_cb, (void*)OK_CB);
   buttons_.push_back(ok_);

   // Actionbar / Cancel
   button=new Fl_Button(UI::default_window_width-(UI::default_button_width*2+UI::default_field_gap), actionbar->y(), UI::default_button_width, UI::default_field_height, "&Cancel");
   button->callback(all_cb, (void*)CANCEL_CB);
   //buttons_.push_back(button); this is the only button that won't ever be deactivated

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
   window_->size_range(UI::default_button_width * 5 + UI::default_field_gap * 3, 330);
}

TaskEditUI::~TaskEditUI()
{
   delete_task();
}

void
TaskEditUI::all_cb(Fl_Widget* widget, void* data)
{
   TaskEditUI* ui=0;
   int option=(long)data;

   if(!widget->window())
   {
      ui=(TaskEditUI*)widget->user_data();
      option=CANCEL_CB;
   }
   else
   {
      ui=(TaskEditUI*)widget->window()->user_data();
   }

   switch(option)
   {
      case ADD_OCCURRENCE_CB:
         ui->add_occurrence();
         break;
      case BEGIN_CB:
         ui->begin_changed();
         break;
      case CANCEL_CB:
         ui->hide();
         break;
      case DELETE_OCCURRENCE_CB:
         ui->delete_occurrence();
         break;
      case DESCRIPTION_CB:
         ui->description_changed();
         break;
      case DUE_CB:
         ui->due_changed();
         break;
      case EDIT_OCCURRENCE_CB:
         ui->edit_occurrence();
         break;
      case END_CB:
         ui->end_changed();
         break;
      case LIST_CB:
         if(Fl::event_clicks() || Fl::event_key() == FL_Enter)
         {
            Fl::event_clicks(0);
            ui->edit_occurrence();
         }
         break;
      case MARK_CANCELED_CB:
         ui->mark_canceled();
         break;
      case MARK_COMPLETED_CB:
         ui->mark_completed();
         break;
      case OK_CB:
         ui->ok();
         break;
      case STATUS_CB:
         ui->status_changed();
         break;
   }
}

void
TaskEditUI::edit(Task* task)
{
   // check if we are still editing anything
   fl_message_title("Question");
   if(task_ && !ui_.ask_cancel_pending_work())
   {
      window_->show();
      return;
   }
   clean();
   task_=task;
   set_read_only(false);
   if(task)
   {
      is_new_=false;

      std::string new_title="Task: " + task_->get_description();
      window_->copy_label(new_title.c_str());

      Project* project=task_->get_project();
      Person* person=task_->get_person();
      if(project)
      {
         project_->set_selected(project);
      }
      if(person)
      {
         person_->set_selected(person);
      }
      description_->value(task_->get_description().c_str());
      begin_->value(Date::time_to_string(task_->get_begin()).c_str());
      end_->value(Date::time_to_string(task_->get_end()).c_str());
      due_->value(Date::time_to_string(task_->get_due()).c_str());
      int status=task_->get_status();
      status_->set_selected(status);
      priority_->set_selected(task_->get_priority());
      bnotes_->text(task_->get_notes().c_str());
      list_->show(task_);

      if(status >= Task::COMPLETED)
      {
         set_read_only();
      }
   }
   else
   {
      is_new_=true;

      window_->copy_label("Task: <NEW>");
      // Choice size is size of the list + 1
      if(project_->size() == 2)
      {
         project_->value(0);
      }
      // Choice size is size of the list + 1
      if(person_->size() == 2)
      {
         person_->value(0);
      }
   }
   description_->take_focus();
   window_->show();
}

void
TaskEditUI::set_project(Project* project)
{
   if(project)
   {
      project_->set_selected(project);
   }
}

void
TaskEditUI::set_person(Person* person)
{
   if(person)
   {
      person_->set_selected(person);
   }
}

void
TaskEditUI::add_occurrence()
{
   if(save() && start_task())
   {
      UI::get_ui().edit_occurrence(task_);
   }
}

void
TaskEditUI::hide()
{
   delete_task();
   window_->hide();
}

void
TaskEditUI::redraw()
{
   if(window_->shown())
   {
      window_->redraw();
   }
}

void
TaskEditUI::edit_occurrence()
{
   Occurrence* occurrence=list_->get_selected();
   if(occurrence != 0 && save())
   {
      start_task();
      UI::get_ui().edit_occurrence(task_, occurrence);
   }
}

void
TaskEditUI::delete_occurrence()
{
   Occurrence* occurrence=list_->get_selected();
   if(occurrence != 0 && save() && start_task())
   {
      UI::get_ui().remove_occurrence(occurrence);
   }
}

bool
TaskEditUI::start_task()
{
   int status=status_->get_selected();
   if(status != Task::IN_PROGRESS)
   {
      fl_message_title("Question");
      if(fl_choice("Task is in %s status!\nAre you sure you want to start it?",
                   "Don't Start It",
                   "Start Task",
                   0,
                   Task::status_to_string(status).c_str()) == 1)
      {
         status_->set_selected(Task::IN_PROGRESS);
         status_changed(Task::IN_PROGRESS);
         // save these changes
         save();
      }
      else
      {
         return false;
      }
   }
   return true;
}

bool
TaskEditUI::save()
{
   if(stop_save_)
   {
      stop_save_->take_focus();
      return false;
   }

   if(!is_new_ && !task_)
   {
      return false;
   }

   Project *project=project_->get_selected();
   Person *person=person_->get_selected();

   fl_message_title("Information");
   if(description_->size() == 0)
   {
      fl_alert("Please enter task description");
      description_->take_focus();
      return false;
   }
   else if(project == 0)
   {
      fl_alert("Please choose a project");
      project_->take_focus();
      return false;
   }
   else if(person == 0)
   {
      fl_alert("Please choose a person");
      person_->take_focus();
      return false;
   }

   new_task();

   task_->set_project(project);
   task_->set_person(person);
   task_->set_description(description_->value());
   task_->set_begin(Date::string_to_time(begin_->value()));
   task_->set_end(Date::string_to_time(end_->value()));
   task_->set_due(Date::string_to_time(due_->value()));
   task_->set_status(status_->get_selected());
   task_->set_priority(priority_->get_selected());
   // Text for notes is kept in a buffer object and text()
   // method returns a new allocated copy of that text,
   // so we need to free that memory after using it.
   char* aux=bnotes_->text();
   task_->set_notes(aux);
   if(aux)
   {
      free(aux);
   }

   // save task
   if(TaskDAO::get_dao().save(*task_))
   {
      // delete local object and use the global one
      if(is_new_)
      {
         Task* aux=TaskDAO::get_dao().get(task_->get_id());
         delete_task();
         task_=aux;
         is_new_=false;
         // update occurrence list
         list_->show(task_);
      }
      return true;
   }
   return false;
}

void
TaskEditUI::begin_changed()
{
   stop_save_=begin_;
   if(Date::string_to_time(begin_->value()) == -1)
   {
      fl_message_title("Information");
      fl_alert("Invalid begin date [%s]", begin_->value());
      begin_->take_focus();
      return;
   }
   if(begin_->size() > 0)
   {
      if(status_->get_selected() < Task::COMPLETED) // fltk bug wa
      {
         status_->set_selected(Task::IN_PROGRESS);
      }
   }
   else
   {
      if(end_->size() > 0)
      {
         end_->cut(0, end_->size());
      }
      status_->set_selected(Task::NOT_STARTED);
   }
   stop_save_=0;
}

void
TaskEditUI::end_changed()
{
   stop_save_=end_;
   time_t end_in_time=Date::string_to_time(end_->value());
   fl_message_title("Information");
   if(end_in_time == -1)
   {
      fl_alert("Invalid end date [%s]", end_->value());
      end_->take_focus();
      return;
   }

   if(end_->size() > 0)
   {
      if(begin_->size() == 0)
      {
         begin_->value(end_->value());
      }
      else
      {
         time_t begin_in_time=Date::string_to_time(begin_->value());
         if(end_in_time < begin_in_time)
         {
            fl_alert("End date [%s] can't be earlier\nthan start date [%s]",
                  end_->value(), begin_->value());
            end_->take_focus();
            return;
         }
      }
      if(status_->get_selected() < Task::COMPLETED) // fltk bug wa
      {
         status_->set_selected(Task::COMPLETED);
         set_read_only();
         // keeps OK button enabled, so we can save it :)
         ok_->activate();
      }
   }
   stop_save_=0;
}

void
TaskEditUI::due_changed()
{
   stop_save_=due_;
   if(Date::string_to_time(due_->value()) == -1)
   {
      fl_message_title("Information");
      fl_alert("Invalid due date [%s]", due_->value());
      due_->take_focus();
      return;
   }
   stop_save_=0;
}

void
TaskEditUI::status_changed(int status)
{
   std::string today=Date::get_today().get_string();
   if(status == 0)
   {
      status=status_->get_selected();
   }

   if(status == Task::COMPLETED || status == Task::CANCELED)
   {
      if(begin_->size() == 0)
      {
         begin_->value(today.c_str());
      }
      if(end_->size() == 0)
      {
         end_->value(today.c_str());
      }
   }
   else if(status == Task::IN_PROGRESS)
   {
      if(begin_->size() == 0)
      {
         begin_->value(today.c_str());
      }
      end_->cut(0, end_->size());
   }
   else if(status == Task::NOT_STARTED)
   {
      begin_->cut(0, begin_->size());
      end_->cut(0, end_->size());
   }

   if(status >= Task::COMPLETED)
   {
      set_read_only();
      // keeps OK button enabled, so we can save it :)
      ok_->activate();
   }
   else
   {
      set_read_only(false);
   }
}

void
TaskEditUI::description_changed()
{
      std::string new_title=description_->value();
      new_title="Task: " + new_title;
      window_->copy_label(new_title.c_str());
}

void
TaskEditUI::mark_canceled()
{
   if(save() && ui_.mark_task_canceled(task_))
   {
      hide();
   }
}

void
TaskEditUI::mark_completed()
{
   if(save() && ui_.mark_task_completed(task_))
   {
      hide();
   }
}

void
TaskEditUI::set_read_only(const bool& read_only)
{
   ui_.set_read_only(description_, read_only);
   ui_.set_read_only(due_, read_only);
   ui_.set_read_only(begin_, read_only);
   ui_.set_read_only(end_, read_only);
   ui_.set_read_only(notes_read_only_, read_only);

   if(read_only)
   {
      priority_->deactivate();
      project_->deactivate();
      person_->deactivate();
      notes_->hide();
      notes_read_only_->show();
      // buttons
      for(std::vector<Fl_Button*>::iterator it=buttons_.begin(), end=buttons_.end(); it != end; it++)
      {
         (*it)->deactivate();
      }
   }
   else
   {
      priority_->activate();
      project_->activate();
      person_->activate();
      notes_read_only_->hide();
      notes_->show();
      // buttons
      for(std::vector<Fl_Button*>::iterator it=buttons_.begin(), end=buttons_.end(); it != end; it++)
      {
         (*it)->activate();
      }
   }
}

void
TaskEditUI::ok()
{
   if(save())
   {
      hide();
   }
}

void
TaskEditUI::new_task()
{
   if(is_new_ && !task_)
   {
      task_=new Task;
   }
}

void
TaskEditUI::delete_task()
{
   if(is_new_ && task_)
   {
      delete task_;
   }
   task_=0;
}

void
TaskEditUI::clean()
{
   delete_task();
   stop_save_=0;
   description_->value("");
   status_->set_selected(Task::default_status);
   due_->value("");
   priority_->set_selected(Task::default_priority);
   project_->deselect();
   person_->deselect();
   begin_->value("");
   end_->value("");
   bnotes_->text("");
   list_->clean();
}
