//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License 
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Garrett Potts (gpotts@imagelinks)
// Description:
//
//*************************************************************************
// $Id: ossimImageSourceHistogramFilter.cpp,v 1.12 2005/08/31 15:47:42 gpotts Exp $
#include "ossimImageSourceHistogramFilter.h"
#include "base/common/ossimHistogramSource.h"
#include "base/data_types/ossimMultiResLevelHistogram.h"
#include "base/data_types/ossimMultiBandHistogram.h"
#include "base/data_types/ossimHistogram.h"
#include "base/common/ossimCommon.h"
#include "imaging/ossimImageData.h"
#include "base/data_types/ossimKeyword.h"
#include "base/data_types/ossimKeywordlist.h"

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
                                                                 ossimMultiResLevelHistogram* histogram)
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
   if(theHistogram)
   {
      delete theHistogram;
      theHistogram = NULL;
   }   
}


void ossimImageSourceHistogramFilter::setHistogram(ossimMultiResLevelHistogram* histogram)
{
   if(theHistogram&&
      theHistogram!=histogram)
   {
      delete theHistogram;
      theHistogram = NULL;
      
   }
   theHistogram = histogram;
   
   if(theHistogram)
   {
      theHistogram->setBinCount(0, 0);
   }
}

bool ossimImageSourceHistogramFilter::setHistogram(const ossimFilename& filename)
{
   ossimMultiResLevelHistogram* histogram = new ossimMultiResLevelHistogram;

   bool result = histogram->importHistogram(filename);

   if(result)
   {
      theFilename = filename;
      setHistogram(histogram);
   }
   else
   {
      delete histogram;
      histogram = 0;
   }
   
   return result;
}

ossimMultiResLevelHistogram* ossimImageSourceHistogramFilter::getHistogram()
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

   return (ossimMultiResLevelHistogram*)NULL;
}

const ossimMultiResLevelHistogram* ossimImageSourceHistogramFilter::getHistogram()const
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

   return (ossimMultiResLevelHistogram*)NULL;
}

bool ossimImageSourceHistogramFilter::canConnectMyInputTo(ossim_int32 inputIndex,
                                                          const ossimConnectableObject* object)const
{
   if(object)
   {
      if(PTR_CAST(ossimImageSourceInterface, object) && ( inputIndex == 0 ))
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
   theInputConnection = PTR_CAST(ossimImageSourceInterface, getInput(0));
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
   if(theHistogram)
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
