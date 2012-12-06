//----------------------------------------------------------------------------
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:  Base class for overview builders.
//
//----------------------------------------------------------------------------
// $Id: ossimOverviewBuilderBase.h 21653 2012-09-07 15:03:08Z dburken $
#ifndef ossimOverviewBuilderBase_HEADER
#define ossimOverviewBuilderBase_HEADER 1

#include <ossim/base/ossimSource.h>
#include <ossim/base/ossimProcessInterface.h>
#include <ossim/base/ossimConnectableObjectListener.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/base/ossimRtti.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimString.h>
#include <ossim/imaging/ossimBitMaskWriter.h>
#include <ossim/imaging/ossimMaskFilter.h>
#include <vector>

class ossimImageFileWriter;
class ossimFilename;

class OSSIM_DLL ossimOverviewBuilderBase
   :
      public ossimSource,
      public ossimProcessInterface,
      public ossimConnectableObjectListener
{
public:

   /** default constructor */
   ossimOverviewBuilderBase();

   /**
    * @brief Sets an optional file writer to the builder.
    * Currently used by the virtual overview builder.
    * 
    * @param outputWriter The file writer of the builder.
    * 
    * @return True on successful initialization, false on error.
    */
   virtual bool setOutputWriter(ossimImageFileWriter* outputWriter);

   /**
    * @brief Sets the input to the builder.
    * 
    * @param imageSource The input to the builder.
    * 
    * @return True on successful initialization, false on error.
    */
   virtual bool setInputSource(ossimImageHandler* imageSource);

   /**
    * @brief Sets the output file name. (pure virtual)
    * @prama file This will be the output file name like foo.ovr
    */
   virtual void setOutputFile(const ossimFilename& file)=0;

   /**
    * @brief Gets the output file name. (pure virtual)
    *
    * @return The output file name or ossimFilename::NIL if it was not set
    * yet and the image handle has not been initialized.
    *
    * @note This will return ossimFilename::NIL unless one of was called,
    * setInputSource or setOutputFile.
    */
   virtual ossimFilename getOutputFile() const=0;

   /**
    * @brief Sets the overview output type. (pure virtual)
    * @param type This should be the string representing the type.  This method
    * will do nothing if type is not handled and return false.
    * @return true if type is handled, false if not.

    * @note Currently handled types are 
    */
   virtual bool setOverviewType(const ossimString& type)=0;

   /**
    * @brief Gets the overview type. (pure virtual)
    * @return The overview output type as a string. */
   virtual ossimString getOverviewType() const=0;

   /**
    * @brief Method to check if builder can handle type.
    * @return true if type is handled by builder, false if not.
    */
   virtual bool hasOverviewType(const ossimString& type) const;

   /**
    * @brief Method to populate class supported types. (pure virtual)
    * Example:
    * ossimTiffOverviewNearest
    * ossimTiffOverviewBox
    *
    * @param typeList List of ossimStrings to add to.
    */
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const=0;

   /**
    * @brief Get the overview stop dimension.
    * @return The overview stop dimension.
    */
   virtual ossim_uint32 getOverviewStopDimension() const;

   /**
    * @brief Sets the overview stop dimension.
    *
    * This controls how many layers will be built. If set to 64 then the
    * builder will stop when height and width for current level are less
    * than or equal to 64.  Note a default can be set in the ossim preferences
    * file, setting the keyword "overview_stop_dimension".
    * 
    * @param dim The overview stop dimension
    */
   virtual void setOverviewStopDimension(ossim_uint32 dim);

   /**
    * @brief Gets the histogram accumulation mode.
    * @return mode NONE, NORMAL or FAST.
    */
   ossimHistogramMode getHistogramMode() const;

   /**
    * @brief Sets the histogram accumulation mode.
    * @param mode NONE, NORMAL or FAST.
    */
   void setHistogramMode(ossimHistogramMode mode);
 
   /**
    * @brief Builds the overviews. (pure virtual)
    *
    * @return true on success, false on error.
    */
   virtual bool execute()=0;

   /**
    * @brief Finalize method.  Should be called at end of execute.
    *
    * This implementation will reset the the output band list if the source
    * image handler is a band selector.
    */
   virtual void finalize();

   /**
    * @brief Specifies parameters (in KWL) for generation of an alpha (bit)
    * mask such that any full or partial null pixels will be masked out. A
    * mask file will be written to the source image directory with the image
    * file name and extension ".mask". 
    */
   void setBitMaskSpec(const ossimKeywordlist& bit_mask_spec);

   /**
    * @brief Turn on/off scan for min max flag.
    * This method assumes the null is known.
    * @param flag true turns scan on, false off. Default=off.
    */
   void setScanForMinMax(bool flag);

   /** @return scan for min max flag. */
   bool getScanForMinMax() const;

   /**
    * @brief Turn on/off scan for min, max, null flag.
    * Attempts to find null, min and max where null is the minimum value found,
    * min is the second most min and max is max.
    * @param flag true turns scan on, false off. Default=off.
    */
   void setScanForMinMaxNull(bool flag);

   /** @return scan for min max flag. */
   bool getScanForMinMaxNull() const;

protected:
   /** virtual destructor */
   virtual ~ossimOverviewBuilderBase();

   /**
    *  @brief Gets the required number of res levels.
    *
    *  Convenience method to get the required number of reduced resolution
    *  data sets to get to the smallest dimension of the output tile size.
    *  Note that this include r0.
    *
    *  @param ih Pointer to the image handler.
    *
    *  @return number of res levels.
    */
   ossim_uint32 getRequiredResLevels(const ossimImageHandler* ih) const;
  
   /**
    * @brief Initializes preference settings.
    *
    * Currently:
    *
    * 1) Gets the default stop dimension.
    *
    * Looks for overview_stop_dimension, then will use minimum default tile
    * size if that is not found.
    * 
    * 2) Set scan float data flag.
    *
    * Looks for overview_builder.scan_for_min_max_null_if_float.
    *
    * If true this will turn the m_scanForMinMaxNull on if the scalar type of
    * image handler is float.
    */
   void initializePreferenceSettings();

   /**
    * @brief Checks scalar type and turns on scanning for min, max, nulls if
    * needed.
    */
   void initializeScanOptions();
   
   ossim_uint32 m_overviewStopDimension;
   ossimHistogramMode m_histoMode; 
   ossimKeywordlist m_bitMaskSpec;
   ossimRefPtr<ossimImageHandler>  m_imageHandler;
   std::vector<ossim_uint32>       m_originalBandList;
   ossimRefPtr<ossimBitMaskWriter> m_maskWriter;
   ossimRefPtr<ossimMaskFilter>    m_maskFilter;
   ossimFilename                   m_outputFile;
   bool                            m_scanForMinMax;
   bool                            m_scanForMinMaxNull;
   bool                            m_scanFloatData;

   /** for rtti stuff */
   TYPE_DATA
};

#endif /* End of "#ifndef ossimOverviewBuilderBase_HEADER" */
