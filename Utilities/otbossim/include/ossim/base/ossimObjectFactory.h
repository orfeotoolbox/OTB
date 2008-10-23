//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts (gpotts@imagelinks)
// Description:
//
//*************************************************************************
// $Id: ossimObjectFactory.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimObjectFactory_HEADER
#define ossimObjectFactory_HEADER

#include <vector>
#include <ossim/base/ossimObject.h>

class OSSIMDLLEXPORT ossimObjectFactory : public ossimObject
{
public:
   virtual ~ossimObjectFactory(){}

   /*!
    * Creates an object given a type name.
    */
   virtual ossimObject* createObject(const ossimString& typeName)const=0;

   /*!
    * Creates and object given a keyword list.
    */
   virtual ossimObject* createObject(const ossimKeywordlist& kwl,
                                     const char* prefix=0)const=0;
   
   /*!
    * This should return the type name of all objects in all factories.
    * This is the name used to construct the objects dynamially and this
    * name must be unique.
    */
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const=0;
   
protected:


TYPE_DATA
};

#endif
