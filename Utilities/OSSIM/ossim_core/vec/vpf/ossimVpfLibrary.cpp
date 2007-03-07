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
// $Id: ossimVpfLibrary.cpp,v 1.5 2004/11/25 22:46:23 dburken Exp $
#include <algorithm>

#include "ossimVpfLibrary.h"
#include "ossimVpfDatabase.h"
#include "ossimVpfTable.h"
#include "ossimVpfDatabase.h"
#include "ossimVpfExtent.h"
#include "ossimVpfCoverage.h"
#include "base/common/ossimErrorCodes.h"
#ifndef NULL
#include <stddef.h>
#endif

ossimVpfLibrary::ossimVpfLibrary()
   :theDatabase(NULL),
    theLibraryName(""),
    theLibraryNameFullPath(""),
    theNumberOfCoverages(0)
{
}

ossimVpfLibrary::~ossimVpfLibrary()
{
   theDatabase = NULL;
}

bool ossimVpfLibrary::openLibrary(ossimVpfDatabase* database,
                                  const ossimString& name,
                                  const ossimFilename& libraryPath)
{
   bool returnCode = true;

   theNumberOfCoverages = 0;
   theLibraryName = "";
   theLibraryNameFullPath = "";
   
   
   theLibraryName         = name;
   theLibraryNameFullPath =  libraryPath;
   theDatabase            = database;
   
   if(!ossimFilename(theLibraryNameFullPath).exists())
   {
      returnCode = false;
   }
   if(returnCode)
   {
      setCoverageNames();
      
      ossimVpfTable table;
      
      theNumberOfCoverages = theCoverageNames.size();
      returnCode = (theNumberOfCoverages> 0);
   }
   
   return returnCode;
}

bool ossimVpfLibrary::getCoverage(long coverageNumber,
                                  ossimVpfCoverage& coverage)
{
   
   bool result = false;
   
   if((coverageNumber >=0) && (coverageNumber < (int)theCoverageNames.size()))
   {
      result = coverage.openCoverage(this,
                                     theCoverageNames[coverageNumber],
                                     theLibraryNameFullPath.dirCat(theCoverageNames[coverageNumber]));
   }
   
   return result;
}

bool ossimVpfLibrary::getCoverage(const ossimString& name,
                                  ossimVpfCoverage& coverage)
{
  for(int idx = 0; idx < (int)theCoverageNames.size(); ++idx)
    {
      if(theCoverageNames[idx] == name)
	{
	  return getCoverage(idx,
			     coverage);
	}
    }

  return false;
}

bool ossimVpfLibrary::getExtent(ossimVpfExtent& extent)const
{
   bool result = false;
   ossimVpfTable tempTable;


   // this code was basically cut paste from vhcl with just
   // a couple modifications.
   
    /* Get library extent from Library Attribute Table (LAT) */
/*    char* buf;*/
/*    long int n;*/
/*    double xmin,ymin,xmax,ymax;*/
    int libraryNamePosition, xminPosition, yminPosition;
    int xmaxPosition, ymaxPosition;
    int i;
    bool found;
    row_type row;
    extent_type libextent;

    if(!theDatabase)
    {
       return false;
    }
    ossimFilename file(theDatabase->getLibraryAttributeTable());
    if(!tempTable.openTable(file))
    {
       return result;
    }
    vpf_table_type *table = tempTable.getVpfTableData();
       
    libraryNamePosition = table_pos( "LIBRARY_NAME", *table );
    found = false;
    for (i=1;(i<=tempTable.getNumberOfRows())&&(!found);i++)
    {
       row = read_row( i, *table );
       ossimString libraryName = tempTable.getColumnValueAsString(row, libraryNamePosition);
       libraryName = libraryName.trim();
       if (libraryName == theLibraryName)
       {
          xminPosition = table_pos( "XMIN", *table );
          yminPosition = table_pos( "YMIN", *table );
          xmaxPosition = table_pos( "XMAX", *table );
          ymaxPosition = table_pos( "YMAX", *table );

          libextent.x1 = tempTable.getColumnValueAsString(row, xminPosition).toDouble();
          libextent.y1 = tempTable.getColumnValueAsString(row, yminPosition).toDouble();
          libextent.x2 = tempTable.getColumnValueAsString(row, xmaxPosition).toDouble();
          libextent.y2 = tempTable.getColumnValueAsString(row, ymaxPosition).toDouble();
          found = true;
       }
       else
       {
          result = false;
       }
       free_row( row, *table );
    }
    
    extent =  ossimVpfExtent(libextent);
    return result;
}

ossimFilename ossimVpfLibrary::getPath()const
{
   return theLibraryNameFullPath;
}

ossimString ossimVpfLibrary::getName()const
{
   return theLibraryName;
}


bool ossimVpfLibrary::getCoverageNames(std::vector<ossimString>& coverageNames)const
{
   bool result = true;

   coverageNames = theCoverageNames;
   
   return result;
}

int ossimVpfLibrary::getNumberOfCoverages()const
{
   return theNumberOfCoverages;
}

bool ossimVpfLibrary::hasCoverage(const ossimString& coverageName)const
{
   return (std::find(theCoverageNames.begin(),
                    theCoverageNames.end(),
                    coverageName)!=theCoverageNames.end());
}

ossimString ossimVpfLibrary::getTileName(ossim_int32 id)const
{
   ossimString result;
   if(theLibraryNameFullPath.dirCat("tileref").exists())
   {
      if(theTileNameMap.size() <1)
      {
         setTileNames();
      }
      std::map<ossim_int32, ossimString>::iterator tileNameIter = theTileNameMap.find(id);

      if(tileNameIter != theTileNameMap.end())
      {
         result = (*tileNameIter).second;
      }
   }

   return result;
}

void ossimVpfLibrary::getTileNames(std::vector<ossimString>& tileNames)const
{
   if(theLibraryNameFullPath.dirCat("tileref").exists())
   {
      if(theTileNameMap.size() <1)
      {
         setTileNames();
      }
      std::map<ossim_int32, ossimString>::iterator tileNameIter = theTileNameMap.begin();
      while(tileNameIter != theTileNameMap.end())
      {
         tileNames.push_back((*tileNameIter).second);
         ++tileNameIter;
      }
   }   
}

void ossimVpfLibrary::setCoverageNames()
{
   ossimVpfTable table;
   theCoverageNames.clear();
   // open up the coverage attribute table for this library
   if(table.openTable(theLibraryNameFullPath.dirCat("cat")))
   {
      // we need to have a coverage attribute table validator before
      // we proceed but for now assume that it is a good table.
      //
      theCoverageNames = table.getColumnValues("COVERAGE_NAME");
   }
}

void ossimVpfLibrary::setTileNames()const
{
   ossimVpfTable table;
   theTileNameMap.clear();
   row_type row;
   if(table.openTable(theLibraryNameFullPath.dirCat("tileref").dirCat("tileref.aft")))
   {
      table.reset();
      const int ROWS = table.getNumberOfRows();
      for (int rowIdx = 1; rowIdx <= ROWS; ++rowIdx)
      {
         // Note:  read_row takes a "one based" index.
         row = read_row( rowIdx, *(table.getVpfTableData()) );
         ossim_int32 namePosition   = table.getColumnPosition("TILE_NAME");
         ossim_int32 tileIdPosition = table.getColumnPosition("ID");
         
         ossimString tileName = table.getColumnValueAsString(row,
                                                             namePosition);;
         ossimString tileId   = table.getColumnValueAsString(row,
                                                             tileIdPosition);
         theTileNameMap.insert(make_pair(tileId.toUInt32(), tileName.trim()));
         free_row( row, *(table.getVpfTableData()) );
      }
   }
}
