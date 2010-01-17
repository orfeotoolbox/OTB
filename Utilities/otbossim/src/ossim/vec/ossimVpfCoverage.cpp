//*******************************************************************
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
// 
// Description: This class extends the stl's string class.
//
//********************************************************************
// $Id: ossimVpfCoverage.cpp 9963 2006-11-28 21:11:01Z gpotts $
#include <algorithm>
using namespace std;

#include <ossim/vec/ossimVpfCoverage.h>
#include <ossim/vec/ossimVpfLibrary.h>
#include <ossim/vec/ossimVpfFeatureClassSchema.h>
#include <ossim/vec/ossimVpfBoundingRecordTable.h>
#include <ossim/base/ossimErrorCodes.h>

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
