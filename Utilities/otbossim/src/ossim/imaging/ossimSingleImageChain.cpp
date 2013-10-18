//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: Utility class definition for a single image chain.
// 
//----------------------------------------------------------------------------
// $Id$

#include <ossim/imaging/ossimSingleImageChain.h>

#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/support_data/ossimSrcRecord.h>

ossimSingleImageChain::ossimSingleImageChain()
   :
   ossimImageChain(),
   m_handler(0),
   m_bandSelector(0),
   m_histogramRemapper(0),
   m_resamplerCache(0),
   m_resampler(0),
   m_scalarRemapper(0),
   m_chainCache(0),
   m_addHistogramFlag(false),
   m_addResamplerCacheFlag(false),
   m_addChainCacheFlag(false),
   m_remapToEightBitFlag(false),
   m_threeBandFlag(false),
   m_threeBandReverseFlag(false)
{
}

ossimSingleImageChain::ossimSingleImageChain(bool addHistogramFlag,
                                             bool addResamplerCacheFlag,
                                             bool addChainCacheFlag,
                                             bool remapToEightBitFlag,
                                             bool threeBandFlag,
                                             bool threeBandReverseFlag)
   :
   ossimImageChain(),
   m_handler(0),
   m_bandSelector(0),
   m_histogramRemapper(0),
   m_resamplerCache(0),
   m_resampler(0),
   m_scalarRemapper(0),
   m_chainCache(0),
   m_addHistogramFlag(addHistogramFlag),
   m_addResamplerCacheFlag(addResamplerCacheFlag),
   m_addChainCacheFlag(addChainCacheFlag),
   m_remapToEightBitFlag(remapToEightBitFlag),
   m_threeBandFlag(threeBandFlag),
   m_threeBandReverseFlag(threeBandReverseFlag)   
{
}

ossimSingleImageChain::~ossimSingleImageChain()
{
}

void ossimSingleImageChain::reset()
{
   bool result = true;
   do
   {
      result = deleteLast();
   } while (result);

   m_handler               = 0;
   m_bandSelector          = 0;
   m_histogramRemapper     = 0;
   m_resamplerCache        = 0;
   m_resampler             = 0;
   m_scalarRemapper        = 0;
   m_chainCache            = 0;
   m_addHistogramFlag      = false;
   m_addResamplerCacheFlag = false;
   m_addChainCacheFlag     = false;
   m_remapToEightBitFlag   = false;
   m_threeBandFlag         = false;
   m_threeBandReverseFlag  = false;
}

void ossimSingleImageChain::close()
{
   if ( m_handler.valid() )
   {
      if ( removeChild(m_handler.get()) )
      {
         m_handler = 0;
      }
   }
}

ossimFilename ossimSingleImageChain::getFilename() const
{
   ossimFilename result;
   if ( m_handler.valid() )
   {
      result = m_handler->getFilename();
   }
   return result;
}

bool ossimSingleImageChain::open(const ossimFilename& file, bool openOverview)
{
   return addImageHandler(file, openOverview);
}

bool ossimSingleImageChain::isOpen() const
{
   return m_handler.valid();
}

bool ossimSingleImageChain::open(const ossimSrcRecord& src)
{
   return addImageHandler(src);
}

void ossimSingleImageChain::createRenderedChain()
{
   // Band selector after image handler only if needed.
   if ( m_handler.valid() )
   {
      // Only add if multiple bands.
      if ( m_handler->getNumberOfOutputBands() != 1 )
      {
         addBandSelector();
         if ( m_threeBandReverseFlag )
         {
            setToThreeBandsReverse();
         }
      }
   }
   else // No image handler so just add it.
   {
      addBandSelector();
   }
   
   // histogram
   if ( m_addHistogramFlag )
   {
      addHistogramRemapper();
   }

   // resampler cache
   if ( m_addResamplerCacheFlag )
   {
      m_resamplerCache = addCache();
   }

   // scalar remapper
   if ( m_remapToEightBitFlag )
   {
      if ( m_handler.valid() )
      {
         // See if it's eight bit.
         if (m_handler->getOutputScalarType() != OSSIM_UINT8)
         {
            addScalarRemapper();
         }
      }
      else
      {
         // Just add...
         addScalarRemapper(); 
      }
   }
   
   // resampler
   addResampler();

   //---
   // Do this here so that if a band selector is added to the end of the
   // chain it will go in before the end of chain cache.
   //---
   if (m_threeBandFlag)
   {
      if (!m_bandSelector)
      {
         addBandSelector();
      }
      setToThreeBands();
   }

   //---
   // Ditto...
   //---
   if ( m_threeBandReverseFlag && !m_bandSelector )
   {
      addBandSelector();
      setToThreeBandsReverse();
   }

   // End of chain cache.
   if ( m_addChainCacheFlag )
   {
      m_chainCache = addCache();
   }

   initialize();

}

void ossimSingleImageChain::createRenderedChain(const ossimSrcRecord& src)
{
   // Band selector after image handler only if needed.
   if ( m_handler.valid() )
   {
      // Only add if needed.
      if ( ( m_handler->getNumberOfOutputBands() != 1 ) || src.getBands().size() )
      {
         addBandSelector(src);
         if ( m_threeBandReverseFlag )
         {
            setToThreeBandsReverse();
         }
      }
   }
   else // No image handler so just add it.
   {
      addBandSelector(src);
   }
   
   // histogram
   if ( m_addHistogramFlag || src.getHistogramOp().size() )
   {
      addHistogramRemapper(src);
   }

   // resampler cache
   if ( m_addResamplerCacheFlag )
   {
      m_resamplerCache = addCache();
   }

   // scalar remapper
   if ( m_remapToEightBitFlag )
   {
      if ( m_handler.valid() )
      {
         // See if it's eight bit.
         if (m_handler->getOutputScalarType() != OSSIM_UINT8)
         {
            addScalarRemapper();
         }
      }
      else
      {
         // Just add...
         addScalarRemapper(); 
      }
   }
   
   // resampler
   addResampler();

   //---
   // Do this here so that if a band selector is added to the end of the
   // chain it will go in before the end of chain cache.
   //---
   if (m_threeBandFlag)
   {
      if (!m_bandSelector) // Input must be one band.
      {
         addBandSelector(src);
      }
      setToThreeBands();
   }

   //---
   // Ditto...
   //---
   if ( m_threeBandReverseFlag && !m_bandSelector )
   {
      addBandSelector();
      setToThreeBandsReverse();
   }

   // End of chain cache.
   if ( m_addChainCacheFlag )
   {
      m_chainCache = addCache();
   }

   initialize();

}

bool ossimSingleImageChain::addImageHandler(const ossimFilename& file, bool openOverview)
{
   bool result = false;

   close();
   
   m_handler = ossimImageHandlerRegistry::instance()->open(file, true, openOverview);
   
   if ( m_handler.valid() )
   {
      // Add to the chain.  Note: last is really first.
      addLast( m_handler.get() );
      
      result = true;
   }

   return result;
}

bool ossimSingleImageChain::addImageHandler(const ossimSrcRecord& src)
{
   bool result = addImageHandler( src.getFilename() );
   if (result)
   {
      //---
      // When loading from ossimSrcRecord typically the overview/histograms are
      // not in the same directory and the "support" keyword is not set.  For
      // the ossimImageHandler::getFilenameWithThisExtension to work correctly
      // the ossimImageHandler::theSupplementaryDirectory must be set.
      // So if the ossimSrcRecord::getSupportDir() is empty and the overview
      // or histogram is not co-located with the image we will set it here.
      //---
      ossimFilename supportDir = src.getSupportDir();
      if ( supportDir.empty() )
      {
         if ( src.getOverviewPath().size() )
         {
            if ( src.getOverviewPath().isDir() )
            {
               supportDir = src.getOverviewPath();
            }
            else
            {
               supportDir = src.getOverviewPath().path();
            }
         }
         else if ( src.getHistogramPath().size() )
         {
            if ( src.getHistogramPath().isDir() )
            {
               supportDir = src.getHistogramPath();
            }
            else
            {
               supportDir = src.getHistogramPath().path();
            }
         }
         else if ( src.getMaskPath().size() )
         {
            if ( src.getMaskPath().isDir() )
            {
               supportDir = src.getMaskPath();
            }
            else
            {
               supportDir = src.getMaskPath().path();
            }
         }
      }
      if ( supportDir.size() && (src.getFilename().path() != supportDir) )
      {
         m_handler->setSupplementaryDirectory( supportDir );
      }
      if ( src.getEntryIndex() > 0 ) // defaulted to -1.
      {
         m_handler->setCurrentEntry( static_cast<ossim_uint32>( src.getEntryIndex() ) );
      }
      if ( m_handler->getOverview() == 0 )
      {
         if ( src.getOverviewPath().size() )
         {
            m_handler->openOverview( src.getOverviewPath() );
         }
         else
         {
            ossimFilename ovrFile = m_handler->getFilenameWithThisExtension(ossimString(".ovr"));
            m_handler->openOverview( ovrFile ); 
         }
      }
   }
   return result;
}

void ossimSingleImageChain::addBandSelector()
{
   if (!m_bandSelector)
   {
      m_bandSelector = new ossimBandSelector();

      // Add to the end of the chain.
      addFirst(m_bandSelector.get());
   }
}

void ossimSingleImageChain::addBandSelector(const ossimSrcRecord& src)
{
   if (!m_bandSelector)
   {
      m_bandSelector = new ossimBandSelector();
      
      // Add to the end of the chain.
      addFirst(m_bandSelector.get());
   }
   if ( src.getBands().size() )
   {
      m_bandSelector->setOutputBandList( src.getBands() );
   }
}

void ossimSingleImageChain::addHistogramRemapper()
{
   if (!m_histogramRemapper)
   {
      m_histogramRemapper = new ossimHistogramRemapper();
      
      m_histogramRemapper->setEnableFlag(false);

      // Add to the end of the chain.
      addFirst(m_histogramRemapper.get());
   } 
}

void ossimSingleImageChain::addHistogramRemapper(const ossimSrcRecord& src)
{
   static const char MODULE[] =
      "ossimSingleImageChain::addHistogramRemapper(const ossimSrcRecord&)";
   
   if (!m_histogramRemapper)
   {
      m_histogramRemapper = new ossimHistogramRemapper();
      
      m_histogramRemapper->setEnableFlag(false);

      // Add to the end of the chain.
      addFirst(m_histogramRemapper.get());
   }

   if ( src.getHistogramOp().size() && m_handler.valid() )
   {
      // Create histogram code here???
      
      // Open the histogram if needed.
      if ( m_histogramRemapper->getHistogramFile() == ossimFilename::NIL )
      {
         ossimFilename f;
         if ( src.getSupportDir().size() )
         {
            f = src.getSupportDir();
            f.dirCat( m_handler->getFilename().fileNoExtension() );
            f.setExtension(".his");
         }
         else
         {
            f = m_handler->getFilenameWithThisExtension( ossimString("his") );
         }
         if ( m_histogramRemapper->openHistogram( f ) == false )
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE << "\nCould not open:  " << f << "\n";
         }
      }

      // Set the histogram strech mode.
      if ( src.getHistogramOp().size() )
      {
         // Enable.
         m_histogramRemapper->setEnableFlag(true);
         
         // Set the histo mode:
         ossimString op = src.getHistogramOp();
         op.downcase();
         if ( op == "auto-minmax" )
         {
            m_histogramRemapper->setStretchMode( ossimHistogramRemapper::LINEAR_AUTO_MIN_MAX );
         }
         else if ( (op == "std-stretch-1") || (op == "std-stretch 1") )
         {
            m_histogramRemapper->setStretchMode( ossimHistogramRemapper::LINEAR_1STD_FROM_MEAN );
         } 
         else if ( (op == "std-stretch-2") || (op == "std-stretch 2") )
         {
            m_histogramRemapper->setStretchMode( ossimHistogramRemapper::LINEAR_2STD_FROM_MEAN );
         } 
         else if ( (op == "std-stretch-3") || (op == "std-stretch 3") )
         {
            m_histogramRemapper->setStretchMode( ossimHistogramRemapper::LINEAR_3STD_FROM_MEAN );
         }
         else
         {
            m_histogramRemapper->setEnableFlag(false);
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE << "\nUnhandled operation: " << op << "\n";
         }
      }
      
   } // End: if ( src.setHistogramOp().size() && m_handler.valid() )
}

ossimRefPtr<ossimCacheTileSource> ossimSingleImageChain::addCache()
{
   ossimRefPtr<ossimCacheTileSource> cache = new ossimCacheTileSource();

   // Add to the end of the chain.
   addFirst(cache.get());

   return cache;
}

void ossimSingleImageChain::addResampler()
{
   if ( !m_resampler )
   {
      m_resampler = new ossimImageRenderer();

      // Add to the end of the chain.
      addFirst(m_resampler.get());
   }
}

void ossimSingleImageChain::addScalarRemapper()
{
   if ( !m_scalarRemapper )
   {
      m_scalarRemapper = new ossimScalarRemapper();

      if ( m_resamplerCache.valid() )
      {
         // Add to the left of the resampler cache.
         insertLeft(m_scalarRemapper.get(), m_resamplerCache.get());
      }
      else
      {
         // Add to the end of the chain.
         addFirst(m_scalarRemapper.get());
      }
   }
}

ossimRefPtr<ossimImageHandler> ossimSingleImageChain::getImageHandler() const
{
   return m_handler;
}

ossimRefPtr<ossimImageHandler> ossimSingleImageChain::getImageHandler()
{
   return m_handler;
}

ossimRefPtr<ossimBandSelector> ossimSingleImageChain::getBandSelector() const
{
   return m_bandSelector;
}

ossimRefPtr<ossimBandSelector> ossimSingleImageChain::getBandSelector()
{
   return m_bandSelector;
}

ossimRefPtr<ossimHistogramRemapper> ossimSingleImageChain::getHistogramRemapper() const
{
   return m_histogramRemapper;
}

ossimRefPtr<ossimHistogramRemapper> ossimSingleImageChain::getHistogramRemapper()
{
   return m_histogramRemapper;
}

ossimRefPtr<ossimCacheTileSource> ossimSingleImageChain::getResamplerCache() const
{
   return m_resamplerCache;
}

ossimRefPtr<ossimCacheTileSource> ossimSingleImageChain::getResamplerCache()
{
   return m_resamplerCache;
}

ossimRefPtr<ossimImageRenderer> ossimSingleImageChain::getImageRenderer() const
{
   return m_resampler;
}

ossimRefPtr<ossimImageRenderer> ossimSingleImageChain::getImageRenderer()
{
   return m_resampler;
}

ossimRefPtr<ossimScalarRemapper> ossimSingleImageChain::getScalarRemapper() const
{
   return m_scalarRemapper;
}

ossimRefPtr<ossimScalarRemapper> ossimSingleImageChain::getScalarRemapper()
{
   return m_scalarRemapper;
}

ossimRefPtr<ossimCacheTileSource> ossimSingleImageChain::getChainCache() const
{
   return m_chainCache;
}

ossimRefPtr<ossimCacheTileSource> ossimSingleImageChain::getChainCache()
{
   return m_chainCache;
}

void ossimSingleImageChain::setAddHistogramFlag(bool flag)
{
   m_addHistogramFlag = flag;
}

bool ossimSingleImageChain::getAddHistogramFlag() const
{
   return m_addHistogramFlag;
}

void ossimSingleImageChain::setAddResamplerCacheFlag(bool flag)
{
   m_addResamplerCacheFlag = flag;
}

bool ossimSingleImageChain::getAddResamplerCacheFlag() const
{
   return m_addResamplerCacheFlag;
}

void ossimSingleImageChain::setAddChainCacheFlag(bool flag)
{
   m_addChainCacheFlag = flag;
}

bool ossimSingleImageChain::getAddChainCacheFlag() const
{
   return m_addChainCacheFlag;
}

void ossimSingleImageChain::setRemapToEightBitFlag(bool flag)
{
   m_remapToEightBitFlag = flag;
}

bool ossimSingleImageChain::getRemapToEightBitFlag() const
{
   return m_remapToEightBitFlag;
}

void ossimSingleImageChain::setThreeBandFlag(bool flag)
{
   m_threeBandFlag = flag;
}

bool ossimSingleImageChain::getThreeBandFlag() const
{
   return m_threeBandFlag;
}
   
void ossimSingleImageChain::setThreeBandReverseFlag(bool flag)
{
   m_threeBandReverseFlag = flag;
}

bool ossimSingleImageChain::getThreeBandReverseFlag() const
{
   return m_threeBandReverseFlag;
}

void ossimSingleImageChain::setToThreeBands()
{
   if ( m_handler.valid() )
   {
      std::vector<ossim_uint32> bandList(3);
      const ossim_uint32 BANDS = m_handler->getNumberOfInputBands();
      if(BANDS >= 3)
      {
         bandList[0] = 0;
         bandList[1] = 1;
         bandList[2] = 2;
      }
      else
      {
         bandList[0] = 0;
         bandList[1] = 0;
         bandList[2] = 0;
      }
      setBandSelection(bandList);
   }
}
   
void ossimSingleImageChain::setToThreeBandsReverse()
{
   if ( m_handler.valid() )
   {
      std::vector<ossim_uint32> bandList(3);
      const ossim_uint32 BANDS = m_handler->getNumberOfInputBands();
      if(BANDS >= 3)
      {
         bandList[0] = 2;
         bandList[1] = 1;
         bandList[2] = 0;
      }
      else
      {
         bandList[0] = 0;
         bandList[1] = 0;
         bandList[2] = 0;
      }
      setBandSelection(bandList);
   }
}

void ossimSingleImageChain::setBandSelection(
   const std::vector<ossim_uint32>& bandList)
{
   if (!m_bandSelector)
   {
      addBandSelector();
   }
   m_bandSelector->setEnableFlag(true);
   m_bandSelector->setOutputBandList(bandList);
   if ( m_histogramRemapper.valid() )
   {
      m_histogramRemapper->initialize();
   }
}
ossimScalarType ossimSingleImageChain::getImageHandlerScalarType() const
{
   ossimScalarType result = OSSIM_SCALAR_UNKNOWN;
   if ( m_handler.valid() )
   {
      result = m_handler->getOutputScalarType();
   }
   return result;
}

bool ossimSingleImageChain::openHistogram( ossimHistogramRemapper::StretchMode mode )
{
   bool result = false;
   ossimRefPtr<ossimImageHandler> ih = getImageHandler();
   if ( ih.valid() )
   {
      ossimRefPtr<ossimHistogramRemapper> hr = getHistogramRemapper();
      if ( hr.valid() )
      {
         ossimFilename f = ih->getFilenameWithThisExtension( ossimString("his") );
         if ( hr->openHistogram( f ) == true )
         {
            // Enable:
            hr->setEnableFlag(true);

            // Set the mode:
            hr->setStretchMode( mode );

            result = true;
         }
      }
   }
   return result;
}
