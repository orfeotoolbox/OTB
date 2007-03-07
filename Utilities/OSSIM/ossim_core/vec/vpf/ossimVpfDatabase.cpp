//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License 
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: This class give the capability to access tiles from an
//              vpf file.
//
//********************************************************************
// $Id: ossimVpfDatabase.cpp,v 1.2 2003/09/03 17:30:14 gpotts Exp $
#include "ossimVpfDatabase.h"
#include "ossimVpfDatabaseHeader.h"
#include "ossimVpfDatabaseHeaderTableValidator.h"
#include "ossimVpfLibraryAttributeTableValidator.h"
#include "ossimVpfTable.h"
#include "ossimVpfLibrary.h"
#include "base/common/ossimErrorCodes.h"

ossimVpfDatabase::ossimVpfDatabase()
   :theDatabaseHeaderTable(""),
    theLibraryAttributeTable(""),
    theOpenedFlag(false)
{
}

ossimVpfDatabase::~ossimVpfDatabase()
{
   deleteLibraryList();
   
   closeDatabase();
}

bool ossimVpfDatabase::openDatabase(const ossimFilename& filename)
{
   bool result = true;

   ossimFilename tempFilename = filename;

   tempFilename.convertBackToForwardSlashes();

   // we should have two table to look at for the database.
   // 1) database header (dht) 2) and the Library attribute (lat)
   theDatabaseHeaderTable   = ossimFilename(tempFilename.path()).dirCat("/dht");
   theLibraryAttributeTable = ossimFilename(tempFilename.path()).dirCat("/lat");

   if(theDatabaseHeaderTable.exists() &&
      theLibraryAttributeTable.exists())
   {
      ossimVpfTable table;
      // now lets see if they are valid tables
      result = table.openTable(theDatabaseHeaderTable);
      
      if(result&&
         !ossimVpfDatabaseHeaderTableValidator().isValid(table))
      {
         result =  false;
      }
      
      result = table.openTable(theLibraryAttributeTable);
      if((result) &&!ossimVpfLibraryAttributeTableValidator().isValid(table))
      {
         result = false;
      }
   }
   else
   {
      result = false;
   }

   if(result)
   {
      theOpenedFlag = true;
      initializeLibraryList();
   }
   
   return result;
}

void ossimVpfDatabase::closeDatabase()
{
   theLibraryAttributeTable = "";
   theDatabaseHeaderTable   = "";
   theOpenedFlag = false;
   deleteLibraryList();
}


ossimFilename ossimVpfDatabase::getPath()const
{
   // we can use either table name since all we need is the path
   return theLibraryAttributeTable.path();
}

long ossimVpfDatabase::getNumberOfLibraries()const
{
   return (long)theVpfLibraryList.size();
}

ossimVpfLibrary* ossimVpfDatabase::getLibrary(unsigned long libraryNumber)
{

   if(libraryNumber < theVpfLibraryList.size())
   {
      return theVpfLibraryList[libraryNumber];
   }
   
   return NULL;
}

ossimVpfLibrary* ossimVpfDatabase::getLibrary(const ossimString& name)
{
   for(long index = 0; index < (long)theVpfLibraryList.size(); ++index)
   {
      if(theVpfLibraryList[index]->getName() == name)
      {
         return theVpfLibraryList[index];
      }
   }
   
   return NULL;
}


bool ossimVpfDatabase::isOpened()const
{
   return theOpenedFlag;
}

vector<ossimString> ossimVpfDatabase::getLibraryNames()const
{

   ossimVpfTable table;

   if(table.openTable(theLibraryAttributeTable))
   {
      return table.getColumnValues("LIBRARY_NAME");
   }
   
   return vector<ossimString>();
}

vector<ossimString> ossimVpfDatabase::getLibraryNamesFullPath()const
{
   vector<ossimString> result;
   
   ossimVpfTable table;

   if(table.openTable(theLibraryAttributeTable))
   {
      result = table.getColumnValues("LIBRARY_NAME");
   }

   for(long index=0; index < (long)result.size();index++)
   {
      result[index] = ossimString(theLibraryAttributeTable.path()) +
                      ossimString("/") +
                      result[index];
   }
   
   return result;
}

ossimFilename ossimVpfDatabase::getLibraryAttributeTable()const
{
   return theLibraryAttributeTable;
}

void ossimVpfDatabase::initializeLibraryList()
{
   // make sure the library list is deleted
   deleteLibraryList();
   vector<ossimString> libraryNames = getLibraryNames();
   for(long index = 0; index < (long)libraryNames.size(); index++)
   {
      ossimVpfLibrary *library = new ossimVpfLibrary;
      library->openLibrary(this,
                           libraryNames[index],
                           getPath().dirCat(libraryNames[index]));
      theVpfLibraryList.push_back(library);
   }
}

void ossimVpfDatabase::deleteLibraryList()
{
   for(long index = 0; index <(long)theVpfLibraryList.size(); index++)
   {
      delete theVpfLibraryList[index];
   }

   theVpfLibraryList.clear();
}
