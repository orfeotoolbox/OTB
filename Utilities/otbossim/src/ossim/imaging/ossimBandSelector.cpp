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
//  $Id: ossimBandSelector.cpp 21660 2012-09-07 20:56:53Z dburken $

#include <ossim/imaging/ossimBandSelector.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimVisitor.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/base/ossimStringProperty.h>
#include <iostream>
#include <algorithm>

static ossimTrace traceDebug("ossimBandSelector:debug");

RTTI_DEF1(ossimBandSelector,"ossimBandSelector", ossimImageSourceFilter)

ossimBandSelector::ossimBandSelector()
   :
      ossimImageSourceFilter(),
      theTile(0),
      theOutputBandList(0),
      theWithinRangeFlag(ossimBandSelectorWithinRangeFlagState_NOT_SET),
      thePassThroughFlag(false)

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
   ossimRefPtr<ossimImageData> t = theInputConnection->getTile(tileRect, resLevel);

   if (!isSourceEnabled())
   {
      return t;  // This tile source bypassed, return the input tile source.
   }

//   if (theOrderedCorrectlyFlag)
//   {
//      return t; // Input band order same as output band order.
//   }

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
      theWithinRangeFlag = ((outputBandsWithinInputRange() == true) ?
                            ossimBandSelectorWithinRangeFlagState_IN_RANGE:
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

void ossimBandSelector::setOutputBandList( const vector<ossim_uint32>& outputBandList )
{
   if (outputBandList.size())
   {
      theOutputBandList = outputBandList;  // Assign the new list.

      bool setBands = false;
      ossimRefPtr<ossimImageHandler> ih = getBandSelectableImageHandler();
      if ( ih.valid() )
      {
         // Our input is a single image chain that can do band selection.
         if ( ih->setOutputBandList( outputBandList ) )
         {
            thePassThroughFlag = true;
            setBands = true;
            theTile = 0; // Don't need.
         }
      }

      if ( setBands == false )
      {
         if ( theTile.valid() && ( theTile->getNumberOfBands() != outputBandList.size() ) )
         {
            theTile = 0;       // Force an allocate call next getTile.
         }
         theWithinRangeFlag = ossimBandSelectorWithinRangeFlagState_NOT_SET;
         checkPassThrough();
         //theOrderedCorrectlyFlag = isOrderedCorrectly();
      }
   }
}

ossim_uint32 ossimBandSelector::getNumberOfOutputBands() const
{
   ossim_uint32 bands;
   
   if(isSourceEnabled())
   {
      bands = theOutputBandList.size();
   }
   else
   {
      //---
      // Note:
      // This returns theInputConnection->getNumberOfOutputBands() which is our
      // input.  Calling ossimBandSelector::getNumberOfInputBands() will produce
      // an error if we are bypassed due to a band selectable image handler.
      //---
      bands = ossimImageSourceFilter::getNumberOfInputBands();
   }

   return bands;
}

ossim_uint32 ossimBandSelector::getNumberOfInputBands() const
{
   ossim_uint32 bands;

   // See if we have a single image chain with band selectable image handler.
   ossimRefPtr<ossimImageHandler> ih = getBandSelectableImageHandler();
   if ( ih.valid() )
   {
      bands = ih->getNumberOfInputBands();
   }
   else
   {
      bands = ossimImageSourceFilter::getNumberOfInputBands();
   }
   
   return bands;
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
         // First time through set the output band list to input.
         theInputConnection->getOutputBandList(theOutputBandList);
      }

      // See if we have a single image chain with band selectable image handler.
      ossimRefPtr<ossimImageHandler> ih = getBandSelectableImageHandler();
      if ( ih.valid() )
      {
         if ( theOutputBandList.size() )
         {
            ih->setOutputBandList( theOutputBandList );
         }
         thePassThroughFlag = true;
      }
      else
      {
         checkPassThrough();
      }
      
      if ( isSourceEnabled() )
      {
         // theOrderedCorrectlyFlag = isOrderedCorrectly();
         
         if ( theTile.valid() )
         {
            //---
            // Check for:
            // - ordered correctly
            // - band change
            // - scalar change
            //---
            if( ( theTile->getNumberOfBands() != theOutputBandList.size() ) ||
                ( theTile->getScalarType() !=
                  theInputConnection->getOutputScalarType() ) )
            {
               theTile = 0; // Don't need it.
            }
         }
      }
   }
   else // No input connection.
   {
      thePassThroughFlag = true;
   }

   if ( !isSourceEnabled() )
   {
      theTile = 0;
   }
}

void ossimBandSelector::allocate()
{
   //initialize(); // Update the connection.
   //theOrderedCorrectlyFlag = isOrderedCorrectly();
   theTile = ossimImageDataFactory::instance()->create(this, this);
   theTile->initialize();
}

bool ossimBandSelector::isSourceEnabled()const
{
   bool result = ossimImageSourceFilter::isSourceEnabled();
   if(result)
   {
      // if I am not marked to pass information on through then enable me
      result = !thePassThroughFlag;
   }
   
   return result;
}

double ossimBandSelector::getMinPixelValue(ossim_uint32 band)const
{
   if(theInputConnection)
   {
      if (isSourceEnabled())
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
      if (isSourceEnabled())
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
      if (isSourceEnabled())
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
   
   ossimString bandsString;
   ossim::toSimpleStringList(bandsString,
                             theOutputBandList);
   kwl.add(prefix,
           ossimKeywordNames::BANDS_KW,
           bandsString,
           true);
/*   
   for(ossim_uint32 counter = 0; counter < theOutputBandList.size();counter++)
   {
      temp  = ossimKeywordNames::BAND_KW;
      temp += ossimString::toString(counter+1);
      
      kwl.add(prefix,
              temp.c_str(),
              ossimString::toString(theOutputBandList[counter]+1).c_str());
   }
*/   
   return ossimImageSourceFilter::saveState(kwl, prefix);
}

bool ossimBandSelector::loadState(const ossimKeywordlist& kwl,
                                  const char* prefix)
{
   ossimImageSourceFilter::loadState(kwl, prefix);

   theOutputBandList.clear();
   
   ossimString copyPrefix = prefix;
   
   ossimString bands = kwl.find(prefix, ossimKeywordNames::BANDS_KW);
   if(!bands.empty())
   {
      ossim::toSimpleVector(theOutputBandList, bands);
   }
   else
   {
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
   }

   initialize();
   
   return true;
}

void ossimBandSelector::checkPassThrough()
{
   thePassThroughFlag = ((theInputConnection == 0)||!outputBandsWithinInputRange());
   
   // check if marked with improper bands
   if(thePassThroughFlag) return;
   
   if(theInputConnection)
   {
      std::vector<ossim_uint32> inputList;
      theInputConnection->getOutputBandList(inputList);

      if ( inputList.size() == theOutputBandList.size() )
      {
         const std::vector<ossim_uint32>::size_type SIZE =
            theOutputBandList.size();
      
         std::vector<ossim_uint32>::size_type i = 0;
         while (i < SIZE)
         {
            if ( inputList[i] != theOutputBandList[i] )
            {
               break;
            }
            ++i;
         }
         if (i == SIZE)
         {
            thePassThroughFlag = true;
         }
      }
   }
   else
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
         << "ossimBandSelector::isOrderedCorrectly() ERROR:"
         << "Method called prior to initialization!\n";
      }
   }

}

bool ossimBandSelector::outputBandsWithinInputRange() const
{
   bool result = false;
   
   if(theInputConnection)
   {
      result = true;
      const ossim_uint32 HIGHEST_BAND = getNumberOfInputBands() - 1;
      const ossim_uint32 OUTPUT_BANDS = (ossim_uint32)theOutputBandList.size();
      for (ossim_uint32 i=0; i<OUTPUT_BANDS; ++i)
      {
         if (theOutputBandList[i] > HIGHEST_BAND)
         {
            if(traceDebug())
            {
               ossimNotify(ossimNotifyLevel_WARN)
               << "ossimBandSelector::outputBandsWithinInputRange() ERROR:"
               << "Output band greater than highest input band. "
               << theOutputBandList[i] << " > " << HIGHEST_BAND << "."
               << std::endl;
            }
            result = false;
            break;
         }
      }
   }
   else
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
         << "ossimBandSelector::outputBandsWithinInputRange() ERROR:"
         << "Method called prior to initialization!" << std::endl;
      }
   }
   return result;
}

void ossimBandSelector::getOutputBandList(std::vector<ossim_uint32>& bandList) const
{
   if ( isSourceEnabled()&&theOutputBandList.size() )
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

   if(property->getName() == "bandSelection")
   {
     ossimString str = property->valueToString();
     std::vector<ossimString> str_vec;
     std::vector<ossim_uint32> int_vec;
     
     str.split( str_vec, " " );
     
     for ( ossim_uint32 i = 0; i < str_vec.size(); ++i )
     {
        if(!str_vec[i].empty())
        {
           int_vec.push_back( str_vec[i].toUInt32() );
        }
     }
     setOutputBandList( int_vec );
   }
   else if(property->getName() == "bands")
   {
      std::vector<ossim_uint32> selection;
      if(ossim::toSimpleVector(selection, property->valueToString()))
      {
         theOutputBandList = selection;
      }
   }
   else
   {
      ossimImageSourceFilter::setProperty(property);
   }
}

ossimRefPtr<ossimProperty> ossimBandSelector::getProperty(const ossimString& name)const
{
   if(name == "bandSelection")
   {
      std::vector<ossim_uint32> bands;
	  
	  getOutputBandList( bands );
	  	  
      std::vector<ossimString> bandNames;

      for(ossim_uint32 i = 0; i < bands.size(); i++)
      {
         bandNames.push_back( ossimString::toString( bands[i] ) );
      }
	  
	  ossimString str;
	  
	  str.join( bandNames, " " );
	  
      ossimStringProperty* stringProp = new ossimStringProperty(name, str);
									
      stringProp->clearChangeType();
      stringProp->setReadOnlyFlag(false);
      stringProp->setCacheRefreshBit();
      
      return stringProp;
   }
   else if(name == "bands")
   {
      ossimString bandsString;
      ossim::toSimpleStringList(bandsString,
                                theOutputBandList);
      ossimStringProperty* stringProp = new ossimStringProperty(name, bandsString);
      
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
   propertyNames.push_back("bands");
}

ossimRefPtr<ossimImageHandler> ossimBandSelector::getBandSelectableImageHandler() const
{
   ossimRefPtr<ossimImageHandler> ih = 0;

   if ( theInputConnection )
   {
      ossimTypeNameVisitor visitor(ossimString("ossimImageHandler"),
                                   true,
                                   ossimVisitor::VISIT_CHILDREN|ossimVisitor::VISIT_INPUTS);
      
      theInputConnection->accept(visitor);

      // If there are multiple image handlers, e.g. a mosaic do not uses.
      if ( visitor.getObjects().size() == 1 )
      {
         ih = visitor.getObjectAs<ossimImageHandler>( 0 );
         if ( ih.valid() )
         {
            if ( ih->isBandSelector() == false )
            {
               ih = 0;
            }
         }
      }
      
   } // Matches: if ( theInputConnection )
   return ih;
   
} // End: ossimBandSelector::getBandSelectableImageHandler()
