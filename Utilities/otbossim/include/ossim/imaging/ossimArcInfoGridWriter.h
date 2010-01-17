//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimArcInfoGridWriter.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimArcInfoGridWriter_HEADER
#define ossimArcInfoGridWriter_HEADER

#include <ossim/imaging/ossimImageFileWriter.h>
#include <ossim/base/ossimKeywordlist.h>

//*******************************************************************
// CLASS:  ossimArdcInfoGridWriterFactory
//*******************************************************************

class OSSIM_DLL ossimArcInfoGridWriter : public ossimImageFileWriter
{
public:
   ossimArcInfoGridWriter();
   virtual              ~ossimArcInfoGridWriter();
   
   ossimArcInfoGridWriter(ossimImageSource *inputSource,
                          const ossimFilename& filename);
   virtual bool isOpen()const;
   
   virtual bool open();
   virtual void close();

   /*!
    * saves the state of the object.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;
   
   /*!
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   
private:
   virtual bool writeFile();

   virtual bool writeBinaryAigHeader();
   virtual bool writeBinaryAigBounds();
   virtual bool writeBinaryAigStats();
   
   ossimIpt theOutputTileSize;
   
   
TYPE_DATA
};

#endif
