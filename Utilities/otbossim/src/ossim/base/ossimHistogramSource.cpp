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
// $Id: ossimHistogramSource.cpp 11721 2007-09-13 13:19:34Z gpotts $

#include <ossim/base/ossimHistogramSource.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimMultiResLevelHistogram.h>

RTTI_DEF1(ossimHistogramSource, "ossimHistogramSource", ossimSource);


ossimHistogramSource::ossimHistogramSource(ossimObject* owner,
                                           ossim_uint32 numberOfInputs,
                                           ossim_uint32 numberOfOutputs,
                                           bool inputListFixedFlag,
                                           bool outputListFixedFlag)
   : ossimSource(owner,
                 numberOfInputs,
                 numberOfOutputs,
                 inputListFixedFlag,
                 outputListFixedFlag),
     theHistogram(0),
     theFilename()
{
}

ossimHistogramSource::~ossimHistogramSource()
{
}

ossimRefPtr<ossimMultiResLevelHistogram> ossimHistogramSource::getHistogram()
{
   return theHistogram;
}

bool ossimHistogramSource::loadState(const ossimKeywordlist& kwl,
				     const char* prefix)
{   
   theHistogram = 0;
   const char* externalFile = kwl.find(prefix,
                                       ossimKeywordNames::FILENAME_KW);

   theHistogram = new ossimMultiResLevelHistogram;
   if(externalFile)
   {
      if(!theHistogram->importHistogram(ossimFilename(externalFile)))
      {
         theHistogram = 0;
         theFilename = "";
      }
      theFilename = externalFile;
   }
   else
   {
      ossimString newPrefix = ossimString(prefix) + "histogram.";
      if(!theHistogram->loadState(kwl, newPrefix))
      {
         theHistogram = 0;
      }
   }
   
   return ossimSource::loadState(kwl, prefix);
}

bool ossimHistogramSource::saveState(ossimKeywordlist& kwl,
				     const char* prefix)const
{
   if(theHistogram.valid())
   {
      if(!theFilename.empty())      {
         ossimKeywordlist kwl2;
         
         if(theHistogram->saveState(kwl2))
         {
            kwl2.write(theFilename.c_str());
            kwl.add(prefix,
                    ossimKeywordNames::FILENAME_KW,
                    theFilename.c_str(),
                    true);
         }
      }
      else
      {
         ossimString newPrefix = ossimString(prefix) + "histogram.";
         theHistogram->saveState(kwl, newPrefix);
      }
   }
   
   return ossimSource::saveState(kwl, prefix);
}

// Hidden from use.
ossimHistogramSource::ossimHistogramSource(const ossimHistogramSource&)
   :
   theHistogram(0),
   theFilename()
{
}
