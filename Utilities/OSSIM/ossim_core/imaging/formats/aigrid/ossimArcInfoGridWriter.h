//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimArcInfoGridWriter.h,v 1.5 2006/01/06 16:46:46 dburken Exp $
#ifndef ossimArcInfoGridWriter_HEADER
#define ossimArcInfoGridWriter_HEADER

#include <imaging/formats/ossimImageFileWriter.h>
#include <base/data_types/ossimKeywordlist.h>

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
