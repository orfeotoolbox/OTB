//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimOrthoImageMosaic.cpp 21631 2012-09-06 18:10:55Z dburken $
#include <ossim/imaging/ossimOrthoImageMosaic.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageGeometry.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>

static ossimTrace traceDebug ("ossimOrthoImageMosaic:debug");

RTTI_DEF1(ossimOrthoImageMosaic, "ossimOrthoImageMosaic", ossimImageMosaic);

//**************************************************************************************************
// 
//**************************************************************************************************
ossimOrthoImageMosaic::ossimOrthoImageMosaic()
   :ossimImageMosaic()
{
   m_Delta.makeNan();
   m_UpperLeftTie.makeNan();
}

//**************************************************************************************************
// 
//**************************************************************************************************
ossimOrthoImageMosaic::ossimOrthoImageMosaic(ossimConnectableObject::ConnectableObjectList& inputSources)
   :ossimImageMosaic(inputSources)
{
   m_Delta.makeNan();
   m_UpperLeftTie.makeNan();
}

//**************************************************************************************************
// 
//**************************************************************************************************
ossimOrthoImageMosaic::~ossimOrthoImageMosaic()
{
}

//**************************************************************************************************
// 
//**************************************************************************************************
ossim_uint32 ossimOrthoImageMosaic::getNumberOfOverlappingImages(const ossimIrect& rect,
                                                                 ossim_uint32 resLevel)const
{
   ossim_uint32 result = 0;
   for(ossim_uint32 i = 0; i < m_InputTiePoints.size(); ++i)
   {
      ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                      getInput(i));
      if(interface&&
         !m_InputTiePoints[i].hasNans())
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

//**************************************************************************************************
// 
//**************************************************************************************************
void ossimOrthoImageMosaic::getOverlappingImages(std::vector<ossim_uint32>& result,
                                                 const ossimIrect& rect,
                                                 ossim_uint32 resLevel)const
{
   result.clear();
   for(ossim_uint32 i = 0; i < m_InputTiePoints.size(); ++i)
   {
      ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                      getInput(i));
      if(interface&&
         !m_InputTiePoints[i].hasNans())
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

//**************************************************************************************************
// Returns the image geometry for the complete mosaic
//**************************************************************************************************
ossimRefPtr<ossimImageGeometry> ossimOrthoImageMosaic::getImageGeometry()
{
   if ( !m_Geometry.valid() )
   {
      // The geometry (projection) associated with this mosaic is necessarily the same for all
      // single-image objects feeding into this combiner, So we will copy the first image source's
      // geometry, and modify our copy to reflect the mosaic-specific items.
      ossimImageSource* interface = PTR_CAST(ossimImageSource, getInput(0));
      if(interface)
      {
         ossimRefPtr<ossimImageGeometry> inputGeom = interface->getImageGeometry();
         if ( inputGeom.valid() )
         {
            m_Geometry = new ossimImageGeometry(*inputGeom);
            updateGeometry();
         }
      }
   }
   return m_Geometry;
}

//**************************************************************************************************
//! If this object is maintaining an ossimImageGeometry, this method needs to be called after 
//! each time the contents of the mosaic changes.
//**************************************************************************************************
void ossimOrthoImageMosaic::updateGeometry()
{
   if (!m_Geometry.valid())
      return;

   ossimMapProjection* mapProj = PTR_CAST(ossimMapProjection, m_Geometry->getProjection());
   if (mapProj)
   {
      if(m_Units == "degrees")
      {
         mapProj->setDecimalDegreesPerPixel(m_Delta);
         mapProj->setUlGpt(ossimGpt(m_UpperLeftTie.y, m_UpperLeftTie.x, 0.0,
                                    mapProj->origin().datum()));
      }
      else if(m_Units == "meters")
      {
         mapProj->setMetersPerPixel(m_Delta);
         mapProj->setUlEastingNorthing(m_UpperLeftTie);
      }
   }
}

//**************************************************************************************************
// 
//**************************************************************************************************
ossimIrect ossimOrthoImageMosaic::getBoundingRect(ossim_uint32 resLevel) const
{
   ossimDpt decimation;

   decimation.makeNan();

   ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                   getInput(0));
   if(interface)
   {
      interface->getDecimationFactor(resLevel,
                                     decimation);
   }
   if(decimation.hasNans())
   {
      return m_BoundingRect;
   }
   return m_BoundingRect*decimation;
}

//**************************************************************************************************
// 
//**************************************************************************************************
void ossimOrthoImageMosaic::initialize()
{
   m_InputTiePoints.clear();
   m_Delta.makeNan();
   m_UpperLeftTie.makeNan();

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimOrthoImageMosaic::initialize() DEBUG: Entered..."
         << "Number of inputs = " << getNumberOfInputs()
         << std::endl;
   }
   if(getNumberOfInputs())
   {
      m_InputTiePoints.resize(getNumberOfInputs());
      for(ossim_uint32 i = 0; i < getNumberOfInputs(); ++i)
      {
         ossimImageSource *interface = PTR_CAST(ossimImageSource, getInput(i));
         m_InputTiePoints[i].makeNan();
         if(interface)
         {
            ossimRefPtr<ossimImageGeometry> geom = interface->getImageGeometry();
            if( geom.valid() )
            {
               const ossimMapProjection* mapPrj = PTR_CAST(ossimMapProjection, geom->getProjection());
               if(mapPrj)
               {
                  if(!mapPrj->isGeographic())
                  {
                     m_Units = "meters";
                     m_Delta = mapPrj->getMetersPerPixel();
                  }
                  else
                  {
                     m_Units = "degrees";
                     m_Delta = mapPrj->getDecimalDegreesPerPixel();
                  }
                  if(m_Units == "degrees")
                  {
                     m_InputTiePoints[i].x = mapPrj->getUlGpt().lond();
                     m_InputTiePoints[i].y = mapPrj->getUlGpt().latd();
                  }
                  else if(m_Units == "meters")
                  {
                     m_InputTiePoints[i].x = mapPrj->getUlEastingNorthing().x;;
                     m_InputTiePoints[i].y = mapPrj->getUlEastingNorthing().y;
                  }

                  if(traceDebug())
                  {
                     ossimNotify(ossimNotifyLevel_DEBUG)
                        << "tie points for input " << i << " = "
                        << m_InputTiePoints[i] << std::endl;
                  }

               }
            }
            if(!m_InputTiePoints[i].hasNans())
            {
               if(m_UpperLeftTie.hasNans())
               {
                  m_UpperLeftTie = m_InputTiePoints[i];
               }
               else if(!m_InputTiePoints[i].hasNans())
               {
                  if(m_Units == "meters")
                  {
                     m_UpperLeftTie.x = std::min(m_UpperLeftTie.x, m_InputTiePoints[i].x);
                     m_UpperLeftTie.y = std::max(m_UpperLeftTie.y, m_InputTiePoints[i].y);
                  }
                  else
                  {
                     m_UpperLeftTie.lon = std::min(m_UpperLeftTie.lon, m_InputTiePoints[i].lon);
                     m_UpperLeftTie.lat = std::max(m_UpperLeftTie.lat, m_InputTiePoints[i].lat);
                  }
               }
            }
         }
         else
         {
            m_InputTiePoints[i].makeNan();
//            CLOG << "Input " << i << " will not be used since no ortho information exists" << endl;
         }
      }
   }

   computeBoundingRect();
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "m_UpperLeftTie = " << m_UpperLeftTie << std::endl
         << "delta per pixel = " << m_Delta        << std::endl
         << "bounding rect   = " << m_BoundingRect << std::endl;
   }
   ossimImageMosaic::initialize();

   // Finally, update the geometry (if there was one already defined), to reflect the change in input
   updateGeometry();

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimOrthoImageMosaic::initialize() DEBUG: Leaving..." << std::endl;
   }
}

//**************************************************************************************************
// 
//**************************************************************************************************
ossimRefPtr<ossimImageData> ossimOrthoImageMosaic::getNextTile(ossim_uint32& returnedIdx,
                                                               const ossimIrect& origin,
                                                               ossim_uint32 resLevel)
{
   const char *MODULE="ossimOrthoImageMosaic::getNextTile";
   
   ossim_uint32 size = getNumberOfInputs();
   
   if(theCurrentIndex >= size)
   {
      return ossimRefPtr<ossimImageData>();
   }
   
   ossimImageSource* temp = NULL;
   ossimRefPtr<ossimImageData> result;
   ossimDataObjectStatus status = OSSIM_NULL;
   
   do
   {
      temp = PTR_CAST(ossimImageSource, getInput(theCurrentIndex));
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

      // Go to next source.
      ++theCurrentIndex;
      
   } while(!result.valid() && (theCurrentIndex<size));

   returnedIdx = theCurrentIndex;
   if(result.valid())
   {
      --returnedIdx;
   }
   return result;
}

//**************************************************************************************************
// 
//**************************************************************************************************
void ossimOrthoImageMosaic::computeBoundingRect(ossim_uint32 resLevel)
{
   const char* MODULE = "ossimOrthoImageMosaic::computeBoundingRect";
   
   m_BoundingRect.makeNan();

   for(ossim_uint32 i = 0; i < m_InputTiePoints.size(); ++ i)
   {
      ossimIrect shiftedRect = getRelativeRect(i, resLevel);

      if(traceDebug())
      {
         CLOG << "bounding rect for input " << (i+1)
              << ":" << shiftedRect << endl;
      }
      if(!shiftedRect.hasNans())
      {
         if(m_BoundingRect.hasNans())
         {
            m_BoundingRect = shiftedRect;
         }
         else
         {
            m_BoundingRect = m_BoundingRect.combine(shiftedRect);
         }
      }
   }
}

//**************************************************************************************************
// 
//**************************************************************************************************
ossimIrect ossimOrthoImageMosaic::getRelativeRect(ossim_uint32 index,
                                                  ossim_uint32 resLevel)const
{
   ossimIrect result;
   ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                   getInput(index));
   result.makeNan();
   if(interface&&
      !m_InputTiePoints[index].hasNans())
   {
      ossimIrect inputRect = interface->getBoundingRect();
      result = inputRect;
      
      if(!inputRect.hasNans())
      {
         ossimDpt shift = (m_InputTiePoints[index] - m_UpperLeftTie);
         
         shift.x/= m_Delta.x;
         shift.y/=-m_Delta.y;
         
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
