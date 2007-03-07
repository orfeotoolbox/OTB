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
//
//*************************************************************************
// $Id: ossimHistogramWriter.h,v 1.28 2005/09/02 18:06:31 dburken Exp $
#ifndef ossimHistogramWriter_HEADER
#define ossimHistogramWriter_HEADER
#include "base/common/ossimOutputSource.h"
#include "base/data_types/ossimIrect.h"
#include "base/data_types/ossimFilename.h"
#include "base/common/ossimProcessInterface.h"
#include "base/common/events/ossimObjectEvents.h"
#include "base/common/events/ossimProcessProgressEvent.h"
#include "base/common/events/ossimProcessListener.h"
#include "base/common/events/ossimConnectableObjectListener.h"
#include "base/common/events/ossimConnectionEvent.h"
#include "base/common/ossimHistogramSource.h"
#include "imaging/histogram/ossimImageHistogramSource.h"

#include "imaging/ossimImageSource.h"

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
