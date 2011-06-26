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
#include <ossim/imaging/ossimBandSelector.h>
#include <ossim/imaging/ossimCacheTileSource.h>
#include <ossim/imaging/ossimHistogramRemapper.h>
#include <ossim/imaging/ossimImageChain.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimImageRenderer.h>
#include <ossim/imaging/ossimScalarRemapper.h>


// Forward class declarations:
class ossimFilename;
class ossimSrcRecord;

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
 *
 * Just a clarification on "start of chain" versus "end of chain" in this
 * file.
 *
 * Given chain of:
 * 1) image handler
 * 2) band selector (optional)
 * 3) histogram remapper(optional)
 * 4) scalar remapper (optional)
 * 5) resampler cache
 * 6) resampler
 * 7) band selector (optional when going one band to three)
 * 8) chain cache
 *
 * The "image handle" is the "start of chain".
 * The "chain cache" is the "end of chain".
 */
class OSSIM_DLL ossimSingleImageChain : public ossimImageChain
{
public:

   /** default constructor */
   ossimSingleImageChain();

   /** Constructor that takes flags.*/
   ossimSingleImageChain(bool addHistogramFlag,
                         bool addResamplerCacheFlag,
                         bool addChainCacheFlag,
                         bool remapToEightBitFlag,
                         bool threeBandFlag,
                         bool threeBandReverseFlag);
   
   /** virtual destructor */
   virtual ~ossimSingleImageChain();

   /**
    * @brief reset method
    * This deletes all links in the chain, zero's out all data members, and
    * sets all flags back to default.
    */
   void reset();
   
   /**
    * @brief open method that takes an image file.
    *
    * Opens file and creates a simple chain with ossimImageHandler.
    *
    * @param file File to open.

    * @param openOverview If true image handler will attempt to open overview.
    * Note that if you are planning on doing a rendered chain or want to go
    * between res levels you should set this to true.  default = true
    *
    * @return true on success, false on error.
    *
    * @note This will close previous chain if one was opened.
    */
   bool open(const ossimFilename& file, bool openOverview=true);

   /**
    * @brief open method that takes an ossimSrcRecord.
    *
    * Opens file and creates a simple chain with ossimImageHandler.
    *
    * @return true on success, false on error.
    *
    * @note This will close previous chain if one was opened.
    */
   bool open(const ossimSrcRecord& src);
   
   /** @return true if image handler is opened. */
   bool isOpen() const;

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
    * 3) histogram remapper(optional)
    * 4) scalar remapper (optional)
    * 5) resampler cache
    * 6) resampler
    * 7) band selector (optional when going one band to three)
    * 8) chain cache
    *
    * NOTES:
    * 1) If doing a sequential write where tiles to the right of the
    *    resampler will not be revisited the chain cache could be
    *    disabled to save memory.
    */
   void createRenderedChain();

   /**
    * @brief Create a rendered image chain that takes an ossimSrcRecord.
    */
   void createRenderedChain(const ossimSrcRecord& src);

   /**
    * @brief Adds an image handler for file.
    * 
    * @param file File to open.
    *
    * @param openOverview If true image handler will attempt to open overview.
    * Note that if you are planning on doing a rendered chain or want to go
    * between res levels you should set this to true. default = true
    *
    * @return true on success, false on error.
    */
   bool addImageHandler(const ossimFilename& file,
                        bool openOverview=true);

   /**
    * @brief Adds an image handler from src record.
    *
    * This take an ossimSrcRecord which can contain a supplemental directory
    * to look for overviews.
    * 
    * @param rec Record to open.
    * @return true on success, false on error.
    */
   bool addImageHandler(const ossimSrcRecord& src);
   
   /** @brief Adds a band selector to the end of the chain. */
   void addBandSelector();

   /**
    * @brief Adds a band selector.
    *
    * This takes an ossimSrcRecord which can contain a band selection list.
    * 
    * @param src Record to initialize band selector from.
    * 
    */
   void addBandSelector(const ossimSrcRecord& src);

   /** @brief Adds histogram remapper to the chain. */
   void addHistogramRemapper();

   /**
    * @brief Adds a band selector.
    *
    * This takes an ossimSrcRecord which can contain a histogram
    * operation to be performed.
    * 
    * @param src Record to initialize band selector from.
    */
   void addHistogramRemapper(const ossimSrcRecord& src);

   /**
    * @brief Adds a new cache to the current end of the chain.
    * @return Pointer to cache.
    */
   ossimRefPtr<ossimCacheTileSource> addCache();

   /** @brief Adds a resampler to the end of the chain. */ 
   void addResampler();

   /**
    * @brief Adds scalar remapper either to the left of the resampler cache
    * or at the end of the chain if not present.
    */
   void addScalarRemapper();

   /**
    * @return ossimRefPtr containing the image handler.
    * @note Can contain a null pointer so callers should validate.
    */
   ossimRefPtr<ossimImageHandler> getImageHandler() const;

   /**
    * @return ossimRefPtr containing the image handler.
    * @note Can contain a null pointer so callers should validate.
    */
   ossimRefPtr<ossimImageHandler> getImageHandler();

   /**
    * @return ossimRefPtr containing  the band selector.
    * @note Can contain a null pointer so callers should validate.
    */
   ossimRefPtr<ossimBandSelector> getBandSelector() const;

   /**
    * @return ossimRefPtr containing  the band selector.
    * @note Can contain a null pointer so callers should validate.
    */
   ossimRefPtr<ossimBandSelector> getBandSelector();

   /**
    * @return ossimRefPtr containing the histogram remapper.
    * @note Can contain a null pointer so callers should validate.
    */
   ossimRefPtr<ossimHistogramRemapper> getHistogramRemapper() const;

   /**
    * @return ossimRefPtr containing the histogram remapper.
    * @note Can contain a null pointer so callers should validate.
    */
   ossimRefPtr<ossimHistogramRemapper> getHistogramRemapper();

   /**
    * @return  ossimRefPtr containing the resampler cache.
    * @note Can contain a null pointer so callers should validate.
    */
   ossimRefPtr<ossimCacheTileSource> getResamplerCache() const;

   /**
    * @return  ossimRefPtr containing the resampler cache.
    * @note Can contain a null pointer so callers should validate.
    */
   ossimRefPtr<ossimCacheTileSource> getResamplerCache();

   /**
    * @return ossimRefPtr containing the resampler.
    * @note Can contain a null pointer so callers should validate.
    */
   ossimRefPtr<ossimImageRenderer> getImageRenderer() const;

   /**
    * @return ossimRefPtr containing the resampler.
    * @note Can contain a null pointer so callers should validate.
    */
   ossimRefPtr<ossimImageRenderer> getImageRenderer();

   /**
    * @return ossimRefPtr containing the scalar remapper.
    * @note Can contain a null pointer so callers should validate.
    */
   ossimRefPtr<ossimScalarRemapper> getScalarRemapper() const;

   /**
    * @return ossimRefPtr containing the scalar remapper.
    * @note Can contain a null pointer so callers should validate.
    */
   ossimRefPtr<ossimScalarRemapper> getScalarRemapper();

   /**
    * @return ossimRefPtr containing the chain cache.
    * @note Can contain a null pointer so callers should validate.
    */
   ossimRefPtr<ossimCacheTileSource> getChainCache() const;

   /**
    * @return ossimRefPtr containing the chain cache.
    * @note Can contain a null pointer so callers should validate.
    */
   ossimRefPtr<ossimCacheTileSource> getChainCache();

   /**
    * @brief If flag is true a histogram will be added to the chain at create time.
    * @param flag
    */
   void setAddHistogramFlag(bool flag);

   /**
    * @brief Gets the add histogram flag.
    * @return true or false.
    */
   bool getAddHistogramFlag() const;

   /**
    * @brief If flag is true a resampler cache will be added to the chain at create time.
    * This is a cache to the left of the resampler.
    * @param flag
    */
   void setAddResamplerCacheFlag(bool flag);

   /**
    * @brief Gets the add resampler cache flag.
    * @return true or false.
    */
   bool getAddResamplerCacheFlag() const;

   /**
    * @brief If flag is true a chain cache will be added to the chain at create time.
    * This is a cache at the end of the chain.
    * @param flag
    */
   void setAddChainCacheFlag(bool flag);

   /**
    * @brief Gets the add chain cache flag.
    * @return true or false.
    */
   bool getAddChainCacheFlag() const;

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
   ossimRefPtr<ossimImageHandler>      m_handler;
   ossimRefPtr<ossimBandSelector>      m_bandSelector;
   ossimRefPtr<ossimHistogramRemapper> m_histogramRemapper;
   ossimRefPtr<ossimCacheTileSource>   m_resamplerCache;
   ossimRefPtr<ossimImageRenderer>     m_resampler;
   ossimRefPtr<ossimScalarRemapper>    m_scalarRemapper;
   ossimRefPtr<ossimCacheTileSource>   m_chainCache;

   /** control flags */
   bool m_addHistogramFlag;
   bool m_addResamplerCacheFlag;
   bool m_addChainCacheFlag;
   bool m_remapToEightBitFlag;
   bool m_threeBandFlag;
   bool m_threeBandReverseFlag;

};

#endif /* #ifndef ossimSingleImageChain_HEADER */
