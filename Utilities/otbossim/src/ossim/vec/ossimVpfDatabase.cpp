//*******************************************************************
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
// 
// Description: This class extends the stl's string class.
//
//********************************************************************
// $Id: ossimVpfDatabase.cpp 19636 2011-05-24 16:48:45Z gpotts $
#include <ossim/vec/ossimVpfDatabase.h>
#include <ossim/vec/ossimVpfDatabaseHeader.h>
#include <ossim/vec/ossimVpfDatabaseHeaderTableValidator.h>
#include <ossim/vec/ossimVpfLibraryAttributeTableValidator.h>
#include <ossim/vec/ossimVpfTable.h>
#include <ossim/vec/ossimVpfLibrary.h>
#include <ossim/base/ossimErrorCodes.h>

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
   theDatabaseHeaderTable   = ossimFilename(tempFilename.path()).dirCat("dht");
   theLibraryAttributeTable = ossimFilename(tempFilename.path()).dirCat("lat");

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
   for(long idx = 0; idx <(long)theVpfLibraryList.size(); idx++)
   {
      delete theVpfLibraryList[idx];
      theVpfLibraryList[idx] = 0;
   }

   theVpfLibraryList.clear();
}
