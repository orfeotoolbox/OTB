// $Id: ossimOrthoIgen.h 17543 2010-06-07 21:26:25Z dburken $
#ifndef ossimOrthoIgen_HEADER
#define ossimOrthoIgen_HEADER
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
#include <map>

class ossimConnectableObject;
class ossimMapProjection;
class ossimImageSource;
class ossimImageHandler;

class OSSIM_DLL ossimOrthoIgen : public ossimReferenced
{
public:
   enum ossimOrthoIgenProjectionType
   {
      OSSIM_UNKNOWN_PROJECTION = 0,
      OSSIM_UTM_PROJECTION,
      OSSIM_GEO_PROJECTION,
      OSSIM_INPUT_PROJECTION,
      OSSIM_EXTERNAL_PROJECTION
   };
   enum OriginType
   {
      OSSIM_CENTER_ORIGIN     = 0,
      OSSIM_UPPER_LEFT_ORIGIN = 1
   };
   class ossimOrthoIgenFilename
   {
   public:
      ossimOrthoIgenFilename()
         :theFilename(""),
          theEntry(-1),
          theSupplementaryDir(""),
          theBands()
         {}
      ossimOrthoIgenFilename(const ossimFilename& file, bool decodeEntry);
      ossimOrthoIgenFilename(const ossimFilename& file, ossim_int32 entryNumber)
         :theFilename(file),
          theEntry(entryNumber),
          theSupplementaryDir(),
          theBands()
      {
      }
      void setFilenameWithDecoding(const ossimFilename& file);
      void setFilenameAndEntry(const ossimFilename& file,
                               ossim_int32 entry);
      ossimFilename theFilename;
      ossim_int32   theEntry;
      ossimFilename theSupplementaryDir;
      std::vector<ossim_uint32> theBands;
   };
   typedef std::map<ossimString,ossimString> PropertyMap;
   
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
   void initialize(ossimArgumentParser& argumentParser);
   void addFiles(ossimArgumentParser& argumentParser,
                 bool withDecoding,
                 ossim_uint32 startIdx = 1);
   void clearFilenameList();
   void addFile(const ossimFilename& file, bool withDecoding=false);
   void addFile(const ossimString& fileName, bool withDecoding=false);
   bool execute();
   void setDefaultValues();

protected:
   ossimString   theThumbnailRes;
   bool          theThumbnailFlag;
   ossimUnitType theDeltaPerPixelUnit;
   ossimDpt      theDeltaPerPixelOverride;
   ossimOrthoIgenProjectionType theProjectionType;
   ossimString   theProjectionName;
   ossimString   theSrsString;
   ossim_float64 theGeographicOriginOfLatitude;
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
   bool          theScaleToEightBitFlag;
   bool          theStdoutFlag;
   PropertyMap   theWriterProperties;
   bool          theCutRectSpecIsConsolidated;
   ossimFilename theTargetHistoFileName;
   ossimRefPtr<ossimConnectableContainer> theContainer;
   ossimRefPtr<ossimMapProjection>  theProductProjection;
   ossimRefPtr<ossimImageChain>  theProductChain;
   ossimKeywordlist theKwl;
   std::vector<ossimOrthoIgenFilename> theFilenames;
  
   /**
    * @brief Sets up the igen keyword list for the process.
    *
    * This throws ossimException on error.
    */
   void setupIgenKwl();
   
   bool setupTiling();
   void setupCutter();
   
   /**
    * @brief Set up the writer for the process.
    *
    * This throws ossimException on error.
    */
   void setupWriter();
   
   /**
    * @brief Set up the view for the process.
    *
    * This throws ossimException on error.
    */
   void setupView();
   
   /**
    * Sets up any annotation from --annotate option.
    * @note This must be called after "setupView" as it needs a projection.
    */
   void setupAnnotation();
   
   void addFiles(ossimString fileInfoStr, 
                 std::vector<ossimString> fileInfos,
                 bool withEncodedEntry);
   
   /**
    * Implemented to fix the misalignment between the input projection and the product. This was
    * due to an UL corner in the product that was not an integral distance (in pixels) from the UL
    * corner of the input image (assuming single input). OLK 3/10
    *
    * @param inproj Input source map projection.
    *
    * @param outproj Product projection that will receive adjusted tiepoint (UL corner).
    */
   void snapTiePointToInputProj(ossimMapProjection* inproj);
   
   /**
    * Consolidates specification of bounding rect given various ways of specifying on the command
    * line. This avoids multiple, redundant checks scattered throughout the code.
    */
   void consolidateCutRectSpec();
   
   /**
    * Called when histogram operation is requested. Sets up additional filters in image chain
    * for performing matching, stretching or clipping. If chain=0,
    * this implies that the target histogram is being enabled for the output mosaic. */
   void setupHistogram(ossimImageChain* chain=0, ossimImageHandler* handler=0);
   
   /**
    * @brief Adds cache to the left of resampler.
    * @param chain The chain to add to.
    */
   void addChainCache(ossimImageChain* chain) const;
   
};

#endif
