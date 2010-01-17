//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts 
//
//*************************************************************************
// $Id: ossimImageSourceFactoryBase.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimImageSourceFactoryBase_HEADER
#define ossimImageSourceFactoryBase_HEADER
#include <ossim/base/ossimObjectFactory.h>
class ossimImageSource;

class OSSIMDLLEXPORT ossimImageSourceFactoryBase : public ossimObjectFactory
{
public:
   /*!
    * Convenient conversion method.  Gurantees an ossimImageSource is returned.  Returns
    * NULL otherwise
    */
   virtual ossimImageSource* createImageSource(const ossimString& name)const;
   virtual ossimImageSource* createImageSource(const ossimKeywordlist& kwl,
                                               const char* prefix=0)const;
   
protected:
   // Hide from use.
   ossimImageSourceFactoryBase();
   ossimImageSourceFactoryBase(const ossimImageSourceFactoryBase&);
   const ossimImageSourceFactoryBase& operator=(const ossimImageSourceFactoryBase&);
};

#endif
