//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: This class give the capability to access tiles from an
//              vpf file.
//
//********************************************************************
// $Id: ossimVpfDatabase.h 9967 2006-11-29 02:01:23Z gpotts $
#ifndef ossimVpfDatabase_HEADER
#define ossimVpfDatabase_HEADER
#include <vector>
using namespace std;

#include <ossim/base/ossimErrorContext.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimString.h>

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
