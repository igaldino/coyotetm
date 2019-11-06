// coyotetm.cxx
// This file contains the main function for CoyoteTM.
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

#include <cstdlib>
#include <iostream>
#include <FL/filename.H>
#include <FL/fl_ask.H>
#include <Database.h>
#include <KeyDAO.h>
#include <PersonDAO.h>
#include <ProjectDAO.h>
#include <TaskDAO.h>
#include <OccurrenceDAO.h>
#include <UI.h>

int main(int argc, char* argv[])
{
   UI &ui=UI::get_ui();
   char homepath[FL_PATH_MAX];
   fl_filename_expand(homepath, FL_PATH_MAX, "~");
   homepath[FL_PATH_MAX-1]='\0';

   std::string basedir=homepath;

#ifdef _WIN32
   if(argc > 1 && !strncmp(argv[1], "--dev", 5))
   {
      basedir+="\\ctm-dev";
      argv++;
      argc--;
   }
   else
   {
      basedir+="\\coyotetm";
   }
#else
   if(argc > 1 && !strncmp(argv[1], "--dev", 5))
   {
      basedir+="/ctm-dev/";
      argv++;
      argc--;
   }
   else
   {
      basedir+="/coyotetm/";
   }
#endif
   if(!fl_filename_isdir(basedir.c_str()))
   {
      ui.init();
      std::string command="mkdir \"" + basedir + "\"";
      fl_message_title("Information");
      if(system(command.c_str()) == 0)
      {
         fl_message("Directory %s created", basedir.c_str());
      }
      else
      {
         fl_alert("Directory %s creation failure", basedir.c_str());
         return 1;
      }
   }
#ifdef _WIN32
   basedir+="\\";
#endif
   Database::set_basedir(basedir);

   if(argc > 1 &&
         (!strncmp(argv[1], "--dump", 6) || !strncmp(argv[1], "--reindex", 9)))
   {
      if(!strncmp(argv[1], "--reindex", 9))
      {
         Database::set_reindex(true);
      }
      std::vector<std::string> tmp=KeyDAO::get_dao().dump();
      for(std::vector<std::string>::const_iterator it=tmp.begin(), end=tmp.end(); it != end; it++)
      {
         std::cout << *it << std::endl;
      }
      tmp=PersonDAO::get_dao().dump();
      for(std::vector<std::string>::const_iterator it=tmp.begin(), end=tmp.end(); it != end; it++)
      {
         std::cout << *it << std::endl;
      }
      tmp=ProjectDAO::get_dao().dump();
      for(std::vector<std::string>::const_iterator it=tmp.begin(), end=tmp.end(); it != end; it++)
      {
         std::cout << *it << std::endl;
      }
      tmp=TaskDAO::get_dao().dump();
      for(std::vector<std::string>::const_iterator it=tmp.begin(), end=tmp.end(); it != end; it++)
      {
         std::cout << *it << std::endl;
      }
      tmp=OccurrenceDAO::get_dao().dump();
      for(std::vector<std::string>::const_iterator it=tmp.begin(), end=tmp.end(); it != end; it++)
      {
         std::cout << *it << std::endl;
      }
      return 0;
   }
   else if(argc > 1 && !strncmp(argv[1], "--version", 9))
   {
      std::cout << "CoyoteTM v" << VERSION << std::endl;
      std::cout << "Manage tasks like a coyote" << std::endl;
      std::cout << "http://coyotetm.sourceforge.net" << std::endl;
      std::cout << "Copyright (C) 2000-2015 igaldino" << std::endl;
      return 0;
   }

   return ui.run(argc, argv);
}

