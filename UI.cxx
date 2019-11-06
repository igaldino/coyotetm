// UI.cxx <<GUI>> <<Singleton>>
//
// This file contains the UI class implementation.
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
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
// 02111-1307, USA.

#include <UI.h>

const int UI::default_window_width=500;
const int UI::default_field_height=25;
const int UI::default_field_indentation=100;
const int UI::default_field_gap=1;
const int UI::default_button_width=80;

// Meyers singleton
UI&
UI::get_ui()
{
   static UI ui;
   return ui;
}

int
UI::run(int argc, char *argv[])
{
   view_task(argc, argv);
   return Fl::run();
}

void
UI::init()
{
   // set fonts up before we create the first window
   set_font(FL_HELVETICA);
   set_fontsize(13);
   Fl_Tooltip::font(get_font());
   Fl_Tooltip::size(get_fontsize());
   Fl::set_labeltype(FL_NORMAL_LABEL, UI::draw_label, UI::measure_label);

   // set boxtypes up after we create our first window
   Fl::set_boxtype(FL_UP_FRAME,        FL_BORDER_FRAME);
   Fl::set_boxtype(FL_DOWN_FRAME,      FL_BORDER_FRAME);
   Fl::set_boxtype(FL_THIN_UP_FRAME,   FL_BORDER_FRAME);
   Fl::set_boxtype(FL_THIN_DOWN_FRAME, FL_BORDER_FRAME);
   Fl::set_boxtype(FL_UP_BOX,          FL_BORDER_BOX);
   Fl::set_boxtype(FL_DOWN_BOX,        FL_BORDER_BOX);
   Fl::set_boxtype(FL_THIN_UP_BOX,     FL_BORDER_BOX);
   Fl::set_boxtype(FL_THIN_DOWN_BOX,   FL_BORDER_BOX);
   Fl::set_boxtype(_FL_ROUND_UP_BOX,   FL_BORDER_BOX);
   Fl::set_boxtype(_FL_ROUND_DOWN_BOX, FL_BORDER_BOX);

   // set standard colors up
   int saved_theme=KeyDAO::get_dao().current(KeyDAO::THEME_KEY);
   set_theme(saved_theme);

   // set message box icons
   Fl_Widget *mbox_icon=fl_message_icon();
   mbox_icon->image(ImageDB::get_image_db().get(ImageDB::COYOTETM));
   mbox_icon->align(FL_ALIGN_IMAGE_BACKDROP);
   mbox_icon->box(FL_NO_BOX);
   mbox_icon->labelcolor(FL_FOREGROUND_COLOR);
   mbox_icon->labeltype(FL_NO_LABEL);
}

// Hidden constructor
UI::UI():
      person_ui_(0), project_ui_(0), task_ui_(0), occurrence_ui_(0),
      person_edit_ui_(0), project_edit_ui_(0), task_edit_ui_(0), occurrence_edit_ui_(0),
      theme_(DEFAULT_THEME)
{
}

// hidden destructor
UI::~UI()
{
}

// Public methods
void
UI::view_person()
{
   if(!person_ui_)
   {
      person_ui_=new PersonUI;
   }
   person_ui_->show();
}

void
UI::view_project()
{
   if(!project_ui_)
   {
      project_ui_=new ProjectUI;
   }
   project_ui_->show();
}

void
UI::view_task(int argc, char *argv[])
{
    if(!task_ui_)
    {
       task_ui_=new TaskUI(argc, argv);
    }
    task_ui_->show();
}

void
UI::view_occurrence()
{
   if(!occurrence_ui_)
   {
      occurrence_ui_=new OccurrenceUI;
   }
   occurrence_ui_->show();
}

bool
UI::remove_person(Person* person)
{
   if(!person)
   {
      fl_message_title("Information");
      fl_alert("Please choose a person!");
      return false;
   }
   fl_message_title("Remove Person?");
   if(fl_choice("%s", "Don't Remove", "Remove Person", 0,
         person->get_name().c_str()) == 1)
   {
      PersonDAO::get_dao().remove(person->get_id());
      return true;
   }
   return false;
}

bool
UI::remove_project(Project* project)
{
   if(!project)
   {
      fl_message_title("Information");
      fl_alert("Please choose a project!");
      return false;
   }
   fl_message_title("Remove Project?");
   if(fl_choice("%s", "Don't Remove", "Remove Project", 0,
         project->get_description().c_str()) == 1)
   {
      ProjectDAO::get_dao().remove(project->get_id());
      return true;
   }
   return false;
}

bool
UI::mark_task_canceled(Task* task)
{
   if(!task)
   {
      fl_message_title("Information");
      fl_alert("Please choose a task!");
      return false;
   }
   if(task->get_status() == Task::CANCELED)
   {
      fl_message_title("Information");
      fl_alert("Task: %s\nis already canceled!", task->get_description().c_str());
      return false;
   }
   fl_message_title("Mark Task Canceled?");
   if(fl_choice("%s", "Don't Cancel", "Cancel Task", 0,
         task->get_description().c_str()) == 1)
   {
      task->cancel();
      TaskDAO::get_dao().save(*task);
      return true;
   }
   return false;
}

bool
UI::mark_task_completed(Task* task)
{
   if(!task)
   {
      fl_message_title("Information");
      fl_alert("Please choose a task!");
      return false;
   }
   if(task->get_status() == Task::COMPLETED)
   {
      fl_message_title("Information");
      fl_alert("Task: %s\nis already completed!", task->get_description().c_str());
      return false;
   }
   fl_message_title("Mark Task Completed?");
   if(fl_choice("%s", "Don't Complete", "Complete Task", 0,
         task->get_description().c_str()) == 1)
   {
      task->mark_completed();
      TaskDAO::get_dao().save(*task);
      return true;
   }
   return false;
}

bool
UI::remove_occurrence(Occurrence* occurrence)
{
   if(!occurrence)
   {
      fl_message_title("Information");
      fl_alert("Please choose a history!");
      return false;
   }
   fl_message_title("Remove History?");
   if(fl_choice("%s", "Don't Remove", "Remove History", 0,
         Date::time_to_string(occurrence->get_when()).c_str()) == 1)
   {
      OccurrenceDAO::get_dao().remove(occurrence->get_id());
      return true;
   }
   return false;
}

void
UI::about()
{
   fl_message_title("About");
   fl_message("CoyoteTM v%s\nManage tasks like a coyote\n\n"
         "http://coyotetm.sourceforge.net\n\n"
         "Copyright (C) 2000-2015 igaldino", VERSION);
}

void
UI::quit()
{
   const Fl_Window* task_window=0;
   if(task_ui_)
   {
      task_window=task_ui_->get_window();
   }
   // close history window
   if(occurrence_ui_)
   {
      occurrence_ui_->hide();
   }
   // close person list window
   if(person_ui_)
   {
      person_ui_->hide();
   }
   // close project list window
   if(project_ui_)
   {
      project_ui_->hide();
   }
   // check if there is anything else opened besides task list window
   for(Fl_Window* w=Fl::first_window(); w; w=Fl::next_window(w))
   {
      if(w != task_window)
      {
         w->show();
         fl_message_title("Information");
         fl_alert("You still have work in progress.\n\nPlease check that before quitting!");
         return;
      }
   }
   // if nothing else is opened, close task list window
   if(task_ui_)
   {
       task_ui_->hide();
   }
}

void
UI::edit_person(Person* person)
{
   if(!person_edit_ui_)
   {
      person_edit_ui_=new PersonEditUI;
   }
   person_edit_ui_->edit(person);
}

void
UI::edit_project(Project* project)
{
   if(!project_edit_ui_)
   {
      project_edit_ui_=new ProjectEditUI;
   }
   project_edit_ui_->edit(project);
}

void
UI::edit_task(Task* task, Project* project, Person* person)
{
   if(!task_edit_ui_)
   {
      task_edit_ui_=new TaskEditUI;
   }
   task_edit_ui_->edit(task);
   task_edit_ui_->set_project(project);
   task_edit_ui_->set_person(person);
}

void
UI::edit_task(Project* project, Task* task)
{
   edit_task(task, project);
}

void
UI::edit_task(Person* person, Task* task)
{
   edit_task(task, 0, person);
}

void
UI::edit_occurrence(Occurrence* occurrence)
{
   if(!occurrence_edit_ui_)
   {
      occurrence_edit_ui_=new OccurrenceEditUI;
   }
   occurrence_edit_ui_->edit(occurrence);
}

void
UI::edit_occurrence(Task* task, Occurrence* occurrence)
{
   if(!occurrence_edit_ui_)
   {
      occurrence_edit_ui_=new OccurrenceEditUI;
   }
   occurrence_edit_ui_->edit(occurrence);
   occurrence_edit_ui_->set_task(task);
}

bool
UI::ask_cancel_pending_work()
{
   return fl_choice("You still have pending work.\nDo you want to cancel it?",
                "Don't Cancel",
                "Cancel It",
                0);
}

const Fl_Font&
UI::get_font() const
{
   return font_;
}

void
UI::set_font(const Fl_Font& font)
{
   font_=font;
}

const Fl_Fontsize&
UI::get_fontsize() const
{
   return fontsize_;
}

void
UI::set_fontsize(const Fl_Fontsize& fontsize)
{
   fontsize_=fontsize;
}

void
UI::draw_label(const Fl_Label* label,
      int x, int y, int width, int height, Fl_Align align)
{
   UI& ui=get_ui();
   fl_font(ui.get_font(), ui.get_fontsize());
   fl_color((Fl_Color)label->color);
   fl_draw(label->value, x, y, width, height, align, label->image);
}

void
UI::measure_label(const Fl_Label* label, int& width, int& height)
{
   UI& ui=get_ui();
   fl_font(ui.get_font(), ui.get_fontsize());
   fl_measure(label->value, width, height);
}

void
UI::apply_font()
{
   fl_font(font_, fontsize_);
}

void
UI::apply_font(Fl_Input* input)
{
   input->textsize(fontsize_);
   input->textfont(font_);
}

void
UI::apply_font(Fl_Text_Display* text)
{
   text->textsize(fontsize_);
   text->textfont(font_);
}

void
UI::set_read_only(Fl_Input* input, const bool& read_only)
{
   if(read_only)
   {
      input->readonly(1);
      input->color(fl_inactive(FL_BACKGROUND2_COLOR));
      input->textcolor(fl_inactive(FL_FOREGROUND_COLOR));
      input->labelcolor(fl_inactive(FL_FOREGROUND_COLOR));
   }
   else
   {
      input->readonly(0);
      input->color(FL_BACKGROUND2_COLOR);
      input->textcolor(FL_FOREGROUND_COLOR);
      input->labelcolor(FL_FOREGROUND_COLOR);
   }
}

void
UI::set_read_only(Fl_Text_Display* input, const bool& read_only)
{
   if(read_only)
   {
      input->color(fl_inactive(FL_BACKGROUND2_COLOR));
      input->textcolor(fl_inactive(FL_FOREGROUND_COLOR));
      input->labelcolor(fl_inactive(FL_FOREGROUND_COLOR));
   }
   else
   {
      input->color(FL_BACKGROUND2_COLOR);
      input->textcolor(FL_FOREGROUND_COLOR);
      input->labelcolor(FL_FOREGROUND_COLOR);
   }
}

void
UI::set_icon(Fl_Window* window)
{
   window->icon(reinterpret_cast<Fl_RGB_Image*>(ImageDB::get_image_db().get(ImageDB::ICON)));
   window->xclass("CoyoteTM");
}

const int&
UI::get_theme() const
{
   return theme_;
}

void
UI::set_theme(const int& theme)
{
   switch(theme)
   {
      case DEFAULT_THEME:
         Fl::set_color(FL_BACKGROUND_COLOR,  0xed, 0xed, 0xed);
         Fl::set_color(FL_BACKGROUND2_COLOR, 0xff, 0xff, 0xff);
         Fl::set_color(FL_FOREGROUND_COLOR,  0x31, 0x37, 0x39);
         Fl::set_color(FL_INACTIVE_COLOR,    0xba, 0xbd, 0xb6);
         Fl::set_color(FL_SELECTION_COLOR,   0x4a, 0x90, 0xd9);
         break;
      case DARK_THEME:
         Fl::set_color(FL_BACKGROUND_COLOR,  0x2e, 0x34, 0x36);
         Fl::set_color(FL_BACKGROUND2_COLOR, 0x55, 0x57, 0x53);
         Fl::set_color(FL_FOREGROUND_COLOR,  0xee, 0xee, 0xec);
         Fl::set_color(FL_INACTIVE_COLOR,    0xba, 0xbd, 0xb6);
         Fl::set_color(FL_SELECTION_COLOR,   0x34, 0x65, 0xa4);
         break;
      case AMBIANCE_THEME:
         Fl::set_color(FL_BACKGROUND_COLOR,  0xf2, 0xf1, 0xf0);
         Fl::set_color(FL_BACKGROUND2_COLOR, 0xff, 0xff, 0xff);
         Fl::set_color(FL_FOREGROUND_COLOR,  0x3c, 0x3c, 0x3c);
         Fl::set_color(FL_INACTIVE_COLOR,    0x8d, 0x66, 0xd0);
         Fl::set_color(FL_SELECTION_COLOR,   0xf0, 0x77, 0x46);
         break;
      case NUMIX_THEME:
         Fl::set_color(FL_BACKGROUND_COLOR,  0x2d, 0x2d, 0x2d);
         Fl::set_color(FL_BACKGROUND2_COLOR, 0x33, 0x33, 0x33);
         Fl::set_color(FL_FOREGROUND_COLOR,  0xf9, 0xf9, 0xf9);
         Fl::set_color(FL_INACTIVE_COLOR,    0xde, 0xde, 0xde);
         Fl::set_color(FL_SELECTION_COLOR,   0xd6, 0x49, 0x37);
         break;
      default:
         return;
   }
   theme_=theme;
   KeyDAO::get_dao().set(KeyDAO::THEME_KEY, theme_);
   if(person_ui_)
   {
      person_ui_->redraw();
   }
   if(project_ui_)
   {
      project_ui_->redraw();
   }
   if(task_ui_)
   {
      task_ui_->redraw();
   }
   if(occurrence_ui_)
   {
      occurrence_ui_->redraw();
   }
   if(person_edit_ui_)
   {
      person_edit_ui_->redraw();
   }
   if(project_edit_ui_)
   {
      project_edit_ui_->redraw();
   }
   if(task_edit_ui_)
   {
      task_edit_ui_->redraw();
   }
   if(occurrence_edit_ui_)
   {
      occurrence_edit_ui_->redraw();
   }
}

std::string
UI::clean_choice_text(const std::string& text)
{
   if(text.length() == 0)
   {
      return text;
   }
   std::string tmp=text;
   for(size_t i=tmp.find('/'); i != std::string::npos; i=tmp.find('/'))
   {
      tmp.erase(i, 1);
   }
   for(size_t i=tmp.find('&'); i != std::string::npos; i=tmp.find('&'))
   {
      tmp.erase(i, 1);
   }
   for(size_t i=tmp.find('\\'); i != std::string::npos; i=tmp.find('\\'))
   {
      tmp.erase(i, 1);
   }
   if(tmp[0] == '_')
   {
      tmp.erase(0, 1);
   }
   return tmp;
}

void
UI::set_color_menu(Fl_Menu_Bar *menubar)
{
    Fl_Menu_Item* item=0;
    switch(UI::get_ui().get_theme())
    {
       case UI::DEFAULT_THEME:
          item=(Fl_Menu_Item*)menubar->find_item("&View/&Colors/&Default         ");
          break;
       case UI::DARK_THEME:
          item=(Fl_Menu_Item*)menubar->find_item("&View/&Colors/Dar&k            ");
          break;
       case UI::AMBIANCE_THEME:
          item=(Fl_Menu_Item*)menubar->find_item("&View/&Colors/&Ambiance        ");
          break;
       case UI::NUMIX_THEME:
          item=(Fl_Menu_Item*)menubar->find_item("&View/&Colors/&Numix           ");
          break;
    }
    if(item)
    {
       item->setonly();
    }
}
