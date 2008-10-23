//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
// Description:
//
//*************************************************************************
// $Id: ossimBaseObjectFactory.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimBaseObjectFactory_HEADER
#define ossimBaseObjectFactory_HEADER
#include <ossim/base/ossimObjectFactory.h>

class OSSIMDLLEXPORT ossimBaseObjectFactory : public ossimObjectFactory
{
public:
   virtual ~ossimBaseObjectFactory(){theInstance = NULL;}
   
   static ossimBaseObjectFactory* instance();
   virtual ossimObject* createObject(const ossimString& typeName)const;
   virtual ossimObject* createObject(const ossimKeywordlist& kwl,
                                     const char* prefix=0)const;
   /*!
    * This should return the type name of all objects in all factories.
    * This is the name used to construct the objects dynamially and this
    * name must be unique.
    */
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const;
   
protected:
   static ossimBaseObjectFactory* theInstance;
   
TYPE_DATA
};
#endif
