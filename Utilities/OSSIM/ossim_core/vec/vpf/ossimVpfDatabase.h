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
// $Id: ossimVpfDatabase.h,v 1.2 2003/09/03 17:30:14 gpotts Exp $
#ifndef ossimVpfDatabase_HEADER
#define ossimVpfDatabase_HEADER
#include <vector>
using namespace std;

#include "base/context/ossimErrorContext.h"
#include "base/data_types/ossimFilename.h"
#include "base/data_types/ossimString.h"

class ossimVpfLibrary;

class ossimVpfDatabase
{
public:
   /*!
    * Initializes everything to null;
    */
   ossimVpfDatabase();

   /*!
    * Destroys any allocated space.
    */
   virtual ~ossimVpfDatabase();

   /*!
    * This will open a database.  This will either
    * be a dht(database header table) table or a
    * library attribut table (lat) file.  Both of these
    * files should exist in a vpf database.
    */
   virtual bool openDatabase(const ossimFilename& filename);

   void closeDatabase();
   ossimFilename getPath()const;

   long getNumberOfLibraries()const;
   /*!
    *
    */
   ossimVpfLibrary* getLibrary(unsigned long libraryNumber);

   /*!
    *
    */
   ossimVpfLibrary* getLibrary(const ossimString& name);

   bool isOpened()const;
   
   vector<ossimString> getLibraryNames()const;
   vector<ossimString> getLibraryNamesFullPath()const;

   ossimFilename getLibraryAttributeTable()const;
protected:

   void initializeLibraryList();
   void deleteLibraryList();
   
   /*!
    * This is the complete path to the Database Header Table (dht file).
    */
   ossimFilename theDatabaseHeaderTable;

   /*!
    * This is the complete path to the library attribute table (lat file).
    */
   ossimFilename theLibraryAttributeTable;

   /*!
    * Will hold a constructed set of libraries.
    */
   vector<ossimVpfLibrary*> theVpfLibraryList;

   /*!
    * This jhust specifies whether the database has been opened or not.
    */
   bool  theOpenedFlag;
};

#endif
