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
// $Id: ossimHistogramWriter.cpp,v 1.25 2005/09/02 18:02:25 gpotts Exp $
#include <base/common/events/ossimProcessListener.h>
#include <imaging/histogram/ossimHistogramWriter.h>
#include <imaging/ossimImageSource.h>
#include <base/data_types/ossimKeywordlist.h>
#include <imaging/ossimImageSourceSequencer.h>
#include <base/data_types/ossimMultiResLevelHistogram.h>
#include <base/data_types/ossimMultiBandHistogram.h>

class ossimHistogramWriterProcessListener : public ossimProcessListener
{
public:
   ossimHistogramWriterProcessListener(ossimConnectableObject* passon)
      :thePassonTo(passon)
      {
      }
   virtual void processProgressEvent(ossimProcessProgressEvent& event)
      {
         if(thePassonTo)
         {
            event.setObject(thePassonTo);
            thePassonTo->fireEvent(event);
         }
      }

   ossimConnectableObject* thePassonTo;  
   
};

RTTI_DEF3(ossimHistogramWriter,
          "ossimHistogramWriter",
          ossimOutputSource,
          ossimProcessInterface,
          ossimConnectableObjectListener);

ossimHistogramWriter::ossimHistogramWriter(ossimImageSource* inputSource,
                                           ossimObject* owner)
   : ossimOutputSource(owner,
                       1,
                       0,
                       true,
                       false),
     theFileStream(NULL)
{
   theProcessListener = new ossimHistogramWriterProcessListener(this);
   
   theAreaOfInterest.makeNan();
   connectMyInputTo(0, inputSource);
}

ossimHistogramWriter::~ossimHistogramWriter()
{
   if(isOpen())
   {
      close();
   }
   if(theProcessListener)
   {
      delete theProcessListener;
      theProcessListener = 0;
   }
   
}

bool ossimHistogramWriter::saveState(ossimKeywordlist& kwl,
                                     const char* prefix)const
{
   bool result = ossimOutputSource::saveState(kwl, prefix);


   return result;
}

bool ossimHistogramWriter::loadState(const ossimKeywordlist& kwl,
                                     const char* prefix)
{
   bool result = ossimOutputSource::loadState(kwl, prefix);

   if(result)
   {
      if(!getNumberOfInputs())
      {
         setNumberOfInputs(1);
      }
   }

   return result;
}

bool ossimHistogramWriter::execute()
{
   writeHistogram();
   
   return true;
}

void ossimHistogramWriter::writeHistogram()
{
   if(!isOpen())
   {
      open();
      if(!isOpen())
      {
          cerr << "unable to open file " << theFilename << endl;
          return;
       }
    }
    if(!getInput(0))
    {
       cerr << "ossimHistogramWriter::writeHistogram is not connected" << endl;
       return;
    }
   ossimHistogramSource* histoSource = PTR_CAST(ossimHistogramSource, getInput(0));
   bool deleteHistoSource = false;
   if(!histoSource)
   {
     histoSource = new ossimImageHistogramSource;
     histoSource->connectMyInputTo(0, getInput(0));
     histoSource->enableSource();
     deleteHistoSource = true;
   }
   
   histoSource->addListener( theProcessListener);
   
   ossimMultiResLevelHistogram* histo = histoSource->getHistogram();

   if(histo)
   {
      ossimKeywordlist kwl;
      histo->saveState(kwl);

      kwl.writeToStream(*theFileStream);
   }
   histoSource->removeListener(theProcessListener);
   
   if(deleteHistoSource)
   {
      delete histoSource;
      histoSource = NULL;
   }
   
   close();
}

