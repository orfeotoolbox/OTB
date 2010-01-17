//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimHistogramWriter.h 15766 2009-10-20 12:37:09Z gpotts $
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
#include <ossim/imaging/ossimImageSource.h>

#include <fstream>

class ossimImageSource;
class ossimHistogramWriterProcessListener;
class ossimImageHistogramSource;

class OSSIMDLLEXPORT ossimHistogramWriter : public ossimOutputSource,
                                            public ossimProcessInterface,
                                            public ossimConnectableObjectListener
{
public:
   friend class ossimHistogramWriterProcessListener;
   
   ossimHistogramWriter(ossimImageSource* inputSource=0,
                        ossimObject* owner=0);

   void setAreaOfInterest(const ossimIrect& rect);

   virtual ossimObject* getObject();

   virtual const ossimObject* getObject()const;

   virtual bool execute();
   
   virtual bool isOpen()const;

   /*!
    * open the histogram file for output.  Note if the file was
    * previously open then it will close it and re-opens the
    * file.
    */
   virtual bool open();

   virtual bool open(const ossimFilename& filename);

   virtual void close();

   /*!
    * Overrides the base class and maps the outputName to a filename.
    */
   virtual void setOutputName(const ossimString& outputName);
   
   virtual void setFilename(const ossimFilename& filename);
   
   bool canConnectMyInputTo(ossim_int32 inputIndex,
                            const ossimConnectableObject* object)const;
   
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;

   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   virtual void processProgressEvent(ossimProcessProgressEvent& event);

   virtual void connectInputEvent(ossimConnectionEvent& event);

   virtual void disconnectInputEvent(ossimConnectionEvent& event);

   /**
    * Overrides base to pass on abort to theHistogramSource.
    */ 
   virtual void abort();
   
protected:
   virtual ~ossimHistogramWriter();
   virtual void writeHistogram();
   ossimIrect                           theAreaOfInterest;
   ossimFilename                        theFilename;
   std::ofstream*                       theFileStream;
   ossimHistogramWriterProcessListener* theProcessListener;

   /** stored for abort call only. */
   ossimHistogramSource*                theHistogramSource;
   
TYPE_DATA
};

#endif
