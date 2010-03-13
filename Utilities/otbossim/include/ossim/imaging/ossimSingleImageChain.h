//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: Utility class declaration for a single image chain.
// 
//----------------------------------------------------------------------------
// $Id$
#ifndef ossimSingleImageChain_HEADER
#define ossimSingleImageChain_HEADER 1

#include <vector>

#include <ossim/base/ossimConstants.h> /* OSSIM_DLL */
#include <ossim/imaging/ossimImageChain.h>

// Forward class declarations:
class ossimBandSelector;
class ossimCacheTileSource;
class ossimFilename;
class ossimHistogramRemapper;
class ossimImageHandler;
class ossimImageRenderer;
class ossimScalarRemapper;

/**
 * @brief Single image chain class.
 *
 * Typical usage:
 * ossimRefPtr<ossimSingleImageChain> myChain = new ossimSingleImageChain();
 * if ( myChain->open(myFile) == true )
 * {
 *    myChain->createRenderedChain();
 *    your-code-goes-here();
 * }
 */
class OSSIM_DLL ossimSingleImageChain : public ossimImageChain
{
public:

   /** default constructor */
   ossimSingleImageChain();

   /** virtual destructor */
   virtual ~ossimSingleImageChain();

   /**
    * @brief reset method
    * This deletes all links in the chain, zero's out all data members, and
    * sets all flags back to default.
    */
   void reset();
   
   /**
    * @brief open method
    *
    * Opens file and creates a simple chain with ossimImageHandler.
    *
    * @return true on success, false on error.
    *
    * @note This will close previous chain if one was opened.
    */
   bool open(const ossimFilename& file);

   /** @brief close method to delete the image handler. */
   void close();

   /** @return The filename of the image. */
   ossimFilename getFilename() const;

   /**
    * @brief Create a rendered image chain.
    *
    * Typical usage is to call this after "open" method returns true like:
    * if ( myChain->open(myFile) == true )
    * {
    *    myChain->createRenderedChain();
    *    code-goes-here();
    * }
    *
    * Typical chain is:
    * 
    * 1) image handler
    * 2) band selector (optional)
    * 3) histogram remapper
    * 4) scalar remapper (optional)
    * 5) resampler cache
    * 6) resampler
    * 7) band selector (optional when going one band to three)
    * 8) chain cache
    */
   void createRenderedChain();

   /**
    * @brief Adds an image handler for file.
    * @param file File to open.
    * @return true on success, false on error.
    */
   bool addImageHandler(const ossimFilename& file);
   
   /** @brief Adds a band selector. */
   void addBandSelector();

   void addHistogramRemapper();

   ossimCacheTileSource* addCache();

   void addResampler();

   void addScalarRemapper();

   /** @return Const point to image handler or 0 if not set. */
   const ossimImageHandler* getImageHandler() const;

   /** @return Pointer to the image handler or 0 if not set. */
   ossimImageHandler* getImageHandler();

   /** @return Const point to band selector or 0 if not set. */
   const ossimBandSelector* getBandSelector() const;

   /** @return Pointer to the band selector or 0 if not set. */
   ossimBandSelector* getBandSelector();

   /** @return Const pointer to histogram remapper or 0 if not set. */
   const ossimHistogramRemapper* getHistogramRemapper() const;

   /** @return Pointer to histogram remapper or 0 if not set. */
   ossimHistogramRemapper* getHistogramRemapper();

   /** @return Const pointer to resampler or 0 if not set. */
   const ossimImageRenderer* getImageRenderer() const;

   /** @return Pointer to resampler or 0 if not set. */
   ossimImageRenderer* getImageRenderer();

    /** @return Const pointer to scalar remapper or 0 if not set. */
   const ossimScalarRemapper* getScalarRemapper() const;

   /** @return Pointer to scalar remapper or 0 if not set. */
   ossimScalarRemapper* getScalarRemapper();

   /** @return Const pointer to or 0 if not set. */
   // const ossim* get() const;
   
   /** @return Pointer to or 0 if not set. */
   // ossim* get();

   /**
    * @brief Sets remap to eigth bit flag.
    * @param flag
    */
   void setRemapToEightBitFlag(bool flag);

   /**
    * @brief Get the remap to eight bit flag.
    * @return true or false.
    */
   bool getRemapToEightBitFlag() const;
   
   /**
    * @brief Sets the three band flag.
    *
    * If set will for a three band output.  So if one band it will duplicate
    * so that rgb = b1,b1,b1.
    * 
    * @param flag
    */
   void setThreeBandFlag(bool flag);

   /**
    * @brief Get the three band flag.
    * @return true or false.
    */
   bool getThreeBandFlag() const;
   
   /**
    * @brief Sets the three band reverse flag.
    *
    * @param flag
    */
   void setThreeBandReverseFlag(bool flag);

   /**
    * @brief Get the three band reverse flag.
    * @return true or false.
    */
   bool getThreeBandReverseFlag() const;

   /**
    * @brief Utility method to force 3 band output.
    *
    * Set band selector to a three band (rgb) output. If image has less than
    * three bands it will set to rgb = b1,b1,b1.  If image has three or
    * more bands the band selector will be see to rgb = b1, b2, b3.
    *
    * @note This will not work unless the image handler is initialized.
    */
   void setToThreeBands();
   
   /**
    * @brief Utility method to set to 3 bandsand reverse them.  This is
    * mainly used by NITF and Lndsat color data where the bands are in bgr
    * format and you want it in rgb combination.  If image has less than
    * three bands it will set to rgb = b1,b1,b1.  If image has three or
    * more bands the band selector will be see to rgb = b3, b2, b1.
    *
    * @note This will not work unless the image handler is initialized.
    */
   void setToThreeBandsReverse();

   /**
    * @brief method to set band selector.
    *
    * This will set the band selection to bandList.  If a band selector is
    * not in the chain yet it will be added.
    *
    * @param bandList The list of bands.
    */
   void setBandSelection(const std::vector<ossim_uint32>& bandList);

   /**
    * @brief Convenience method to return the scalar type of the image handler.
    * 
    * @return Scalar type of the image handler.
    *
    * This can return OSSIM_SCALAR_UNKNOWN if the image handler has not been
    * set yet.  Also, this is NOT the same as calling getOutputScalarType
    * which could have a different scalar type than the image if the
    * m_remapToEightBitFlag has been set.
    */
   ossimScalarType getImageHandlerScalarType() const;

private:

   /**  Pointers to links in chain. */
   ossimImageHandler*      m_handler;
   ossimBandSelector*      m_bandSelector;
   ossimHistogramRemapper* m_histogramRemapper;
   ossimCacheTileSource*   m_resamplerCache;
   ossimImageRenderer*     m_resampler;
   ossimScalarRemapper*    m_scalarRemapper;
   ossimCacheTileSource*   m_chainCache;

   /** control flags */
   bool m_remapToEightBitFlag;
   bool m_threeBandFlag;
   bool m_threeBandReverseFlag;
};

#endif /* #ifndef ossimSingleImageChain_HEADER */
