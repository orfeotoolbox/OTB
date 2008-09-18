//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// LICENSE:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfImageHeaderV2_0.h 13101 2008-07-01 18:44:31Z dburken $
#ifndef ossimNitfImageHeaderV2_0_HEADER
#define ossimNitfImageHeaderV2_0_HEADER
#include <ossim/support_data/ossimNitfImageHeaderV2_X.h>
#include <ossim/support_data/ossimNitfImageBandV2_0.h>
#include <iterator>

class OSSIMDLLEXPORT ossimNitfImageHeaderV2_0 : public ossimNitfImageHeaderV2_X
{
public:
   ossimNitfImageHeaderV2_0();
   virtual ~ossimNitfImageHeaderV2_0();

   virtual void parseStream(std::istream &in);
   virtual void writeStream(std::ostream &out);
   
   virtual std::ostream& print(std::ostream &out)const;
   
   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;
   
   virtual ossimDrect  getImageRect()const;
   virtual ossimDrect  getBlockImageRect()const;
   virtual bool        isCompressed()const;
   virtual bool        isEncrypted()const;

   virtual ossim_int32  getNumberOfBands()const;
   virtual ossim_int32  getNumberOfRows()const;
   virtual ossim_int32  getNumberOfCols()const;
   virtual ossim_int32  getNumberOfBlocksPerRow()const;
   virtual ossim_int32  getNumberOfBlocksPerCol()const;
   virtual ossim_int32  getNumberOfPixelsPerBlockHoriz()const;
   virtual ossim_int32  getNumberOfPixelsPerBlockVert()const;
   virtual ossimString  getImageId()const;
   virtual ossim_int32  getBitsPerPixelPerBand()const;
   virtual ossim_int32  getActualBitsPerPixelPerBand()const;
   virtual ossimString  getIMode()const;
   virtual ossimString  getSecurityClassification()const;

   /** @return The IDATIM field unparsed. */
   virtual ossimString  getImageDateAndTime() const;
   
   virtual ossimString  getAcquisitionDateMonthDayYear(
      ossim_uint8 separationChar='-')const;
   virtual ossimString  getCategory()const;
   virtual ossimString  getImageSource()const;
   virtual ossimString  getRepresentation()const;
   virtual ossimString  getCoordinateSystem()const;
   virtual ossimString  getGeographicLocation()const;
   virtual ossimString  getPixelValueType()const;
   virtual bool         hasBlockMaskRecords()const;
   virtual bool         hasPadPixelMaskRecords()const;
   virtual bool         hasTransparentCode()const;
   virtual ossim_uint32 getTransparentCode()const;
   virtual ossim_uint32 getBlockMaskRecordOffset(ossim_uint32 blockNumber,
                                                 ossim_uint32 bandNumber)const;
   virtual ossim_uint32 getPadPixelMaskRecordOffset(ossim_uint32 blockNumber,
                                                    ossim_uint32 bandNumber)const;
   
   virtual const ossimRefPtr<ossimNitfCompressionHeader> getCompressionHeader()const;
   virtual const ossimRefPtr<ossimNitfImageBand> getBandInformation(ossim_uint32 idx)const;

   static const ossimString ISCODE_KW;
   static const ossimString ISCTLH_KW;
   static const ossimString ISREL_KW;
   static const ossimString ISCAUT_KW;
   static const ossimString CTLN_KW;
   static const ossimString ISDWNG_KW;
   static const ossimString ISDEVT_KW;
   
TYPE_DATA
private:
   //
   
   void clearFields();

   /*!
    * FIELD ISCODE:
    * is a 40 byte field
    */
   char theCodewords[41];

   /*!
    * FIELD ISCTLH:
    * is a 40 byte field
    */
   char theControlAndHandling[41];

   /*!
    * FIELD ISREL:
    * Is a 40 byte field.
    */
   char theReleasingInstructions[41];

   /*!
    * FIELD ISCAUT:
    * Is a 20 byte field
    */
   char theClassificationAuthority[21];

   /*!
    * FIELD CTLN:
    */
   char theSecurityControlNumber[21];

   /*!
    * FIELD ISDWNG:
    * Is a 6 byte field with form:
    *
    *  YYMMDD.  Year month day
    *
    * 999999 indicates that he Originating
    *        agencies determination is required
    *        (OADR)
    * 999998 downgrade event will specify at what
    *        point and time the declassification
    *        or downgrading is to take place.
    */
   char theSecurityDowngrade[7];

   /*!
    * FIELD ISDEVT:
    * This is a conditional field and is present
    * if the Security downgrade is 999998.
    */
   char theDowngradingEvent[41];

   /**
    * Will contain also the field NBANDS and we will have to recreate the prefix for
    * 
    * Container for repetitive fields:
    * IREPBAND
    * ISUBCAT
    * IFC
    * IMFLT
    * NLUTS
    * NELUT1
    * LUTD
    */ 
   std::vector<ossimRefPtr<ossimNitfImageBandV2_0> > theImageBands;
};

#endif
