//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//********************************************************************
// $Id: ossimVpfFeatureClassSchema.cpp 20936 2012-05-18 14:45:00Z oscarkramer $
#include <ossim/vec/ossimVpfLibrary.h>
#include <ossim/vec/ossimVpfCoverage.h>
#include <ossim/vec/ossimVpfFeatureClassSchema.h>
#include <ossim/vec/ossimVpfFeatureClass.h>

ossimVpfFeatureClassSchema::ossimVpfFeatureClassSchema()
   :theCoverage(NULL)
{
}

bool ossimVpfFeatureClassSchema::openSchema(ossimVpfCoverage* coverage)
{
   theCoverage = coverage;
   bool result = false;

   if(theCoverage)
   {
      theSchemaTableName = theCoverage->getPath().dirCat("fcs");

      if(theSchemaTableName.exists())
      {
         result = openTable(theSchemaTableName);

         if(result)
         {
            result = validateColumnNames();
         }
         if(result)
         {
            setFeatureClassMapping();
         }
      }
   }
   
   return result;
}

void ossimVpfFeatureClassSchema::closeTable()
{
   ossimVpfTable::closeTable();
   theFeatureClassMap.clear();
}

void ossimVpfFeatureClassSchema::getFeatureClasses(std::vector<ossimString>& featureClassArray)const
{
   featureClassArray.clear();
   std::map<ossimString, ossimVpfFeatureClassSchemaNode, ossimStringLtstr>::const_iterator featureClassIter = theFeatureClassMap.begin();

   while(featureClassIter != theFeatureClassMap.end())
   {
      featureClassArray.push_back((*featureClassIter).first);
      ++featureClassIter;
   }
}

bool ossimVpfFeatureClassSchema::getFeatureClassNode(const ossimString& featureClass,
                                                     ossimVpfFeatureClassSchemaNode& featureClassNode)const
{
   std::map<ossimString, ossimVpfFeatureClassSchemaNode, ossimStringLtstr>::const_iterator fIter = theFeatureClassMap.find(featureClass);

   if(fIter != theFeatureClassMap.end())
   {
      featureClassNode = (*fIter).second;
      return true;
   }

   return false;
}

ossim_int32 ossimVpfFeatureClassSchema::getNumberOfFeatureClasses()const
{
   return (ossim_int32)theFeatureClassMap.size();
}

bool ossimVpfFeatureClassSchema::validateColumnNames()const
{
   if(isClosed()) return false;
   return ((getColumnPosition("feature_class")>=0)&&
           (getColumnPosition("table1")>=0)&&
           (getColumnPosition("table1_key")>=0)&&
           (getColumnPosition("table2")>=0)&&
           (getColumnPosition("table2_key")>=0));
}

void ossimVpfFeatureClassSchema::setFeatureClassMapping()
{
   if(!isClosed())
   {
      ossim_int32 featureIdx = getColumnPosition("feature_class");
      ossim_int32 table1Idx = getColumnPosition("table1");
      ossim_int32 table1KeyIdx = getColumnPosition("table1_key");
      ossim_int32 table2Idx = getColumnPosition("table2");
      ossim_int32 table2KeyIdx = getColumnPosition("table2_key");
      reset();
      if(getNumberOfRows() > 0)
      {
         row_type row;
         const int ROWS = getNumberOfRows();
         for(int rowIdx = 1; rowIdx <= ROWS; ++rowIdx)
         {
            if(rowIdx == 1)
            {
               row = read_row(rowIdx,
                              *theTableInformation);
            }
            else
            {
               row = read_next_row(*theTableInformation);
            }
            ossimFilename primitiveTable =  getColumnValueAsString(row,
                                                                   table2Idx);
            if(ossimVpfFeatureClass::isPrimitive(primitiveTable))
            {
               ossimString primitiveTableKey = getColumnValueAsString(row,
                                                                      table2KeyIdx);
               ossimFilename table = getColumnValueAsString(row,
                                                            table1Idx);
               ossimString tableKey = getColumnValueAsString(row,
                                                             table1KeyIdx);
               ossimString featureClass = getColumnValueAsString(row,
                                                                 featureIdx);
               ossimVpfFeatureClassSchemaNode node(table,
                                                   tableKey,
                                                   primitiveTable,
                                                   primitiveTableKey);
               
               theFeatureClassMap.insert(make_pair(featureClass,
                                                   node));
            }
            
            free_row(row, *theTableInformation);
         }
      }
   }
}


