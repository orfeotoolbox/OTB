//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimBandAverageFilter.cpp 17206 2010-04-25 23:20:40Z dburken $
#include <ossim/imaging/ossimBandAverageFilter.h>

// used for constructing and an ossimImageData object
#include <ossim/imaging/ossimImageDataFactory.h>

// used for error reporting and for general info reporting such as warnings
#include <ossim/base/ossimErrorContext.h>

#include <sstream>
#include <iterator>

static const char* WEIGHTS_KW = "weights";

RTTI_DEF1(ossimBandAverageFilter,
          "ossimBandAverageFilter",
          ossimImageSourceFilter);

ossimBandAverageFilter::ossimBandAverageFilter()
   :ossimImageSourceFilter(),
    theTile(NULL)
{
}

ossimBandAverageFilter::ossimBandAverageFilter(ossimImageSource* input,
                                               const std::vector<double>& weights)
   :ossimImageSourceFilter(input),
    theTile(NULL),
    theWeights(weights)
{
}

ossimBandAverageFilter::~ossimBandAverageFilter()
{
}

ossim_uint32 ossimBandAverageFilter::getNumberOfOutputBands()const
{
   if(isSourceEnabled())
   {
      return 1;
   }
   
   return ossimImageSourceFilter::getNumberOfOutputBands();
}

ossimRefPtr<ossimImageData> ossimBandAverageFilter::getTile(const ossimIrect& rect, ossim_uint32 resLevel)
{
   // first we will check to see if we have an input connection
   //
   if(!theInputConnection)
   {
      // this will probably be null since we aren't initialized.
      // we really need to creae a default blank tile in OSSIM that is returned
      return theTile;
   }

   ossimRefPtr<ossimImageData> inputData =
      theInputConnection->getTile(rect, resLevel);

   if(!inputData.valid())
   {
      return inputData;
   }
   
   // If we are disabled or if the data object is empty or NULL or
   // not enough bands we don't need to run our algorithm so return the input
   // tile.
   if(!isSourceEnabled() ||
      (inputData->getDataObjectStatus() == OSSIM_NULL)||
      (inputData->getDataObjectStatus() == OSSIM_EMPTY)||
      (inputData->getNumberOfBands() < 2))
   {
      return inputData;
   }

   // check to see if we are initialized
   if(!theTile.valid())
   {
      initialize();
   }
   
   theTile->setImageRectangle(rect);

   // now lets set up the template method to operate in native type and run
   // the algorithm
   //
   switch(inputData->getScalarType())
   {
   case OSSIM_UCHAR:
   {
      averageInput((ossim_uint8)0, // setup template variable
                   inputData);
      break;
   }
   case OSSIM_USHORT11:
   case OSSIM_USHORT16:
   {
      averageInput((ossim_uint16)0, // setup template variable
                   inputData);
      break;
   }
   case OSSIM_SSHORT16:
   {
      averageInput((ossim_sint16)0, // setup template variable
                   inputData);
      break;
   }
   case OSSIM_NORMALIZED_FLOAT:
   case OSSIM_FLOAT:
   {
      averageInput((float)0, // setup template variable
                   inputData);
      break;
   }
   case OSSIM_NORMALIZED_DOUBLE:
   case OSSIM_DOUBLE:
   {
      averageInput((double)0, // setup template variable
                   inputData);
      break;
   }
   default:
   {
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "Unsupported scalar type in file %d at line %d",
                    __FILE__,
                    __LINE__);

      
      theTile->makeBlank();
      break;
   }
   }
   
   theTile->validate();

   
   return theTile;
}



void ossimBandAverageFilter::initialize()
{
   ossimImageSourceFilter::initialize();
   
   // theInputConnection is defined in ossimImageSourceFilter
   // It is automatically set when an input is connected to this
   // object
   if(theInputConnection)
   {
      theTile = ossimImageDataFactory::instance()->create(this, this);

      // now initialize the tile
      theTile->initialize();
      checkWeights();
   }
}

double ossimBandAverageFilter::getNullPixelValue(ossim_uint32 band)const
{
   if(!isSourceEnabled())
   {
      return ossimImageSourceFilter::getNullPixelValue(band);
   }
   // lets use the first band's null value as our null
   return ossimImageSourceFilter::getMinPixelValue(0);
}

double ossimBandAverageFilter::getMinPixelValue(ossim_uint32 band)const
{
   if(!isSourceEnabled()||!theInputConnection)
   {
      return ossimImageSourceFilter::getMinPixelValue(band);
   }

   // loop through each band and find the min pix value
   ossim_uint32 bandIndex = 0;
   ossim_uint32 inputBands = theInputConnection->getNumberOfOutputBands();
   double minValue = ossim::defaultMax(OSSIM_DOUBLE);
   for(bandIndex = 0; bandIndex < inputBands; ++bandIndex)
   {
      double minPix = theInputConnection->getMinPixelValue(bandIndex);

      if(minPix < minValue)
      {
         minValue = minPix;
      }
   }

   return minValue;
}

double ossimBandAverageFilter::getMaxPixelValue(ossim_uint32 band)const
{
   if(!isSourceEnabled())
   {
      return ossimImageSourceFilter::getMaxPixelValue(band);
   }
   
   // loop through each band and find the max pix value
   ossim_uint32 bandIndex = 0;
   ossim_uint32 inputBands = theInputConnection->getNumberOfOutputBands();
   double maxValue = ossim::defaultMin(OSSIM_DOUBLE);
   for(bandIndex = 0; bandIndex < inputBands; ++bandIndex)
   {
      double maxPix = theInputConnection->getMaxPixelValue(bandIndex);

      if(maxPix > maxValue)
      {
         maxValue = maxPix;
      }
   }
   
   return maxValue;
}

void ossimBandAverageFilter::checkWeights()
{
   // we have not been initialzed yet
   //
   if(!theTile)
   {
      return;
   }

   // Check to see if the weights array is the same size as the input
   // list.  If not then resize it and populate them with the same weight
   // value.  It does not matter if they sum to one since we will perform
   // a weighted average in the execution of the algorithm
   //
   if(theWeights.size() != theInputConnection->getNumberOfOutputBands())
   {
      theWeights.resize(theInputConnection->getNumberOfOutputBands());  

      std::fill(theWeights.begin(), theWeights.end(), 1);
   }
}


template<class T> void ossimBandAverageFilter::averageInput(
   T /* dummyVariable */, // used only for template type, value ignored
   ossimRefPtr<ossimImageData> inputDataObject)
{
   // since we have already checked the input for empty or
   // null we will now execute the algorithm.  We will make
   //
   // 
   std::vector<T*> inputBands(inputDataObject->getNumberOfBands());
   std::vector<double> nullValues(inputDataObject->getNumberOfBands());
   ossim_uint32 i = 0;

   // for efficiency we will copy the band pointers and
   // null values so we don't have extra function calls
   // on a per band basis
   //
   for(i = 0; i < inputDataObject->getNumberOfBands(); ++i)
   {
      inputBands[i] = static_cast<T*>(inputDataObject->getBuf(i));
      nullValues[i] = inputDataObject->getNullPix(i);
   }

   // store the output buffer pointer
   T* outputBuf = static_cast<T*>(theTile->getBuf(0));

   // setup index values and boundary values
   ossim_uint32 upperBound = theTile->getWidth()*theTile->getHeight();
   ossim_uint32 bandIndex = 0;
   ossim_uint32 numberOfBands = inputDataObject->getNumberOfBands();
   double outputNullPix = theTile->getNullPix(0);
   
   for(i = 0; i < upperBound; ++i)
   {
      double value=0.0;
      double sumWeights=0.0;

      // loop over each band only using the valid pixel data
      // in the weighted average.
      //
      for(bandIndex = 0; bandIndex < numberOfBands; ++bandIndex)
      {
         // if valid then muliply the value by the weight and add it to
         // current pixel value
         if(*inputBands[bandIndex] != nullValues[bandIndex])
         {
            value += theWeights[bandIndex]*((double)*inputBands[bandIndex]);
            sumWeights += theWeights[bandIndex];
         }
         // increment to the next pixel for the next time around
         ++inputBands[bandIndex];
      }

      // check to see if we had any valid data.  If we did the weight
      // should be greater than 0
      if(sumWeights != 0.0)
      {
         value /= sumWeights;
      }
      else
      {
         // else we set the pixel to the output null value
         value = outputNullPix;
      }
      // store the result in the outputBuffer.
      *outputBuf = static_cast<T>(value);

      // advance the output buffer to the next pixel value
      ++outputBuf;
   }
}


bool ossimBandAverageFilter::loadState(const ossimKeywordlist& kwl,
                                       const char* prefix)
{
   // get the value of the stored keyword
   ossimString weightString = kwl.find(prefix, WEIGHTS_KW);
   weightString = weightString.trim();
   

   theWeights.clear();
   if(weightString != "")
   {
      // split the string list into an array of strings
      std::vector<ossimString> weightList = weightString.split(" ");

      // resize the weights to the size of the weight list
      //
      theWeights.resize(weightList.size());
      ossim_uint32 i = 0;

      // now store the weights to the array
      for(i = 0; i < theWeights.size(); ++i)
      {
         theWeights[i] = weightList[i].toDouble();
      }
   }

   // call base class to continue the load state
   return ossimImageSourceFilter::loadState(kwl, prefix);
}

bool ossimBandAverageFilter::saveState(ossimKeywordlist& kwl,
                                       const char* prefix)const
{
   std::stringstream out;

   // copy the weights  to a memory stream separated by space
   std::copy(theWeights.begin(),
             theWeights.end(),
             ostream_iterator<double>(out, " "));

   ossimString weightString = out.str();
   weightString = weightString.trim();

   
   kwl.add(prefix, // prefix to help uniquely id or attributes
           WEIGHTS_KW, // the keyword to identity our attribute
           weightString.c_str(),     // the value 
           true); // overwrite if already in the keywordlist
   
   // call base classes save to allow us to pass the information up
   return ossimImageSourceFilter::saveState(kwl, prefix);
}

double ossimBandAverageFilter::getWeight(ossim_uint32 band)const
{
   if(band < theWeights.size())
   {
      return theWeights[band];
   }
   
   return 0.0;
}

void ossimBandAverageFilter::setWeight(ossim_uint32 band, double weight)
{
   if(band < theWeights.size())
   {
      theWeights[band] = fabs(weight);
   }
}
