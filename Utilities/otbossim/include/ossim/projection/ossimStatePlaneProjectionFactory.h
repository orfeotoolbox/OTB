//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//*******************************************************************
//  $Id: ossimStatePlaneProjectionFactory.h 12081 2007-11-26 21:44:18Z dburken $
#ifndef ossimStatePlaneProjectionFactory_HEADER
#define ossimStatePlaneProjectionFactory_HEADER
#include <ossim/projection/ossimProjectionFactoryBase.h>
class ossimStatePlaneProjectionInfo;

class OSSIMDLLEXPORT ossimStatePlaneProjectionFactory : public ossimProjectionFactoryBase
{
public:
   static ossimStatePlaneProjectionFactory* instance();
   virtual ~ossimStatePlaneProjectionFactory();
   virtual ossimProjection* createProjection(const ossimFilename& filename,
                                             ossim_uint32 entryIdx)const;
   virtual ossimProjection* createProjection(const ossimString& name)const;
   virtual ossimProjection* createProjection(const ossimKeywordlist& kwl,
                                             const char* prefix=0)const;
   const ossimStatePlaneProjectionInfo* getInfo(ossim_int32 pcsCode)const;
   const ossimStatePlaneProjectionInfo* getInfo(const ossimString& name)const;
   virtual ossimObject* createObject(const ossimString& typeName)const;

   /*!
    * Creates and object given a keyword list.
    */
   virtual ossimObject* createObject(const ossimKeywordlist& kwl,
                                     const char* prefix=0)const;
   
   /*!
    * This should return the type name of all objects in all factories.
    * This is the name used to construct the objects dynamially and this
    * name must be unique.
    */
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const;
   
   bool  addStatePlaneInfo(ossimStatePlaneProjectionInfo* info);
protected:
   ossimStatePlaneProjectionFactory();
   ossimStatePlaneProjectionFactory(const ossimStatePlaneProjectionFactory&);
   void operator =(const ossimStatePlaneProjectionFactory& rhs);
   
   static ossimStatePlaneProjectionFactory* theInstance;

   std::map<ossim_int32, ossimStatePlaneProjectionInfo*> theStatePlaneInfoMap;
   std::map<ossimString, ossimStatePlaneProjectionInfo*> theStatePlaneInfoNameMap;

   void initializeMap();
};
#endif
