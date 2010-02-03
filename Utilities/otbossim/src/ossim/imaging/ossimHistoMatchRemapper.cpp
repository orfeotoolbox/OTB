//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimHistoMatchRemapper.cpp 15833 2009-10-29 01:41:53Z eshirschorn $
#include <ossim/imaging/ossimHistoMatchRemapper.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageSource.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimKeywordlist.h>

ossimHistoMatchRemapper::ossimHistoMatchRemapper()
   : ossimImageSourceFilter()
{
   theBlankTile = NULL;
}

ossimHistoMatchRemapper::ossimHistoMatchRemapper(ossimImageSource* inputSource,
                                                 const vector<double>& targetMeanPerBand,
                                                 const vector<double>& targetSigmaPerBand,
                                                 const vector<double>& inputMeanPerBand,
                                                 const vector<double>& inputSigmaPerBand)
   : ossimImageSourceFilter(inputSource),
     theTargetMeanPerBand(targetMeanPerBand),
     theTargetSigmaPerBand(targetSigmaPerBand),
     theInputMeanPerBand(inputMeanPerBand),
     theInputSigmaPerBand(inputSigmaPerBand)
{
   theBlankTile = NULL;
}


ossimHistoMatchRemapper::~ossimHistoMatchRemapper()
{
}

ossimRefPtr<ossimImageData> ossimHistoMatchRemapper::getTile(
   const ossimIrect& tileRect,
   ossim_uint32 resLevel)
{
   ossimRefPtr<ossimImageData> tile;
   if(theInputConnection)
   {
      tile = theInputConnection->getTile(tileRect, resLevel);
   }
   if(!tile.valid())
   {
      return NULL;
   }
   if((tile.valid() &&
       ((tile->getDataObjectStatus()==OSSIM_NULL) ||
        (tile->getDataObjectStatus()==OSSIM_EMPTY))))
      
   {
      return tile;
   }

   theBlankTile->setOrigin(tileRect.ul());
   ossim_uint32 numberOfBands = (ossim_uint32)theInputMeanPerBand.size();

   numberOfBands = numberOfBands>tile->getNumberOfBands()?tile->getNumberOfBands():numberOfBands;
   double result = 0;
   if(isSourceEnabled())
   {
      long offsetBound = (long)(tile->getWidth()*tile->getHeight());
      if(tile->getScalarType() == OSSIM_UCHAR)
      {
         for(ossim_uint32 band = 0; band < numberOfBands; ++band)
         {
            unsigned char* buf = static_cast<unsigned char*>(tile->getBuf(band));
            for(long offset=0; offset < offsetBound;++offset)
            {
               // if the input has no deviation we will just
               // do a shift to the target mean
               if(fabs(theInputSigmaPerBand[band]) < FLT_EPSILON)
               {
                  result = transLean(buf[offset],
                                     theInputMeanPerBand[band],
                                     theTargetMeanPerBand[band],
                                     0,
                                     255);
               }
               else
               {
                  result = transLeanStretch(buf[offset],
                                            theInputMeanPerBand[band],
                                            theInputSigmaPerBand[band],
                                            theTargetMeanPerBand[band],
                                            theTargetSigmaPerBand[band],
                                            0,
                                            255);
               }
               result = ((result>255)?255:result);
               result = ((result<0)?0:result);
               
               buf[offset] = (unsigned char)result;
            }
         }
      }
   }

   return tile;
}

void ossimHistoMatchRemapper::initialize()
{
   if(!theInputConnection)
   {
      return;
   }
   theBlankTile = new ossimImageData(this,
                                     theInputConnection->getOutputScalarType(),
                                     theInputConnection->getNumberOfOutputBands());
}

const vector<double>& ossimHistoMatchRemapper::getInputMeanValues()const
{
   return theInputMeanPerBand;
}

const vector<double>& ossimHistoMatchRemapper::getInputSigmaValues()const
{
   return theInputSigmaPerBand;
}

const vector<double>& ossimHistoMatchRemapper::getTargetMeanValues()const
{
   return theTargetMeanPerBand;
}

const vector<double>& ossimHistoMatchRemapper::getTargetSigmaValues()const
{
   return theTargetSigmaPerBand;
}

void ossimHistoMatchRemapper::setInputMeanValues(const vector<double>& newValues)
{
   theInputMeanPerBand = newValues;
}

void ossimHistoMatchRemapper::setInputSigmaValues(const vector<double>& newValues)
{
   theInputSigmaPerBand = newValues;
}

void ossimHistoMatchRemapper::setTargetMeanValues(const vector<double>& newValues)
{
   theTargetMeanPerBand = newValues;
}

void ossimHistoMatchRemapper::setTargetSigmaValues(const vector<double>& newValues)
{
   theTargetSigmaPerBand = newValues;
}

double  ossimHistoMatchRemapper::transLean   // returns vOut
(
   double vIn         //!< input value to be transformed
   , double vBias       //!< bias value to be removed
   , double vTarget     //!< value that will replace bias
   , double vMin        //!< minimum valid value of vIn (inclusive)
   , double vMax        //!< maximum valid value of vIn (inclusive)
)
{
   // max change
   const double vDelta = vTarget - vBias;
   double weight=0.0;
   
   // weight max change dependend on proximity to end points
   if (vIn <= vBias)
   {
      weight = fabs((vIn - vMin) / (vBias - vMin));
   }
   else
   {
      weight = fabs((vMax - vIn) / (vMax - vBias));
   }
   return  (vIn + (vDelta * weight));
};

double  ossimHistoMatchRemapper::transLeanStretch   // returns vOut
        (
          double vIn              //!< input value to be transformed
        , double vBias            //!< bias value to be removed
        , double vBiasStretch     //!< dispersion (+/-) about vBias
        , double vTarget          //!< value that will replace bias
        , double vTargetStretch   //!< dispersion (+/-) about vTarget
        , double vMin             //!< minimum valid value of vIn (inclusive)
        , double vMax             //!< maximum valid value of vIn (inclusive)
        )
{

        // line segment end points
        const double x1 = vMin;
        const double x2 = vBias - vBiasStretch;
        const double x3 = vBias + vBiasStretch;
        const double x4 = vMax;
        const double y1 = 0.0;
        const double y2 = (vTarget + vTargetStretch) - (vBias + vBiasStretch);
        const double y3 = (vTarget - vTargetStretch) - (vBias - vBiasStretch);
        const double y4 = 0.0;


        // perform appropriate linear lookup operation based on input value
        double vOut = vIn;
        if (vIn <= x2)
        {
                vOut = (vIn - x1) * (y2-y1)/(x2-x1) + y1 + vIn;
        }
        else
        if (vIn <= x3)
        {
                vOut = (vIn - x2) * (y3-y2)/(x3-x2) + y2 + vIn;
        }
        else
        {
                vOut = (vIn - x3) * (y4-y3)/(x4-x3) + y3 + vIn;
        }
        
        return vOut;
}

bool ossimHistoMatchRemapper::loadState(const ossimKeywordlist& kwl,
                                        const char* prefix)
{
   theTargetMeanPerBand.clear();
   theTargetSigmaPerBand.clear();
   theInputMeanPerBand.clear();
   theInputSigmaPerBand.clear();
   ossim_uint32 result = kwl.getNumberOfSubstringKeys(ossimString(prefix) + "target_mean[0-9]");
   ossim_uint32 numberOfMatches=0;
   ossim_uint32 index=0;

   // load target mean
   while(numberOfMatches < result)
   {
      ossimString key = prefix;
      key += ossimString("target_mean");
      key += ossimString::toString(index);
      const char* lookup = kwl.find(key.c_str());
      if(lookup)
      {
         theTargetMeanPerBand.push_back(ossimString(lookup).toDouble());
         ++numberOfMatches;
      }
      ++index;
   }
   numberOfMatches = 0;
   index = 0;
   result = kwl.getNumberOfSubstringKeys(ossimString(prefix) + "target_sigma[0-9]");
   // load target sigma
   while(numberOfMatches < result)
   {
      ossimString key = prefix;
      key += ossimString("target_sigma");
      key += ossimString::toString(index);
      const char* lookup = kwl.find(key.c_str());
      if(lookup)
      {
         theTargetSigmaPerBand.push_back(ossimString(lookup).toDouble());
         ++numberOfMatches;
      }
      ++index;
   }

   numberOfMatches = 0;
   index = 0;
   result = kwl.getNumberOfSubstringKeys(ossimString(prefix) + "input_mean[0-9]");
   // load input mean
   while(numberOfMatches < result)
   {
      ossimString key = prefix;
      key += ossimString("input_mean");
      key += ossimString::toString(index);
      const char* lookup = kwl.find(key.c_str());
      if(lookup)
      {
         theInputMeanPerBand.push_back(ossimString(lookup).toDouble());
         ++numberOfMatches;
      }
      ++index;
   }
   
   numberOfMatches = 0;
   index = 0;
   result = kwl.getNumberOfSubstringKeys(ossimString(prefix) + "input_sigma[0-9]");
   // load input sigma
   while(numberOfMatches < result)
   {
      ossimString key = prefix;
      key += ossimString("input_sigma");
      key += ossimString::toString(index);
      const char* lookup = kwl.find(key.c_str());
      if(lookup)
      {
         theInputSigmaPerBand.push_back(ossimString(lookup).toDouble());
         ++numberOfMatches;
      }
      ++index;
   }
   
   return true;
}

bool ossimHistoMatchRemapper::saveState(ossimKeywordlist& kwl,
                                        const char* prefix)
{
   ossim_uint32 index = 1;
   for(index=1;index <= theTargetMeanPerBand.size();++index)
   {
      ossimString key = ossimString("target_mean") + ossimString::toString(index);
      kwl.add(prefix,
              key.c_str(),
              theTargetMeanPerBand[index-1],
              true);
   }
   for(index=1;index <= theTargetSigmaPerBand.size();++index)
   {
      ossimString key = ossimString("target_sigma") + ossimString::toString(index);
      kwl.add(prefix,
              key.c_str(),
              theTargetSigmaPerBand[index-1],
              true);
   }
   for(index=1;index <= theInputMeanPerBand.size();++index)
   {
      ossimString key = ossimString("input_mean") + ossimString::toString(index);
      kwl.add(prefix,
              key.c_str(),
              theInputMeanPerBand[index-1],
              true);
   }
   for(index=1;index <= theInputSigmaPerBand.size();++index)
   {
      ossimString key = ossimString("input_sigma") + ossimString::toString(index);
      kwl.add(prefix,
              key.c_str(),
              theInputSigmaPerBand[index-1],
              true);
   }
   return true;  
}
                       
