//-----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: Class definition of ossimImageProjectionModel.
//
//-----------------------------------------------------------------------------
// $Id$

#include <string>

#include <ossim/projection/ossimImageProjectionModel.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/projection/ossimProjection.h>

RTTI_DEF1(ossimImageProjectionModel,
          "ossimImageProjectionModel",
          ossimImageModel)

ossimImageProjectionModel::ossimImageProjectionModel()
   : ossimImageModel(),
     theProjection(0)
{
}

ossimImageProjectionModel::~ossimImageProjectionModel()
{
   theProjection = 0;
}

void ossimImageProjectionModel::initialize(const ossimImageHandler& ih)
{
   // Initialize base.
   ossimImageModel::initialize(ih);
   
   theProjection = 0;

   // Fetch the handler's image geometry and see if it has a projection:
   ossimImageHandler* iih = const_cast<ossimImageHandler*>(&ih); // cast away constness
   ossimRefPtr<ossimImageGeometry> image_geom = iih->getImageGeometry();
   if ( image_geom.valid() )
   {
      theProjection = image_geom->getProjection();
   }
}

const ossimProjection* ossimImageProjectionModel::getProjection() const
{
   return theProjection;
}
