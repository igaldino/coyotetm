// Database.cxx
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

#include <Database.h>

Database::Database()
: db_(0, 0),
  cursor_(0),
  join_(0),
  key_(),
  data_(),
  filename_(""),
  is_index_(false)
{
}

Database::Database(const std::string& filename,
                   const bool& is_index)
: db_(0, 0),
  cursor_(0),
  join_(0),
  key_(),
  data_(),
  filename_(filename),
  is_index_(is_index)
{
   try
   {
      // Redirect debugging information to std::cerr
      db_.set_error_stream(&std::cerr);

      // If this is an index database.
      if(is_index_)
      {
         db_.set_flags(DB_DUPSORT);
      }

      // Open the database
      db_.open(0,                 // Transaction pointer
               (basedir_+filename_).c_str(), // Database file name
               0,                 // Optional logical database name
               DB_BTREE,          // Database access method
               DB_CREATE,         // Open flags
               0);                // File mode (using defaults)

      // Create the cursor only if this isn't an index
      // if it is an index, then it must be called associate
      // to create the cursor
      if(!is_index_)
      {
         db_.cursor(0, &cursor_, 0);
      }
   }
   // DbException is not subclassed from std::exception, so
   // need to catch both of these.
   catch(DbException &e)
   {
      std::cerr << "Error opening database " << basedir_+filename_ << ": " << std::endl;
      std::cerr << e.what() << std::endl;
   }
   catch(std::exception &e)
   {
      std::cerr << "Error opening database " << basedir_+filename_ << ": " << std::endl;
      std::cerr << e.what() << std::endl;
   }
}

Database::~Database()
{
   // Close the db
   try
   {
      // Destroy the join cursor
      unjoin();

      // Destroy the cursor
      if(cursor_ != 0)
      {
         cursor_->close();
      }

      // Close the database
      db_.close(0);
   }
   catch(DbException &e)
   {
      std::cerr << "Error closing database " << basedir_+filename_ << ": " << std::endl;
      std::cerr << e.what() << std::endl;
   }
   catch(std::exception &e)
   {
      std::cerr << "Error closing database " << basedir_+filename_ << ": " << std::endl;
      std::cerr << e.what() << std::endl;
   }
}

void*
Database::get(Dbc* cursor,
              const long& id,
              const bool& next)
{
   // fulfill key value
   key_.set_data((void*)&id);
   key_.set_size(sizeof(id));

   if(id > 0)
   {
      try
      {
         // get data by key from db
         if(cursor->get(&key_, &data_, DB_SET) == 0)
         {
            // return data if found
            return data_.get_data();
         }
      }
      // DbException is not subclassed from std::exception, so
      // need to catch both of these.
      catch(DbException &e)
      {
         std::cerr << "Error reading database " << basedir_+filename_ << ": " << std::endl;
         std::cerr << e.what() << std::endl;
      }
      catch(std::exception &e)
      {
         std::cerr << "Error reading database " << basedir_+filename_ << ": " << std::endl;
         std::cerr << e.what() << std::endl;
      }
   }
   else
   {
      if(next)
      {
         if(is_index_)
         {
            // get next data from index
            if(cursor->get(&key_, &data_, DB_NEXT_DUP) == 0)
            {
               // return data if found
               return data_.get_data();
            }
         }
         else
         {
            // get next data from db
            if(cursor->get(&key_, &data_, DB_NEXT) == 0)
            {
               // return data if found
               return data_.get_data();
            }
         }
      }
      else
      {
         // get first data from db
         if(cursor->get(&key_, &data_, DB_FIRST) == 0)
         {
            // return data if found
            return data_.get_data();
         }
      }
   }

   // get failed
   return 0;
}

void*
Database::get(const long& id,
              const bool& next)
{
   // join mode
   if(join_ != 0)
   {
      // get data by using the join
      if(join_->get(&key_, &data_, 0) == 0)
      {
         // return data if found
         return data_.get_data();
      }
      // get failed
      return 0;
   }
   else
   {
      return get(cursor_, id, next);
   }
}

bool
Database::save(const long& id,
               void* data,
               const size_t& size)
{
   // fulfill key value
   key_.set_data((void*)&id);
   key_.set_size(sizeof(id));

   // fulfill data value
   data_.set_data(data);
   data_.set_size(size);

   // save to the db
   if(db_.put(0, &key_, &data_, 0) == 0)
   {
      // sync db
      db_.sync(0);

      // save successfull
      return true;
   }

   // save failed
   return false;
}


bool
Database::remove(const long& id)
{
   // fulfill key value
   key_.set_data((void*)&id);
   key_.set_size(sizeof(id));

   // remove from db
   if(db_.del(0, &key_, 0) == 0)
   {
      // sync db
      db_.sync(0);

      // remove successfull
      return true;
   }

   // remove failed
   return false;
}

bool
Database::associate(Database& db,
                    int (*cb)(Db*, const Dbt*, const Dbt*, Dbt*))
{
   // doesn't work if this isn't an index
   if(!is_index_)
   {
      return false;
   }

   // if rebuild, clean index data first
   if(reindex_)
   {
      db_.truncate(0, 0, 0);
   }

   // associate this index with the master database
   if(db.db_.associate(0, &db_, cb, (reindex_ ? DB_CREATE : 0)) != 0)
   {
      return false;
   }

   // open the cursor
   return !db_.cursor(0, &cursor_, 0);
}

bool
Database::join(std::vector<Database*>& indexes)
{
   // can't make join in indexes database
   if(is_index_)
   {
      return false;
   }

   Dbc* cursors[indexes.size()+1];
   std::vector<Database*>::iterator i=indexes.begin();
   std::vector<Database*>::iterator end=indexes.end();
   int ci=0;
   for(; i != end; i++, ci++)
   {
      cursors[ci]=(*i)->cursor_;
   }
   cursors[ci]=0;

   if(db_.join(cursors, &join_, 0) == 0)
   {
      return true;
   }
   return false;
}

void
Database::unjoin()
{
   if(join_ != 0)
   {
      join_->close();
      join_=0;
   }
}

std::string Database::basedir_="";
bool Database::reindex_=false;

std::string&
Database::get_basedir()
{
   return basedir_;
}

void
Database::set_basedir(const std::string& basedir)
{
   basedir_=basedir;
}

bool&
Database::get_reindex()
{
   return reindex_;
}

void
Database::set_reindex(const bool& reindex)
{
   reindex_=reindex;
}

std::string
Database::encode(const std::string& text)
{
   std::string tmp=text;
   for(size_t i=tmp.find('\n'); i != std::string::npos; i=tmp.find('\n'))
   {
      tmp.replace(i, 1, "%NEWLINE%");
   }
   for(size_t i=tmp.find('\"'); i != std::string::npos; i=tmp.find('\"'))
   {
      tmp.replace(i, 1, "%QUOTE%");
   }
   for(size_t i=tmp.find(','); i != std::string::npos; i=tmp.find(','))
   {
      tmp.replace(i, 1, "%COMMA%");
   }
   return tmp;
}

std::string
Database::decode(const std::string& text)
{
   std::string tmp=text;
   for(size_t i=tmp.find("%NEWLINE%"); i != std::string::npos; i=tmp.find("%NEWLINE%"))
   {
      tmp.replace(i, 9, "\n");
   }
   for(size_t i=tmp.find("%QUOTE%"); i != std::string::npos; i=tmp.find("%QUOTE%"))
   {
      tmp.replace(i, 7, "\"");
   }
   for(size_t i=tmp.find("%COMMA%"); i != std::string::npos; i=tmp.find("%COMMA%"))
   {
      tmp.replace(i, 7, ",");
   }
   return tmp;
}

