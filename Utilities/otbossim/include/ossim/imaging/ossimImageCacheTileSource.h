//-----------------------------------------------------------------------
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Mingjie Su
//
// Description: This class give the capability to access tiles from the
//              input frames.
//
//-----------------------------------------------------------------------
//$Id: ossimImageCacheTileSource.h 2644 2011-05-26 15:20:11Z oscar.kramer $
#ifndef ossimImageCacheTileSource_HEADER
#define ossimImageCacheTileSource_HEADER 1

#include <ossim/imaging/ossimImageCacheBase.h>

class OSSIMDLLEXPORT ossimImageCacheTileSource : public ossimImageCacheBase
{
public:

   /**
    * Default constructor.
    *
    * Initializes all internal attributes to a default state.
    */
   ossimImageCacheTileSource();

   /**
    * This method is defined in the base class ossimObject.
    * This class overrides the default implementation
    * to return its own short name.
    * 
    * @return The short name for this class as an ossimString.
    */
   virtual ossimString getShortName()const;

   /**
    * This method is defined in the base class ossimObject.
    * This class overrides the default implementation
    * to return its own long name.
    * 
    * @return The long name for this class as an ossimString.
    */
   virtual ossimString getLongName()const;

   /**
    * Closes this image handler and deletes any allocated data.
    */
   virtual void close();
   
   /**
    */
   virtual bool open();

   /**
    *  Returns a pointer to an ossimImageDataObject given a rectangluar
    *  region of interest.
    *
    *  @param rect The region of interest to return.
    *  @param resLevel From which resolution set are we querying
    *  (default is 0 full res).
    *
    *  @return The ossimImageData object to the caller.
    */
   virtual ossimRefPtr<ossimImageData> getTile(const  ossimIrect& rect,
                                               ossim_uint32 resLevel = 0);

   /**
    * Method to get a tile.   
    *
    * @param result The tile to stuff.  Note The requested rectangle in full
    * image space and bands should be set in the result tile prior to
    * passing.  It will be an error if:
    * result.getNumberOfBands() != this->getNumberOfOutputBands()
    *
    * @return true on success false on error.  If return is false, result
    *  is undefined so caller should handle appropriately with makeBlank or
    * whatever.
    */
   virtual bool getTile(ossimImageData* result, ossim_uint32 resLevel=0);

   /**
    * This method allows you to query the scalar type of the output data.
    * This is simply the data type, i.e. whether its OSSIM_FLOAT, OSSIM_DOUBLE,
    * OSSIM_UCHAR, ... etc.  This means that if there are bands of different
    * scalar types from a single image then they must be casted to the highest
    * precision type. @see ossimConstants.h for all ossimScalarType.
    * 
    * @return The output scalar type.
    *         
    */
   virtual ossimScalarType getOutputScalarType() const;

   /**
    * This method allows you to query the number of input bands.  If an image
    * is band selectable this will allow one to select the bands you wish
    * to read in.  This indicates that the number of input bands might not
    * match the number of output bands.
    * @return number of input bands.
    */
   virtual ossim_uint32 getNumberOfInputBands()const;

   /**
    * This method allows one to query the number of output bands.  This might not
    * be the same as the n umber of input bands.
    *
    * @see getNumberOfInputBands().
    * @return number of output bands.
    */
   virtual ossim_uint32 getNumberOfOutputBands()const;

    /**
    * Retuns the min pixel value.  If there was an external eta data file
    * then use the meta data from that file.  If not given then we will
    * return the default min
    */
   virtual double getMinPixelValue(ossim_uint32 band=0)const;
   virtual double getMaxPixelValue(ossim_uint32 band=0)const;
   virtual double getNullPixelValue(ossim_uint32 band=0)const;

protected:
   /**
    * Destructor.
    *
    * Will return allocated memory back to the heap.
    */
   virtual ~ossimImageCacheTileSource();
   
   /**
    *
    * @param tileRect Region to fill.
    * @param framesInvolved All intersecting frames used to render the region.
    */
   void fillTile(const ossimIrect& tileRect,
                 const vector<ossimFrameEntryData>& framesInvolved,
                 ossimImageData* tile);

   bool buildFrameEntryArray(ossimFilename imageFile);
 
   /**
    * This is the actual data returned from a getTile request.
    */
   ossimRefPtr<ossimImageData>  m_tile;

   ossimRefPtr<ossimImageHandler>  m_imageHandler;
   double                          m_minPixelValue;
   double                          m_maxPixelValue;
   double                          m_nullPixelValue;
TYPE_DATA
};

#endif /* #ifndef ossimImageCacheTileSource_HEADER */
