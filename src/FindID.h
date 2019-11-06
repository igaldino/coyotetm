// FindID.h <<template>>
// This file contains the FindID template definition
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

#ifndef _FindID_h_
#define _FindID_h_

// to be used by the for_each algorithm in order
// delete T objects
template<typename T>
class FindID
{
   public:
      FindID(const long& id): id_(id) {}
      bool operator()(T* p) { return p->get_id() == id_; }

   private:
      long id_;
};

#endif

