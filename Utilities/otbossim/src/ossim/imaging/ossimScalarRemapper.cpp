//*******************************************************************
// Copyright (C) 2001 ImageLinks Inc.
//
// License:  See top level LICENSE.txt file.
// 
// Author:  David Burken
//
// Description:
//
// Contains class definition for ossimScalarRemapper.
// This class is used to remap image data from one scalar type to another.
//
//*******************************************************************
//  $Id: ossimScalarRemapper.cpp 22135 2013-02-02 16:27:24Z dburken $

#include <iostream>

#include <ossim/imaging/ossimScalarRemapper.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimScalarTypeLut.h>
#include <ossim/base/ossimPropertyEvent.h>
#include <ossim/base/ossimRefreshEvent.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimStringProperty.h>

RTTI_DEF1(ossimScalarRemapper,
          "ossimScalarRemapper",
          ossimImageSourceFilter)

static const ossimTrace traceDebug("ossimScalarRemapper:debug");
   
ossimScalarRemapper::ossimScalarRemapper()
   :
      ossimImageSourceFilter(),
      theNormBuf(NULL),
      theTile(NULL),
      theOutputScalarType(OSSIM_UINT8),
      theByPassFlag(false)
{
}

ossimScalarRemapper::ossimScalarRemapper(ossimImageSource* inputSource,
                                         ossimScalarType outputScalarType)
   :
      ossimImageSourceFilter(inputSource),
      theNormBuf(NULL),
      theTile(NULL),
      theOutputScalarType(outputScalarType),
      theByPassFlag(false)
{
   if(inputSource)
   {
      if (inputSource->getOutputScalarType() == outputScalarType)
      {
         // Disable this filter simply return the inputSource's data.
         theByPassFlag = true;
         
         // Nothing else to do.
      }
      else
      {
         theByPassFlag = false;
      }
   }
   else
   {
      theByPassFlag = true;
   }
}

ossimScalarRemapper::~ossimScalarRemapper()
{
   destroy();
}

void ossimScalarRemapper::destroy()
{
   if (theNormBuf)
   {
      delete [] theNormBuf;
      theNormBuf = NULL;
   }
   theTile      = NULL;
}

ossimRefPtr<ossimImageData> ossimScalarRemapper::getTile(
   const ossimIrect& tileRect, ossim_uint32 resLevel)
{
   if(!theInputConnection)
   {
      return ossimRefPtr<ossimImageData>();
   }

   // Fetch tile from pointer from the input source.
   ossimRefPtr<ossimImageData> inputTile =
      theInputConnection->getTile(tileRect, resLevel);

   // Check for remap bypass:
   if ( !isSourceEnabled()||theByPassFlag )
   {
      return inputTile;
   }

   // Check for first time through.
   if ( !theTile.valid() )
   {
      allocate();
      
      if ( !theTile.valid() )
      {
         // This can happen if input/output scalars are the same.
         return inputTile;
      }
   }

   // Capture the size prior to a possible resize.
   ossim_uint32 oldSize = theTile->getSize();

   // Set the origin,bands of the output tile.
   theTile->setImageRectangle(tileRect);

   ossim_uint32 newSize = theTile->getSize();

   // Check for size change before possible return.
   if(newSize != oldSize)
   {
      if(theNormBuf)
      {
         //---
         // Delete the current buffer since it is the wrong size.
         // 
         // NOTE:
         // We won't reallocate it yet since we could return without using it.
         // It will be checked prior to using later.
         //---
         delete [] theNormBuf;
         theNormBuf = NULL;
      }
   }
   
   if ( !inputTile.valid() ||
        (inputTile->getDataObjectStatus() == OSSIM_NULL) ||
        (inputTile->getDataObjectStatus() == OSSIM_EMPTY) )
   {
      //---
      // Since the filter is enabled, return theTile which is of the
      // correct scalar type.
      //---
      theTile->makeBlank();
      return theTile;
   }

   if (!theNormBuf) // First time through or size changed and was deleted...
   {
      theNormBuf = new double[newSize];
      memset(theNormBuf, '\0', newSize);
   }

   if (inputTile->getScalarType() == theOutputScalarType)
   {
      // Scalar types already the same.  Nothing to do...
      return inputTile;
   }
   
   switch(inputTile->getScalarType())
   {
      case OSSIM_NORMALIZED_DOUBLE:
      {
         // Un-normalize and copy the buffer to the destination tile.
         theTile->copyNormalizedBufferToTile(
            static_cast<double*>( inputTile->getBuf() ) );
         break;
      }
      case OSSIM_NORMALIZED_FLOAT:
      {
         // Un-normalize and copy the buffer to the destination tile.
         theTile->copyNormalizedBufferToTile(
            static_cast<float*>( inputTile->getBuf() ) );
         break;
      }
      default:
      {
         //---
         // NOTE: stretchMinMax commented out as it was incorrectly not resetting
         // the tile's min/max data members; hence, messing up the downstream copy
         // to normalized buffer. (drb 02 Feb. 2013)
         // Special case.  Stretch assuming caller want to view this data.
         //---
         // inputTile->stretchMinMax();

         // Normalize and copy the source tile to a buffer.
         inputTile->copyTileToNormalizedBuffer(theNormBuf);
         
         // Un-normalize and copy the buffer to the destination tile.
         theTile->copyNormalizedBufferToTile(theNormBuf);

         break;
      }
   }
   
   theTile->validate();
   
   return theTile;
}

ossimScalarType ossimScalarRemapper::getOutputScalarType() const
{
   if(isSourceEnabled()&&!theByPassFlag)
   {
      return theOutputScalarType;
   }

   return ossimImageSourceFilter::getOutputScalarType();
}

void ossimScalarRemapper::setOutputScalarType(ossimScalarType scalarType)
{
   if (scalarType == OSSIM_SCALAR_UNKNOWN)
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
         << "ossimScalarRemapper::setOutputScalarType WARN:\n"
         << "OSSIM_SCALAR_UNKNOWN passed to method.  No action taken..."
         << std::endl;
      }
      return;
   }
   
   if (theInputConnection)
   {
      if ( scalarType == theInputConnection->getOutputScalarType() )
      {
         // Input same as output, nothing for us to do...
         theByPassFlag = true;
      }
      else // Types not equal...
      {
         theByPassFlag = false;
         destroy();
      }
   }
   else // No input source, disable.
   {
      theByPassFlag = true;
   }

   theOutputScalarType = scalarType;
}

void ossimScalarRemapper::setOutputScalarType(ossimString scalarType)
{
   int scalar =
      ossimScalarTypeLut::instance()->getEntryNumber(scalarType.c_str());
   
   if (scalar != ossimLookUpTable::NOT_FOUND)
   {
      setOutputScalarType(static_cast<ossimScalarType>(scalar));
   }
   else
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
         << "ossimScalarRemapper ERROR:"
         << "\nUnknown scalar type:  " << scalarType.c_str() << std::endl;
      }
   }
}

void ossimScalarRemapper::initialize()
{
   //---
   // Call the base class initialize.
   // Note:  This will reset "theInputConnection" if it changed...
   //---
   ossimImageSourceFilter::initialize();

   if (theInputConnection)
   {
      // Set the bypass flag accordingly...
      if ( theInputConnection->getOutputScalarType() == 
           theOutputScalarType )
      {
         theByPassFlag = true;
      }
      else
      {
         theByPassFlag = false;
      }
      
      if (theTile.valid())
      {
         //---
         // Check for:
         // - bypass
         // - disabled(!enabled)
         // - scalar change
         // - band count change
         //---
         if ( theByPassFlag ||
              !theEnableFlag ||
              ( theInputConnection->getOutputScalarType() !=
                theOutputScalarType ) ||
              ( theInputConnection->getNumberOfOutputBands() !=
                theTile->getNumberOfBands() ) )
         {
            destroy();  // Reallocated first unbypassed getTile.
         }
      }
   }
}

void ossimScalarRemapper::allocate()
{
   destroy();

   if(!theInputConnection) // Nothing to do here.
   {
      setInitializedFlag(false);
      theByPassFlag = true;
      return;
   }
   
   if (theOutputScalarType == OSSIM_SCALAR_UNKNOWN)
   {
      // default to OSSIM_UINT8
      theOutputScalarType = OSSIM_UINT8;
   }
   
   if(theInputConnection &&
      (getOutputScalarType() != theInputConnection->getOutputScalarType())&&
      (theInputConnection->getOutputScalarType() != OSSIM_SCALAR_UNKNOWN)&&
      (getOutputScalarType() != OSSIM_SCALAR_UNKNOWN))
   {
      theByPassFlag = false;
      
      theTile = ossimImageDataFactory::instance()->create(this, this);

      // Initialize the tile.
      theTile->initialize();
      
      // Set the base class flags to be initialized and enabled.
      setInitializedFlag(true);
      
   } // End of "if(theInputConnection->isConnected()..."
   else
   {
      // Set to not initialized and disabled.
      setInitializedFlag(false);
      theByPassFlag = true;
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimScalarRemapper::allocate() DEBUG"
         << "\ninput scalar:  " << theInputConnection->getOutputScalarType()
         << "\noutput scalar: " << getOutputScalarType()
         << "\nenabled:  " << (isSourceEnabled()?"true":"false")
         << std::endl;
   }
}

void ossimScalarRemapper::setProperty(ossimRefPtr<ossimProperty> property)
{
   if(!property) return;

   if(property->getName() == "Output scalar type")
   {
      theOutputScalarType = ossimScalarTypeLut::instance()->
         getScalarTypeFromString(property->valueToString());
   }
   else
   {
      ossimImageSourceFilter::setProperty(property);
   }
}

ossimRefPtr<ossimProperty> ossimScalarRemapper::getProperty(const ossimString& name)const
{
   if(name == "Output scalar type")
   {
      std::vector<ossimString> scalarNames;

      ossim_int32 tableSize = (ossim_int32)ossimScalarTypeLut::instance()->
         getTableSize();
      ossim_int32 idx;

      for(idx = 0; idx < tableSize; ++idx)
      {
         scalarNames.push_back(ossimScalarTypeLut::instance()->
                               getEntryString(idx));
      }
      ossimStringProperty* stringProp =
         new ossimStringProperty("Output scalar type",
                                 ossimScalarTypeLut::instance()->getEntryString((ossim_int32)theOutputScalarType),
                                 false,
                                 scalarNames);
      stringProp->clearChangeType();
      stringProp->setReadOnlyFlag(false);
      stringProp->setCacheRefreshBit();
      
      return stringProp;
   }

   return ossimImageSourceFilter::getProperty(name);
}

void ossimScalarRemapper::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimImageSourceFilter::getPropertyNames(propertyNames);
   propertyNames.push_back("Output scalar type");
}

bool ossimScalarRemapper::saveState(ossimKeywordlist& kwl,
                                    const char* prefix) const
{
   ossimImageSourceFilter::saveState(kwl, prefix);

   kwl.add(prefix,
           ossimKeywordNames::SCALAR_TYPE_KW,
           ossimScalarTypeLut::instance()->getEntryString(theOutputScalarType),
           true);

   return true;
}

bool ossimScalarRemapper::loadState(const ossimKeywordlist& kwl,
                                    const char* prefix)
{
   ossimImageSourceFilter::loadState(kwl, prefix);

   if (kwl.getErrorStatus() == ossimErrorCodes::OSSIM_ERROR)
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimScalarRemapper::loadState\n"
            << " ERROR detected in keyword list!  State not loaded."
            << std::endl;
      }
      return false;
   }

   int scalar = ossimScalarTypeLut::instance()->getEntryNumber(kwl, prefix);

   if (scalar != ossimLookUpTable::NOT_FOUND)
   {
      setOutputScalarType(static_cast<ossimScalarType>(scalar));
   }

   return true;
}

ossimString ossimScalarRemapper::getOutputScalarTypeString() const
{
   return ossimScalarTypeLut::instance()->getEntryString(theOutputScalarType);
}

void ossimScalarRemapper::propertyEvent(ossimPropertyEvent& event)
{
   // if my properties have changed then just initialize
   //
   if(event.getObject() == this)
   {
      initialize();
   }
   else // if an input property has changed just check to see if the number
   {    // of bands has changed
      
      if(!theTile)
      {
         initialize();
      }
      else
      {
         int b = theInputConnection->getNumberOfOutputBands();
         if((int)theTile->getNumberOfBands() != b)
         {
            initialize();
         }
      }
   }
}

void ossimScalarRemapper::refreshEvent(ossimRefreshEvent& event)
{
   // if my properties have changed then just initialize
   if(event.getObject() == this)
   {
      initialize();
   }
   else // if an input property has changed just check to see if the number
   {    // of bands has changed
      
      if(!theTile)
      {
         initialize();
      }
      else
      {
         int b = theInputConnection->getNumberOfOutputBands();
         if((int)theTile->getNumberOfBands() != b)
         {
            initialize();
         }
      }
   }
}

double ossimScalarRemapper::getNullPixelValue(ossim_uint32 band) const
{
   if(!isSourceEnabled()||theByPassFlag)
   {
      if(theInputConnection)
      {
         return theInputConnection->getNullPixelValue(band);
      }
   }
   else if(theTile.valid())
   {
      if (band < theTile->getNumberOfBands())
      {
         return theTile->getNullPix(band);
      }
   }
   
   return ossim::defaultNull(theOutputScalarType);
}

double ossimScalarRemapper::getMinPixelValue(ossim_uint32 band) const
{
   if(!isSourceEnabled()||theByPassFlag)
   {
      if(theInputConnection)
      {
         return theInputConnection->getMinPixelValue(band);
      }
   }
   else if(theTile.valid())
   {
      if (band < theTile->getNumberOfBands())
      {
         return theTile->getMinPix(band);
      }
   }
   
   return ossim::defaultMin(theOutputScalarType);
}

double ossimScalarRemapper::getMaxPixelValue(ossim_uint32 band) const
{
   if(!isSourceEnabled()||theByPassFlag)
   {
      if(theInputConnection)
      {
         return theInputConnection->getMaxPixelValue(band);
      }
   }
   else if(theTile.valid())
   {
      if (band < theTile->getNumberOfBands())
      {
         return theTile->getMaxPix(band);
      }
   }
   
   return ossim::defaultMax(theOutputScalarType);
}

ossimString ossimScalarRemapper::getLongName()const
{
   return ossimString("Scalar Remapper, filters between different scalar types.");
}

ossimString ossimScalarRemapper::getShortName()const
{
   return ossimString("Scalar Remapper");
}

