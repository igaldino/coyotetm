// Date.cxx
// This file contains the routines to acess the database.
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

#include <Date.h>

Date::Date(const time_t& dt)
: day_(0),
  month_(0),
  year_(0)
{
   if(dt <= 0) return;

   struct tm tmdt=*localtime(&dt);
   day_=tmdt.tm_mday;
   month_=tmdt.tm_mon+1;
   year_=tmdt.tm_year+1900;
}

Date::Date(const std::string& str,
           const int& fmt)
: day_(0),
  month_(0),
  year_(0)
{
   if(str.length() == 0) return;

   time_t today=time(0);
   struct tm tm_today=*localtime(&today);
   if(((fmt == MMDDYYYY || fmt == DDMMYYYY) && str.length() != 10) ||
      ((fmt == MMDD || fmt == DDMM) && str.length() != 5))
   {
      throw InvalidDateFormat();
   }
   switch(fmt)
   {
      case MMDDYYYY:
         day_=atol(str.substr(3,2).c_str());
         month_=atol(str.substr(0,2).c_str());
         year_=atol(str.substr(6,4).c_str());
         break;
      case MMDD:
         day_=atol(str.substr(3,2).c_str());
         month_=atol(str.substr(0,2).c_str());
         year_=tm_today.tm_year+1900;
         break;
      case DDMMYYYY:
         day_=atol(str.substr(0,2).c_str());
         month_=atol(str.substr(3,2).c_str());
         year_=atol(str.substr(6,4).c_str());
         break;
      case DDMM:
         day_=atol(str.substr(0,2).c_str());
         month_=atol(str.substr(3,2).c_str());
         year_=tm_today.tm_year+1900;
         break;
      default:
         throw InvalidDateFormat();
         break;
   }
}

Date::~Date()
{
}

time_t
Date::get_time()
{
   if(day_ == 0) return 0;

   struct tm dt;
   dt.tm_sec=0;
   dt.tm_min=0;
   dt.tm_hour=0;
   dt.tm_mday=day_;
   dt.tm_mon=month_-1;
   dt.tm_year=year_-1900;
   dt.tm_wday=0; // will be set by mktime
   dt.tm_yday=0; // will be set by mktime
   dt.tm_isdst=0; // doesn't matter
   return mktime(&dt);
}

std::string
Date::get_string(const int& fmt)
{
   if(day_ == 0) return "";

   std::stringstream s;
   switch(fmt)
   {
      case MMDDYYYY:
         s << std::setw(2) << std::setfill('0') << month_;
         s << "/";
         s << std::setw(2) << std::setfill('0') << day_;
         s << "/";
         s << std::setw(4) << std::setfill('0') << year_;
         break;
      case MMDD:
         s << std::setw(2) << std::setfill('0') << month_;
         s << "/";
         s << std::setw(2) << std::setfill('0') << day_;
         break;
      case DDMMYYYY:
         s << std::setw(2) << std::setfill('0') << day_;
         s << "/";
         s << std::setw(2) << std::setfill('0') << month_;
         s << "/";
         s << std::setw(4) << std::setfill('0') << year_;
         break;
      case DDMM:
         s << std::setw(2) << std::setfill('0') << day_;
         s << "/";
         s << std::setw(2) << std::setfill('0') << month_;
         break;
   }
   return s.str();
}

std::string
Date::time_to_string(const time_t& dt, const int& fmt)
{
   try
   {
      Date d(dt);
      return(d.get_string(fmt));
   }
   catch(...)
   {
      return "";
   }
}

time_t
Date::string_to_time(const std::string& str, const int& fmt)
{
   try
   {
      Date d(str, fmt);
      return(d.get_time());
   }
   catch(...)
   {
      return -1;
   }
}

Date
Date::get_today()
{
   Date d(time(NULL));
   return d;
}

Date
Date::get_yesterday()
{
   Date d(time(NULL)-(3600*24));
   return d;
}

Date
Date::get_tomorrow()
{
   Date d(time(NULL)+(3600*24));
   return d;
}

