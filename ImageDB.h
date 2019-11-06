// ImageDB.h <<Singleton>>
// This file contains the ImageDB class definition
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

#ifndef _ImageDB_h_
#define _ImageDB_h_

class ImageDB;

#include <vector>
#include <algorithm>
#include <FL/Fl_Pixmap.H>
#include <Delete.h>

class ImageDB
{
   public:
      // Meyers Singleton
      static ImageDB& get_image_db();

      // Images
      enum
      {
         ADD,
         CANCEL,
         CLOCK,
         COMPLETE,
         COYOTETM,
         DELETE,
         EDIT,
         FOLDER,
         ICON,
         PERSON,
         PROJECT,
         TASK,
         TRACK,
         IMAGE_DB_SIZE
      };

      Fl_Image* get(const int& which);

   private:
      static std::vector<Fl_Image*> images_;

      // Meyers Singleton, constructor is hidden
      ImageDB();
      ~ImageDB();
};

#endif

