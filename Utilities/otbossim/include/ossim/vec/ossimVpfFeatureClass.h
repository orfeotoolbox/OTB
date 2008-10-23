#ifndef ossimVpfFeatureClass_HEADER
#define ossimVpfFeatureClass_HEADER
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimErrorContext.h>

class ossimVpfCoverage;

class ossimVpfFeatureClass
{
public:
   ossimVpfFeatureClass();

   /*!
    * Will initialize the feature class for this
    * coverage and do some validation.
    */
   bool openFeatureClass(const ossimString& featureClassName,
                         const ossimFilename& featureClassFullPath,
                         const ossimFilename& table1,
                         const ossimString&   columnNameIdTable1,
                         const ossimFilename& table2,
                         const ossimString&   columnNameIdTable2);

  const ossimString& getName()const;
  
  /*!
   * This will specify if this feature is a primitive.
   * the names of certain primitives are:
   *
   * end   Entity node primitive
   * cnd   Connected node primitive
   * edg   Edge Primitive
   * fac   Face Primitive
   * txt   Text primitive
   */
  static bool isPrimitive(const ossimString& featureName);
  
   /*!
    * This just specifies if the input is a simple feature.
    *
    * Some of the simple features are:
    *
    * .pft  Point feature table
    * .lft  Line feature table
    * .aft  area feature table
    * .tft  text feature table
    */
   static bool isSimpleFeature(const ossimString& featureName);

   /*!
    * This just specifies if the input is a comples feature.
    *
    * .cft  Complex feature table.
    * 
    */
   static bool isComplexFeature(const ossimString& featureName);

   /*!
    * Will use the isComplexFeature and isSimpleFeature to
    * determine if the input is a feature
    */
   static bool isFeature(const ossimString& featureName);

   /*!
    * Will specifiy if it is a join table.
    *
    * Some of the join table type are:
    *
    * .cjt  Complex join table
    * .pjt  Point join table
    * .ljt  Line join table
    * .ajt  area join table
    * .tjt  text join table
    */
   static bool isJoin(const ossimString& featureName);

  ossimFilename getTable1()const;
  ossimFilename getTable2()const;

private:

  ossimFilename     theFeatureClassFullPath;
  ossimString       theFeatureClassName;
  ossimFilename     theTableName1;
  ossimString       theColumnNameId1;
  ossimFilename     theTableName2;
  ossimString       theColumnNameId2;

};

#endif
