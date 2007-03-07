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
// $Id: ossimHistogramSource.cpp,v 1.5 2005/10/06 13:44:05 dburken Exp $

#include <base/common/ossimHistogramSource.h>
#include <base/common/ossimKeywordNames.h>
#include <base/data_types/ossimMultiResLevelHistogram.h>

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
     theHistogram(NULL),
     theFilename()
{
}

ossimHistogramSource::~ossimHistogramSource()
{
   if (theHistogram)
   {
      delete theHistogram;
      theHistogram = NULL;
   }
}

ossimMultiResLevelHistogram* ossimHistogramSource::getHistogram()
{
   return theHistogram;
}

bool ossimHistogramSource::loadState(const ossimKeywordlist& kwl,
				     const char* prefix)
{
   if(theHistogram)
   {
      delete theHistogram;
      theHistogram = NULL;
   }
   
   const char* externalFile = kwl.find(prefix,
                                       ossimKeywordNames::FILENAME_KW);

   theHistogram = new ossimMultiResLevelHistogram;
   if(externalFile)
   {
      if(!theHistogram->importHistogram(ossimFilename(externalFile)))
      {
         delete theHistogram;
         theHistogram = NULL;
         theFilename = "";
      }
      theFilename = externalFile;
   }
   else
   {
      ossimString newPrefix = ossimString(prefix) + "histogram.";
      if(!theHistogram->loadState(kwl, newPrefix))
      {
         delete theHistogram;
         theHistogram = NULL;
      }
   }
   
   return ossimSource::loadState(kwl, prefix);
}

bool ossimHistogramSource::saveState(ossimKeywordlist& kwl,
				     const char* prefix)const
{
   if(theHistogram)
   {
      if( (theFilename != "")&&
          theHistogram)
      {
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
   theHistogram(NULL),
   theFilename()
{
}
