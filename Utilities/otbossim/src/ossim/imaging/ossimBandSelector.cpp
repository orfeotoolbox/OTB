//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See top level LICENSE.txt.
//
// Author:  Garrett Potts
//
// Description:
//
// Contains class declaration for ossimBandSelector.
// 
//*******************************************************************
//  $Id: ossimBandSelector.cpp 13022 2008-06-10 16:25:36Z dburken $

#include <iostream>
#include <algorithm>

#include <ossim/imaging/ossimBandSelector.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/base/ossimStringProperty.h>

RTTI_DEF1(ossimBandSelector,"ossimBandSelector", ossimImageSourceFilter)

ossimBandSelector::ossimBandSelector()
   :
      ossimImageSourceFilter(),
      theTile(0),
      theWithinRangeFlag(ossimBandSelectorWithinRangeFlagState_NOT_SET)

{
//   theEnableFlag = false; // Start off disabled.
   theEnableFlag = true; 
}

ossimBandSelector::~ossimBandSelector()
{
   theTile = 0;
}

ossimRefPtr<ossimImageData> ossimBandSelector::getTile(
   const ossimIrect& tileRect,
   ossim_uint32 resLevel)
{
   if (!theInputConnection)
   {
      return ossimRefPtr<ossimImageData>();
   }

   // Get the tile from the source.
   ossimRefPtr<ossimImageData> t = theInputConnection->getTile(tileRect,
                                                               resLevel);

   if (!theEnableFlag)
   {
      return t;  // This tile source bypassed, return the input tile source.
   }

   if (isOrderedCorrectly())
   {
      return t; // Input band order same as output band order.
   }

   if(!theTile.valid()) // First time through, might not be initialized...
   {
      allocate();
      if (!theTile.valid())
      {
         // Should never happen...
         return t; // initialize failed.
      }
   }

   theTile->setImageRectangle(tileRect);
   if(theWithinRangeFlag == ossimBandSelectorWithinRangeFlagState_NOT_SET)
   {
      theWithinRangeFlag = ((outputBandsWithinInputRange() == true)?ossimBandSelectorWithinRangeFlagState_IN_RANGE:
                            ossimBandSelectorWithinRangeFlagState_OUT_OF_RANGE);
   }
   if(theWithinRangeFlag == ossimBandSelectorWithinRangeFlagState_OUT_OF_RANGE)
   {
      theTile->makeBlank();
      return theTile;
   }

   if ( !t.valid() ||
        (t->getDataObjectStatus() == OSSIM_EMPTY) ||
        (t->getDataObjectStatus() == OSSIM_NULL))
   {
      //---
      // Since we're enabled, we must return our tile not "t" so the
      // correct number of bands goes through the chain.
      //---
      theTile->makeBlank();
      return theTile;
   }

   // Copy selected bands to our tile.
   for (ossim_uint32 i=0; i<theOutputBandList.size(); i++)
   {
      theTile->assignBand(t.get(), theOutputBandList[i], i);
   }
   
   theTile->validate();

   return theTile;
}

void ossimBandSelector::setOutputBandList(
   const vector<ossim_uint32>& outputBandList)
{
   if (outputBandList.size())
   {
      theOutputBandList = outputBandList;  // Assign the new list.
      theTile = 0;       // Force an allocate call next getTile.

      theWithinRangeFlag = ossimBandSelectorWithinRangeFlagState_NOT_SET;
   }
}

ossim_uint32 ossimBandSelector::getNumberOfOutputBands() const
{
   if(theEnableFlag)
   {
      return (ossim_uint32)theOutputBandList.size();
   }
   
   return getNumberOfInputBands();
}

void ossimBandSelector::initialize()
{
   // Base class will recapture "theInputConnection".
   ossimImageSourceFilter::initialize();
   theWithinRangeFlag =  ossimBandSelectorWithinRangeFlagState_NOT_SET;  
   if(theInputConnection)
   {
      if ( !theOutputBandList.size() ) 
      {
         //---
         // First time through set the output band list to input.
         //---
         theInputConnection->getOutputBandList(theOutputBandList);
      }

      if ( theEnableFlag )
      {
         if ( theTile.valid() )
         {
            //---
            // Check for:
            // - ordered correctly
            // - band change
            // - scalar change
            //---
            if( isOrderedCorrectly() ||
                ( theTile->getNumberOfBands() != theOutputBandList.size() ) ||
                ( theTile->getScalarType() !=
                  theInputConnection->getOutputScalarType() ) )
            {
               theTile = 0; // Don't need it.
            }
         }
      }
      else
      {
         theTile = 0;
      }
   }
   else // No input connection.
   {
      theTile = 0;
   }
}

void ossimBandSelector::allocate()
{
   initialize(); // Update the connection.

   theTile = ossimImageDataFactory::instance()->create(this, this);
   theTile->initialize();
}

double ossimBandSelector::getMinPixelValue(ossim_uint32 band)const
{
   if(theInputConnection)
   {
      if (theEnableFlag)
      {
         if(band < theOutputBandList.size())
         {
            return theInputConnection->
               getMinPixelValue(theOutputBandList[band]);
         }
         else
         {
            return theInputConnection->getMinPixelValue(band);
         }
      }
      else
      {
         return theInputConnection->getMinPixelValue(band);
      }
   }
   return 0;
}

double ossimBandSelector::getNullPixelValue(ossim_uint32 band)const
{
   if(theInputConnection)
   {
      if (theEnableFlag)
      {
         if(band < theOutputBandList.size())
         {
            return theInputConnection->
               getNullPixelValue(theOutputBandList[band]);
         }
         else
         {
            return theInputConnection->getNullPixelValue(band);
         }
      }
      else
      {
         return theInputConnection->getNullPixelValue(band);
      }
   }
   
   return 0;
}

double ossimBandSelector::getMaxPixelValue(ossim_uint32 band)const
{
   if(theInputConnection)
   {
      if (theEnableFlag)
      {
         if(band < theOutputBandList.size())
         {
            return theInputConnection->
               getMaxPixelValue(theOutputBandList[band]);
         }
         else
         {
            return theInputConnection->getMaxPixelValue(band);
         }
      }
      else
      {
        return theInputConnection->getMaxPixelValue(band);
      }
   }
   return 1.0/DBL_EPSILON;
}

bool ossimBandSelector::saveState(ossimKeywordlist& kwl,
                                  const char* prefix)const
{
   ossimString temp;

   kwl.add(prefix,
           ossimKeywordNames::NUMBER_OUTPUT_BANDS_KW,
           static_cast<int>(theOutputBandList.size()),
           true);
   
   for(ossim_uint32 counter = 0; counter < theOutputBandList.size();counter++)
   {
      temp  = ossimKeywordNames::BAND_KW;
      temp += ossimString::toString(counter+1);
      
      kwl.add(prefix,
              temp.c_str(),
              ossimString::toString(theOutputBandList[counter]+1).c_str());
   }
   
   return ossimImageSourceFilter::saveState(kwl, prefix);
}

bool ossimBandSelector::loadState(const ossimKeywordlist& kwl,
                                  const char* prefix)
{
   ossimImageSourceFilter::loadState(kwl, prefix);
   
   // call ossimSource method to delete the list of objects   
   theOutputBandList.clear();
   ossimString copyPrefix = prefix;
   
   
   ossimString regExpression =  ossimString("^(") + copyPrefix + "band[0-9]+)";
   
   vector<ossimString> keys = kwl.getSubstringKeyList( regExpression );
   long numberOfBands = (long)keys.size();
   ossim_uint32 offset = (ossim_uint32)(copyPrefix+"band").size();
   std::vector<int>::size_type idx = 0;
   std::vector<int> numberList(numberOfBands);
   for(idx = 0; idx < keys.size();++idx)
   {
      ossimString numberStr(keys[idx].begin() + offset,
                            keys[idx].end());
      numberList[idx] = numberStr.toInt();
   }
   std::sort(numberList.begin(), numberList.end());
   for(idx=0;idx < numberList.size();++idx)
   {
      const char* bandValue =
         kwl.find(copyPrefix,
                  ("band"+ossimString::toString(numberList[idx])).c_str());
      theOutputBandList.push_back( ossimString(bandValue).toLong()-1);
   }
   initialize();
   
   return true;
}

bool ossimBandSelector::isOrderedCorrectly() const
{
   if(theInputConnection)
   {
      std::vector<ossim_uint32> inputList;
      theInputConnection->getOutputBandList(inputList);
      
      ossim_uint32 output_bands = (ossim_uint32)theOutputBandList.size();

      if(inputList.size() != output_bands) return false;
      
      for (ossim_uint32 i=0; i<output_bands; ++i)
      {
         if (inputList[i] != theOutputBandList[i]) return false;
      }
      return true;
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimBandSelector::isOrderedCorrectly() ERROR:"
         << "Method called prior to initialization!\n";
   }

   return false;
}

bool ossimBandSelector::outputBandsWithinInputRange() const
{
   if(theInputConnection)
   {
      const ossim_uint32 HIGHEST_BAND = getNumberOfInputBands() - 1;
      const ossim_uint32 OUTPUT_BANDS = (ossim_uint32)theOutputBandList.size();
      for (ossim_uint32 i=0; i<OUTPUT_BANDS; ++i)
      {
         if (theOutputBandList[i] > HIGHEST_BAND)
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << "ossimBandSelector::outputBandsWithinInputRange() ERROR:"
               << "Output band great than highest input band."
               << std::endl;
            return false;
         }
      }
      return true;
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimBandSelector::outputBandsWithinInputRange() ERROR:"
         << "Method called prior to initialization!" << std::endl;
   }

   return false;
}

void ossimBandSelector::getOutputBandList(
   std::vector<ossim_uint32>& bandList) const
{
   if ( theOutputBandList.size() )
   {
      bandList = theOutputBandList;
   }
   else if (theInputConnection)
   {
      theInputConnection->getOutputBandList(bandList);
   }
   else
   {
      bandList.clear();
   }
}

ossimString ossimBandSelector::getLongName()const
{
   return ossimString("Band Selector, maps an input band to the output band.");
}

ossimString ossimBandSelector::getShortName()const
{
   return ossimString("Band Selector");
}

void ossimBandSelector::setProperty(ossimRefPtr<ossimProperty> property)
{
   if(!property) return;

   if(property->getName() == "Selected bands")
   {
     ossimString str = property->valueToString();
     std::vector<ossimString> str_vec;
     std::vector<ossim_uint32> int_vec;
     
     str.split( str_vec, "," );
     
     for ( ossim_uint32 i = 0; i < str_vec.size(); ++i )
     {
        int_vec.push_back( str_vec[i].toUInt32() );
     }
     setOutputBandList( int_vec );
   }
   else
   {
      ossimImageSourceFilter::setProperty(property);
   }
}

ossimRefPtr<ossimProperty> ossimBandSelector::getProperty(const ossimString& name)const
{
   if(name == "Selected bands")
   {
      std::vector<ossim_uint32> bands;
	  
	  getOutputBandList( bands );
	  	  
      std::vector<ossimString> bandNames;

      for(ossim_uint32 i = 0; i < bands.size(); i++)
      {
         bandNames.push_back( ossimString::toString( bands[i] ) );
      }
	  
	  ossimString str;
	  
	  str.join( bandNames, "," );
	  
      ossimStringProperty* stringProp = new ossimStringProperty("Selected bands", str);
									
      stringProp->clearChangeType();
      stringProp->setReadOnlyFlag(false);
      stringProp->setCacheRefreshBit();
      
      return stringProp;
   }

   return ossimImageSourceFilter::getProperty(name);
}

void ossimBandSelector::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimImageSourceFilter::getPropertyNames(propertyNames);
   propertyNames.push_back("Selected bands");
}

