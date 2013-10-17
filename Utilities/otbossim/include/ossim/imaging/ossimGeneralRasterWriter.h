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
// $Id: ossimGeneralRasterWriter.h 22197 2013-03-12 02:00:55Z dburken $

#ifndef ossimGeneralRasterWriter_HEADER
#define ossimGeneralRasterWriter_HEADER 1

#include <ossim/base/ossimConstants.h>
#include <ossim/imaging/ossimImageFileWriter.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeyword.h>

class OSSIM_DLL ossimGeneralRasterWriter : public ossimImageFileWriter
{
public:

   /** @brief default constructor */
   ossimGeneralRasterWriter();
   
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

   /**
    * @brief Method to write the image to a stream.
    *
    * Callers must call setOutputStream(...) method prior to this.
    *
    * @return true on success, false on error.
    */
   virtual bool writeStream();

   /**
    * @brief Sets the output stream to write to.
    *
    * The stream will not be closed/deleted by this object.
    *
    * @param output The stream to write to.
    */
   virtual bool setOutputStream(std::ostream& stream);
   
protected:

   /**
    * Write out the file.
    * @return true on success, false on error.
    */
   virtual bool writeFile();

   /** @brief Protected ( this is a ossimRefPtr) destructor. */
   virtual ~ossimGeneralRasterWriter();

private:

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
   
   std::ostream*       theOutputStream;
   bool                theOwnsStreamFlag;
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
