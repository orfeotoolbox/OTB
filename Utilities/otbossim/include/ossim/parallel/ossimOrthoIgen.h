//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
// 
// Description: Class declaration for ortho-image generator.
//
//*************************************************************************
// $Id: ossimOrthoIgen.h 20936 2012-05-18 14:45:00Z oscarkramer $

#ifndef ossimOrthoIgen_HEADER
#define ossimOrthoIgen_HEADER

#include <ossim/parallel/ossimIgen.h>
#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimConnectableContainer.h>
#include <ossim/base/ossimArgumentParser.h>
#include <ossim/base/ossimApplicationUsage.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/imaging/ossimImageChain.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/support_data/ossimSrcRecord.h>
#include <map>

class ossimConnectableObject;
class ossimMapProjection;
class ossimImageSource;
class ossimImageHandler;
class ossimImageCombiner;

class OSSIM_DLL ossimOrthoIgen : public ossimIgen
{
public:
   enum ossimOrthoIgenProjectionType
   {
      OSSIM_UNKNOWN_PROJECTION = 0,
      OSSIM_UTM_PROJECTION,
      OSSIM_SRS_PROJECTION,
      OSSIM_GEO_PROJECTION,
      OSSIM_INPUT_PROJECTION,
      OSSIM_EXTERNAL_PROJECTION
   };
   enum OriginType
   {
      OSSIM_CENTER_ORIGIN     = 0,
      OSSIM_UPPER_LEFT_ORIGIN = 1
   };
   typedef std::map<ossimString,ossimString, ossimStringLtstr> PropertyMap;
   
   ossimOrthoIgen();

   /**
    * Will add command line arguments to initialize this object from a command line application
    *
    * <pre>
    * Command Line Arguments:
    *
    * -t or --thumbnail  Takes an integer argument that is used to set both width
    *                    and height.
    *                    example: -t 1024 will produce an image that is 1024x1024
    * --meters           Takes a double precision argument that is used for meters per pixel
    *                    override.
    *                    example: --meters 5 will create a 5 meter product.
    * --slave-buffers    Takes an integer as an argument that specifies the number of slave tile buffers per
    *                    slave node.  The slave nodes are suppose to be asynch send.  It will
    *                    not wait for the send to finish.  It will got to the next buffer and check to
    *                    see if it's been sent and if not it will then wait for the buffer to be free.
    *                    example: --slave-buffers 10  will create 10 buffers per slave node.
    * --chain-template   Takes a file name that is a kewyord list that is used as a template for every
    *                    input file.
    *                    example:
    * 
    * @param argumentParser Argument parser object to add command line options to
    *
    */ 
   void addArguments(ossimArgumentParser& argumentParser);
   virtual void initialize(ossimArgumentParser& argumentParser);
   void addFiles(ossimArgumentParser& argumentParser,
                 bool withDecoding,
                 ossim_uint32 startIdx = 1);
   void clearFilenameList();

   //! Parses the .src file specified in the command line. These contain an alternate specification
   //! of input file and associated attributes as a KWL.
   void addSrcFile(const ossimFilename& fileName);

   bool execute();
   void setDefaultValues();

protected:
   //! Used to parse command line filename specs with piped switches (in lieu of *.src file).
   //! Returns TRUE if successful.
   bool parseFilename(const ossimString& file_spec, bool decodeEntry);

   //! Determines the UL corner tiepoint of the product projection as the overall UL corner of the
   //! mosaic.
   void establishMosaicTiePoint();

   ossimUnitType theDeltaPerPixelUnit;
   ossimDpt      theDeltaPerPixelOverride;
   ossimOrthoIgenProjectionType theProjectionType;
   ossimString   theProjectionName;
   ossimString   theCrsString;
   ossim_float64 theGeoScalingLatitude;
   ossimString   theCombinerType;
   ossimString   theResamplerType;
   ossimString   theWriterType;
   ossimFilename theTemplateView;
   ossimFilename theTilingTemplate;
   ossimFilename theTilingFilename;
   ossimFilename theChainTemplate;
   ossimFilename theCombinerTemplate;
   ossimFilename theAnnotationTemplate;
   ossimFilename theWriterTemplate;
   ossimFilename theSupplementaryDirectory;
   ossimString   theSlaveBuffers;
   OriginType    theCutOriginType;
   ossimDpt      theCutOrigin;
   ossimDpt      theCutDxDy;
   ossimUnitType theCutOriginUnit;
   ossimUnitType theCutDxDyUnit;
   ossim_float64 theLowPercentClip;
   ossim_float64 theHighPercentClip;
   ossim_int32   theStdDevClip;
   bool          theUseAutoMinMaxFlag;
   bool          theClipToValidRectFlag;
   PropertyMap   theReaderProperties;
   PropertyMap   theWriterProperties;   
   ossimFilename theTargetHistoFileName;
   std::vector<ossimSrcRecord> theSrcRecords;
   ossimFilename theProductFilename;
   ossimRefPtr<ossimProjection> theReferenceProj;
   ossimFilename theMaskShpFile;
   bool          theCacheExcludedFlag;
   ossimString   thePixelReplacementMode;
   ossim_float64 theClampPixelMin;
   ossim_float64 theClampPixelMax;
   ossim_float64 theClipPixelMin;
   ossim_float64 theClipPixelMax;
   ossimString   theOutputRadiometry;
   ossimPixelType thePixelAlignment;

   /**
   * @brief Sets up the igen keyword list for the process.
   *
   * This throws ossimException on error.
   */
   void setupIgenChain();

   bool setupTiling();
   void setupCutter();

   /** 
   * Checks for the presence of a raster mask file alongside the image, and inserts the mask 
   * filter in the chain if mask file exists. Returns with current_src pointing to masker object 
   */
   ossimImageSource* setupRasterMask(ossimImageChain* single_image_chain,
                        const ossimSrcRecord& src_record);

   /**
   * Insert a partial-pixel flipper to remap null-valued pixels to min according to info in the
   * source record and/or command line. Returns with current_src pointing to flipper object.
   */
   ossimImageSource* setupPixelFlipper(ossimImageChain* single_image_chain, 
                          const ossimSrcRecord& src_record);

   /**
    * @brief Set up the writer for the process.
    * This throws ossimException on error.
    */
   void setupWriter();

   /**
    * @brief Set up the output projection for the process.
    * This throws ossimException on error.
    */
   void setupProjection();

   /**
    * Sets up any annotation from --annotate option.
    * @note This must be called after "setupView" as it needs a projection.
    */
   void setupAnnotation();

   void addFiles(ossimString fileInfoStr, 
                 std::vector<ossimString> fileInfos,
                 bool withEncodedEntry);

   /**
   * Consolidates specification of bounding rect given various ways of specifying on the command
   * line. This avoids multiple, redundant checks scattered throughout the code.
   */
   void consolidateCutRectSpec();

   /**
   * Called when histogram operation is requested. Sets up additional filters in image chain
   * for performing matching, stretching or clipping. If chain=0,
   * this implies that the target histogram is being enabled for the output mosaic. */
   void setupHistogram(ossimImageChain* chain=0, const ossimSrcRecord& src_record=ossimSrcRecord());

   //! Utility method for creating a histogram for an input image. Returns TRUE if successful.
   bool createHistogram(ossimImageChain* chain, const ossimFilename& histo_filename);

   /**
    * @brief Adds cache to the left of resampler.
    * @param chain The chain to add to.
    */
   void addChainCache(ossimImageChain* chain) const;

   /**
   * @brief Generates a log KWL file that could be fed directly to Igen. Used for verifying chain.
   */
   void generateLog();
  
   /**
   * @brief Adds a scalar remapper to the extreme right of the chain is specified by the 
   * --output-radiometry option.
   */
   void setupOutputRadiometry();

   /**
    * @brief Checks all input image projections to see if elevation is needed.
    * @return true if affected, false if not.
    */
   bool isAffectedByElevation();

   /**
    * @brief Recomputes image gsds.
    *
    * Calls ossimImageGeometry::computeGsd() on all image handlers
    * that have projections affected by elevation.  Used to recompute after a
    * delayed load of elevation.
    */
   void reComputeChainGsds();

   /**
    * GSD Determination is nontrivial since there are various command-line options that control
    * this quantity. This method considers all information before setting the product's GSD.
    */
   void setProductGsd();
};
#endif
