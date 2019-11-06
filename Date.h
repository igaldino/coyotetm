// Date.h
// This file contains the Date class definition.
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

#ifndef _Date_h_
#define _Date_h_

#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cstdlib>

class Date
{
   public:
      // there are only 4 string formats for date below
      // and all of them are separated by '/'
      enum {MMDDYYYY, MMDD, DDMMYYYY, DDMM};

      // exception thrown by this class
      class InvalidDateFormat {};

      Date(const time_t& dt);
      // can throw InvalidDateFormat
      Date(const std::string& str, const int& fmt=MMDDYYYY);
      ~Date();

      time_t get_time();
      std::string get_string(const int& fmt=MMDDYYYY);

      static std::string time_to_string(const time_t& dt, const int& fmt=MMDDYYYY);
      static time_t string_to_time(const std::string& str, const int& fmt=MMDDYYYY);

      static Date get_today();
      static Date get_yesterday();
      static Date get_tomorrow();

   private:
      int day_;
      int month_;
      int year_;
};

#endif

