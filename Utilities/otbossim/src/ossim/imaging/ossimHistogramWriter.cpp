//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimHistogramWriter.cpp 20118 2011-10-05 13:50:55Z dburken $
#include <ossim/base/ossimProcessListener.h>
#include <ossim/imaging/ossimHistogramWriter.h>
#include <ossim/imaging/ossimImageSource.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/imaging/ossimImageSourceSequencer.h>
#include <ossim/base/ossimMultiResLevelHistogram.h>
#include <ossim/base/ossimMultiBandHistogram.h>
#include <ossim/imaging/ossimImageHistogramSource.h>
#include <ossim/base/ossimTrace.h>

static ossimTrace traceDebug("ossimHistogramWriter:debug");
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
     theAreaOfInterest(),
     theFilename(),
     theFileStream(0),
     theProcessListener(0),
     theHistogramSource(0)
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

   // This does not need to be deleted here.  Simply stored for abort.
   if (theHistogramSource)
   {
      theHistogramSource = 0;
   }
}

void ossimHistogramWriter::setAreaOfInterest(const ossimIrect& rect)
{
   theAreaOfInterest = rect;
}

ossimObject* ossimHistogramWriter::getObject()
{
   return this;
}

bool ossimHistogramWriter::isOpen()const
{
   return (theFileStream != 0);
}

bool ossimHistogramWriter::open()
{
   if(isOpen())
   {
      close();
   }
   
   theFileStream = new std::ofstream(theFilename.c_str());
   
   return theFileStream->good();
}

bool ossimHistogramWriter::open(const ossimFilename& filename)
{
   return ossimOutputSource::open(filename);
}

void ossimHistogramWriter::close()
{
   delete theFileStream;
   theFileStream = 0;
}

void ossimHistogramWriter::setOutputName(const ossimString& outputName)
{
   ossimOutputSource::setOutputName(outputName);
   setFilename(outputName);
}

void ossimHistogramWriter::setFilename(const ossimFilename& filename)
{
   theFilename = filename;
}
   
bool ossimHistogramWriter::canConnectMyInputTo(
   ossim_int32 inputIndex, const ossimConnectableObject* object)const
{
   return (object&&(inputIndex == 0)&& 
           (PTR_CAST(ossimHistogramSource, object)||
            PTR_CAST(ossimImageSource, object)));
}

const ossimObject* ossimHistogramWriter::getObject()const
{
   return this;
}

void ossimHistogramWriter::processProgressEvent(
   ossimProcessProgressEvent& event)
{
   // we will raise the event if its coming from our
   // input.  This means that we are connected to a
   // ossimHistogramSource.
   //
   if(event.getObject() != this)
   {
      ossimProcessInterface::setCurrentMessage(event.getMessage());
      setPercentComplete(event.getPercentComplete());
   }
}

void ossimHistogramWriter::connectInputEvent(
   ossimConnectionEvent& event)
{
   if(event.getObject() == this)
   {
      if(event.getOldObject())
      {
         event.getOldObject()->removeListener((ossimProcessListener*)this);
      }
      if(getInput(0)&&
         PTR_CAST(ossimHistogramSource, getInput(0)))
      {
         getInput(0)->addListener( (ossimProcessListener*)this);
      }
   }
}

void ossimHistogramWriter::disconnectInputEvent(
   ossimConnectionEvent& event)
{
   if(event.getOldObject()&&
      PTR_CAST(ossimHistogramSource, getInput(0)))
   {
      event.getOldObject()->removeListener((ossimProcessListener*)this);
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

      //---
      // Capture the pointer for abort call.  Note a ossimHistogramSource has
      // no abort so the abort will only work through the
      // ossimImageHistogramSource pointer.
      //---
      theHistogramSource = (ossimImageHistogramSource*)histoSource;
   }

   // Capture the pointer for abort call.
   theHistogramSource = histoSource;
   
   histoSource->addListener( theProcessListener);
   
   ossimRefPtr<ossimMultiResLevelHistogram> histo = histoSource->getHistogram();

   // Don't write histogram if abort flag was set...
   if(histo.valid() && !isAborted() )
   {
      ossimKeywordlist kwl;
      histo->saveState(kwl);
      if(!isOpen())
      {
         open();
         if(!isOpen())
         {
            if(traceDebug())
            {
               ossimNotify(ossimNotifyLevel_WARN) << "unable to open file " << theFilename << endl;
            }
         }
      }
      if( isOpen() )
      {
         kwl.writeToStream(*theFileStream);
      }
   }
   histoSource->removeListener(theProcessListener);
   
   if(deleteHistoSource)
   {
      delete histoSource;
      histoSource = 0;
   }
   theHistogramSource = 0;
   
   close();
}

void ossimHistogramWriter::abort()
{
   //---
   // Call base abort. This sets the status to PROCESS_STATUS_ABORT_REQUEST so the processor
   // knows to abort.
   //---
   ossimProcessInterface::abort();

   // Propagate to histo source.
   if (theHistogramSource)
   {
      ossimImageHistogramSource* histoSource =
         PTR_CAST(ossimImageHistogramSource, theHistogramSource);
      if (histoSource)
      {
         histoSource->abort();
      }
   }

   //---
   // Now set status to "ABORTED" so ossimProcessInterface::isAborted returns true so the
   // writeHistogram method doesn't write the file if the process was aborted.
   //---
   setProcessStatus(PROCESS_STATUS_ABORTED);
}
