//**************************************************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Description: Class declaration of ossimImageGeometryFactoryBase. See .h file for class documentation.
//
//**************************************************************************************************
// $Id$
#ifndef ossimImageGeometryFactoryBase_HEADER
#define ossimImageGeometryFactoryBase_HEADER
#include <ossim/base/ossimBaseObjectFactory.h>
#include <ossim/imaging/ossimImageGeometry.h>

class ossimImageHandler;
class OSSIM_DLL ossimImageGeometryFactoryBase : public ossimBaseObjectFactory
{
public: 
   /**
    * Creates an object given a type name.
    */
   virtual ossimObject* createObject(const ossimString& typeName)const
   {
      return createGeometry(typeName);
   }
   
   /**
    * Creates and object given a keyword list.
    */
   virtual ossimObject* createObject(const ossimKeywordlist& kwl,
                                     const char* prefix=0)const
   {
      return createGeometry(kwl, prefix);
   }
   
   virtual ossimImageGeometry* createGeometry(const ossimString& typeName)const=0;
   virtual ossimImageGeometry* createGeometry(const ossimKeywordlist& kwl,
                                              const char* prefix=0)const=0;
   virtual ossimImageGeometry* createGeometry(const ossimFilename& filename,
                                              ossim_uint32 entryIdx)const = 0;
   virtual bool extendGeometry(ossimImageHandler* handler)const=0;
};
#endif
