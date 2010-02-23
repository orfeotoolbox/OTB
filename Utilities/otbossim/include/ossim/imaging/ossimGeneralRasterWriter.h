//*******************************************************************
// Copyright (C) 2001 ImageLinks Inc.
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:
//
// Contains class declaration for ossimGeneralRasterWriter.
//
//*******************************************************************
//  $Id: ossimGeneralRasterWriter.h 16081 2009-12-10 20:56:36Z eshirschorn $

#ifndef ossimGeneralRasterWriter_HEADER
#define ossimGeneralRasterWriter_HEADER

#include <ossim/imaging/ossimImageFileWriter.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeyword.h>

class ossimGeneralRasterWriter : public ossimImageFileWriter
{
public:
   ossimGeneralRasterWriter();
   ossimGeneralRasterWriter(ossimImageSource* inputSource,
                            const ossimFilename& file=ossimFilename(""));

   /**
    * void getImageTypeList(std::vector<ossimString>& imageTypeList)const
    *
    * Appends this writer image types to list "imageTypeList".
    *
    * This writer has the following types:
    * general_raster_bip
    * general_raster_bil
    * general_raster_bsq
    *
    * @param imageTypeList stl::vector<ossimString> list to append to.
    */ 
   virtual void getImageTypeList(std::vector<ossimString>& imageTypeList)const;

   /**
    * Returns a 3-letter extension from the image type descriptor 
    * (theOutputImageType) that can be used for image file extensions.
    *
    * @param imageType string representing image type.
    *
    * @return the 3-letter string extension.
    */
   virtual ossimString getExtension() const;

   virtual void set_rr_level(ossim_uint32 rr) { theRlevel = rr; }
   virtual bool isOpen()const;      
   virtual bool open();
   virtual void close();

   /**
    * saves the state of the object.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;
   
   /**
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
protected:
   virtual ~ossimGeneralRasterWriter();

private:
   virtual bool writeFile();

   /**
    *  Writes image data to output file in BIP(Band Interleaved by Pixel)
    *  format.
    *  @return true on success, false on error.
    */
   bool writeToBip();

   /**
    *  Writes image data to output file in BIL(Band Interleaved by Line)
    *  format.
    *  @return true on success, false on error.
    */
   bool writeToBil();
   
   /**
    *  Writes image data to output file in BSQ(Band Sequential) format.
    *  @return true on success, false on error.
    */
   bool writeToBsq();
   
   /**
    *  Writes an ossim header file which can be used to load file in ossim.
    *  Returns true on success, false on error.
    */
   void writeHeader() const;

   /**
    *  Writes an envi header file which can be used to load file in envi.
    *  If the output image file is "foo.ras" then the ossim header file will
    *  be "foo.hdr".
    */
   void writeEnviHeader() const;

   /**
    * @return The interleave as a string of either: bil, bip, or bsq
    */
   ossimString getInterleaveString() const;
   
   
   std::ofstream       theFileStream;
   bool                theOverviewFlag;
   ossim_uint32        theRlevel;
   ossimByteOrder      theOutputByteOrder;
   
  /**
   * Populated while writting the data
   */
  std::vector<double>     theMinPerBand;
  std::vector<double>     theMaxPerBand;

TYPE_DATA
};

#endif
