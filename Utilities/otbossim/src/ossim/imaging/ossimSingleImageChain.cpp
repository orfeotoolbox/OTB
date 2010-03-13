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
#include <ossim/imaging/ossimBandSelector.h>
#include <ossim/imaging/ossimCacheTileSource.h>
#include <ossim/imaging/ossimHistogramRemapper.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/imaging/ossimImageRenderer.h>
#include <ossim/imaging/ossimScalarRemapper.h>

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
   m_remapToEightBitFlag(false),
   m_threeBandFlag(false),
   m_threeBandReverseFlag(false)
{
}

ossimSingleImageChain::~ossimSingleImageChain()
{
   close();
}

void ossimSingleImageChain::reset()
{
   bool result = true;
   do
   {
      result = deleteLast();
   } while (result);

   m_handler = 0;
   m_bandSelector = 0;
   m_histogramRemapper = 0;
   m_resamplerCache = 0;
   m_resampler = 0;
   m_scalarRemapper = 0;
   m_chainCache = 0;
   m_remapToEightBitFlag = false;
   m_threeBandFlag = false;
   m_threeBandReverseFlag = false;
}

void ossimSingleImageChain::close()
{
   if (m_handler)
   {
      if ( removeChild(m_handler) )
      {
         m_handler = 0;
      }
   }
}

ossimFilename ossimSingleImageChain::getFilename() const
{
   ossimFilename result;
   if (m_handler)
   {
      result = m_handler->getFilename();
   }
   return result;
}

bool ossimSingleImageChain::open(const ossimFilename& file)
{
   return addImageHandler(file);
}

void ossimSingleImageChain::createRenderedChain()
{
   // Band selector after image handler only if needed.
   if (m_handler)
   {
      // Only add if multiple bands.
      if (m_handler->getNumberOfOutputBands() != 1)
      {
         addBandSelector();
      }
   }
   else // No image handler so just add it.
   {
      addBandSelector();
   }

   // histogram
   addHistogramRemapper();

   // resampler cache
   if ( !m_resamplerCache )
   {
      m_resamplerCache = addCache();
   }

   // scalar remapper
   if ( m_remapToEightBitFlag )
   {
      if (m_handler)
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
   if (m_threeBandReverseFlag)
   {
      if (!m_bandSelector)
      {
         addBandSelector();
      }
      setToThreeBandsReverse();
   }

   // End of chain cache.
   if ( !m_chainCache)
   {
      m_chainCache = addCache();
   }

   initialize();

}

bool ossimSingleImageChain::addImageHandler(const ossimFilename& file)
{
   bool result = false;

   close();
   
   ossimRefPtr<ossimImageHandler> ih =
      ossimImageHandlerRegistry::instance()->open(file);
   
   if (ih.valid())
   {
      // Capture the pointer.
      m_handler = ih.get();
      
      // Add to the chain.  Note: last is really first.
      addLast( m_handler );
      
      result = true;
   }

   return result;
}

void ossimSingleImageChain::addBandSelector()
{
   if (!m_bandSelector)
   {
      m_bandSelector = new ossimBandSelector();

      // Add to the end of the chain.
      addFirst(m_bandSelector);
   }
}

void ossimSingleImageChain::addHistogramRemapper()
{
   if (!m_histogramRemapper)
   {
      m_histogramRemapper = new ossimHistogramRemapper();
      
      m_histogramRemapper->setEnableFlag(false);

      // Add to the end of the chain.
      addFirst(m_histogramRemapper);
   } 
}

ossimCacheTileSource* ossimSingleImageChain::addCache()
{
   ossimCacheTileSource* cache = new ossimCacheTileSource();

   // Add to the end of the chain.
   addFirst(cache);

   return cache;
}

void ossimSingleImageChain::addResampler()
{
   if ( !m_resampler )
   {
      m_resampler = new ossimImageRenderer();

      // Add to the end of the chain.
      addFirst(m_resampler);
   }
}

void ossimSingleImageChain::addScalarRemapper()
{
   if ( !m_scalarRemapper )
   {
      m_scalarRemapper = new ossimScalarRemapper();

      if (m_resamplerCache)
      {
         // Add to the left of the resampler cache.
         insertLeft(m_scalarRemapper, m_resamplerCache);
      }
      else
      {
         // Add to the end of the chain.
         addFirst(m_scalarRemapper);
      }
   }
}

const ossimImageHandler* ossimSingleImageChain::getImageHandler() const
{
   return m_handler;
}

ossimImageHandler* ossimSingleImageChain::getImageHandler()
{
   return m_handler;
}

const ossimBandSelector* ossimSingleImageChain::getBandSelector() const
{
   return m_bandSelector;
}

ossimBandSelector* ossimSingleImageChain::getBandSelector()
{
   return m_bandSelector;
}

const ossimHistogramRemapper* ossimSingleImageChain::getHistogramRemapper()
   const
{
   return m_histogramRemapper;
}

ossimHistogramRemapper* ossimSingleImageChain::getHistogramRemapper()
{
   return m_histogramRemapper;
}

const ossimImageRenderer* ossimSingleImageChain::getImageRenderer() const
{
   return m_resampler;
}

ossimImageRenderer* ossimSingleImageChain::getImageRenderer()
{
   return m_resampler;
}

const ossimScalarRemapper* ossimSingleImageChain::getScalarRemapper() const
{
   return m_scalarRemapper;
}

ossimScalarRemapper* ossimSingleImageChain::getScalarRemapper()
{
   return m_scalarRemapper;
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
   if (m_handler)
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
   if (m_handler)
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
   if (m_histogramRemapper)
   {
      m_histogramRemapper->initialize();
   }
}
ossimScalarType ossimSingleImageChain::getImageHandlerScalarType() const
{
   ossimScalarType result = OSSIM_SCALAR_UNKNOWN;
   if (m_handler)
   {
      result = m_handler->getOutputScalarType();
   }
   return result;
}
