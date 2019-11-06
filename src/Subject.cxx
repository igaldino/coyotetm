// Subject.cxx
// This file implements the Subject class.
//
// Copyright (C) 2000-2015 Isaque Galdino de Araujo.
// Written by Isaque Galdino de Araujo <igaldino@gmail.br>
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

#include <Subject.h>

// constructor
Subject::Subject(const std::string& message):
   message_(message)
{
}

// attach an observer
void
Subject::attach(Observer& observer)
{
   observers_.push_back(&observer);
}

// detach an observer
void
Subject::detach(Observer& observer)
{
   observers_.remove(&observer);
}

// update all observers
void
Subject::notify_observers()
{
   std::list<Observer*>::iterator i=observers_.begin();
   std::list<Observer*>::iterator maxi=observers_.end();
   for(; i!=maxi; i++)
   {
      (*i)->notify(*this, message_);
   }
}

