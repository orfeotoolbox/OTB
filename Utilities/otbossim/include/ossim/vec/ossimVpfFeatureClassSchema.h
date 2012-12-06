//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
//
//********************************************************************
// $Id: ossimVpfFeatureClassSchema.h 20936 2012-05-18 14:45:00Z oscarkramer $
#ifndef ossimVpfFeatureClassSchema_HEADER
#define ossimVpfFeatureClassSchema_HEADER
#include <iostream>
#include <map>
#include <vector>
#include <ossim/vec/ossimVpfTable.h>

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
   std::map<ossimString, ossimVpfFeatureClassSchemaNode, ossimStringLtstr> theFeatureClassMap;
   
   bool validateColumnNames()const;
   void setFeatureClassMapping();
};

#endif
