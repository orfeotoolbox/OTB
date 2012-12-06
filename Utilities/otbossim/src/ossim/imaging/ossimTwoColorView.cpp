//-------------------------------------------------------------------
// License:  LGPL.  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
//-------------------------------------------------------------------
//  $Id$

#include <ossim/imaging/ossimTwoColorView.h>
#include <ossim/imaging/ossimImageDataFactory.h>

RTTI_DEF1(ossimTwoColorView,
          "ossimTwoColorView" ,
          ossimImageCombiner);

ossimTwoColorView::ossimTwoColorView()
   :
   ossimImageCombiner(0, 2, 0, true, false) ,
   m_byPassFlag(true),
   m_nativeFlag(false),
   m_newInput(0),
   m_oldInput(0),
   m_newInputBandIndex(0),
   m_oldInputBandIndex(0),
   m_redSource(ossimTwoColorView::OLD),
   m_grnSource(ossimTwoColorView::NEW),
   m_bluSource(ossimTwoColorView::NEW)
{
}

ossim_uint32 ossimTwoColorView::getNumberOfOutputBands() const
{
   if(m_byPassFlag||!isSourceEnabled())
   {
      return ossimImageCombiner::getNumberOfOutputBands();
   }
   return 3;
}

ossimScalarType ossimTwoColorView::getOutputScalarType() const
{
   if(m_byPassFlag||!isSourceEnabled())
   {
      return ossimImageCombiner::getOutputScalarType();
   }
   return OSSIM_UINT8;
}

void ossimTwoColorView::setBandIndexMapping(
   ossim_uint32 oldInputBandIndex,
   ossim_uint32 newInputBandIndex,
   ossimTwoColorMultiViewOutputSource redOutputSource,
   ossimTwoColorMultiViewOutputSource grnOutputSource,
   ossimTwoColorMultiViewOutputSource bluOutputSource)
{
   m_oldInputBandIndex = oldInputBandIndex;
   m_newInputBandIndex = newInputBandIndex;
   m_redSource = redOutputSource;
   m_grnSource = grnOutputSource;
   m_bluSource = bluOutputSource;
}

double ossimTwoColorView::getNullPixelValue(ossim_uint32 band)const
{
   if(m_byPassFlag||!isSourceEnabled())
   {
      return ossimImageCombiner::getNullPixelValue(band);
   }
   return 0;
}

double ossimTwoColorView::getMinPixelValue(ossim_uint32 band)const
{
   if(m_byPassFlag||!isSourceEnabled())
   {
      return ossimImageCombiner::getMinPixelValue(band);
   }
   return 1;
}

double ossimTwoColorView::getMaxPixelValue(ossim_uint32 band)const
{
   if(m_byPassFlag||!isSourceEnabled())
   {
      return ossimImageCombiner::getMaxPixelValue(band);
   }
   return 256;
}

ossimRefPtr<ossimImageData> ossimTwoColorView::getTile(const ossimIrect& rect,
                                                       ossim_uint32 resLevel)
{
   ossim_uint32 tileIdx = 0;
   if(m_byPassFlag||!isSourceEnabled())
   {
      return getNextTile(tileIdx, 0, rect, resLevel);
   }
   if(!m_twoColorTile.valid())
   {
      allocate();
   }
   if(!m_twoColorTile.valid())
   {
      return m_twoColorTile;
   }
   m_twoColorTile->setImageRectangle(rect);
   m_twoColorTile->makeBlank();
   
   ossimRefPtr<ossimImageData> newData = m_newInput->getTile(rect, resLevel);
   ossimRefPtr<ossimImageData> oldData = m_oldInput->getTile(rect, resLevel);

   runAlgorithm(newData.get(), oldData.get());

   m_twoColorTile->validate();

   return m_twoColorTile;
}
void ossimTwoColorView::runAlgorithm(ossimImageData* newData, ossimImageData* oldData)
{
   if(m_nativeFlag)
   {
      runNative8(newData, oldData);
   }
   else 
   {
      runNorm(newData, oldData);
   }
}

void ossimTwoColorView::runNative8(ossimImageData* newData, ossimImageData* oldData)
{
   if ( newData && oldData && m_twoColorTile.valid() &&
        ( m_twoColorTile->getNumberOfBands() == 3 ) )
   {
      // old, new, red, green, blue buffers...
      ossim_uint8* o = static_cast<ossim_uint8*>( oldData->getBuf(m_oldInputBandIndex) );
      ossim_uint8* n = static_cast<ossim_uint8*>( newData->getBuf(m_newInputBandIndex) );
      ossim_uint8* r = static_cast<ossim_uint8*>( m_twoColorTile->getBuf(0) );
      ossim_uint8* g = static_cast<ossim_uint8*>( m_twoColorTile->getBuf(1) );
      ossim_uint8* b = static_cast<ossim_uint8*>( m_twoColorTile->getBuf(2) );

      if ( o && n && r && g && b )
      {
         // Assuming null pix of 0 for 8 bit.
         const ossim_uint8 MP = 1;
         const ossim_uint8 NP = 0;

         ossim_uint8 newPix = 0;
         ossim_uint8 oldPix = 0;
         
         const ossim_uint32 MAX_IDX = m_twoColorTile->getSizePerBand();
         
         for(ossim_uint32 idx = 0; idx < MAX_IDX; ++idx)
         {
            if( ( *n == NP ) && ( *o == NP ) )
            {
               // Both inputs null, set all outputs null.
               *r = NP;
               *b = NP;
               *g = NP;
            }
            else 
            {
               // At least one input is not null.
               newPix = (*n != NP) ? *n : MP;
               oldPix = (*o != NP) ? *o : MP;

               // Set red, OLD is default so check first:
               if ( m_redSource == ossimTwoColorView::OLD )
               {
                  *r = oldPix;
               }
               else if ( m_redSource == ossimTwoColorView::NEW )
               {
                  *r = newPix;
               }
               else
               {
                  *r = MP;
               }

               // Set green, NEW is default so check first:
               if ( m_grnSource == ossimTwoColorView::NEW )
               {
                  *g = newPix;
               }
               else if ( m_grnSource == ossimTwoColorView::OLD )
               {
                  *g = oldPix;
               }
               else
               {
                  *g = MP;
               }

               // Set blue, NEW is default so check first:
               if ( m_grnSource == ossimTwoColorView::NEW )
               {
                  *b = newPix;
               }
               else if ( m_grnSource == ossimTwoColorView::OLD )
               {
                  *b = oldPix;
               }
               else
               {
                  *b = MP;
               }
            }

            // Next pixel:
            ++n;
            ++o;
            ++r;
            ++g;
            ++b;
         }
      }
   }
}

void ossimTwoColorView::runNorm(ossimImageData* newData, ossimImageData* oldData)
{
   if ( newData && oldData && m_twoColorTile.valid() &&
        ( m_twoColorTile->getNumberOfBands() == 3 ) )
   {
      const ossim_uint32 MAX_IDX = m_twoColorTile->getSizePerBand();
      
      // Buffers for normalized oldData and newData tiles.
      std::vector<ossim_float32> oldDataBuffer(MAX_IDX);
      std::vector<ossim_float32> newDataBuffer(MAX_IDX);

      // old and new input buffers.
      ossim_float32* o = &oldDataBuffer.front();      
      ossim_float32* n = &newDataBuffer.front();

      // Normalize/copy to buffers.
      newData->copyTileBandToNormalizedBuffer(m_newInputBandIndex, n);
      oldData->copyTileBandToNormalizedBuffer(m_oldInputBandIndex, o);
      
      // Get the output buffers.
      ossim_uint8* r = static_cast<ossim_uint8*>( m_twoColorTile->getBuf(0) );
      ossim_uint8* g = static_cast<ossim_uint8*>( m_twoColorTile->getBuf(1) );
      ossim_uint8* b = static_cast<ossim_uint8*>( m_twoColorTile->getBuf(2) );

      if ( o && n && r && g && b )
      {
         // Assuming null pix of 0 for 8 bit.
         const ossim_uint8 MP = 1;
         const ossim_uint8 NP = 0;

         ossim_uint8   newPix = 0;
         ossim_uint8   oldPix = 0;
         ossim_float32 tmpPix = 0.0;
         
         const ossim_uint32 MAX_IDX = m_twoColorTile->getSizePerBand();
         
         for(ossim_uint32 idx = 0; idx < MAX_IDX; ++idx)
         {
            if( ( *n == NP ) && ( *o == NP ) )
            {
               // Both inputs null, set all outputs null.
               *r = NP;
               *b = NP;
               *g = NP;
            }
            else 
            {
               // At least one input is not null.
               
               // Set the newPix:
               if ( *n != NP )
               {
                  // Un-normalize:
                  tmpPix = (*n) * 255.0;

                  // Clamp to min/max.
                  tmpPix = (tmpPix <= 255.0) ? ( (tmpPix >= 1.0) ? tmpPix : 1.0) : 255.0;

                  // Copy
                  newPix = static_cast<ossim_uint8>( tmpPix );
               }
               else
               {
                  newPix = MP;
               }

               // Set the oldPix:
               if ( *o != NP )
               {
                  // Un-normalize:
                  tmpPix = (*o) * 255.0;

                  // Clamp to min/max.
                  tmpPix = (tmpPix <= 255.0) ? ( (tmpPix >= 1.0) ? tmpPix : 1.0) : 255.0;

                  // Copy
                  oldPix = static_cast<ossim_uint8>( tmpPix );
               }
               else
               {
                  oldPix = MP;
               }

               // Set red, OLD is default so check first:
               if ( m_redSource == ossimTwoColorView::OLD )
               {
                  *r = oldPix;
               }
               else if ( m_redSource == ossimTwoColorView::NEW )
               {
                  *r = newPix;
               }
               else
               {
                  *r = MP;
               }

               // Set green, NEW is default so check first:
               if ( m_grnSource == ossimTwoColorView::NEW )
               {
                  *g = newPix;
               }
               else if ( m_grnSource == ossimTwoColorView::OLD )
               {
                  *g = oldPix;
               }
               else
               {
                  *g = MP;
               }

               // Set blue, NEW is default so check first:
               if ( m_grnSource == ossimTwoColorView::NEW )
               {
                  *b = newPix;
               }
               else if ( m_grnSource == ossimTwoColorView::OLD )
               {
                  *b = oldPix;
               }
               else
               {
                  *b = MP;
               }
            }

            // Next pixel:
            ++n;
            ++o;
            ++r;
            ++g;
            ++b;
         }
      }
   }
}

void ossimTwoColorView::allocate()
{
   m_twoColorTile = ossimImageDataFactory::instance()->create(this, this);
   if(m_twoColorTile.valid())
   {
      m_twoColorTile->initialize();
   }
}


void ossimTwoColorView::initialize()
{
   ossimImageCombiner::initialize();
   m_newInput = 0;
   m_oldInput = 0;
   m_twoColorTile = 0;
   m_nativeFlag = false;
   m_byPassFlag = false;

   if(getNumberOfInputs() < 2)
   {
      m_byPassFlag = true;
   }
   else 
   {
      m_oldInput = dynamic_cast<ossimImageSource*>( getInput(0) );
      m_newInput = dynamic_cast<ossimImageSource*>( getInput(1) );

      //---
      // Range check band selection. This can be set from setBandIndexMapping method which
      // does no error checking because inputs may not be set.
      //----
      if ( m_oldInput.valid() )
      {
         if ( m_oldInputBandIndex >= m_oldInput->getNumberOfOutputBands() )
         {
            m_oldInputBandIndex = 0;
         }
      }
      
      if ( m_newInput.valid() )
      {
         if ( m_newInputBandIndex >= m_newInput->getNumberOfOutputBands() )
         {
            m_newInputBandIndex = 0;
         }
      }
         
      if(!m_newInput||!m_oldInput)
      {
         m_byPassFlag = true;
      }
      else
      {
         if((m_newInput->getOutputScalarType() == OSSIM_UINT8)&&
            (m_oldInput->getOutputScalarType() == OSSIM_UINT8))
         {
            m_nativeFlag = true;
         }
      }
   }
}
