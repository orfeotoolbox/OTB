//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// LICENSE: See top level LICENSE.txt file.
// 
// Author: Garrett Potts
// 
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfImageHeaderV2_1.h 20123 2011-10-11 17:55:44Z dburken $

#ifndef ossimNitfImageHeaderV2_1_HEADER
#define ossimNitfImageHeaderV2_1_HEADER

#include <stdexcept>

#include <ossim/support_data/ossimNitfImageHeaderV2_X.h>
#include <ossim/support_data/ossimNitfImageBandV2_1.h>

class OSSIMDLLEXPORT ossimNitfImageHeaderV2_1 : public ossimNitfImageHeaderV2_X
{
public:
   ossimNitfImageHeaderV2_1();
   virtual ~ossimNitfImageHeaderV2_1();

   virtual void parseStream(std::istream &in);
   virtual void writeStream(std::ostream &out);

   /**
    * @brief print method that outputs a key/value type format adding prefix
    * to keys.
    */
   virtual std::ostream& print(std::ostream& out,
                               const std::string& prefix) const;
   
   virtual bool        isCompressed()const;
   virtual bool        isEncrypted()const;

   virtual ossim_int32 getNumberOfBands()const;
   virtual ossim_int32 getNumberOfRows()const;
   virtual ossim_int32 getNumberOfCols()const;
   virtual ossim_int32 getNumberOfBlocksPerRow()const;
   virtual ossim_int32 getNumberOfBlocksPerCol()const;
   virtual ossim_int32 getNumberOfPixelsPerBlockHoriz()const;
   virtual ossim_int32 getNumberOfPixelsPerBlockVert()const;
   virtual ossimString getImageId()const;
   virtual ossim_int32 getBitsPerPixelPerBand()const;
   virtual ossim_int32 getActualBitsPerPixelPerBand()const;
   virtual ossimString getIMode()const;

   /** @return The IDATIM field unparsed. */
   virtual ossimString  getImageDateAndTime() const;
   
   virtual ossimString getAcquisitionDateMonthDayYear(ossim_uint8 separationChar='-')const;
   virtual ossimString getSecurityClassification()const;
   virtual ossimString getCategory()const;
   virtual ossimString getRepresentation()const;
   virtual ossimString getCoordinateSystem()const;
   virtual ossimString getPixelValueType()const;


   virtual bool hasBlockMaskRecords()const;
   virtual bool hasPadPixelMaskRecords()const;
   virtual bool hasTransparentCode()const;
   virtual ossim_uint32 getTransparentCode()const;
   virtual ossim_uint32 getBlockMaskRecordOffset(ossim_uint32 blockNumber,
                                                 ossim_uint32 bandNumber)const;
   virtual ossim_uint32 getPadPixelMaskRecordOffset(ossim_uint32 blockNumber,
                                                    ossim_uint32 bandNumber)const;
   
   virtual void setNumberOfBands(ossim_uint32 nbands);
   virtual void setBandInfo(ossim_uint32 idx,
                            const ossimNitfImageBandV2_1& info);
   
   virtual void setNumberOfRows(ossim_uint32 rows);
   virtual void setNumberOfCols(ossim_uint32 cols);

   virtual void setSecurityClassificationSystem(const ossimString& value);
   virtual void setCodewords(const ossimString& value);
   virtual void setControlAndHandling(const ossimString& value);
   virtual void setReleasingInstructions(const ossimString& value);
   virtual void setDeclassificationType(const ossimString& value);
   virtual void setDeclassificationDate(const ossimString& value);
   virtual void setDeclassificationExempt(const ossimString& value);
   virtual void setDowngrade(const ossimString& value);
   virtual void setDowngradeDate(const ossimString& value);
   virtual void setClassificationText(const ossimString& value);
   virtual void setClassificationAuthorityType(const ossimString& value);
   virtual void setClassificationAuthority(const ossimString& value);
   virtual void setClassificationReason(const ossimString& value);
   virtual void setSecuritySourceDate(const ossimString& value);
   virtual void setSecurityControlNumber(const ossimString& value);
   virtual void setImageMagnification(const ossimString& value);
   
   virtual ossimString getImageSource()const; 
   
   virtual const ossimRefPtr<ossimNitfCompressionHeader> getCompressionHeader()const;
   virtual const ossimRefPtr<ossimNitfImageBand> getBandInformation(ossim_uint32 idx)const;
   
   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;

   virtual bool saveState(ossimKeywordlist& kwl, const ossimString& prefix="")const;

   /*!
    * @brief Method to set fields from a keyword list.
    *
    * This is not a true loadState as it does not lookup/initialize all class
    * members.  This was added to allow defaults, e.g ISCLAS, to be set via a
    * site configuration file.
    * Code does not return false if a field(key) is not found..
    *
    * @return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   
   static const ossimString ISCLSY_KW;
   static const ossimString ISCODE_KW;
   static const ossimString ISCTLH_KW;
   static const ossimString ISREL_KW;
   static const ossimString ISDCTP_KW;
   static const ossimString ISDCDT_KW;
   static const ossimString ISDCXM_KW;
   static const ossimString ISDG_KW;
   static const ossimString ISDGDT_KW;
   static const ossimString ISCLTX_KW;
   static const ossimString ISCATP_KW;
   static const ossimString ISCAUT_KW;
   static const ossimString ISCRSN_KW;
   static const ossimString ISSRDT_KW;
   static const ossimString ISCTLN_KW;
   static const ossimString XBANDS_KW;

private:
   void clearFields();

   /**
    * FIELD: ISCLSY
    * 
    * Is a required 2 byte field.
    *
    * Image Security Classification System.
    *
    * This field shall contain valid values indicating the
    * national or multinational security system used to classify the image.
    * Country Codes per FIPS 10-4 shall be used to indicate national security
    * systems; codes found in DIAM 65-19
    * shall be used to indicate multinational security systems.
    * If this field is all BCS spaces (0x20), it shall imply that no security
    * classification system applies to the image. 
    */
   char theSecurityClassificationSys[3];

   /**
    * FIELD: ISCODE
    * 
    * is a 11 byte field
    *
    * Image Codewords.
    *
    * This field shall contain a valid indicator of the security compartments
    * associated with the image. Values include one or more of the
    * tri/digraphs found
    * in DIAM 65-19 and/or Table A-4. Multiple entries shall be separated by
    * single
    * BCS spaces (0x20): The selection of a relevant set of codewords is
    * application
    * specific.  If this field is all BCS spaces (0x20), it shall imply that no
    * codewords apply to the image.
    */
   char theCodewords[12];

   /**
    * FIELD: ISCTLH
    * 
    * is a 2 byte field.
    *
    * Image Control and Handling.  This field shall contain valid additional
    * security control and/or handling instructions (caveats) associated with
    * the image.
    * Values include digraphs found in DIAM 65-19 and/or Table A4.
    * The digraph may indicate single or multiple caveats. The selection of a
    * relevant caveat(s) is application specific.  If this field is all BCS
    * spaces (0x20),
    * it shall imply that no additional control and handling instructions
    * apply to the image. 
    */
   char theControlAndHandling[3];

   /**
    * FIELD: ISREL
    * 
    * Is a 20 byte field.
    *
    * Image Releasing Instructions.  This field shall contain a valid list of
    * country and/or multilateral entity codes to which countries and/or
    * multilateral entities the image is authorized for release.  Valid items
    * in the list are one or
    * more country codes as found in FIPS 10-4 and/or codes identifying
    * multilateral
    * entities as found in DIAM 65-19. If this field is all BCS spaces (0x20),
    * it shall imply that no image release instructions apply.
    */
   char theReleasingInstructions[21];


   /**
    * FIELD: ISDCTP
    * 
    * Is a required 2 byte field.  The valid
    * values are
    * DD = declassify on a specific date
    * DE = Declassify upon occurence of an event
    * GD = downgrade to a specified level upon
    *      occurence of an event
    * O  = OADR
    * X  = exempt from automatic declassification
    * spaces = no image security declassification or
    *          downgrade instruction apply.
    */
   char theDeclassificationType[3];
   

   /**
    * FIELD: ISDCDT
    * 
    * Is a required 8 byte field.
    *  CCYYMMDD
    *
    * CC   is the first 2 chars of the year 00-99
    * YY   is the second 2 chars of the year 00-99
    * MM   is the month 01-12
    * DD   is the day 00-31
    */
   char theDeclassificationDate[9];

   /**
    * FIELD: ISDCXM
    * 
    * is a required 4 byte field.
    * 
    * Image Declassification Exemption.
    *
    * This field shall indicate the reason the image is exempt from automatic
    * declassification if the value in Image Declassification Type is X. Valid
    * values are X1 through X8 and X251 through X259. X1 through X8 correspond
    * to the declassification exemptions found in DOD 5200.1-R, paragraphs
    * 4-202b(1) through (8) for material exempt from the 10-year rule.
    * X251 through X259 correspond to the declassification exemptions found in
    * DOD 5200.1-R, paragraphs 4-301a(1) through (9) for permanently valuable
    * material exempt from the 25-year declassification system. If this field is
    * all BCS spaces (0x20), it shall imply that a image declassification exemption
    * does not apply. 
    */
   char theDeclassificationExempt[5];
   
   /**
    * FIELD: ISDG
    * 
    * Is a required 1 byte field with form:
    *
    *
    * Image Downgrade.  This field shall indicate the classification level
    * to which a image is to be downgraded if the values in Image Declassification Type
    * are GD or GE.
    *
    * Valid values are
    *    S (=Secret),
    *    C (=Confidential),
    *    R (= Restricted).
    *
    * If this field is all BCS spaces (0x20), it shall imply that image security
    * downgrading does not apply.  
    */
   char theDowngrade[2];

   /**
    * FIELD: ISDGDT
    * 
    * is a required 8 byte field
    *
    * CCYYMMDD
    *
    * CC   is the first 2 chars of the year 00-99
    * YY   is the second 2 chars of the year 00-99
    * MM   is the month 01-12
    * DD   is the day 00-31
    */
   char theDowngradeDate[9];

   /**
    * FIELD: ISCLTX
    * 
    * is a required 43 byte field.
    *
    * Image Classification Text.
    *
    * This field shall be used to provide additional information
    * about image classification to include identification of a declassification or downgrading
    * event if the values in Image Declassification Type are DE or GE.. It may also be used
    * to identify multiple classification sources and/or any other special handling rules.
    * Values are user defined free text. If this field is all BCS spaces (0x20), it shall
    * imply that additional information about image classification does not apply. 
    */
   char theClassificationText[44];

   /**
    * FIELD: ISCATP
    * 
    * Is a required 1 byte field.
    *
    * O  = original classification Authority
    * D  = derivative from a single source
    * M  = derivative from multiple sources
    * 
    */
   char theClassificationAuthType[2];

   /**
    * FIELD: ISCAUT
    * 
    * Is a required 40 byte field.
    *
    * Image Classification Authority.
    *
    * This field shall identify the classification authority for the image dependent upon the
    * value in Image Classification Authority Type. Values are user defined free text which
    * should contain the following information: original classification authority name and
    * position or personal identifier if the value in Image Classification Authority Type is O;
    * title of the document or security classification guide used to classify the image
    * if the value in Image Classification Authority Type is D;
    * and Derive-Multiple if the image classification was derived from multiple sources.
    * In the latter case, the image originator will maintain a record of the sources used
    * in accordance with existing security directives. One of the multiple sources may also
    * be identified in Image Classification Text if desired. If this field is all BCS spaces
    * (0x20), it shall imply that no image classification authority applies.
    */
   char theClassificationAuthority[41];

   /**
    * FIELD: ISCRSN
    * 
    * This is a required 1 byte field and indicates
    * the classification reason.
    *
    * Image Classification Reason.
    *
    * This field shall contain values indicating the reason for classifying the image.
    * Valid values are A through G. These correspond to the reasons for
    * original classification per E.O. 12958, Section 1.5.(a) through (g).
    * If this field is all BCS spaces (0x20), it shall imply that no image classification
    * reason applies. 
    */
   char theClassificationReason[2];

   /**
    * FIELD: ISSRDT
    * 
    * This is a required 8 byte field.
    *
    * CCYYMMDD
    *
    * CC   is the first 2 chars of the year 00-99
    * YY   is the second 2 chars of the year 00-99
    * MM   is the month 01-12
    * DD   is the day 00-31
    *
    */
   char theSecuritySourceDate[9];

   /**
    * FIELD: ISCTLN
    * 
    * is a required 15 byte field.
    */
   char theSecurityControlNumber[16];

  
   /**
    * FIELD: XBANDS
    * 
    * This is a conditional 5 byte field and ranges from
    * 00010-99999.  This exists if theNumberOfBands is 0.
    */
   char theNumberOfMultispectralBands[6];

   std::vector<ossimRefPtr<ossimNitfImageBandV2_1> > theImageBands;

TYPE_DATA   
};

#endif
