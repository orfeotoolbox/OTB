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
//
//********************************************************************
// $Id: ossimVpfFeatureClassSchema.h,v 1.5 2003/09/22 00:14:20 dburken Exp $
#ifndef ossimVpfFeatureClassSchema_HEADER
#define ossimVpfFeatureClassSchema_HEADER
#include <iostream>
#include <map>
#include <vector>
#include "ossimVpfTable.h"

class ossimVpfLibrary;

class ossimVpfFeatureClassSchemaNode
{
 public:
   friend ostream& operator <<(std::ostream& out,
                               const ossimVpfFeatureClassSchemaNode& data)
      {
         out << "table    :     " << data.theTable << endl
             << "key      :     " << data.theTableKey << endl
             << "primitive:     " << data.thePrimitiveTable << endl
             << "primitive key: " << data.thePrimitiveTableKey << endl;
         return out;
      }
   
   ossimVpfFeatureClassSchemaNode(const ossimFilename& table=ossimFilename(""),
                                  const ossimString& tableKey=ossimString(""),
                                  const ossimFilename& primitiveTable=ossimFilename(""),
                                  const ossimString& primitiveTableKey=ossimString(""))
      :theTable(table),
      theTableKey(tableKey),
      thePrimitiveTable(primitiveTable),
      thePrimitiveTableKey(primitiveTableKey)
   {
   }
   
   ossimFilename theTable;
   ossimString   theTableKey;
   ossimFilename thePrimitiveTable;
   ossimString   thePrimitiveTableKey;
};

class ossimVpfFeatureClassSchema : public ossimVpfTable
{
public:
   ossimVpfFeatureClassSchema();

   bool openSchema(ossimVpfCoverage* coverage);
   virtual void closeTable();
   
   void getFeatureClasses(std::vector<ossimString>& featureClassArray)const;
   bool getFeatureClassNode(const ossimString& featureClass,
                            ossimVpfFeatureClassSchemaNode& featureClassNode)const;
   ossim_int32 getNumberOfFeatureClasses()const;
protected:
   ossimVpfCoverage* theCoverage;
   ossimFilename     theSchemaTableName;
   std::map<ossimString, ossimVpfFeatureClassSchemaNode> theFeatureClassMap;
   
   bool validateColumnNames()const;
   void setFeatureClassMapping();
};

#endif
