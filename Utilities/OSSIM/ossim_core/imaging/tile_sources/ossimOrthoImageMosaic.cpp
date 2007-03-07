//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimOrthoImageMosaic.cpp,v 1.26 2005/05/12 01:19:54 dburken Exp $
#include <imaging/tile_sources/ossimOrthoImageMosaic.h>
#include <base/common/ossimKeywordNames.h>
#include <base/common/ossimTrace.h>
#include <projections/map_projections/ossimMapProjection.h>
#include <projections/factory/ossimProjectionFactoryRegistry.h>


static ossimTrace traceDebug ("ossimOrthoImageMosaic:debug");

RTTI_DEF1(ossimOrthoImageMosaic, "ossimOrthoImageMosaic", ossimImageMosaic);

ossimOrthoImageMosaic::ossimOrthoImageMosaic()
   :ossimImageMosaic()
{
   theDelta.makeNan();
   theUpperLeftTie.makeNan();
}

ossimOrthoImageMosaic::ossimOrthoImageMosaic(const std::vector<ossimImageSource*>& inputSources)
   :ossimImageMosaic(inputSources)
{
   theDelta.makeNan();
   theUpperLeftTie.makeNan();
}

ossimOrthoImageMosaic::~ossimOrthoImageMosaic()
{
}

ossim_uint32 ossimOrthoImageMosaic::getNumberOfOverlappingImages(const ossimIrect& rect,
                                                           ossim_uint32 resLevel)const
{
   ossim_uint32 result = 0;
   for(ossim_uint32 i = 0; i < theInputTiePoints.size(); ++i)
   {
      ossimImageSourceInterface* interface = PTR_CAST(ossimImageSourceInterface,
                                                      getInput(i));
      if(interface&&
         !theInputTiePoints[i].hasNans())
      {
         ossimIrect tempRect = getRelativeRect(i, resLevel);

         if(!tempRect.hasNans())
         {
            if(tempRect.intersects(rect))
            {
               ++result;
            }
         }
      }
   }
   
   return 0;
}

void ossimOrthoImageMosaic::getOverlappingImages(std::vector<ossim_uint32>& result,
                                                 const ossimIrect& rect,
                                                 ossim_uint32 resLevel)const
{
   result.clear();
   for(ossim_uint32 i = 0; i < theInputTiePoints.size(); ++i)
   {
      ossimImageSourceInterface* interface = PTR_CAST(ossimImageSourceInterface,
                                                      getInput(i));
      if(interface&&
         !theInputTiePoints[i].hasNans())
      {
         ossimIrect tempRect = getRelativeRect(i, resLevel);

         if(!tempRect.hasNans())
         {
            if(tempRect.intersects(rect))
            {
               result.push_back(i);
            }
         }
      }
   }
}

bool ossimOrthoImageMosaic::getImageGeometry(ossimKeywordlist& kwl,
                                             const char* prefix)
{
   bool result = true;
   ossimImageSourceInterface* interface = PTR_CAST(ossimImageSourceInterface,
                                                   getInput(0));
   if(interface)
   {
      interface->getImageGeometry(kwl, prefix);
      if(theUnits == "degrees")
      {
          kwl.add(prefix,
                 ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LAT,
                 theDelta.y,
                 true);
          kwl.add(prefix,
                 ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LON,
                 theDelta.x,
                  true);
          kwl.add(prefix,
                  ossimKeywordNames::TIE_POINT_LAT_KW,
                  theUpperLeftTie.y,
                  true);
          kwl.add(prefix,
                  ossimKeywordNames::TIE_POINT_LON_KW,
                  theUpperLeftTie.x,
                  true);
          kwl.remove(prefix, ossimKeywordNames::TIE_POINT_EASTING_KW);
          kwl.remove(prefix, ossimKeywordNames::TIE_POINT_NORTHING_KW);
                 
      }
      else if(theUnits == "meters")
      {
          kwl.add(prefix,
                 ossimKeywordNames::METERS_PER_PIXEL_X_KW,
                 theDelta.x,
                 true);
          kwl.add(prefix,
                 ossimKeywordNames::METERS_PER_PIXEL_Y_KW,
                 theDelta.x,
                  true);
          kwl.add(prefix,
                  ossimKeywordNames::TIE_POINT_EASTING_KW,
                  theUpperLeftTie.x,
                  true);
          kwl.add(prefix,
                  ossimKeywordNames::TIE_POINT_NORTHING_KW,
                  theUpperLeftTie.y,
                  true);
          kwl.remove(prefix, ossimKeywordNames::TIE_POINT_LAT_KW);
          kwl.remove(prefix, ossimKeywordNames::TIE_POINT_LON_KW);
      }
      else
      {
         result = false;
      }
   }

   return result;
}

ossimIrect ossimOrthoImageMosaic::getBoundingRect(ossim_uint32 resLevel) const
{
   ossimDpt decimation;

   decimation.makeNan();

   ossimImageSourceInterface* interface = PTR_CAST(ossimImageSourceInterface,
                                                   getInput(0));
   if(interface)
   {
      interface->getDecimationFactor(resLevel,
                                     decimation);
   }
   if(decimation.hasNans())
   {
      return theBoundingRect;
   }
   return theBoundingRect*decimation;
}

void ossimOrthoImageMosaic::initialize()
{
   theInputTiePoints.clear();
   theDelta.makeNan();
   theUpperLeftTie.makeNan();

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimOrthoImageMosaic::initialize() DEBUG: Entered..."
         << "Number of inputs = " << getNumberOfInputs()
         << std::endl;
   }
   if(getNumberOfInputs())
   {
      theInputTiePoints.resize(getNumberOfInputs());
      for(ossim_uint32 i = 0; i < getNumberOfInputs(); ++i)
      {
         ossimImageSourceInterface *interface =
            PTR_CAST(ossimImageSourceInterface, getInput(i));
         theInputTiePoints[i].makeNan();
         if(interface)
         {
            ossimKeywordlist kwl;

            bool status = interface->getImageGeometry(kwl);
            if(status)
            {
               ossimProjection* prj = ossimProjectionFactoryRegistry::instance()->createProjection(kwl);
               ossimMapProjection* mapPrj = PTR_CAST(ossimMapProjection, prj);
               if(mapPrj)
               {
                  if(!mapPrj->isGeographic())
                  {
                     theUnits = "meters";
                     theDelta = mapPrj->getMetersPerPixel();
                  }
                  else
                  {
                     theUnits = "degrees";
                     theDelta = mapPrj->getDecimalDegreesPerPixel();
                  }
                  if(theUnits == "degrees")
                  {
                     theInputTiePoints[i].x = mapPrj->getUlGpt().lond();
                     theInputTiePoints[i].y = mapPrj->getUlGpt().latd();
                  }
                  else if(theUnits == "meters")
                  {
                     theInputTiePoints[i].x = mapPrj->getUlEastingNorthing().x;;
                     theInputTiePoints[i].y = mapPrj->getUlEastingNorthing().y;
                  }

                  if(traceDebug())
                  {
                     ossimNotify(ossimNotifyLevel_DEBUG)
                        << "tie points for input " << i << " = "
                        << theInputTiePoints[i] << std::endl;
                  }

               }
               if(prj)
               {
                  delete prj;
                  prj = NULL;
                  mapPrj = NULL;
               }
            }
            if(!theInputTiePoints[i].hasNans())
            {
               if(theUpperLeftTie.hasNans())
               {
                  theUpperLeftTie = theInputTiePoints[i];
               }
               else if(!theInputTiePoints[i].hasNans())
               {
                  if(theUnits == "meters")
                  {
                     theUpperLeftTie.x = std::min(theUpperLeftTie.x, theInputTiePoints[i].x);
                     theUpperLeftTie.y = std::max(theUpperLeftTie.y, theInputTiePoints[i].y);
                  }
                  else
                  {
                     theUpperLeftTie.lon = std::min(theUpperLeftTie.lon, theInputTiePoints[i].lon);
                     theUpperLeftTie.lat = std::max(theUpperLeftTie.lat, theInputTiePoints[i].lat);
                  }
               }
            }
         }
         else
         {
            theInputTiePoints[i].makeNan();
//            CLOG << "Input " << i << " will not be used since no ortho information exists" << endl;
         }
      }
   }

   computeBoundingRect();
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "theUpperLeftTie = " << theUpperLeftTie << std::endl
         << "delta per pixel = " << theDelta        << std::endl
         << "bounding rect   = " << theBoundingRect << std::endl;
   }
   ossimImageMosaic::initialize();
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimOrthoImageMosaic::initialize() DEBUG: Leaving..." << std::endl;
   }
}

ossimRefPtr<ossimImageData> ossimOrthoImageMosaic::getNextTile(
   const ossimIrect& origin,
   ossim_uint32 resLevel)
{
   const char *MODULE="ossimOrthoImageMosaic::getNextTile";
   
   ossim_int32 size = getNumberOfInputs();
   
   if((theCurrentIndex+1) >= size)
   {
      return ossimRefPtr<ossimImageData>();
   }
   
   ossimImageSourceInterface* temp = NULL;
   ossimRefPtr<ossimImageData> result;
   ossimDataObjectStatus status = OSSIM_NULL;
   
   do
   {
      ++theCurrentIndex;
      temp = PTR_CAST(ossimImageSourceInterface, getInput(theCurrentIndex));
      if(temp)
      {
         ossimIrect relRect = getRelativeRect(theCurrentIndex, resLevel);

         if(traceDebug())
         {
            CLOG << "Rel rect for input " << theCurrentIndex
                 << " = " << relRect
                 << "\nRes level:  " << resLevel
                 << endl;
         }

         if(origin.intersects(relRect))
         {
            // get the rect relative to the input rect
            //
            ossimIrect shiftedRect = origin + (ossimIpt(-relRect.ul().x,
                                                        -relRect.ul().y));

            // request that tile from the input space.
            result = temp->getTile(shiftedRect, resLevel);

            // now change the origin to the output origin.
            if (result.valid())
            {
               result->setOrigin(origin.ul());
               
               status = result->getDataObjectStatus();

               if((status == OSSIM_NULL)||(status == OSSIM_EMPTY))
               {
                  result = NULL;
               }
            }
         }
         else
         {
            result = NULL;
         }
      }
      
   } while(!result.valid() && (theCurrentIndex<size));

   return result;
}

void ossimOrthoImageMosaic::computeBoundingRect(ossim_uint32 resLevel)
{
   const char* MODULE = "ossimOrthoImageMosaic::computeBoundingRect";
   
   theBoundingRect.makeNan();

   for(ossim_uint32 i = 0; i < theInputTiePoints.size(); ++ i)
   {
      ossimIrect shiftedRect = getRelativeRect(i, resLevel);

      if(traceDebug())
      {
         CLOG << "bounding rect for input " << (i+1)
              << ":" << shiftedRect << endl;
      }
      if(!shiftedRect.hasNans())
      {
         if(theBoundingRect.hasNans())
         {
            theBoundingRect = shiftedRect;
         }
         else
         {
            theBoundingRect = theBoundingRect.combine(shiftedRect);
         }
      }
   }
}

ossimIrect ossimOrthoImageMosaic::getRelativeRect(ossim_uint32 index,
                                                  ossim_uint32 resLevel)const
{
   ossimIrect result;
   ossimImageSourceInterface* interface = PTR_CAST(ossimImageSourceInterface,
                                                   getInput(index));
   result.makeNan();
   if(interface&&
      !theInputTiePoints[index].hasNans())
   {
      ossimIrect inputRect = interface->getBoundingRect();
      result = inputRect;
      
      if(!inputRect.hasNans())
      {
         ossimDpt shift = (theInputTiePoints[index] - theUpperLeftTie);
         
         shift.x/= theDelta.x;
         shift.y/=-theDelta.y;
         
	 result = result + shift;
         if(!resLevel)
         {
            return result;
         }
 	 ossimDpt decimation;
 	 interface->getDecimationFactor(resLevel, decimation);
	 if(!decimation.hasNans())
         {
            result = result * decimation;
         }
      }
   }
   
   return result;
}
