//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimImageMpiSWriterSequenceConnection.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimImageMpiSWriterSequenceConnection_HEADER
#define ossimImageMpiSWriterSequenceConnection_HEADER
#include <ossim/imaging/ossimImageSourceSequencer.h>
class ossimImageData;
class ossimImageMpiSWriterSequenceConnection : public ossimImageSourceSequencer
{
public:
   ossimImageMpiSWriterSequenceConnection(ossimObject* owner=NULL,
                                          long numberOfTilesToBuffer = 2);
   
   ossimImageMpiSWriterSequenceConnection(ossimImageSource* inputSource,
                                          ossimObject* owner=NULL,
                                          long numberOfTilesToBuffer = 2);
   
  virtual ~ossimImageMpiSWriterSequenceConnection();
   virtual bool isMaster()const
      {
         return false;
      }

   virtual void initialize();
   virtual void setToStartOfSequence();
   /*!
    * Will allow you to get the next tile in the sequence.
    */
   virtual ossimRefPtr<ossimImageData> getNextTile(ossim_uint32 resLevel=0);

   virtual void slaveProcessTiles();
   
protected:
   int theNumberOfProcessors;
   int theRank;
   int theNumberOfTilesToBuffer;
   
   ossimRefPtr<ossimImageData>* theOutputTile;

   void deleteOutputTiles();

TYPE_DATA
};

#endif
