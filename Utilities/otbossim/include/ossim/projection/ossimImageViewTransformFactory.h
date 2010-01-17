//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//
//*******************************************************************
//  $Id: ossimImageViewTransformFactory.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimImageViewTransformFactory_HEADER
#define ossimImageViewTransformFactory_HEADER
#include <ossim/base/ossimObject.h>

class ossimImageViewTransform;
class ossimKeywordlist;

class ossimImageViewTransformFactory : public ossimObject
{
public:
   static ossimImageViewTransformFactory* instance();
   virtual ossimImageViewTransform* createTransform(const ossimKeywordlist& kwl,
                                                    const char* prefix = 0);
   
protected:
   ossimImageViewTransformFactory(){}//hide
   ossimImageViewTransformFactory(const ossimImageViewTransformFactory&){}
   void operator =(const ossimImageViewTransformFactory&){}

   static ossimImageViewTransformFactory* theInstance;

TYPE_DATA
};

#endif
