//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Frank Warmerdam (warmerda@home.com)
//
// Description:
//
// Contains class declaration for TiffWriter.
//
//*******************************************************************
//  $Id: ossimTiffWriter.h 20026 2011-09-01 16:33:18Z dburken $
#ifndef ossimTiffWriter_HEADER
#define ossimTiffWriter_HEADER

#include <ossim/imaging/ossimImageFileWriter.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/projection/ossimMapProjectionInfo.h>
#include <ossim/base/ossimNBandLutDataObject.h>
#include <ossim/imaging/ossimNBandToIndexFilter.h>

class ossimMapProjectionInfo;

class OSSIMDLLEXPORT ossimTiffWriter : public ossimImageFileWriter
{
public:

   ossimTiffWriter();
      
   enum UnitType
   {
      UNDEFINED                    =     0,
      LINEAR_METER                 =  9001,
      LINEAR_FOOT                  =  9002,
      LINEAR_FOOT_US_SURVEY        =  9003,
      ANGULAR_DEGREES              =  9102
   };

   enum
   {
      USER_DEFINED = 32767
   };

   /**
    * void getImageTypeList(std::vector<ossimString>& imageTypeList)const
    *
    * Appends this writer image types to list "imageTypeList".
    *
    * This writer has the following types:
    * tiff_strip
    * tiff_strip_band_separate
    * tiff_tiled
    * tiff_tiled_band_separate
    *
    * @param imageTypeList stl::vector<ossimString> list to append to.
    */   
   virtual void getImageTypeList(std::vector<ossimString>& imageTypeList)const;
   
   virtual bool isOpen()const;
   virtual bool open();
   virtual void close();
   
   /**
    * Ranges from 1 to 100 with 100 being the best.  This value is only used
    * if the compression type is set to jpeg.  (default = 75) 
    *
    * @param quality Value to set theJpegQuality to.
    *
    * @note If quality is out of range it will be set to DEFAULT_JPEG_QUALITY.
    */
   virtual void setJpegQuality(ossim_int32 quality);

   /**
    * @return theJpegQuality
    */
   virtual ossim_int32 getJpegQuality()const;

   /**
    * compression type can be JPEG, PACKBITS, or ZIP/DEFLATE
    */
   virtual void setCompressionType(const ossimString& type);

   virtual ossimString getCompressionType()const;

   virtual bool getGeotiffFlag()const;

   virtual void setGeotiffFlag(bool flag);

   virtual void setTileSize(const ossimIpt& tileSize);

   virtual ossimIpt getOutputTileSize()const;
   
   virtual ossim_int32 setProjectionInfo(const ossimMapProjectionInfo& proj);

   void setLut(const ossimNBandLutDataObject& lut);
      
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
    * Will set the property whose name matches the argument
    * "property->getName()".
    *
    * @param property Object containing property to set.
    */
   virtual void setProperty(ossimRefPtr<ossimProperty> property);

   /**
    * @param name Name of property to return.
    * 
    * @returns A pointer to a property object which matches "name".  Returns
    * NULL if no match is found.
    */
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;

   /**
    * Pushes this's names onto the list of property names.
    *
    * @param propertyNames array to add this's property names to.
    */
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;

   /**
    * Returns a 3-letter extension from the image type descriptor 
    * (theOutputImageType) that can be used for image file extensions.
    *
    * @param imageType string representing image type.
    *
    * @return the 3-letter string extension.
    */
   virtual ossimString getExtension() const;

   bool hasImageType(const ossimString& imageType) const;
   
   //! Will take an ossimIMageData tile and write it to disk as a general raster file.
   static void dumpTileToFile(ossimRefPtr<ossimImageData> t,  const ossimFilename& f);

protected:
   virtual ~ossimTiffWriter();
   
   virtual bool writeFile();

   /**
    *  @return true on success, false on error.
    */
   bool openTiff();

   /**
    *  @return true on success, false on error.
    */
   bool closeTiff();

   /**
    *  Writes image data to a tiled tiff format.
    *  @return true on success, false on error.
    */
   bool writeToTiles();

   /**
    *  Writes image data to a tiled tiff band separate format.
    *  @return true on success, false on error.
    */
   bool writeToTilesBandSep();
   
   /**
    *  Writes image data to a strip tiff format.
    *  @return true on success, false on error.
    */
   bool writeToStrips();

   /**
    *  Writes image data to a strip tiff band separate format.
    *  @return true on success, false on error.
    */
   bool writeToStripsBandSep();
   
   /**
    *  Writes tiff tags from ossimMapProjectionInfo to tiff file.
    *  @return true on success, false on error.
    */
   bool writeTiffTags();

   /**
    *  Writes geotiff tags from ossimMapProjectionInfo to tiff file.
    *  @return true on success, false on error.
    */
   bool writeGeotiffTags(ossimRefPtr<ossimMapProjectionInfo> projectionInfo);

   /**
    *  @return true if the output type is tiled, false if not.
    */
   bool isTiled() const;

   void writeMinMaxTags(const vector<ossim_float64>& minBand,
                        const vector<ossim_float64>& maxBand);

   bool isLutEnabled()const;

   UnitType getUnitType(ossim_int32 pcsCode,
                        const ossimString& projName) const;
   UnitType getPcsUnitType(ossim_int32 pcsCode) const;

   void checkColorLut();
   
   void*                   theTif;
   ossimString             theCompressionType;
   ossimString             thePhotoMetric;
   ossim_int32             theJpegQuality;
   bool                    theOutputGeotiffTagsFlag;
   bool                    theColorLutFlag;
   ossimRefPtr<ossimMapProjectionInfo> theProjectionInfo;
   ossimIpt                theOutputTileSize;
   ossimRefPtr<ossimNBandLutDataObject> theColorLut;
   ossimFilename           theLutFilename;
   bool                    theForceBigTiffFlag;
   bool                    theBigTiffFlag;
   mutable ossimRefPtr<ossimNBandToIndexFilter> theNBandToIndexFilter;
TYPE_DATA
};

#endif /* End of "#ifndef ossimTiffWriter_HEADER" */
