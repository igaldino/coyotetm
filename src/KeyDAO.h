// KeyDAO.h <<DataAccessObject>> <<Singleton>>
// This file contains the KeyDAO class definition
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

#ifndef _KeyDAO_h_
#define _KeyDAO_h_

class KeyDAO;

#include <sstream>
#include <Database.h>

class KeyDAO
{
   public:
      // Meyers Singleton
      static KeyDAO& get_dao();

      // Keys
      enum
      {
         PERSON_KEY=1,
         PROJECT_KEY,
         TASK_KEY,
         OCCURRENCE_KEY,
         THEME_KEY,
         SHOW_ALL_KEY
      };

      long get(const int& which);
      long current(const int& which);
      void set(const int& which, int& value);
      void set(const int& which, bool& value);

      std::vector<std::string> dump();

   private:
      Database db_;

      // Meyers Singleton, constructor is hidden
      KeyDAO();
};

#endif

