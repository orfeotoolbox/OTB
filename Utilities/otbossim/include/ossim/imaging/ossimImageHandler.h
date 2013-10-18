//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
// 
// Description:
//
// This class defines an abstract image handler which all loaders should
// derive from.
//
//********************************************************************
// $Id: ossimImageHandler.h 21745 2012-09-16 15:21:53Z dburken $
#ifndef ossimImageHandler_HEADER
#define ossimImageHandler_HEADER 1

#include <ossim/imaging/ossimImageSource.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimNBandLutDataObject.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/imaging/ossimFilterResampler.h>
#include <ossim/imaging/ossimImageMetaData.h>

/**
 *  This class defines an abstract Handler which all image handlers(loaders)
 *  should derive from.
 */
class OSSIMDLLEXPORT ossimImageHandler : public ossimImageSource
{
public:

   enum ossimImageHandlerOverviewCompressionType
   {
      OSSIM_OVERVIEW_COMPRESSION_NONE= 0,
      OSSIM_OVERVIEW_COMPRESSION_JPEG,
      OSSIM_OVERVIEW_COMPRESSION_LZW,
      OSSIM_OVERVIEW_COMPRESSION_DEFLATE,
      OSSIM_OVERVIEW_COMPRESSION_PACKBITS
   };

   /** Constructor (default): */
   ossimImageHandler();
   
   /**
    * Destructor:
    * Derived classes should implement.
    */
   virtual ~ossimImageHandler();
   
   /**
    * initialize
    * Does nothing in this class.  Implemented to satisfy pure virtual.
    */
   virtual void initialize();
   
   /**
    *  Searches for an overview.  Uses theImageFile as a key.
    *  If theImageFile = "foo.tif" it will look for "foo.ovr" first,
    *  then "foo_ovr.tif" next.
    *  Calls openOverview(const ossimFilename& overview_file)
    *  @return true on success, false on error.
    *  @see openOverview(const ossimFilename& overview_file)
    */
   virtual bool openOverview();

   /**
    *  Calls closeOverview(), then attempts to open the overview_file.
    *  On success, theOverviewFile will be set and theOverview will be
    *  initialized.  On failure, theOverviewFile will be cleared and
    *  theOverview deleted and set to NULL.
    *  @param overview_file ossimFilename representing overview file.
    *  @return true on success, false on error.
    *  @see closeOverview()
    */
   virtual bool openOverview(const ossimFilename& overview_file);

   /**
    *  If theOverview is initialized it will be deleted and set to NULL. Also
    *  clears theOverviewFile.
    *  @return void
    */
   virtual void closeOverview();
   
   /**
    *  @return the image handler of the overview, if it has been
    *  successfully opened.
    */
   virtual const ossimImageHandler* getOverview() const;

   /**
    *  @return true if getNumberOfReducedResSets > 1, false if not.
    *  @see getNumberOfReducedResSets()
    */
   virtual bool hasOverviews() const;

   /**
    *  @return ossimFilename represents an external OSSIM overview filename.
    */
   virtual ossimFilename createDefaultOverviewFilename() const;
   
   /**
    *  @return ossimFilename represents an external OSSIM geometry filename.
    */
   virtual ossimFilename createDefaultGeometryFilename() const;

   /**
    *  @return ossimFilename represents an external OSSIM metadata filename.
    */
   virtual ossimFilename createDefaultMetadataFilename() const;

   /**
    *  @return ossimFilename represents an external OSSIM histogram filename.
    */
   virtual ossimFilename createDefaultHistogramFilename() const;

   /**
    *  @return ossimFilename represents an external OSSIM valid vertices filename.
    */
   virtual ossimFilename createDefaultValidVerticesFilename() const;
   
   /**
    *  Outputs vertices to file and updates the internal vertex
    *  array variable "theValidImageVertices".
    *  @param vertices Array of ossimIpts to write.
    *  @param file Name of output file to write to.  If file is empty, the
    *  output file will be the image file with no extension + _vertices.kwl.
    *  So if the image is "foo.tif" you'll get "foo_vertices.kwl".
    *  @return true on success, false on error.
    */
   virtual bool writeValidImageVertices(const std::vector<ossimIpt>& vertices, 
					const ossimFilename& file=ossimFilename(""));

   /**
    *  Opens the valid image vertices file and sets theValidImageVerticesFile
    *  variable.
    *  @param vertices_file File name containing valid vertices for image.
    *  @return true on success, false on error.
    */
   virtual bool openValidVertices(const ossimFilename& vertices_file);

   /**
    *  Creates vertices file name based on image, then tries to open.
    *  If theImageFile = "foo.tif" it will look for "foo_vertices.kwl".
    *  @return true on success, false on error.
    */
   virtual bool openValidVertices();

   /**
    *  Pure virtual open.  Derived classes must implement.
    *  
    *  @return Returns true on success, false on error.
    *
    *  @note This method relies on the data member ossimImageData::theImageFile
    *  being set.  Callers should do a "setFilename" prior to calling this
    *  method or use the ossimImageHandler::open that takes a file name and an
    *  entry index.
    */
   virtual bool open() = 0;

   /**
    *  Opens the image file.
    *  
    *  @param imageFile File to open.
    *
    *  @param entryIndex
    *  @return true on success, false on error.
    */
   virtual bool open(const ossimFilename& imageFile,
                     ossim_uint32 entryIndex);

   virtual bool open(const ossimFilename& imageFile);

   /**
    *  Deletes the overview and clears the valid image vertices.  Derived
    *  classes should implement.
    */
   virtual void close();

   /**
    *  Derived classes must implement this method to be concrete.
    *
    *  @return true if open, false if not.
    */
   virtual bool isOpen()const=0;

   /**
    *  Build a histogram for image file.
    *
    *  @param numberOfRLevels sets the maximum number of reduced resolution
    *  level to compute histogram for.
    *
    *  @return true on success, false if not open.
    */
   virtual bool buildHistogram(int numberOfRLevels=0);
   
   
   /**
    *  Build a histograms for all image entries.
    *
    *  @param numberOfRLevels sets the maximum number of reduced resolution
    *  level to compute histogram for.
    *
    *  @return true on success, false if not open.
    */
   virtual bool buildAllHistograms(int numberOfRLevels=0);
   
   /**
    *  Will build over file for theImageFile.
    *
    *  @param includeFullResFlag if true the full resolution layer will also
    *  be put in the overview format.  This is handly for inefficeint formats.
    */
   virtual bool buildOverview(ossimImageHandlerOverviewCompressionType compressionType = OSSIM_OVERVIEW_COMPRESSION_NONE,
                              ossim_uint32 quality = 75,
                              ossimFilterResampler::ossimFilterResamplerType resampleType = ossimFilterResampler::ossimFilterResampler_BOX,
                              bool includeFullResFlag=false);

   /**
    *  Will build over file for all image entries.
    *
    *  @param includeFullResFlag if true the full resolution layer will also
    *  be put in the overview format.  This is handly for inefficeint formats.
    */
   virtual bool buildAllOverviews(ossimImageHandlerOverviewCompressionType compressionType = OSSIM_OVERVIEW_COMPRESSION_NONE,
                                  ossim_uint32 quality = 75,
                                  ossimFilterResampler::ossimFilterResamplerType resampleType = ossimFilterResampler::ossimFilterResampler_BOX,
                                  bool includeFullResFlag=false);
   
   /**
    *  Will build an over file for filename.
    *  @param filename Name of image file.
    *
    *  @param includeFullResFlag if true the full resolution layer will also
    *  be put in the overview format.  This is handly for inefficeint formats.
    */
   virtual bool buildOverview(const ossimFilename& filename,
                              ossimImageHandlerOverviewCompressionType compressionType = OSSIM_OVERVIEW_COMPRESSION_NONE,
                              ossim_uint32 quality = 75,
                              ossimFilterResampler::ossimFilterResamplerType resampleType = ossimFilterResampler::ossimFilterResampler_BOX,
                              bool includeFullResFlag=false);
   
   /**
    * Returns the image geometry object associated with this tile source or
    * NULL if non defined.  The geometry contains full-to-local image
    * transform as well as projection (image-to-world).
    */
   virtual ossimRefPtr<ossimImageGeometry> getImageGeometry();
   
   /**
    * Sets the image geometry object.
    *
    * @note Callers should note that this will override any existing
    *       geometry.
    *
    * @note This does not write the geometry to disk, to do so call
    *       one of the saveImageGeometry methods. 
    */
   virtual void setImageGeometry(ossimImageGeometry* geom);

   /**
    * Saves the image geometry to a file that will be made based on the image
    * file name.
    */
   virtual void saveImageGeometry() const;
   
   /**
    * Saves the image geometry to a file that will be made based on the image
    * file name.
    */
   virtual void saveImageGeometry(const ossimFilename& geometry_file) const;   
   
   /**
    *  Pure virtual, derived classes must implement.
    *
    *  @param resLevel Reduced resolution level to return lines of.
    *  Default = 0
    *
    *  @return The number of lines for specified reduced resolution level.
    */
   virtual ossim_uint32 getNumberOfLines(ossim_uint32 resLevel = 0) const = 0;

   /**
    *  Pure virtual, derived classes must implement.
    *
    *  @param resLevel Reduced resolution level to return samples of.
    *  Default = 0
    *
    *  @return The number of samples for specified reduced resolution level.
    */
   virtual ossim_uint32 getNumberOfSamples(ossim_uint32 resLevel = 0) const = 0;
   
   /**
    * Returns the zero-based (relative) image rectangle for the reduced
    * resolution data set (rrds) passed in.  Note that rrds 0 is the highest
    * resolution rrds.
    */
   virtual ossimIrect getImageRectangle(ossim_uint32 resLevel = 0) const; 

   /**
    * Returns the absolute bounding rectangle of the image. The upper left
    * corner may be non-zero if this is a subimage.
    */
   virtual ossimIrect getBoundingRect(ossim_uint32 resLevel = 0) const;

   /**
    * @param resLevel Reduced resolution set for requested decimation.
    *
    * @param result ossimDpt to initialize with requested decimation.
    * 
    * @note Initialized "result" with the decimation factor for the passed in
    * resLevel.
    * Most of the time the returned factor is a square decimation along x
    * and y indicated by result.x and .y  = 1.0/(resLevel^2) where ^
    * means rasing to the power of.  If the resLevel is 1 then the return
    * decimation .5, .5. this is not the decimation to each resolution
    * level but the total decimation from res level 0.
    * So if resLevel is 2 then the return is .25, .25.
    *
    * @note Derived classes should override if the decimation is anything other
    * than a power of two change in each direction per res level.
    */
   virtual void getDecimationFactor(ossim_uint32 resLevel, ossimDpt& result) const;

   /**
    * This returns all decimation for all levels.
    */
   virtual void getDecimationFactors(vector<ossimDpt>& decimations) const;

   /**
    * This returns the total number of decimation levels.  It is important to
    * note that res level 0 or full resolution is included in the list and has
    * decimation values 1.0, 1.0
    */
   virtual ossim_uint32 getNumberOfDecimationLevels() const;

   /**
    * This method is obsolete.  Please use getNumberOfDecimationLevels.
    *
    * For backwards compatibility this calls getNumberOfDecimationLevels.
    */
   virtual ossim_uint32 getNumberOfReducedResSets()const;
   
   /**
    * Sets the filename
    */
   virtual void setFilename(const ossimFilename& filename);

   /**
    * Returns the filename
    */
   virtual const ossimFilename& getFilename()const;
   
   /**
    * Method to save the state of an object to a keyword list.
    * Return true if ok or false on error.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;

   /**
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   /**
    * @brief Determines if the passed in reslution level is valid.
    *
    * If this reader is used as an overview the caller should pass in a
    * resLevel relative to the full image.
    * 
    * @param resLevel zero base resolution level.
    * @return true on success, false on error.
    */
   virtual bool isValidRLevel(ossim_uint32 resLevel) const;

   /**
    * Connection rule.  Since image handler currently don't have any
    * inputs this will just return false saying we can't connect anything
    * to an image handler.
    */
   bool canConnectMyInputTo(ossim_int32 inputIndex,
                            const ossimConnectableObject* object) const;
      
   /**
    * ordering specifies how the vertices should be arranged.
    * valid image vertices is basically the tightly fit convex hull
    * of the image.  Usually an image has NULL values and are
    * internally not upright rectangular.  This can cause
    * problems some spatial filters.
    *
    * The default implementation is to return the bounding rect.
    */
   virtual void getValidImageVertices(vector<ossimIpt>& validVertices,
                                      ossimVertexOrdering ordering=OSSIM_CLOCKWISE_ORDER,
                                      ossim_uint32 resLevel=0)const;

   /**
    * @brief Indicates whether or not the image handler can control output
    * band selection via the setOutputBandList method.
    * @return true if band selector; false, if not.
    */
   virtual bool isBandSelector() const;

   /**
    * @brief If the image handler "isBandSeletor()" then the band selection
    * of the output chip can be controlled.
    *
    * This method returns false. Derived classes that are band selectors should
    * override.
    *
    * @param band_list Requested bands.
    * 
    * @return true on success, false on error.
    */
   virtual bool setOutputBandList(const std::vector<ossim_uint32>& band_list);
 
   /**
    * @brief If the image handler "isBandSeletor()" then the band selection
    * of the output are set to input or identity.  Does nothing and returns
    * false if not a band selector.
    * 
    * @return If band selector, returns the output of setOutputBandList; if
    * not, returns false.
    */
   virtual bool setOutputToInputBandList();
 
   /**
    * @brief Convenience method to see if band list is identity.
    * @param bandList List to check.
    * @return true if band selector; false, if not.
    */
   virtual bool isIdentityBandList( const std::vector<ossim_uint32>& bandList ) const;
   
   /**
    * Indicates whether or not the image is tiled internally.
    * This implementation returns true if (getImageTileWidth() &&
    * getImageTileHeight()) are both non zero.
    * Override in the image handler if something else is desired.
    * Returns true if tiled, false if not.
    */
   virtual bool isImageTiled() const;

   /**
    * Returns the tile width of the image or 0 if the image is not tiled.
    * Note: this is not the same as the ossimImageSource::getTileWidth which
    * returns the output tile width which can be different than the internal
    * image tile width on disk.
    */
   virtual ossim_uint32 getImageTileWidth() const = 0;

   /**
    * Returns the tile width of the image or 0 if the image is not tiled.
    * Note: this is not the same as the ossimImageSource::getTileWidth which
    * returns the output tile width which can be different than the internal
    * image tile width on disk.
    */
   virtual ossim_uint32 getImageTileHeight() const = 0;

   virtual bool hasMetaData() const;

   virtual ossimRefPtr<ossimNBandLutDataObject> getLut()const;

   virtual bool hasLut() const;
   
   /**
    * There is an external file with an omd extension.  this file contains
    * additional information about the image such as the null pixel, min and
    * max values.
    * For general raster it will contain much more than that.
    */
   virtual void loadMetaData();

   /**
    * Retuns the min pixel value.  If there was an external eta data file
    * then use the meta data from that file.  If not given then we will
    * return the default min
    */
   virtual double getMinPixelValue(ossim_uint32 band=0)const;
   virtual double getMaxPixelValue(ossim_uint32 band=0)const;
   virtual double getNullPixelValue(ossim_uint32 band=0)const;

   /**
    * @brief convenience method to set min pixel value.
    *
    * Added for overview readers so that the image handler that owns the
    * overview reader can pass on it's min value.
    *
    * @param band Zero based band to set.
    *
    * @param pix Min pixel value.
    */
   virtual void setMinPixelValue(ossim_uint32 band, const ossim_float64& pix);

   /**
    * @brief convenience method to set max pixel value.
    *
    * Added for overview readers so that the image handler that owns the
    * overview reader can pass on it's max value.
    *
    * @param band Zero based band to set.
    *
    * @param pix Max pixel value.
    */
   virtual void setMaxPixelValue(ossim_uint32 band, const ossim_float64& pix);

   /**
    * @brief convenience method to set null pixel value.
    *
    * Added for overview readers so that the image handler that owns the
    * overview reader can pass on it's max value.
    *
    * @param band Zero based band to set.
    *
    * @param pix Null pixel value.
    */
   virtual void setNullPixelValue(ossim_uint32 band, const ossim_float64& pix);
    
   /**
    * @return The current entry number.
    *
    * @note This implementation returns 0.  Derived classes should override
    * if of multiple entry image type.
    */
   virtual ossim_uint32 getCurrentEntry()const;
   
   /**
    * @return The number of entries (images) in the image file.
    */
   virtual ossim_uint32 getNumberOfEntries()const;
   
   /**
    * @param entryList This is the list to initialize with entry indexes.
    *
    * @note This implementation returns puts one entry "0" in the list.
    */
   virtual void getEntryList(std::vector<ossim_uint32>& entryList) const;
   
   /**
    * @param entryStringList List to initialize with strings associated with
    * entries.
    *
    * @note This implementation calls getEntryList and converts entries in
    * the list to strings.  Derived classes that support multiple entries
    * with associated strings should re-implement.
    */
   virtual void getEntryStringList(
      std::vector<ossimString>& entryStringList) const;

   /**
    * @brief Convenience method to determine if entry index should be used for
    * support files, e.g. file.his or file_e1.his.
    * 
    * @returns true if file is open, &&
    * ( (entryList.size() > 1) || (entryList[0] != 0) )
    * false otherwise.
    *
    * @note This is just a hint.  Put here as code was duplicated everywhere.
    * Callers can do what they wish.
    */
   bool useEntryIndex() const;
   
   ossimFilename getOverviewFile()const;

   /**
    * @param entryIdx Entry number to select.
    *
    * @note The implementation does nothing.  Derived classes that handle
    * multiple images should override.
    *
    * @return true if it was able to set the current entry and false otherwise.
    */
   virtual bool setCurrentEntry(ossim_uint32 entryIdx);
   
   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;
   
   /**
    * Returns the image file with extension set.
    *
    * Examples:
    * 
    * - theImageFile          = "foo.hdf"
    * - The number of entries = 1
    * - ext parameter         = "geom"
    * - return of method will = "foo.geom"
    *
    * - theImageFile          = "foo.hdf"
    * - The number of entries = 12
    * - The current entry     = 3
    * - ext parameter         = "geom"
    * - return of method will = "foo_e3.geom"
    *
    * @param ext Extension to tack onto file.  Can have or have not ".", it
    * will be added if "." is not the first character.
    *
    * @param set_e0_prefix If true and the number of entries = 1 then
    * "foo.geom" would come out "foo_e0.geom" instead. Default = false.
    * 
    * @return theImageFile with sent extension.
    */
   ossimFilename getFilenameWithThisExtension(const ossimString& ext,
                                              bool set_e0_prefix=false) const;
   
   ossim_uint32 getStartingResLevel() const;
   
   void setStartingResLevel(ossim_uint32 level);

   /** @return theOpenOverviewFlag */
   bool getOpenOverviewFlag() const;

   /**
    * @brief Sets theOpenOverviewFlag.
    *
    * If true opening of overviews will be attempted on normal open.  If
    * false only the base image will be opened (shallow open).
    * Defaulted to true in constructor.
    */
   void setOpenOverviewFlag(bool flag);
   
   /**
    * Sets the supplementary directory
    */
   virtual void setSupplementaryDirectory(const ossimFilename& dir);
   
   /**
    * Returns the supplementary directory
    */
   virtual const ossimFilename& getSupplementaryDirectory()const;
   
  //! Fetches the image ID. This is initialized to -1 in the constructor but is searched for in 
  //! loadState():
  const ossimString& getImageID() const { return theImageID; }

  //! Sets the image ID in case it is externally generated
  void setImageID(const ossimString&  id) { theImageID = id; }

  //! Returns the raster pixel alignment type. OSSIM treats all alignments internally as
  //! pixel-is-point. This is only a flag to identify the source's alignment type.
  ossimPixelType getPixelType() const { return thePixelType; }

protected:
   
   /**
    * Returns the image geometry object associated with this tile source or
    * NULL if non defined.  The geometry contains full-to-local image
    * transform as well as projection (image-to-world).
    *
    * This method just looks for external .geom style override only.
    * If you want to go through a registry then call getImageGeometry().
    */
   virtual ossimRefPtr<ossimImageGeometry> getExternalImageGeometry() const;
   
   virtual ossimRefPtr<ossimImageGeometry> getInternalImageGeometry() const;
   
   /**
    * @brief Method to get an overview tile.  Derived classes should override if
    * they have built in overviews or something different than the standard
    * external overview file.  Image handlers can call this method from getTile
    * in place of inlining code or if derived class needs to override this
    * method.
    *
    * @param resLevel The resolution level to pull from with resLevel 0 being
    * full res.
    * 
    * @param result The tile to stuff. 
    * passing. 
    *
    * @return true on success false on error.  Typically this will return false
    * if resLevel==0 unless the overview has r0.  If return is false, result
    * is undefined so caller should handle appropriately with makeBlank or
    * whatever.
    */
   virtual bool getOverviewTile(ossim_uint32 resLevel, ossimImageData* result);  
   
   /**
    *  Opens file and attempts to initialize the valid image vertices.
    *  Returns true on success, false on error.
    */
   bool initVertices(const char* file);
   
   /**
    * Will complete the opening process.  
    * If there are overviews it will open them. 
    * If there is meta data it will open that and if there
    * is valid vertices it will open that.
    */
   virtual void completeOpen();
   
   /**
    * @brief Convenience method to set things needed in the image geometry from
    * the image handler.  At time of writing sets the decimation and image size.
    * @param geom ossimImageGeometry to initiale.
    */
   void initImageParameters(ossimImageGeometry* geom) const;
   
   /**
    * @brief Virtual method determines the decimation factors at each resolution level. This
    * base class implementation computes the decimation by considering the ratios in image size
    * between resolution levels, with fuzzy logic for rounding ratios to the nearest power of 2
    * if possible. Derived classes need to override this method if the decimations are provided
    * as part of the image metadata.
    */
   virtual void establishDecimationFactors();

   /**
    * @brief Convenience method to set output band list.
    *
    * This performs range checking and calls theOverview->setOutputBandList
    * on success.
    * 
    * @param inBandList The new band list.
    * @param outBandList Band list to initialize.
    * @return true on success, false on error.
    */
   virtual bool setOutputBandList(const std::vector<ossim_uint32>& inBandList,
                                  std::vector<ossim_uint32>& outBandList);


   ossimFilename theImageFile;
   ossimFilename theOverviewFile;
   ossimFilename theSupplementaryDirectory;
   ossimRefPtr<ossimImageHandler> theOverview;
   vector<ossimIpt> theValidImageVertices;
   ossimImageMetaData theMetaData;
   mutable ossimRefPtr<ossimImageGeometry> theGeometry;
   ossimRefPtr<ossimNBandLutDataObject> theLut;
   std::vector<ossimDpt> theDecimationFactors;
   ossimString theImageID;

   /**
    * theStartingResLevel If set to something other than zero(default) this is
    * indicative that the reader is an overview.
    */
   ossim_uint32 theStartingResLevel; // 0 being full or highest res.

   /**
    * If true opening of overviews will be attempted.  If false only the base
    * image will be opened (shallow open). Defaulted to true in constructor.
    */
   bool theOpenOverviewFlag;
   mutable ossimPixelType thePixelType; // pixel-is-point or pixel-is-area
   
TYPE_DATA
};

#endif /* #ifndef ossimImageHandler_HEADER */
