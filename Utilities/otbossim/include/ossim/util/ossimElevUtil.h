//----------------------------------------------------------------------------
//
// File: ossimElevUtil.h
// 
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:
// 
// Utility class to orthorectify imagery with an added slant toward doing
// digital elevation model(DEM) operations.
// 
//----------------------------------------------------------------------------
// $Id$

#ifndef ossimElevUtil_HEADER
#define ossimElevUtil_HEADER 1

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimReferenced.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/imaging/ossimImageSource.h>
#include <ossim/imaging/ossimSingleImageChain.h>
#include <ossim/projection/ossimMapProjection.h>

#include <map>
#include <vector>

// Forward class declarations:
class ossimArgumentParser;
class ossimDpt;
class ossimFilename;
class ossimGpt;
class ossimImageFileWriter;
class ossimIrect;
class ossimKeywordlist;

/**
 * @brief ossimElevUtil class.
 *
 * This is a utility class to orthorectify imagery with an added slant toward
 * doing digital elevation model(DEM) operations.
 *
 * See the ossim-dem application for a code usage example.
 *
 * @note Almost all methods use throw for stack unwinding.  This is not in
 * method declarations to alleviate build errors on windows.  Sorry...
 *
 * @note "bumpshade" and "hillshade" intermixed throughout.  The class to do
 * a hillshade is the ossimBumpShadeTileSource.
 */
class OSSIM_DLL ossimElevUtil : public ossimReferenced
{
public:

   /** emumerated operations */
   enum ossimDemOperation
   {
      OSSIM_DEM_OP_UNKNOWN      = 0,
      OSSIM_DEM_OP_HILL_SHADE   = 1,
      OSSIM_DEM_OP_COLOR_RELIEF = 2,
      OSSIM_DEM_OP_ORTHO        = 3
   };

   /** emumerated output projections */
   enum ossimDemOutputProjection
   {
      OSSIM_DEM_PROJ_UNKNOWN    = 0,
      OSSIM_DEM_PROJ_GEO        = 1,
      OSSIM_DEM_PROJ_GEO_SCALED = 2,
      OSSIM_DEM_PROJ_INPUT      = 3,
      OSSIM_DEM_PROJ_UTM        = 4
   };

   /** default constructor */
   ossimElevUtil();

   /** virtual destructor */
   virtual ~ossimElevUtil();

   /**
    * @brief Initial method to be ran prior to execute.
    * @param ap Arg parser to initialize from.
    * @note Throws ossimException on error.
    * @note A throw with an error message of "usage" is used to get out when
    * a usage is printed.
    */
   bool initialize(ossimArgumentParser& ap);

   /**
    * @brief Initial method to be ran prior to execute.
    *
    * This was separated out from initialize(ossimArgumentParser& ap) because
    * you could make an initialize(const ossimKeywordlist& kwl) and then call
    * initialize().
    * 
    * @note Throws ossimException on error.
    */
   void initialize();

   /**
    * @brief execute method.  Performs the actual product write.
    * @note Throws ossimException on error.
    */
   void execute();

   /**
    * @brief Gets the output file name.
    * @param f Filename to initialize.
    * @return The output file name.
    */
   void getOutputFilename(ossimFilename& f) const;

private:

   /** @brief Create chains for all dems. */
   void addDemSources();
   
   /** @brief Method to create a chain and add to dem layers from file. */
   void addDemSource(const ossimFilename& file);

   /**
    * @brief  Method to create a chain and add to dem layers from a
    * ossimSrcRecord.
    */
   void addDemSource(const ossimSrcRecord& rec);

   /** @brief Creates chains for all images. */
   void addImgSources();
   
   /** @brief Method to create a chain and add to img layers from file. */
   void addImgSource(const ossimFilename& file);

   /**
    * @brief  Method to create a chain and add to img layers from a
    * ossimSrcRecord.
    */
   void addImgSource(const ossimSrcRecord& rec);

   /**
    * @brief Creates a ossimSingleImageChain from file.
    * @param file File to open.
    * @return Ref pointer to ossimSingleImageChain.
    * @note Throws ossimException on error.
    */
   ossimRefPtr<ossimSingleImageChain> createChain(const ossimFilename& file) const;

   /**
    * @brief Creates a ossimSingleImageChain from ossimSrcRecord.
    * @param src Record.
    * @return Ref pointer to ossimSingleImageChain.
    * @note Throws ossimException on error.
    */
   ossimRefPtr<ossimSingleImageChain> createChain(const ossimSrcRecord& rec) const;

   /**
    * @brief Creates the output or view projection.
    * @note All chains should be constructed prior to calling this.
    */
   void createOutputProjection();

   /**
    * @brief Gets the first input projection.
    *
    * This gets the output projection of the first dem layer if present;
    * if not, the first image layer.
    * 
    * @return ref ptr to projection, could be null.
    */
   ossimRefPtr<ossimMapProjection> getFirstInputProjection();

   /**
    * @brief Convenience method to get geographic projection.
    * @return new ossimEquDistCylProjection.
    */
   ossimRefPtr<ossimMapProjection> getNewGeoProjection();

   /**
    * @brief Convenience method to get geographic projection.
    *
    * This method sets the origin to the center of the scene bounding rect
    * of all layers.
    * @return new ossimEquDistCylProjection.
    */
   ossimRefPtr<ossimMapProjection> getNewGeoScaledProjection();

    /**
    * @brief Convenience method to get a projection from an srs code.
    * @return new ossimMapProjection.
    */  
   ossimRefPtr<ossimMapProjection> getNewProjectionFromSrsCode(const ossimString& code);

   /**
    * @brief Convenience method to get a utm projection.
    * @return new ossimUtmProjection.
    */     
   ossimRefPtr<ossimMapProjection> getNewUtmProjection();

   /**
    * @brief Sets the projection tie point to the scene bounding rect corner.
    * @note Throws ossimException on error.
    */
   void intiailizeProjectionTiePoint();

   /**
    * @brief Initializes the projection gsd.
    *
    * This loops through all chains to find the best resolution gsd.
    *
    * @note Throws ossimException on error.
    */
   void initializeProjectionGsd();   

   /**
    * @brief Loops through all layers to get the upper left tie point.
    * @param tie Point to initialize.
    */
   void getTiePoint(ossimGpt& tie);

   /**
    * @brief Gets the upper left tie point from a chain.
    * @param chain The chain to get tie point from.
    * @param tie Point to initialize.
    * @note Throws ossimException on error.
    */
   void getTiePoint(ossimSingleImageChain* chain, ossimGpt& tie);

   /**
    * @brief Loops through all layers to get the best gsd.
    * @param gsd Point to initialize.
    */
   void getMetersPerPixel(ossimDpt& gsd);

   /**
    * @brief Gets the gsd from a chain.
    * @param chain The chain to get gsd from.
    * @param gsd Point to initialize.
    * @note Throws ossimException on error.
    */   
   void getMetersPerPixel(ossimSingleImageChain* chain, ossimDpt& gsd);

   /**
    * @brief Gets the origin for projection.
    *
    * This is conditional.  If keywords origin_latitude or central_meridan are
    * set from optional args those are used; else, the scene center is use.
    * 
    * @param gpt Point to initialize.
    * 
    * @note Throws ossimException on error.
    */
   void getOrigin(ossimGpt& gpt);

   /**
    * @brief Loops through all layers to get the scene center ground point.
    * @param gpt Point to initialize.
    * @note Throws ossimException on error.
    */
   void getSceneCenter(ossimGpt& gpt);

   /**
    * @brief Gets the scene center from a chain.
    * @param chain The chain to get scene center from.
    * @param gpt Point to initialize.
    * @note Throws ossimException on error.
    */   
   void getSceneCenter(ossimSingleImageChain* chain, ossimGpt& gpt);

   /**
    * @brief Creates a new writer.
    *
    * This will use the writer option (-w or --writer), if present; else,
    * it will be derived from the output file extention.
    *
    * This will also set any writer properties passed in.
    *
    * @return new ossimImageFileWriter.
    * @note Throws ossimException on error.
    */
   ossimRefPtr<ossimImageFileWriter> createNewWriter() const;

   /**
    * @brief loops through all chains and sets the output projection.
    * @note Throws ossimException on error.
    */
   void propagateOutputProjectionToChains();

   /**
    * @brief Combines all layers into an ossimImageMosaic.
    * @return ossimRefPtr with pointer to ossimImageSource.  Can be null.
    */
   ossimRefPtr<ossimImageSource> combineLayers(
      std::vector< ossimRefPtr<ossimSingleImageChain> >& layers) const;

   /** @brief Combines dems(m_demLayer) and images(m_imgLayer). */
   ossimRefPtr<ossimImageSource> combineLayers();

   /**
    * @brief Creates ossimIndexToRgbLutFilter and connects to source.
    * @param Source to connect to.
    * @return End of chain with lut filter on it.
    * @note Throws ossimException on error.
    */
   ossimRefPtr<ossimImageSource> addIndexToRgbLutFilter(
      ossimRefPtr<ossimImageSource> &source) const;

   /**
    * @brief Creates ossimScalarRemapper and connects to source.
    * @param Source to connect to.
    * @param scalar Scalar type.
    * @return End of chain with remapper on it.
    * @note Throws ossimException on error.
    */
   ossimRefPtr<ossimImageSource> addScalarRemapper(
      ossimRefPtr<ossimImageSource> &source,
      ossimScalarType scalar) const;

   /**
    * @brief Set up ossimHistogramRemapper for a chain.
    * @param chain Chain to set up.
    * @return true on success, false on error.
    */
   bool setupChainHistogram( ossimRefPtr<ossimSingleImageChain> &chain) const;

   /**
    * @brief Initializes "rect" with the output area of interest.
    *
    * Initialization will either come from user defined cut options or the
    * source bounding rect with user options taking precidence.
    *
    * @param source Should be the end of the processing chain.
    * @param rect Rectangle to initialize.  This is in output (view) space.
    *
    * @note Throws ossimException on error.
    */
   void getAreaOfInterest(const ossimImageSource* source, ossimIrect& rect) const;

   /**
    * @brief Method to calculate and initialize scale and area of interest
    * for making a thumbnail.
    *
    * Sets the scale of the output projection so that the adjusted rectangle
    * meets the cut rect and demension requirements set in options.
    *
    * @param originalRect Original scene area of interest.
    * @param adjustedRect New rect for thumbnail.
    *
    * @note Throws ossimException on error.
    */
   void initializeThumbnailProjection(const ossimIrect& originalRect,
                                      ossimIrect& adjustedRect);

   /** @return true if color table (lut) is set; false, if not. */
   bool hasLutFile() const;

   /**
    * @return true if any bump share options have been set by user; false,
    * if not.
    */
   bool hasBumpShadeArg() const;

   /** @return true if thumbnail option is set; false, if not. */
   bool hasThumbnailResolution() const;

   bool hasHistogramOperation() const;

   /** @return true if file extension is "hgt", "dem" or contains "dtN" (dted). */
   bool isDemFile(const ossimFilename& file) const;

   /** @return true if file extension is "src" */
   bool isSrcFile(const ossimFilename& file) const;

   /** @brief Initializes m_srcKwl if option was set. */
   void initializeSrcKwl();

   /**
    * @return The number of DEM_KW and IMG_KW found in the m_kwl and m_srcKwl
    * keyword list.
    */
   ossim_uint32 getNumberOfInputs() const;

   /**
    * @brief Gets the emumerated output projection type.
    *
    * This looks in m_kwl for ossimKeywordNames::PROJECTION_KW.
    * @return The enumerated output projection type.
    * @note This does not cover SRS keyword which could be any type of projection.
    */
   ossimDemOutputProjection getOutputProjectionType() const;

   /**
    * @brief Returns the scalar type from OUTPUT_RADIOMETRY_KW keyword if
    * present. Deprecated SCALE_2_8_BIT_KW is also checked.
    *
    * @return ossimScalarType Note this can be OSSIM_SCALAR_UNKNOWN if the
    * keywords are not present.
    */
   ossimScalarType getOutputScalarType() const;

   /** @return true if scale to eight bit option is set; false, if not. */
   bool scaleToEightBit() const;

   /**
    * @brief Adds application arguments to the argument parser.
    * @param ap Parser to add to.
    */
   void addArguments(ossimArgumentParser& ap);
   
   /** @brief Initializes arg parser and outputs usage. */
   void usage(ossimArgumentParser& ap);

   /** Enumerated operation to perform. */
   ossimDemOperation m_operation;
   
   /** Hold all options passed into intialize except writer props. */
   ossimRefPtr<ossimKeywordlist> m_kwl;

   /** Hold contents of src file if --src is used. */
   ossimRefPtr<ossimKeywordlist> m_srcKwl;

   /** The output (view) projection. */
   ossimRefPtr<ossimMapProjection> m_outputProjection;

   /**  Array of dem chains. */
   std::vector< ossimRefPtr<ossimSingleImageChain> > m_demLayer;

   /**  Array of image source chains. */
   std::vector< ossimRefPtr<ossimSingleImageChain> > m_imgLayer;
};

#endif /* #ifndef ossimElevUtil_HEADER */
