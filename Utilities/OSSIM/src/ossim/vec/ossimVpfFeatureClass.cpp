#include <algorithm>
using namespace std;

#include <ossim/vec/ossimVpfFeatureClass.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/vec/ossimVpfCoverage.h>
#include <ossim/base/ossimErrorCodes.h>
#include <ossim/vec/ossimVpfTable.h>

#include <string.h> 

#ifndef NULL
#include <stddef.h>
#endif

ossimVpfFeatureClass::ossimVpfFeatureClass()
{
}


bool ossimVpfFeatureClass::openFeatureClass(const ossimString& featureClassName,
                                            const ossimFilename& featureClassFullPath,
                                            const ossimFilename& table1,
                                            const ossimString&   columnNameIdTable1,
                                            const ossimFilename& table2,
                                            const ossimString&   columnNameIdTable2)
{
   theFeatureClassFullPath = featureClassFullPath;
   theFeatureClassName     = featureClassName;
   theTableName1           = table1;
   theColumnNameId1        = columnNameIdTable1;
   theTableName2           = table2;
   theColumnNameId2        = columnNameIdTable2;
   
   ossimVpfTable t1;
   ossimVpfTable t2;

   ossimFilename t1Name = getTable1();
   ossimFilename t2Name = getTable2();

   if((!t1.openTable(t1Name))||
      (!t2.openTable(t2Name)))
     {
       
       return false;
     }
   
   return true;
}

const ossimString& ossimVpfFeatureClass::getName()const
{
  return theFeatureClassName;
  
}

bool ossimVpfFeatureClass::isPrimitive(const ossimString& featureName)
{
   // make sure we have forward slashes
   ossimFilename filename(featureName.trim());   
   filename.convertBackToForwardSlashes();
   
   ossimString copyOfFeature = filename;

   // find
   const char* temp = strrchr(copyOfFeature.c_str(), '/');

   ossimString value;
   if(temp)
   {
      ++temp;
      value = temp;
   }
   else
   {
      
      value = copyOfFeature;
   }

   value = value.downcase();
   return( (value == "end") ||
           (value == "cnd") ||
           (value == "edg") ||
           (value == "fac") ||
           (value == "txt") );
}

bool ossimVpfFeatureClass::isSimpleFeature(const ossimString& featureName)
{
   // find the suffix
   ossimString value = strrchr(featureName.trim().c_str(), '.');

   // the suffix will be null or empty string if not found.
   if(value == "")
   {
      return false;
   }
   // make case insensitive
   value = value.downcase();

   return ( (value == ".pft")||
            (value == ".lft")||
            (value == ".aft")||
            (value == ".tft"));
}

bool ossimVpfFeatureClass::isComplexFeature(const ossimString& featureName)
{
   const char* suffix = strrchr(featureName.trim(), '.');
   
   if (!suffix)
   {
      return false;
   }

   ossimString value = ossimString(suffix).downcase();
   
   return (value == ".cft");
}

bool ossimVpfFeatureClass::isFeature(const ossimString& featureName)
{
   return (isSimpleFeature(featureName) || isComplexFeature(featureName));
}

bool ossimVpfFeatureClass::isJoin(const ossimString& featureName)
{
   ossimString value = strrchr(featureName.c_str(), '.');
   if(value == "")
   {
      return false;
   }
   
   value = value.downcase();
   
   return ( (value == ".cjt")||
            (value == ".pjt")||
            (value == ".ljt")||
            (value == ".ajt")||
            (value == ".tjt"));
}

ossimFilename ossimVpfFeatureClass::getTable1()const
{
  return theFeatureClassFullPath.dirCat(theTableName1);
}

ossimFilename ossimVpfFeatureClass::getTable2()const
{
  return theFeatureClassFullPath.dirCat(theTableName2);
}
