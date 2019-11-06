// Database.h
// This file contains the Database class definition.
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

#ifndef _Database_h_
#define _Database_h_

#include <iostream>
#include <vector>
#include <db_cxx.h>

class Database
{
   public:
      Database(const std::string& filename,
               const bool& is_index=false);
      ~Database();

      void* get(const long& id,
                const bool& next);

      bool save(const long& id,
                void* data,
                const size_t& size);

      bool remove(const long& id);

      bool associate(Database& db,
                     int (*cb)(Db*, const Dbt*, const Dbt*, Dbt*));

      bool join(std::vector<Database*>& indexes);
      void unjoin();

      static std::string& get_basedir();
      static void set_basedir(const std::string& basedir="");

      static bool& get_reindex();
      static void set_reindex(const bool& reindex=false);

      static std::string encode(const std::string& text);
      static std::string decode(const std::string& text);


   private:
      Db db_;
      Dbc* cursor_;
      Dbc* join_;

      Dbt key_;
      Dbt data_;

      std::string filename_;
      bool is_index_;

      // this is to prevent from being created
      // without filename
      Database();

      static std::string basedir_; // directory where all dbs will be created
      static bool reindex_; // flag to rebuild index when opening it

      void* get(Dbc* cursor,
                const long& id,
                const bool& next);

};

#endif
