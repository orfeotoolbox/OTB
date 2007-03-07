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
// $Id: ossimVpfCoverage.cpp,v 1.6 2003/12/01 22:11:38 gpotts Exp $
#include <algorithm>
using namespace std;

#include "ossimVpfCoverage.h"
#include "ossimVpfLibrary.h"
#include "ossimVpfFeatureClassSchema.h"
#include "ossimVpfBoundingRecordTable.h"
#include "base/common/ossimErrorCodes.h"

ossimVpfCoverage::ossimVpfCoverage()
   :theLibrary(NULL),
    theCoverageName(""),
    theCoverageNameFullPath("")
{
}

ossimVpfCoverage::~ossimVpfCoverage()
{
   theLibrary = NULL;
}

bool ossimVpfCoverage::openCoverage(ossimVpfLibrary* library,
                                    const ossimString& name,
                                    const ossimFilename& fullPath)
{
   bool result = true;
   theCoverageName         = name;
   theCoverageNameFullPath = fullPath;
   theLibrary = library;

   if(!theCoverageNameFullPath.dirCat("fcs").exists())
   {
      result = false;
   }
   
   return result;
}

const ossimFilename& ossimVpfCoverage::getPath()const
{
   return theCoverageNameFullPath;
}

const ossimString& ossimVpfCoverage::getName()const
{
   return theCoverageName;
}

void ossimVpfCoverage::print(ostream& out)const
{
  ossimVpfTable table;
  if(table.openTable(theCoverageNameFullPath.dirCat("fcs")))
  {
     table.print(out);
  }
}

bool ossimVpfCoverage::getExtent(ossimVpfExtent& extent)const
{
   bool result = false;
   bool firstOneSetFlag = false;
   ossimVpfExtent resultExtent;
   ossimVpfExtent tempExtent;
   if(theLibrary)
   {
      if(theCoverageNameFullPath.dirCat("ebr").exists())
      {
         ossimVpfBoundingRecordTable boundingRecord;
         
         if(boundingRecord.openTable(theCoverageNameFullPath.dirCat("ebr")))
         {
            boundingRecord.getExtent(tempExtent);

            if(!firstOneSetFlag)
            {
               resultExtent = tempExtent;
               firstOneSetFlag = true;
            }
            else
            {
               resultExtent = resultExtent + tempExtent;
            }
            
            result = true;
         }
      }
      if(theCoverageNameFullPath.dirCat("fbr").exists())
      {
         ossimVpfBoundingRecordTable boundingRecord;
         
         if(boundingRecord.openTable(theCoverageNameFullPath.dirCat("fbr")))
         {
            boundingRecord.getExtent(tempExtent);
            
            if(!firstOneSetFlag)
            {
               resultExtent = tempExtent;
               firstOneSetFlag = true;
            }
            else
            {
               resultExtent = resultExtent + tempExtent;
            }
            
            result = true;
         }
      }
      std::vector<ossimString> tileNames;
      
      theLibrary->getTileNames(tileNames);
      
      for(int i = 0; i < (int)tileNames.size(); ++i)
      {
         ossimFilename tileName = tileNames[0];
         ossimFilename tileDir;
         if(theCoverageNameFullPath.dirCat(tileName).exists())
         {
            tileDir = theCoverageNameFullPath.dirCat(tileName);
         }
         else if(theCoverageNameFullPath.dirCat(tileName.downcase()).exists())
         {
            tileDir = theCoverageNameFullPath.dirCat(tileName.downcase());
         }
         if(tileDir != "")
         {
            if(tileDir.dirCat("ebr").exists())
            {
               ossimVpfBoundingRecordTable boundingRecord;
               
               if(boundingRecord.openTable(tileDir.dirCat("ebr")))
               {
                  boundingRecord.getExtent(tempExtent);
                  
                  if(!firstOneSetFlag)
                  {
                     resultExtent = tempExtent;
                     firstOneSetFlag = true;
                  }
                  else
                  {
                     resultExtent = resultExtent + tempExtent;
                  }
                  
                  result = true;
               }
            }
            if(tileDir.dirCat("fbr").exists())
            {
               ossimVpfBoundingRecordTable boundingRecord;
               
               if(boundingRecord.openTable(tileDir.dirCat("fbr")))
               {
                  boundingRecord.getExtent(tempExtent);
                  
                  if(!firstOneSetFlag)
                  {
                     resultExtent = tempExtent;
                     firstOneSetFlag = true;
                  }
                  else
                  {
                     resultExtent = resultExtent + tempExtent;
                  }
                  
                  result = true;
               }
            }
         }
      }
   }

   extent = resultExtent;
   
   return result;
}

ossimVpfLibrary* ossimVpfCoverage::getLibrary()
{
   return theLibrary;
}

bool ossimVpfCoverage::openFeatureClassSchema(ossimVpfFeatureClassSchema& schema)
{
   if(!schema.isClosed()) schema.closeTable();

   return schema.openSchema(this);
}
