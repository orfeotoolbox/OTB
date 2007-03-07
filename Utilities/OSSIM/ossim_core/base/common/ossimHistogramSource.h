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
// $Id: ossimHistogramSource.h,v 1.11 2005/10/06 13:44:05 dburken Exp $
#ifndef ossimHistogramSource_HEADER
#define ossimHistogramSource_HEADER

#include <base/common/ossimSource.h>
#include <base/data_types/ossimFilename.h>

class OSSIMDLLEXPORT ossimMultiResLevelHistogram;

class OSSIMDLLEXPORT ossimHistogramSource : public ossimSource
{
public:
   
   virtual ~ossimHistogramSource();
   
   virtual ossimMultiResLevelHistogram* getHistogram();
   
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;
protected:
   ossimHistogramSource(ossimObject* owner=NULL,
                        ossim_uint32 numberOfInputs=0,
                        ossim_uint32 numberOfOutputs=0,
                        bool inputListFixedFlag=true,
                        bool outputListFixedFlag=true);
   
   ossimMultiResLevelHistogram* theHistogram;
   
   /*!
    * if the filename is not "" then it will use
    * this to store the histogram outside the keywordlist.
    *
    * it will set the filename keyword to the filename and
    * during a save state will save it out to the file
    * instead of inline to the keyword list.
    */
   ossimFilename                theFilename;
   
private:
   /** Hidden from use copy constructor. */
   ossimHistogramSource(const ossimHistogramSource&);

TYPE_DATA
};

#endif /* End of "#ifndef ossimHistogramSource_HEADER" */
