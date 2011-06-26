//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
// 
// Description: Nitf support class
// 
//********************************************************************
// $Id$

#ifndef ossimNitfTextHeaderV2_1_HEADER
#define ossimNitfTextHeaderV2_1_HEADER
#include <ossim/support_data/ossimNitfTextHeader.h>

class OSSIMDLLEXPORT ossimNitfTextHeaderV2_1 : public ossimNitfTextHeader
{
public:
   ossimNitfTextHeaderV2_1();
   virtual ~ossimNitfTextHeaderV2_1();
   
   virtual void parseStream(std::istream &in);
   virtual std::ostream& print(std::ostream &out)const;
   
   virtual void writeStream(std::ostream &out);

   void setSecurityClassification(const ossimString& value);
   void setSecurityClassificationSystem(const ossimString& value);
   void setCodewords(const ossimString& value);
   void setControlAndHandling(const ossimString& value);
   void setReleasingInstructions(const ossimString& value);
   void setDeclassificationType(const ossimString& value);
   
   static const ossimString TE_KW;
   static const ossimString TEXTID_KW;
   static const ossimString TXTALVL_KW;
   static const ossimString TXTDT_KW;
   static const ossimString TXTITL_KW;
   static const ossimString TSCLAS_KW;
   static const ossimString TSCLSY_KW;
   static const ossimString TSCODE_KW;
   static const ossimString TSCTLH_KW;
   static const ossimString TSREL_KW;
   static const ossimString TSDCTP_KW;
   static const ossimString TSDCDT_KW;
   static const ossimString TSDCXM_KW;
   static const ossimString TSDG_KW;
   static const ossimString TSDGDT_KW;
   static const ossimString TSCLTX_KW;
   static const ossimString TSCATP_KW;
   static const ossimString TSCAUT_KW;
   static const ossimString TSCRSN_KW;
   static const ossimString TSSRDT_KW;
   static const ossimString TSCTLN_KW;
   static const ossimString ENCRYP_KW;
   static const ossimString TXTFMT_KW;
   static const ossimString TXSHDL_KW;
   static const ossimString TXSOFL_KW;
   static const ossimString TXSHD_KW;

private:
   void clearFields();
   
   /*!
    * This is a required 2 byte fields and has
    * a value of TE.
    */
   char theFilePartType[3];
   
   /*!
    * This is a required 7 byte field.  Text Identifier
    */
   char theTextId[8];
   
   /*!
    * This is a required 3 byte field.
    */

   char theTextAttLevel[4];
   /*!
    * This is a required 14 byte field.  It has the
    * format of:
    *
    *  DDHHMMSSZMONYY
    *
    * DD   2 character day
    * HH   2 characters for the Hour
    * MM   2 characters for the minute
    * SS   2 characters for the seconds
    * Z    required
    * MON  3 characters of the month
    * YY   2 characters for the year
    */
   char theDataAndTime[15];

   /*!
    * is an optional 80 byte field
    */
   char theTextTitle[81];
   
   /*!
    * This is a required 1 byte field. Can have
    * of either:
    *
    * T   Top secret
    * S   Secret
    * C   Confidential
    * R   Restricted
    * U   Unclassified
    */
   char theTextSecurityClassification[2];
   
   /*!
    *  2 byte field Security Classification System
    */
   char theTextSecuritySystem[3];
   
   /*!
    *  11 byte field Text Codewords
    */
   char theTextCodewords[12];
   
   /*!
    *  2 byte field Text Control and Handling
    */
   char theTextControlAndHandling[3];
   
   /*!
    * This is an optional 20 byte field.
    */
   char theTextReleasingInstructions[21];
   
   /*!
    * optional 2 byte field Text Declassification Type
    */
   char theTextDeclassificationType[3];
   
   /*!
    * optional 8 byte field Text Declassification Date
    */
   char theTextDeclassificationDate[9];
   
   /*!
    * optional 4 byte field Text Declassification Exemption
    */
   char theTextDeclassificationExemption[5];

   /*!
    * optional 1 byte field Text Declassification Exemption
    */
   char theTextSecurityDowngrade[2];
   
   /*!
    * optional 8 byte field
    */
   char theTextSecurityDowngradeDate[9];
   
   /*!
    * 43 byte field providing additional information on text classification
    */
   char theTextClassificationText[44];
   
   /*!
    *	Optional 1 byte flag for type of classification athority
    */
   char theTextClassificationAthorityType[2];
   
   /*!
    *	Optional 40 byte flag for classification athority
    */
   char theTextClassificationAthority[41];
   
   /*!
    *	Optional 1 byte flag for classification athority
    */
   char theTextClassificationReason[2];
   
   /*!
    *	Optional 8 byte entry
    */
   char theTextSecuritySourceDate[9];

   /*!
    *	Optional 15 byte entry for security control number
    */
   char theTextSecurityControlNumber[16];
   
   /*!
    * This is the encription type and is a
    * required 1 byte field.  It can have values
    *
    * 0  Not encrypted
    * 1  Encrypted
    */
   char theTextEncyption[2];
   
   /*!
    * This is a required 3 byte field and will hold
    * either:
    *
    * MTF  indicates USMTF ( refer to JCS PUB 6-04
    *      for examples
    * STA  indicates NITF ASCII
    * OTH  indicates other or user defined
    */
   char theTextFormat[4];
   
   /*!
    * is a required 5 byute field.
    */
   char theExtSubheaderDataLength[6];
   
   /*!
    * is a conditional 3 byte field.  It exists if
    * theExtSubheaderDataLength is not 0
    */
   char theExtSubheaderOverflow[4];
   
   TYPE_DATA
};

#endif
