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
//  $Id: ossimOrthoImageMosaic.h 17932 2010-08-19 20:34:35Z dburken $
#ifndef ossimOrthoImageMosaic_HEADER
#define ossimOrthoImageMosaic_HEADER

#include <ossim/imaging/ossimImageMosaic.h>

class OSSIMDLLEXPORT ossimOrthoImageMosaic : public ossimImageMosaic
{
public:
   ossimOrthoImageMosaic();
   ossimOrthoImageMosaic(ossimConnectableObject::ConnectableObjectList& inputSources);
   virtual ossimIrect getBoundingRect(ossim_uint32 resLevel=0) const;
   virtual void initialize();

   //! Returns the geometry associated with the full mosaic. This object maintains it's own 
   //! geometry instance.
   virtual ossimRefPtr<ossimImageGeometry> getImageGeometry();

   //! Used to retrieve the number of overlapping images withint the given rect.
   virtual ossim_uint32 getNumberOfOverlappingImages(const ossimIrect& rect, ossim_uint32 resLevel=0)const;

   //! Used to populate the result with the index of the overlapping images.
   virtual void getOverlappingImages(std::vector<ossim_uint32>& result,
                                     const ossimIrect& rect,
                                     ossim_uint32 resLevel=0)const;
   
   virtual ossimRefPtr<ossimImageData> getNextTile(ossim_uint32& returnedIdx,
                                                   const ossimIrect& origin,
                                                   ossim_uint32 resLevel=0);
   
   ossimIrect getRelativeRect(ossim_uint32 index,
                              ossim_uint32 resLevel = 0)const;
protected:
   virtual ~ossimOrthoImageMosaic();   
   void computeBoundingRect(ossim_uint32 resLevel=0);
   
   //! If this object is maintaining an ossimImageGeometry, this method needs to be called after 
   //! each time the contents of the mosaic changes.
   void updateGeometry();

   std::vector<ossimDpt>  m_InputTiePoints;
   ossimDpt    m_Delta; //!< Holds R0 delta and will be scaled for different r-level requests
   ossimDpt    m_UpperLeftTie; //!< Will hold the upper left tie of the mosaic.
   ossimIrect  m_BoundingRect;
   ossimString m_Units;
   ossimRefPtr<ossimImageGeometry> m_Geometry; //!< The input image geometry, altered by the map tiepoint

TYPE_DATA
};

#endif
