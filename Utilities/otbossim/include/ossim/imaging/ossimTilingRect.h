//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Mingjie Su
// 
// Description: implementation for image generator
//
//*************************************************************************
// $Id: ossimTilingRect.h 2725 2011-06-15 18:13:07Z david.burken $
#ifndef ossimTilingRect_HEADER
#define ossimTilingRect_HEADER 1

#include <ossim/imaging/ossimTiling.h>
#include <ossim/imaging/ossimGeoPolyCutter.h>
#include <ossim/imaging/ossimGeoAnnotationPolyObject.h>
#include <ossim/imaging/ossimGeoAnnotationMultiPolyObject.h>


class OSSIM_DLL ossimTilingRect : public ossimTiling
{
public:

   ossimTilingRect();

   virtual ~ossimTilingRect();

   virtual bool initialize(const ossimMapProjection& proj,
                           const ossimIrect& boundingRect);

   virtual bool next(ossimRefPtr<ossimMapProjection>& resultProjection,
                     ossimIrect& resultingBounds,
                     ossimString& resultingName);

   virtual bool saveState(ossimKeywordlist& kwl,
      const char* prefix=0)const;

   virtual bool loadState(const ossimKeywordlist& kwl,
      const char* prefix=0);

protected:

   bool initializeBase(const ossimMapProjection& proj, 
                       const ossimIrect& boundingRect,
                       ossimDpt& convertedTilingDistance);

   void getConvertedDistance(ossimDpt& pt, ossimDpt inputDistance) const;

   ossimDpt getPaddingSize() const;

   bool                   m_clipToAoi;
   ossim_float64          m_lastWidth;
   ossim_float64          m_lastHeight;
   bool                   m_useOffset;
   ossimDpt               m_offsetInPixels;
   ossimDpt               m_tilingDistanceInPixels;
   ossimDpt               m_tilinResolutionDistance;
   ossimUnitType          m_tilingResolutionUnitType;
   ossimDpt               m_fractionalPixelMisalignment;
                    
TYPE_DATA
};

#endif

