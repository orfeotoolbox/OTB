//-----------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author: Mingjie Su
//
// Description: This class give the capability to access tiles from the
//              input frames.
//
//-----------------------------------------------------------------------
//$Id: ossimImageCacheBase.h 2685 2011-06-07 16:24:41Z david.burken $
#ifndef ossimImageCacheBase_HEADER
#define ossimImageCacheBase_HEADER 1
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/support_data/ossimRpfFrameEntry.h>

class ossimRpfToc;
class ossimRpfTocEntry;
class ossimRpfFrame;

class OSSIMDLLEXPORT ossimImageCacheBase : public ossimImageHandler
{
public:

   /**
    * Default constructor.
    *
    * Initializes all internal attributes to a default state.
    */
   ossimImageCacheBase();

   /**
    * Closes this image handler and deletes any allocated data.
    */
   virtual void close();
   
   /**
    */
   virtual bool open() = 0;

   /**
    *  Satisfies pure virtual requirement from ImageHandler class.
    *
    *  @param reduced_res_level The reduced res level to query number of lines from.
    *  @return The number of lines in the image.
    */
   virtual ossim_uint32 getNumberOfLines(ossim_uint32 reduced_res_level = 0) const;
   
   /**
    *  Satisfies pure virtual requirement from ImageHandler class.
    *
    *  @param reduced_res_level The reduced res level to query number of samples from.
    *  @return The number of samples in the image.
    */
   virtual ossim_uint32 getNumberOfSamples(ossim_uint32 reduced_res_level = 0) const;

   /**
    * @param reduced_res_level Which res level to get the bounds for.  0 represents
    *                          the default and is the full resolution.
    * 
    * @return A rectangle representing the bounds for the passed in resolution level.
    */
   virtual ossimIrect getImageRectangle(ossim_uint32 reduced_res_level = 0) const ;
   

  /**
    * @param kwl Keywordlist to save the state of the object to.
    * @param prefix prefix to prepend to the keywords.  This will
    *               help uniquely identify each keyword if multiple
    *               objects are saved to the same list.
    * @return True if successfully saved the state of the object and
    *         false otherwise.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;
   
   /**
    * Method to load the state of an object from a keyword list.
    *
    * Loading with a prefix.
    * @code
    *  cibCadrgHandler->laodState(kwl, "handler1.");
    * @endcode
    * 
    * Loading without a prefix.
    * @code
    *  cibCadrgHandler->laodState(kwl);
    * @endcode
    *
    * Since all objects are factory created you can go through the
    * registry and pass the keyword list in.
    * @code
    * ossimImageHandler* handler = ossimImageHandlerRegistry::instance()->open(kwl, prefix);
    * @endcode
    * 
    * @param kwl Keywordlist that holds the state information for this
    *            object.
    *
    *
    * @param prefix prefix value prepended to all keywords to uniquely
    *                      locate the state attributes for this object.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   

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
    * Returns the width of the output tile.
    */
   virtual ossim_uint32 getTileWidth() const;
   
   /**
    * Returns the height of the output tile.
    */
   virtual ossim_uint32 getTileHeight() const;

   /**
    * Returns the tile width of the image or 0 if the image is not tiled.
    * Note: this is not the same as the ossimImageSource::getTileWidth which
    * returns the output tile width which can be different than the internal
    * image tile width on disk.
    */
   virtual ossim_uint32 getImageTileWidth() const;

   /**
    * Returns the tile width of the image or 0 if the image is not tiled.
    * Note: this is not the same as the ossimImageSource::getTileWidth which
    * returns the output tile width which can be different than the internal
    * image tile width on disk.
    */
   virtual ossim_uint32 getImageTileHeight() const;

   virtual void getEntryList(std::vector<ossim_uint32>& entryList)const;
  
   /**
    * @param reduced_res_level passed in resolution level.
    * @return True if the reduced_res_level is within an acceptable range
    *         and false otherwise.
    */
   bool isValidRLevel(ossim_uint32 reduced_res_level) const;

   /**
    * Determines if its open if there eists a table of contents object.
    * 
    * @return True if the handler is open and flase otherwise.
    */
   bool isOpen()const;

   /**
    *  Populates the geometry object with image geometry information.  This
    *  method is used to relay projection/model information to users.
    *  Returns true if geometry info is present, false if not.
    *  Keywords supported by this class are:
    *  @verbatim
    *  datum:    // datum code>   // Datum code
    *  ul_lat:   // upper left latitude
    *  ul_lon:   // upper left longitude
    *  ll_lat:   // lower left latitude
    *  ll_lon:   // lower left longitude
    *  lr_lat:   // lower right latitude
    *  lr_lon:   // lower right longitude
    *  ur_lat:   // upper right latitude
    *  ur_lon:   // upper right longitude
    *  number_input_bands:     
    *  number_output_bands:    
    *  number_lines:           
    *  number_samples:
    *  decimal_degrees_per_pixel_lat:  // specifies the spacing per pixel along Y direction
    *  decimal_degrees_per_pixel_lon:  // specifies the spacing per pixel along X direction
    *  origin_latitude:                // specifies the origin or center of the projector for the latitude
    *  central_meridian:               // specifies the origin or center of the projector for the longitude
    *  tie_point_lat:     // tie point lat.  Represents the latitude of the upper left center of pixel
    *  tie_point_lon:     // tie point lon.  Represents the longitude of the upper left center of pixel
    *  zone:              // zone read from the CIB/CADRG boundary info
    *  type:              // class name of the object for factory driven reconstruction
    *  map_scale:         // Map scale of the image
    *  @endverbatim
    *  @see ossimImageSource for further code example on using the geometry.
    */
   virtual ossimRefPtr<ossimImageGeometry> getImageGeometry();

protected:
   /**
    * Destructor.
    *
    * Will return allocated memory back to the heap.
    */
   virtual ~ossimImageCacheBase();
   
   /**
    * Will shift the rect and clamp it so not to go outside the -180 to 180
    * degree range.
    */
   void setActualImageRect();

   /**
    * @brief Method to test for 180.0 <--> 180.00 and set leftLon to -180 if
    * both left and right are 180.0 degrees.
    * 
    * @param leftLon Left hand side longitude of scene.  This is the value
    * that will be flipped to -180 if test is true.
    *
    * @param rightLon Right hand side of scene.
    */
   void checkLongitude(ossim_float64& leftLon,  const ossim_float64& rightLon) const;
   
   /**
    * Protected structure that is only used internally by this class.
    * 
    * This is used to establish which entry data is being used.
    * all we need is the ro, col of the entry.  We can compute the
    * image rect from this since the CibCadrg are 1536x1536 frame.
    * 
    */
   struct ossimFrameEntryData
   {
      ossimFrameEntryData()
         :theRow(-1),
          theCol(-1),
          thePixelRow(-1),
          thePixelCol(-1)
         {}
      ossimFrameEntryData(ossim_int32 row,
                          ossim_int32 col,
                          ossim_int32 pixelRow,
                          ossim_int32 pixelCol,
                          const ossimRpfFrameEntry& entry)
         :theRow(row),
          theCol(col),
          thePixelRow(pixelRow),
          thePixelCol(pixelCol),
          theFrameEntry(entry)
         {}
      ossimFrameEntryData(const ossimFrameEntryData& rhs)
         :theRow(rhs.theRow),
          theCol(rhs.theCol),
          thePixelRow(rhs.thePixelRow),
          thePixelCol(rhs.thePixelCol),
          theFrameEntry(rhs.theFrameEntry)
         {}
      ossim_int32 theRow;
      ossim_int32 theCol;
      ossim_int32 thePixelRow;
      ossim_int32 thePixelCol;
      ossimRpfFrameEntry theFrameEntry;
   };


   /**
    * It is important to note that each frame is organized into an easting northing
    * type orientation.  This means that a frame at 0,0 is at the lower left corner.
    * Each frame's pixel data is has 0,0 at the upper left.
    *
    * It will take the curent region to render and then find all entries that intersect
    * that region.
    *
    * @param rect the current region to render.
    * @return The list of entry data objects found for this rect.
    */
   vector<ossimFrameEntryData> getIntersectingEntries(const ossimIrect& rect);

   void deleteAll();

   bool buildFrameEntryArray(ossimFilename imageFile, ossim_uint32 frameWidth, ossim_uint32 frameHeight);
   virtual void establishDecimationFactors();
   
   /**
    * This holds the image bounds for the current entry.
    */
   ossimIrect                  m_actualImageRect;
   
   /**
    * This will be computed based on the frames organized within
    * the directory.  The CibCadrg have fixed size frames of 1536x1536
    * and we must count how many there are.  
    */
   ossim_uint32                m_numberOfLines;

   /**
    * This will be computed based on the frames organized within
    * the directory.  The CibCadrg have fixed size frames of 1536x1536
    * and we must count how many there are.  
    */
   ossim_uint32                m_numberOfSamples;

    /**
    * This is the actual data returned from a getTile request.
    */
   ossim_uint32                m_numberOfBands;

    /**
    * The size of the input frames.
    */
   std::vector<ossimString> m_fileNames;
  
   /**
    * The size of the output tile.
    */
   ossimIpt                     m_tileSize;
   
   mutable ossimRpfFrame*       m_workFrame;

   // data to use in property retrieval

   ossim_float64 m_bBox_LL_Lon;
   ossim_float64 m_bBox_LL_Lat;
   ossim_float64 m_bBox_UR_Lon;
   ossim_float64 m_bBox_UR_Lat;
   ossim_uint32  m_numOfFramesVertical;
   ossim_uint32  m_numOfFramesHorizontal;
   ossim_uint32  m_frame_width;
   ossim_uint32  m_frame_height;

   std::vector< std::vector<ossimRpfFrameEntry> > m_frameEntryArray;
	
TYPE_DATA
};

#endif /* #ifndef ossimImageCacheBase_HEADER */
