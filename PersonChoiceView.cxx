// PersonChoiceView.cxx <<GUI>>
//
// This file contains the PersonChoiceView class implementation.
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

#include <PersonChoiceView.h>

PersonChoiceView::PersonChoiceView(int x, int y, int width, int height, const char* label):
   Fl_Choice(x, y, width, height, label), dao_(PersonDAO::get_dao())
{
   dao_.attach(*this);
   load();
}

PersonChoiceView::~PersonChoiceView()
{
   dao_.detach(*this);
}

Person*
PersonChoiceView::get_selected()
{
   const Fl_Menu_Item* i=mvalue();
   if(i)
   {
      return (Person*)i->user_data_;
   }
   return 0;
}

void
PersonChoiceView::set_selected(Person* person)
{
   value(find_item(person->get_name().c_str()));
}

void
PersonChoiceView::deselect()
{
   value((Fl_Menu_Item*)NULL);
   load();
}

void
PersonChoiceView::notify(const Subject& subject __attribute__((unused)),
      const std::string& message __attribute__((unused)))
{
   if(this->visible_r())
   {
      load();
   }
}

void
PersonChoiceView::load()
{
   Person *p=get_selected();
   Person *aux=0; // this will make sure that person still exists
   std::string text;

   clear();

   std::vector<Person*> tmp=dao_.get_all();
   std::sort(tmp.begin(), tmp.end(), Person::sort_by_name);

   std::vector<Person*>::const_iterator it=tmp.begin();
   std::vector<Person*>::const_iterator end=tmp.end();

   while(it != end)
   {
      text=UI::clean_choice_text((*it)->get_name());
      add(text.c_str(), 0, 0, (*it));
      if(p == (*it))
      {
         aux=(*it); // previous selected person still exists
      }
      it++;
   }

   if(aux)
   {
      set_selected(aux);
   }
}

