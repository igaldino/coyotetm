// ImageDB.cxx <<Singleton>>
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

#include <ImageDB.h>

#include <add.xpm>
#include <cancel.xpm>
#include <clock.xpm>
#include <complete.xpm>
#include <coyotetm.xpm>
#include <delete.xpm>
#include <edit.xpm>
#include <folder.xpm>
#include <person.xpm>
#include <project.xpm>
#include <task.xpm>
#include <track.xpm>

#include <iostream>

std::vector<Fl_Image*> ImageDB::images_(IMAGE_DB_SIZE);

// Meyers singleton
ImageDB&
ImageDB::get_image_db()
{
   static ImageDB image_db;
   return image_db;
}

// Hidden constructor
ImageDB::ImageDB()
{
   images_[ADD]=new Fl_Pixmap(add_xpm);
   images_[CANCEL]=new Fl_Pixmap(cancel_xpm);
   images_[CLOCK]=new Fl_Pixmap(clock_xpm);
   images_[COMPLETE]=new Fl_Pixmap(complete_xpm);
   images_[COYOTETM]=new Fl_Pixmap(coyotetm_xpm);
   images_[DELETE]=new Fl_Pixmap(delete_xpm);
   images_[EDIT]=new Fl_Pixmap(edit_xpm);
   images_[FOLDER]=new Fl_Pixmap(folder_xpm);
   images_[PERSON]=new Fl_Pixmap(person_xpm);
   images_[PROJECT]=new Fl_Pixmap(project_xpm);
   images_[TASK]=new Fl_Pixmap(task_xpm);
   images_[TRACK]=new Fl_Pixmap(track_xpm);

#if FLTK_ABI_VERSION >= 10303
   images_[ICON]=new Fl_RGB_Image(images_[COYOTETM], Fl_Color(0));
#else
   // Create a RGBA buffer based on coyotetm.xpm file
   static unsigned char buffer[48 * 48 * 4];
   unsigned char* it=buffer;
   for(int y=0; y < 48; y++)
   {
      for(int x=0; x < 48; x++)
      {
         switch(coyotetm_xpm[4+y][x])
         {
            case ' ':
               *it++ = (char)0;
               *it++ = (char)0;
               *it++ = (char)0;
               *it++ = (char)0;
               break;
            case '.':
               *it++ = (char)0xee;
               *it++ = (char)0xee;
               *it++ = (char)0xec;
               *it++ = (char)0xff;
               break;
            case '+':
               *it++ = (char)0x34;
               *it++ = (char)0x65;
               *it++ = (char)0xa4;
               *it++ = (char)0xff;
               break;
         }
      }
   }
   images_[ICON]=new Fl_RGB_Image(buffer, 48, 48, 4);
#endif
}

ImageDB::~ImageDB()
{
   for_each(images_.begin(), images_.end(), Delete<Fl_Image>());
}

// Public methods
Fl_Image*
ImageDB::get(const int& which)
{
   return images_[which];
}

