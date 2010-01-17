//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks)
// Description:
//
//*************************************************************************
// $Id: ossimImageSourceHistogramFilter.cpp 13312 2008-07-27 01:26:52Z gpotts $
#include <ossim/imaging/ossimImageSourceHistogramFilter.h>
#include <ossim/base/ossimHistogramSource.h>
#include <ossim/base/ossimMultiResLevelHistogram.h>
#include <ossim/base/ossimMultiBandHistogram.h>
#include <ossim/base/ossimHistogram.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/base/ossimKeyword.h>
#include <ossim/base/ossimKeywordlist.h>

static const ossimKeyword PROPRIETARY_FILENAME_KW("proprietary_filename",
                                                  "imports a proprietary file");

static const ossimKeyword HISTOGRAM_FILENAME_KW("histogram_filename",
                                                "read an OSSIM histogram file");


RTTI_DEF1(ossimImageSourceHistogramFilter, "ossimImageSourceHistogramFilter", ossimImageSourceFilter)
   
ossimImageSourceHistogramFilter::ossimImageSourceHistogramFilter()
   :ossimImageSourceFilter(),
    theCurrentResLevel(0),
    theHistogram(NULL),
    theFilename("")
{
   setNumberOfInputs(2);
   theInputListIsFixedFlag = true;
}

ossimImageSourceHistogramFilter::ossimImageSourceHistogramFilter(ossimImageSource* inputSource,
                                                                 ossimRefPtr<ossimMultiResLevelHistogram> histogram)
   : ossimImageSourceFilter(inputSource),
     theCurrentResLevel(0),
     theHistogram(histogram),
     theFilename("")
{
   setNumberOfInputs(2);
   theInputListIsFixedFlag = true;
}

ossimImageSourceHistogramFilter::~ossimImageSourceHistogramFilter()
{
}


void ossimImageSourceHistogramFilter::setHistogram(ossimRefPtr<ossimMultiResLevelHistogram> histogram)
{
   theHistogram = histogram;
   
   if(theHistogram.valid())
   {
      theHistogram->setBinCount(0, 0);
   }
}

bool ossimImageSourceHistogramFilter::setHistogram(const ossimFilename& filename)
{
   ossimRefPtr<ossimMultiResLevelHistogram> histogram = new ossimMultiResLevelHistogram;

   bool result = histogram->importHistogram(filename);

   if(result)
   {
      theFilename = filename;
      setHistogram(histogram);
   }
   
   return result;
}

ossimRefPtr<ossimMultiResLevelHistogram> ossimImageSourceHistogramFilter::getHistogram()
{
   if(!getInput(1))
   {
      return theHistogram;
   }
   else
   {
      ossimHistogramSource* histoSource = PTR_CAST(ossimHistogramSource, getInput(1));
      if(histoSource)
      {
         return histoSource->getHistogram();
      }
   }

   return (ossimMultiResLevelHistogram*)0;
}

const ossimRefPtr<ossimMultiResLevelHistogram> ossimImageSourceHistogramFilter::getHistogram()const
{
   if(!getInput(1))
   {
      return theHistogram;
   }
   else
   {
      ossimHistogramSource* histoSource = PTR_CAST(ossimHistogramSource, getInput(1));
      if(histoSource)
      {
         return histoSource->getHistogram();
      }
   }

   return (ossimMultiResLevelHistogram*)0;
}

bool ossimImageSourceHistogramFilter::canConnectMyInputTo(ossim_int32 inputIndex,
                                                          const ossimConnectableObject* object)const
{
   if(object)
   {
      if(PTR_CAST(ossimImageSource, object) && ( inputIndex == 0 ))
      {
         return true;
      }
      else if(PTR_CAST(ossimHistogramSource, object) && ( inputIndex == 1 ))
      {
         return true;
      }
   }

   return false;
}

void ossimImageSourceHistogramFilter::connectInputEvent(ossimConnectionEvent& event)
{
   theInputConnection = PTR_CAST(ossimImageSource, getInput(0));
   if(PTR_CAST(ossimHistogramSource, event.getNewObject()))
   {
      // only initialize if it's a new object
      initialize();
   }
}

bool ossimImageSourceHistogramFilter::saveState(ossimKeywordlist& kwl,
                                                const char* prefix)const
{
   bool result = ossimImageSourceFilter::saveState(kwl, prefix);

   kwl.add(prefix,
           HISTOGRAM_FILENAME_KW,
           theFilename,
           true);
   
   return result;
}

bool ossimImageSourceHistogramFilter::loadState(const ossimKeywordlist& kwl,
                                                const char* prefix)
{
   const char* proprietaryName = kwl.find(prefix, PROPRIETARY_FILENAME_KW);
   const char* ossimName       = kwl.find(prefix, HISTOGRAM_FILENAME_KW);
   bool result = true;
   ossimFilename file;

   if(proprietaryName)
   {
       theFilename = ossimFilename(proprietaryName);
   }
   else if(ossimName)
   {
      if(!theHistogram)
      {
         theHistogram = new ossimMultiResLevelHistogram;
      }
      theFilename = ossimFilename(ossimName);
      
   }

   if(theFilename.exists()&&(theFilename!=""))
   {
      if(!theHistogram)
      {
         theHistogram = new ossimMultiResLevelHistogram;
      }
      result = theHistogram->importHistogram(theFilename);
   }
   if(theHistogram.valid())
   {
      theHistogram->setBinCount(0, 0);
   }
   
   result =  ossimImageSourceFilter::loadState(kwl, prefix);

   setNumberOfInputs(2);
   theInputListIsFixedFlag = true;
   
   return result;
}

std::ostream& ossimImageSourceHistogramFilter::print(std::ostream& out) const
{

   out << "ossimImageSourceHistogramFilter::print:\n"
       << "theFilename:  " << endl;

   return ossimImageSourceFilter::print(out);
}
