//*******************************************************************
// Copyright (C) 2003 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:
//
// Table remapper class definition.
//
// Takes input tile, remaps it through a table, then output tile in the desired
// scalar type.
//
// Two modes one that works on native remap tables, that being of the same
// scalar type (like ossim_uint8) of the input connection, and another that
// uses a normalized remap table (more scalar independent).
//
//*************************************************************************
// $Id: ossimTableRemapper.cpp,v 1.20 2005/09/09 14:28:19 gpotts Exp $

#include <imaging/tile_sources/ossimTableRemapper.h>
#include <base/common/ossimTrace.h>
#include <base/misc/lookup_tables/ossimScalarTypeLut.h>
#include <base/context/ossimNotifyContext.h>
#include <imaging/factory/ossimImageDataFactory.h>

RTTI_DEF1(ossimTableRemapper, "ossimTableRemapper", ossimImageSourceFilter)

static ossimTrace traceDebug("ossimTableRemapper:debug");

static const char* TABLE_TYPE[] = { "UNKNOWN", "NATIVE", "NORMALIZED" };

ossimTableRemapper::ossimTableRemapper(ossimObject* owner)
   :
      ossimImageSourceFilter(owner),  // base class
      theTile(NULL),
      theTmpTile(NULL),
      theTable(NULL),
      theNormBuf(NULL),
      theTableBinCount(0),
      theTableBandCount(0),
      theTableType(ossimTableRemapper::UKNOWN),
      theInputScalarType(OSSIM_SCALAR_UNKNOWN),
      theOutputScalarType(OSSIM_SCALAR_UNKNOWN),
      theTableOwnerFlag(true)
{
   //***
   // Set the base class "theEnableFlag" to off since no adjustments have been
   // made yet.
   //***
   disableSource();
}


ossimTableRemapper::ossimTableRemapper(ossimImageSource* inputSource)
   :
      ossimImageSourceFilter      (NULL, inputSource),  // base class
      theTile(NULL),
      theTmpTile(NULL),
      theTable(NULL),
      theNormBuf(NULL),
      theTableBinCount(0),
      theTableBandCount(0),
      theTableType(ossimTableRemapper::UKNOWN),
      theInputScalarType(OSSIM_SCALAR_UNKNOWN),
      theOutputScalarType(OSSIM_SCALAR_UNKNOWN),
      theTableOwnerFlag(true)
{
   //***
   // Set the base class "theEnableFlag" to off since no adjustments have been
   // made yet.
   //***
   disableSource();
}


ossimTableRemapper::ossimTableRemapper(ossimObject* owner,
                                       ossimImageSource* inputSource)
   :
      ossimImageSourceFilter      (owner, inputSource),  // base class
      theTile(NULL),
      theTmpTile(NULL),
      theTable(NULL),
      theNormBuf(NULL),
      theTableBinCount(0),
      theTableBandCount(0),
      theTableType(ossimTableRemapper::UKNOWN),
      theInputScalarType(OSSIM_SCALAR_UNKNOWN),
      theOutputScalarType(OSSIM_SCALAR_UNKNOWN),
      theTableOwnerFlag(true)
{
   //***
   // Set the base class "theEnableFlag" to off since no adjustments have been
   // made yet.
   //***
   disableSource();
}

ossimTableRemapper::~ossimTableRemapper()
{
   destroy();
}

void ossimTableRemapper::destroy()
{
   if (theNormBuf)
   {
      delete [] theNormBuf;
      theNormBuf = NULL;
   }
   if (theTableOwnerFlag && theTable)
   {
      delete [] theTable;
      theTable = NULL;
   }
   theTmpTile = NULL;
   theTile    = NULL;
}

void ossimTableRemapper::initialize()
{
   //---
   // Call the base class initialize.
   // Note:  This will reset "theInputConnection" if it changed...
   //---
   ossimImageSourceFilter::initialize();
   if (theInputConnection)
   {
      // Since we override "getOutputScalarType" make sure something's set.
      if (theOutputScalarType == OSSIM_SCALAR_UNKNOWN)
      {
         theOutputScalarType = theInputConnection->getOutputScalarType();
      }

      if (theTile.valid())
      {
         if ( theTile->getNumberOfBands() !=
              theInputConnection->getNumberOfOutputBands() )
         {
            // Wipe everything slick. The next getTile will call allocate.
            destroy();
         }
      }
   }

   // Nothing else to do for this...
}

void ossimTableRemapper::allocate(const ossimIrect& rect)
{
   //---
   // It's either first getTile or we've had either a resize, so allocate
   // needed stuff...
   //---
   destroy();

   if (theInputConnection)
   {
      ossim_uint32 width  = rect.width();
      ossim_uint32 height = rect.height();
      theInputScalarType  = theInputConnection->getOutputScalarType();
      
      theTile =
         ossimImageDataFactory::instance()->create(this,
                                                   getOutputScalarType(),
                                                   getNumberOfOutputBands(),
                                                   width,
                                                   height);

      theTile->initialize();

      if (theInputScalarType !=  getOutputScalarType() &&
          theTableType == ossimTableRemapper::NATIVE)
      {
         //---
         // We need a temp tile so we can do two remaps
         // inputTile->remapTable->tmpTile then
         // tmpTile->normalize->theTile(unnormalize to new scalar)...
         //---
         theTmpTile 
         = ossimImageDataFactory::instance()->create(this,
                                                     theInputScalarType,
                                                     getNumberOfOutputBands(),
                                                     width,
                                                     height);
         theTmpTile->initialize();
      }

      if (theTableType == ossimTableRemapper::NORMALIZED ||
          theInputScalarType != theOutputScalarType)
      {
         theNormBuf = new ossim_float64[theTile->getSize()];
      }
   }
}

ossimRefPtr<ossimImageData> ossimTableRemapper::getTile(
   const ossimIrect& tile_rect,
   ossim_uint32 resLevel)
{
   if(!theInputConnection)
   {
      return ossimRefPtr<ossimImageData>();
   }

   // Fetch tile from pointer from the input source.
   ossimRefPtr<ossimImageData> inputTile =
      theInputConnection->getTile(tile_rect, resLevel);

   if (!inputTile.valid())  // Just in case...
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimTableRemapper::getTile ERROR:"
            << "\nReceived null pointer to tile from input source!"
            << "\nReturning blank tile.\n";
      }
      return inputTile;
   }

   // Get its status.
   ossimDataObjectStatus tile_status = inputTile->getDataObjectStatus();

   // Check for remap bypass:
   if ( !theEnableFlag ||
        tile_status == OSSIM_NULL ||
        tile_status == OSSIM_EMPTY )
   {
      return inputTile;
   }

   // OK we have an input tile... and it's not null or empty.

   // Check for first time through or size or band change.
   if ( !theTile ||  
        tile_rect.height() != theTile->getHeight() ||
        tile_rect.width()  != theTile->getWidth()  ||
        inputTile->getNumberOfBands() != theTile->getNumberOfBands())
   {
      allocate(tile_rect);
   }

   // Set the origin of the output tile.
   theTile->setImageRectangle(tile_rect);
   
   // Think things are good.  Do the real work...
   if (theTableType == ossimTableRemapper::NATIVE)
   {
      // Most efficient case...
      remapFromNativeTable(inputTile);
   }
   else
   {
      remapFromNormalizedTable(inputTile);
   }
   
   theTile->validate();

   return theTile;
}

void ossimTableRemapper::remapFromNativeTable(
   ossimRefPtr<ossimImageData>& inputTile)
{
   switch (theOutputScalarType)
   {
      case OSSIM_UCHAR:
      {
         remapFromNativeTable(ossim_uint8(0), inputTile);
         break;
      }
      
      case OSSIM_USHORT11:
      case OSSIM_USHORT16:
      {
         remapFromNativeTable(ossim_uint16(0), inputTile);
         break;
      }
      
      case OSSIM_SSHORT16:
      {
         remapFromNativeTable(ossim_sint16(0), inputTile);
         break;
      }
      
      case OSSIM_NORMALIZED_FLOAT:
      case OSSIM_FLOAT:
      {
         remapFromNativeTable(ossim_float32(0), inputTile);
         break;
      }
      
      case OSSIM_NORMALIZED_DOUBLE:
      case OSSIM_DOUBLE:
      {
         remapFromNativeTable(ossim_float64(0), inputTile);
         break;
      }
      
      case OSSIM_SCALAR_UNKNOWN:
      default:
      {
         // Shouldn't hit this.
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimTableRemapper::remapFromNativeTable"
            << "\nOutput scalar type is OSSIM_SCALAR_UNKNOWN!\n";
         break;
      }

   } // End of "switch (theTableType)"
}

template <class T> void ossimTableRemapper::remapFromNativeTable(
   T /* dummy */,
   ossimRefPtr<ossimImageData>& inputTile)
{
   
   const ossim_uint32 BANDS = theTile->getNumberOfBands();
   const ossim_uint32 PPB   = theTile->getSizePerBand(); // pixels per band

   //---
   // NOTE:
   // If (theTableBandCount == one) that means we only have one band; or,
   // the user wanted to remap all bands using the same table...
   //---
   const ossim_uint32 BAND_OFFSET =
      (theTableBandCount != 1) ? theTableBinCount: 0;

   const T* rt = reinterpret_cast<T*>(theTable); // remap table (rt)
   ossimRefPtr<ossimImageData> id = NULL;
   if (theInputScalarType == theOutputScalarType)
   {
      id = theTile;
   }
   else
   {
      id = theTmpTile;
   }
   
   for (ossim_uint32 band = 0; band < BANDS; ++band)
   {
      const T NULL_PIX = static_cast<T>(id->getNullPix(band));
      const T MIN_PIX  = static_cast<T>(id->getMinPix(band));

      const T* s = static_cast<T*>(inputTile->getBuf(band)); // source (s)
      T*       d = static_cast<T*>(id->getBuf(band));   // destination (d)

      for (ossim_uint32 pixel = 0; pixel < PPB; ++pixel)
      {
         //---
         // Get the source pixel and use it as an index to the table.
         // Note Null is always the "0" index, min always the "1".
         //---
         T p = s[pixel];
         if (p == NULL_PIX)  // Null is not always zero (dted).
         {
            p = 0;
         }
         else
         {
            p = (p+1-MIN_PIX);  // If min == 1 we want that to map to index 1.
         }

         ossim_uint32 table_index = static_cast<ossim_uint32>(p);

         //---
         // If within range use to index the remap table; else, null.
         // Note:
         // There is no min, max range checking on value retrieved from table.
         // Range checking should be performed when the table is built.
         //---
         d[pixel] =(table_index < theTableBinCount) ? rt[table_index] : NULL_PIX;
      }

      rt += BAND_OFFSET; // Go to next band in the table.
   }

   if (theInputScalarType != theOutputScalarType)
   {
      // Normalize and copy the temp tile to a buffer.
      id->copyTileToNormalizedBuffer(theNormBuf);

      // Un-normalize and copy the buffer to the destination tile.
      theTile->copyNormalizedBufferToTile(theNormBuf);
   }
}

void ossimTableRemapper::remapFromNormalizedTable(
   ossimRefPtr<ossimImageData>& inputTile)
{
   if (!theNormBuf) // First time...
   {
      theNormBuf = new ossim_float64[theTile->getSize()];
   }
   
   // Normalize and copy the source tile to a buffer.
   inputTile->copyTileToNormalizedBuffer(theNormBuf);

   const ossim_uint32 BANDS = theTile->getNumberOfBands();
   const ossim_uint32 PPB   = theTile->getSizePerBand(); // pixels per band

   //---
   // NOTE:
   // If (theTableBandCount == one) that means we only have one band; or,
   // the user wanted to remap all bands using the same table...
   //---
   const ossim_uint32 BAND_OFFSET = (theTableBandCount == 1) ? 0 : PPB;

   // remap table (rt)
   ossim_float64* rt = reinterpret_cast<ossim_float64*>(theTable); 

   ossim_float64* buf = theNormBuf;
   
   for (ossim_uint32 band = 0; band < BANDS; ++band)
   {
      for (ossim_uint32 pixel = 0; pixel < PPB; ++pixel)
      {
         // Get the source pixel...
         ossim_uint32 index
            = static_cast<ossim_uint32>(buf[pixel]*theTableBinCount+0.5);

         // If within range use to index the remap table; else, null.
         ossim_float64 p = (index < theTableBinCount) ? rt[index] : 0.0;

         // Range check (in case table bad) and assign to destination pixel.
         buf[pixel]
            = (p >= 0.0) ? ( (p <=1.0) ? p : 1) : 0.0;
      }

      // Go to next band.
      rt += BAND_OFFSET;
      buf += PPB;
   }
   
   // Un-normalize and copy the buffer to the destination tile.
   theTile->copyNormalizedBufferToTile(theNormBuf);
}

ossimScalarType ossimTableRemapper::getOutputScalarType() const
{
   //---
   // Need to fix this as output scalar can be different from input. (drb)
   //---
   return ossimImageSourceFilter::getOutputScalarType();
   
//    if (theEnableFlag)
//    {
//       return theOutputScalarType;
//    }
//    else if (!theEnableFlag && theInputConnection)
//    {
//       // disabled return input scalar.
//       theInputConnection->getOutputScalarType();
//    }

//    // No connection and disabled!
//    return theOutputScalarType;
}

void ossimTableRemapper::setTable(ossim_uint8* table,
                                  ossim_uint32 table_bin_count,
                                  ossim_uint32 table_band_count,
                                  RemapTableType table_type,
                                  ossimScalarType output_scalar_type,
                                  bool own_table)
{
   // Start with a clean slate...
   destroy();
   
   theTable            = table;
   theTableBinCount    = table_bin_count;
   theTableBandCount   = table_band_count;
   theTableType        = table_type;
   theOutputScalarType = output_scalar_type;

   if (theInputConnection)
   {
      theEnableFlag = true;
   }
}

bool ossimTableRemapper::loadState(const ossimKeywordlist& kwl,
                                   const char* prefix)
{
   return ossimImageSourceFilter::loadState(kwl, prefix);
}

bool ossimTableRemapper::saveState(ossimKeywordlist& kwl,
                                   const char* prefix) const
{
   return ossimImageSourceFilter::saveState(kwl, prefix);
}

ostream& ossimTableRemapper::print(ostream& os) const
{
   ossimScalarTypeLut* sl = ossimScalarTypeLut::instance();

   os << "\nossimTableRemapper::print:"
      << "\ntheTableBinCount:    " << theTableBinCount
      << "\ntheTableBandCount:   " << theTableBandCount
      << "\ntheTableType:        " << TABLE_TYPE[theTableType]
      << "\ntheInputScalarType:  " << (*sl)[theInputScalarType]
      << "\ntheOutputScalarType: " << (*sl)[theOutputScalarType]
      << "\ntheTableOwnerFlag:   " << (theTableOwnerFlag ? "true" : "false")
      << endl;
   
   if (theTile.valid())
   {
      os << "theTile:\n" << *theTile << endl;
   }
   if (theTmpTile.valid())
   {
      os << "theTmpTile:\n" << *theTmpTile << endl;
   }

   switch (theOutputScalarType)
   {
      case OSSIM_UCHAR:
      {
         dumpTable(ossim_uint8(0), os);
         break;
      }
      
      case OSSIM_USHORT11:
      case OSSIM_USHORT16:
      {
         dumpTable(ossim_uint16(0), os);
         break;
      }
      
      case OSSIM_SSHORT16:
      {
         dumpTable(ossim_sint16(0), os);
         break;
      }
      
      case OSSIM_NORMALIZED_FLOAT:
      case OSSIM_FLOAT:
      {
         dumpTable(ossim_float32(0), os);
         break;
      }
      
      case OSSIM_NORMALIZED_DOUBLE:
      case OSSIM_DOUBLE:
      {
         dumpTable(ossim_float64(0), os);
         break;
      }
      
      case OSSIM_SCALAR_UNKNOWN:
      default:
      {
         // Shouldn't hit this.
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimTableRemapper::print OSSIM_SCALAR_UNKNOWN!\n";
         break;
      }

   } // End of "switch (theTableType)"

   return os;
}

template <class T> void ossimTableRemapper::dumpTable(T /*dummy*/,
                                                      ostream& os) const
{
   if (!theTable || !theTableBinCount || !theTableBandCount)
   {
      return;
   }

   T* table = reinterpret_cast<T*>(theTable);

   ossim_uint32 table_index = 0;
   for (ossim_uint32 band = 0; band < theTableBandCount; ++band)
   {
      for (ossim_uint32 index = 0; index < theTableBinCount; ++index)
      {
         os << "tabel[" << band << "][" << index << "]:  "
            << static_cast<int>(table[table_index]) << endl;
         ++table_index;
      }
   }
}

void ossimTableRemapper::setTableOwnership(bool flag)
{
   theTableOwnerFlag = flag;
}

ostream& operator<<(ostream& os, const ossimTableRemapper& tr)
{
   return tr.print(os);
}

// Private to disallow use...
ossimTableRemapper::ossimTableRemapper(const ossimTableRemapper&)
{
}

// Private to disallow use...
ossimTableRemapper& ossimTableRemapper::operator=(const ossimTableRemapper&)
{
   return *this;
}
