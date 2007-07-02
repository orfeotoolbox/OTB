//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks)
//
//*************************************************************************
// $Id: ossimHistogramWriter.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimHistogramWriter_HEADER
#define ossimHistogramWriter_HEADER
#include <ossim/base/ossimOutputSource.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimProcessInterface.h>
#include <ossim/base/ossimObjectEvents.h>
#include <ossim/base/ossimProcessProgressEvent.h>
#include <ossim/base/ossimProcessListener.h>
#include <ossim/base/ossimConnectableObjectListener.h>
#include <ossim/base/ossimConnectionEvent.h>
#include <ossim/base/ossimHistogramSource.h>
#include <ossim/imaging/ossimImageHistogramSource.h>

#include <ossim/imaging/ossimImageSource.h>

#include <fstream>

class ossimImageSource;
class ossimHistogramWriterProcessListener;

class OSSIMDLLEXPORT ossimHistogramWriter : public ossimOutputSource,
                                            public ossimProcessInterface,
                                            public ossimConnectableObjectListener
{
public:
   friend class ossimHistogramWriterProcessListener;
   
   ossimHistogramWriter(ossimImageSource* inputSource=NULL,
                        ossimObject* owner=NULL);

   virtual ~ossimHistogramWriter();
   void setAreaOfInterest(const ossimIrect& rect)
      {
         theAreaOfInterest = rect;
      }

  virtual ossimObject* getObject()
  {
    return this;
  }
  virtual const ossimObject* getObject()const
  {
    return this;
  }

   virtual bool execute();
   
   virtual bool isOpen()const
      {
         return (theFileStream != NULL);
         
      }
   /*!
    * open the histogram file for output.  Note if the file was
    * previously open then it will close it and re-opens the
    * file.
    */
   virtual bool open()
      {
         if(isOpen())
         {
            close();
         }
         
         theFileStream = new std::ofstream(theFilename.c_str());
         
         return theFileStream->good();
      }

   virtual bool open(const ossimFilename& filename)
      {
         return ossimOutputSource::open(filename);
      }
   virtual void close()
      {
         delete theFileStream;
         theFileStream = NULL;
      }

   /*!
    * Overrides the base class and maps the outputName to a filename.
    */
   virtual void setOutputName(const ossimString& outputName)
      {
	ossimOutputSource::setOutputName(outputName);
	setFilename(outputName);
      }
   
   virtual void setFilename(const ossimFilename& filename)
      {
         theFilename = filename;
      }
   
   bool canConnectMyInputTo(ossim_int32 inputIndex,
                            const ossimConnectableObject* object)const
      {
         return (object&&(inputIndex == 0)&& 
                 (PTR_CAST(ossimHistogramSource, object)||
		  PTR_CAST(ossimImageSourceInterface, object)));
      }

   
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;

   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   virtual void processProgressEvent(ossimProcessProgressEvent& event)
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

   virtual void connectInputEvent(ossimConnectionEvent& event)
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

   virtual void disconnectInputEvent(ossimConnectionEvent& event)
      {
         if(event.getOldObject()&&
	    PTR_CAST(ossimHistogramSource, getInput(0)))
         {
            event.getOldObject()->removeListener((ossimProcessListener*)this);
         }
      }
protected:
   virtual void writeHistogram();
   ossimIrect     theAreaOfInterest;
   ossimFilename  theFilename;
   std::ofstream*  theFileStream;
   ossimHistogramWriterProcessListener* theProcessListener;
TYPE_DATA
};

#endif
