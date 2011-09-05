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
// $Id: ossimTilingPoly.h 2725 2011-06-15 18:13:07Z david.burken $
#ifndef ossimTilingPoly_HEADER
#define ossimTilingPoly_HEADER 1

#include <ossim/imaging/ossimTilingRect.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/imaging/ossimGeoPolyCutter.h>

class OSSIM_DLL ossimTilingPoly : public ossimTilingRect
{
public:

   ossimTilingPoly();

   virtual ~ossimTilingPoly();

   virtual bool initialize(const ossimMapProjection& proj,
                           const ossimIrect& boundingRect);

   virtual bool next(ossimRefPtr<ossimMapProjection>& resultProjection,
                     ossimIrect& resultingBounds,
                     ossimString& resultingName);

   virtual bool saveState(ossimKeywordlist& kwl,
      const char* prefix=0)const;

   virtual bool loadState(const ossimKeywordlist& kwl,
      const char* prefix=0);

   bool useMbr() const;

   /** return true if exterior cut is initialized. */
   bool hasExteriorCut() const;

   /** return true if interior cut is initialized. */
   bool hasInteriorCut() const;
   
   ossimRefPtr<ossimGeoPolyCutter>& getExteriorCut();
   
   ossimRefPtr<ossimGeoPolyCutter>& getInteriorCut();

   bool isFeatureBoundingIntersect() const;

protected:

   struct ossimShpFeature
   {
      ossimShpFeature(ossim_int32 fid,
         ossim_int32 polyType,
         std::vector<ossimGpt> polygon,
         std::vector<ossimGeoPolygon> multiPolys)

         :m_fid(fid),
         m_polyType(polyType),
         m_polygon(polygon),
         m_multiPolys(multiPolys)
      {}

      ossim_int32 m_fid;
      ossim_int32 m_polyType;
      std::vector<ossimGpt> m_polygon;
      std::vector<ossimGeoPolygon> m_multiPolys;
   };

   bool parseShpFile();

   bool nextFeature();

   void setRect();

   ossimFilename          m_shpFilename;
   bool                   m_useMbr;
   ossim_float64          m_bufferDistance;
   
   ossimRefPtr<ossimGeoPolyCutter> m_exteriorCut;
   ossimRefPtr<ossimGeoPolyCutter> m_interiorCut;
   
   std::vector<ossimShpFeature> m_features;
   bool m_featureBoundingIntersect;
                    
TYPE_DATA
};

#endif

