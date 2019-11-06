// Subject.h - Defines the Subject class.
//
// This file contains the Subject class definition.
//
// Copyright (C) 2000-2015 Isaque Galdino de Araujo.
// Written by Isaque Galdino de Araujo <igaldino@galdino.br>
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

#ifndef _Subject_h_
#define _Subject_h_

class Subject;

#include <list>
#include <Observer.h>

class Subject
{
   public:
      // constructor
      Subject(const std::string& message=0);

      // attach/detache an observer
      void attach(Observer& observer);
      void detach(Observer& observer);

      // update all observers
      void notify_observers();

   private:
      // a message to be sent to observers
      std::string message_;

      // a list of observers
      std::list<Observer*> observers_;
};

#endif

