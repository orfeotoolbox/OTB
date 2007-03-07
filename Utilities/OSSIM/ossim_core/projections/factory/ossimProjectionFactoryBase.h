//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//*******************************************************************
//  $Id: ossimProjectionFactoryBase.h,v 1.9 2005/10/28 14:01:03 gpotts Exp $
#ifndef ossimProjectionFactoryBase_HEADER
#define ossimProjectionFactoryBase_HEADER

#include <list>
#include <base/data_types/ossimString.h>
#include <base/data_types/ossimKeywordlist.h>
#include <base/factory/ossimObjectFactory.h>

class ossimProjection;

class OSSIM_DLL ossimProjectionFactoryBase : public ossimObjectFactory
{
public:
   
   /**
    * takes a filename. This filename can be an image file or
    * it can also be a ossim .geom file.  It could be other
    * forms of geom files as well.  The factories job will be to
    * determine what parser to use and return a projection if
    * successful.
    */
   virtual ossimProjection* createProjection(const ossimFilename& filename,
                                             ossim_uint32 entryIdx)const=0;

   /**
    * Take a projection type name.
    */
   virtual ossimProjection* createProjection(const ossimString& name)const=0;

   /**
    * Take a keywordlist.
    */
   virtual ossimProjection* createProjection(const ossimKeywordlist& kwl,
                                             const char* prefix)const=0;

   /**
    * Being depricated with getTypeNameList found in ossimObjectFactory
    */
   virtual std::list<ossimString> getList()const=0;

protected:

   /**
    * This method takes the filename and tries to find an external ".geom"
    * file.
    *
    * If the "entryIndx" is 0 and the "imageFile" is foo.ntf this will
    * look for "foo.geom".  If that fails it will look for foo_e0.geom.
    *
    * If the "entryIndx" is 5 and the "imageFile" is foo.ntf this will
    * look for foo_e5.geom.
    *
    * @param imageFile Image file that needs a projection.
    *
    * @param entryIdx For image files that support multiple images this is
    * the zero base index to that image.  Images that don't support multiple
    * indexes should just pass 0.
    *
    * @return An ossimProjection* on success or NULL on failure.
    */
   ossimProjection*
   createProjectionFromGeometryFile(const ossimFilename& imageFile,
                                    ossim_uint32 entryIdx)const;
   
TYPE_DATA
};

#endif
