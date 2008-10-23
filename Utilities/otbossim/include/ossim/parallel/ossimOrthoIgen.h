// $Id: ossimOrthoIgen.h 10692 2007-04-11 02:23:27Z gpotts $
#ifndef ossimOrthoIgen_HEADER
#define ossimOrthoIgen_HEADER
#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimArgumentParser.h>
#include <ossim/base/ossimApplicationUsage.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimFilename.h>
class ossimConnectableObject;
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
         theEntry(-1)
         {}
      ossimOrthoIgenFilename(const ossimFilename& file, bool decodeEntry);
      ossimOrthoIgenFilename(const ossimFilename& file, ossim_int32 entryNumber)
         :theFilename(file),
         theEntry(entryNumber)
      {
      }
      void setFilenameWithDecoding(const ossimFilename& file);
      void setFilenameAndEntry(const ossimFilename& file,
                               ossim_int32 entry)
      {
         theFilename = file;
         theEntry    = entry;
      }
      ossimFilename theFilename;
      ossim_int32  theEntry;
   };
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
   bool execute();
   void setDefaultValues();

   void setCombinerType(const ossimString& combinerName);
   
   void setProjectionName(const ossimString& proj);
   void setResamplerType(const ossimString& resamplerType);

   
   void setCutCenter(const ossimDpt& dpt,
                     ossimUnitType unit = OSSIM_DEGREES);
   void setCutDxDy(const ossimDpt& dpt,
                   ossimUnitType unit = OSSIM_METERS);
   void setMetersPerPixel(const ossimDpt& mpp);
   void setThumbnailResolution(const ossimIpt& res);
   void setThumbnailFlag(bool flag);

   bool setupIgenKwl(ossimKeywordlist& kwl);
   
protected:
   ossimString theThumbnailRes;
   bool        theThumbnailFlag;
   ossimUnitType theDeltaPerPixelUnit;
   ossimDpt    theDeltaPerPixelOverride;
   ossimOrthoIgenProjectionType theProjectionType;
   ossimString                  theProjectionName;
   ossim_float64 theGeographicOriginOfLatitude;
   ossimString   theCombinerType;
   ossimString   theResamplerType;
   ossimFilename theTemplateView;
   ossimFilename theTilingTemplate;
   ossimFilename theTilingFilename;
   ossimFilename theChainTemplate;
   ossimFilename theCombinerTemplate;
   ossimFilename theAnnotationTemplate;
   ossimFilename theWriterTemplate;
   ossimString   theSlaveBuffers;
   ossimOrthoIgen::OriginType theCutOriginType;
   ossimDpt      theCutOrigin;
   ossimDpt      theCutDxDy;
   ossimUnitType theCutOriginUnit;
   ossimUnitType theCutDxDyUnit;
   ossim_float64 theLowPercentClip;
   ossim_float64 theHighPercentClip;
   bool          theUseAutoMinMaxFlag;
   bool          theScaleToEightBitFlag;
   
   std::vector<ossimOrthoIgenFilename> theFilenames;

   bool setupTiling(ossimKeywordlist& kwl);
   
   ossimRefPtr<ossimConnectableObject> setupCutter(ossimKeywordlist& kwl,
                                                   ossimConnectableObject* input);
   bool setupWriter(ossimKeywordlist& kwl,
                    ossimConnectableObject* input);
   bool setupView(ossimKeywordlist& kwl);

   /**
    * Sets up any annotation from --annotate option.
    *
    * @param kwl Main keyword list to save to.
    *
    * @param input Input connection of the annotation filter.
    *
    * @note This must be called after "setupView" as it needs a projection.
    */
   ossimRefPtr<ossimConnectableObject> setupAnnotation(
      ossimKeywordlist& kwl,
      ossimConnectableObject* input) const;
};

#endif
