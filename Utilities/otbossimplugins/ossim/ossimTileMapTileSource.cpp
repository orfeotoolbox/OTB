//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Jordi Inglada
//
// Description:
//
// Contains class implementaiton for the class "ossim TileMapTileSource".
//
//*******************************************************************
//  $Id: ossimTileMapTileSource.cpp 10752 2007-04-23 16:50:08Z dburken $

#include "ossimTileMapTileSource.h"

#include "ossimTileMapModel.h"

#include <ossim/base/ossimDirectory.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/imaging/ossimImageGeometryRegistry.h>


static ossimTrace traceDebug("ossimTileMapTileSource:debug");

namespace ossimplugins
{
RTTI_DEF1_INST(ossimTileMapTileSource,
               "ossimTileMapTileSource",
               ossimGeneralRasterTileSource)

//*******************************************************************
// Public Constructor:
//*******************************************************************
ossimTileMapTileSource::ossimTileMapTileSource()
   :
      ossimGeneralRasterTileSource()
{
}

//*******************************************************************
// Public Constructor:
//*******************************************************************
ossimTileMapTileSource::ossimTileMapTileSource(const ossimKeywordlist& kwl,
                                               const char* prefix)
   :
      ossimGeneralRasterTileSource()
{
   if (loadState(kwl, prefix) == false)
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
   }
}

//*******************************************************************
// Destructor:
//*******************************************************************
ossimTileMapTileSource::~ossimTileMapTileSource()
{
//   theFfHdr = NULL;
}

bool ossimTileMapTileSource::open()
{
   static const char MODULE[] = "ossimTileMapTileSource::open";

  ossimString os = theImageFile.beforePos(4);
  
   if (traceDebug())
   {
      CLOG << " Entered..." << std::endl
           << " trying to open file " << theImageFile << std::endl;
   }
   if(os == "http" || theImageFile.ext() == "otb")
   {
     return true;
   }
  
   return false;
}

ossimRefPtr<ossimImageGeometry> ossimTileMapTileSource::getImageGeometry()
{
  if ( !theGeometry )
  {
     // Check for external geom:
     theGeometry = getExternalImageGeometry();

     if ( !theGeometry )
     {
        theGeometry = new ossimImageGeometry();

        // Make a model
        ossimTileMapModel* model = new ossimTileMapModel ();
        if (model->getErrorStatus() == ossimErrorCodes::OSSIM_OK)
        {
           //initialize the image geometry object with the model
           theGeometry->setProjection(model);
        }

        //---
        // WARNING:
        // Must create/set the geometry at this point or the next call to
        // ossimImageGeometryRegistry::extendGeometry will put us in an infinite loop
        // as it does a recursive call back to ossimImageHandler::getImageGeometry().
        //---

        // Check for set projection.
        if ( !theGeometry->getProjection() )
        {
           // Try factories for projection.
           ossimImageGeometryRegistry::instance()->extendGeometry(this);
        }
     }

     // Set image things the geometry object should know about.
     initImageParameters( theGeometry.get() );
  }

  return theGeometry;
}

bool ossimTileMapTileSource::loadState(const ossimKeywordlist& kwl,
                                       const char* prefix)
{
   //const char* lookup = kwl.find(prefix, ossimKeywordNames::FILENAME_KW);
   return true;

}


ossimString ossimTileMapTileSource::getShortName() const
{
   return ossimString("TileMap");
}

ossimString ossimTileMapTileSource::getLongName() const
{
   return ossimString("TileMap reader");
}

ossimString  ossimTileMapTileSource::className() const
{
   return ossimString("ossimTileMapTileSource");
}

} // End: namespace ossimplugins
