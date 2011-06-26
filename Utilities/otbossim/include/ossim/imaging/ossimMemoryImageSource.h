//*******************************************************************. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
//
#ifndef ossimMemoryImageSource_HEADER
#define ossimMemoryImageSource_HEADER

#include <ossim/base/ossimConstants.h>
#include <ossim/imaging/ossimImageSource.h>

class OSSIM_DLL ossimMemoryImageSource : public ossimImageSource
{
public:
   ossimMemoryImageSource();
   ossimMemoryImageSource(const ossimMemoryImageSource& src)
   :ossimImageSource(src),
   m_image(src.m_image.valid()?(ossimImageData*)src.m_image->dup():(ossimImageData*)0),
   m_result(0),
   m_geometry(m_geometry.valid()?(ossimImageGeometry*)src.m_geometry->dup():(ossimImageGeometry*)0),
   m_boundingRect(src.m_boundingRect)
   {
   }
   ossimMemoryImageSource* dup()const
   {
      return new ossimMemoryImageSource(*this);
   }
   void setImage(ossimRefPtr<ossimImageData> image);
   void setImage(ossimScalarType scalarType,
                 ossim_uint32 numberOfBands,
                 ossim_uint32 width,
                 ossim_uint32 height);
   void setRect(ossim_uint32 ulx,
                ossim_uint32 uly,
                ossim_uint32 width,
                ossim_uint32 height);
   virtual ossim_uint32 getNumberOfInputBands() const;
   virtual ossim_uint32 getNumberOfOutputBands() const;
   virtual ossimScalarType getOutputScalarType() const;
   virtual double getNullPixelValue(ossim_uint32 band=0)const;
   virtual double getMinPixelValue(ossim_uint32 band=0)const;
   virtual double getMaxPixelValue(ossim_uint32 band=0)const;
   
   virtual ossimIrect getBoundingRect(ossim_uint32 resLevel=0)const;
   
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& rect,
                                               ossim_uint32 resLevel=0);
   
   virtual bool canConnectMyInputTo(ossim_int32 myInputIndex,
                                    const ossimConnectableObject* object)const;
   
   virtual void initialize();
	virtual ossim_uint32 getNumberOfDecimationLevels() const;
   virtual void getDecimationFactor(ossim_uint32 resLevel,
                                    ossimDpt& result) const;
   virtual void getDecimationFactors(std::vector<ossimDpt>& decimations)const;
   
   //! Returns the image geometry object associated with this tile source or NULL if not defined.
   //! The geometry contains full-to-local image transform as well as projection (image-to-world)
   //! Default implementation returns the image geometry object associated with the next  
   //! (left) input source (if any) connected to this source in the chain, or NULL.
   virtual ossimRefPtr<ossimImageGeometry> getImageGeometry()
   {
      return m_geometry;
   }
   
   //! Default implementation sets geometry of the first input to the geometry specified.
   virtual void setImageGeometry(ossimImageGeometry* geom)
   {
      m_geometry = geom;
   }
   
   /**
    *  save state is not implemented for this source and only passes to the base.
    */
   virtual bool saveState(ossimKeywordlist& kwl, const char* prefix)const;
   
   /**
    * loadState is here so one can construct a memory source as a blank tile.
    */
   virtual bool loadState(const ossimKeywordlist& kwl, const char* prefix);
   
protected:
   ossimRefPtr<ossimImageData> m_image;
   ossimRefPtr<ossimImageData> m_result;
   ossimRefPtr<ossimImageGeometry> m_geometry;
   ossimIrect m_boundingRect;
   TYPE_DATA
};

#endif
