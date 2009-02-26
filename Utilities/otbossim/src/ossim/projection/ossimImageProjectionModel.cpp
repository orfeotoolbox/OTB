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
#include <ossim/projection/ossimProjectionFactoryRegistry.h>

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
   if (theProjection)
   {
      delete theProjection;
      theProjection = 0;
   }
}

void ossimImageProjectionModel::initialize(const ossimImageHandler& ih)
{
   // Initialize base.
   ossimImageModel::initialize(ih);
   
   if (theProjection)
   {
      delete theProjection;
      theProjection = 0;
   }

   // cast away constness for  ossimImageHandler::getImageGeometry call.
   ossimImageHandler* iih = const_cast<ossimImageHandler*>(&ih);

   ossimKeywordlist kwl;
   if ( iih->getImageGeometry(kwl, 0) )
   {
      theProjection = ossimProjectionFactoryRegistry::instance()->
         createProjection(kwl);
   }
}

const ossimProjection* ossimImageProjectionModel::getProjection() const
{
   return theProjection;
}
