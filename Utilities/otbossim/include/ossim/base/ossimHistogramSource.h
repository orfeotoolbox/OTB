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
// $Id: ossimHistogramSource.h 11721 2007-09-13 13:19:34Z gpotts $
#ifndef ossimHistogramSource_HEADER
#define ossimHistogramSource_HEADER

#include <ossim/base/ossimSource.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimMultiResLevelHistogram.h>

class OSSIM_DLL ossimHistogramSource : public ossimSource
{
public:
   
   virtual ~ossimHistogramSource();
   
   virtual ossimRefPtr<ossimMultiResLevelHistogram> getHistogram();
   
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
   
   ossimRefPtr<ossimMultiResLevelHistogram> theHistogram;
   
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
