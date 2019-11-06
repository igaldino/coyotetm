// KeyDAO.cxx <<DataAccessObject>> <<Singleton>>
// This file contains the routines to manipulate data.
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

#include <KeyDAO.h>

// Meyers singleton
KeyDAO&
KeyDAO::get_dao()
{
   static KeyDAO dao;
   return dao;
}

// Hidden constructor
KeyDAO::KeyDAO()
: db_("key.db")
{
}

// Public methods
long
KeyDAO::get(const int& which)
{
   long id=current(which)+1;

   // save the current key value
   if(db_.save(which, &id, sizeof(id)))
   {
      return id;
   }

   return 0;
}

// Public methods
long
KeyDAO::current(const int& which)
{
   // get the current value for a specific key.
   long* data=(long*)db_.get(which, false);
   if(data)
   {
      return *data;
   }
   return 0;
}

void
KeyDAO::set(const int& which, int& value)
{
   db_.save(which, &value, sizeof(value));
}

void
KeyDAO::set(const int& which, bool& value)
{
   long value_int=value;
   db_.save(which, &value_int, sizeof(value_int));
}

std::vector<std::string>
KeyDAO::dump()
{
   std::vector<std::string> tmp;
   std::ostringstream ss;

   ss.str("");
   ss << "\"KEY\",\"PERSON_KEY\",\"";
   ss << current(PERSON_KEY);
   ss << "\"";
   tmp.push_back(ss.str());

   ss.str("");
   ss << "\"KEY\",\"PROJECT_KEY\",\"";
   ss << current(PROJECT_KEY);
   ss << "\"";
   tmp.push_back(ss.str());

   ss.str("");
   ss << "\"KEY\",\"TASK_KEY\",\"";
   ss << current(TASK_KEY);
   ss << "\"";
   tmp.push_back(ss.str());

   ss.str("");
   ss << "\"KEY\",\"OCCURRENCE_KEY\",\"";
   ss << current(OCCURRENCE_KEY);
   ss << "\"";
   tmp.push_back(ss.str());

   ss.str("");
   ss << "\"KEY\",\"THEME_KEY\",\"";
   ss << current(THEME_KEY);
   ss << "\"";
   tmp.push_back(ss.str());

   ss.str("");
   ss << "\"KEY\",\"SHOW_ALL_KEY\",\"";
   ss << current(SHOW_ALL_KEY);
   ss << "\"";
   tmp.push_back(ss.str());

   return tmp;
}

