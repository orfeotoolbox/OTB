//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
// 
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfImageBandV2_0.h 9153 2006-06-20 20:56:46Z dburken $
#ifndef ossimNitfImageBandV2_0_HEADER
#define ossimNitfImageBandV2_0_HEADER

  // #include <iterator>

#include <ossim/support_data/ossimNitfImageBand.h>
#include <ossim/support_data/ossimNitfImageLutV2_0.h>

class ossimNitfImageBandV2_0 : public ossimNitfImageBand
{
public:
   
   /** default constructor */
   ossimNitfImageBandV2_0();

   /** virtual destructory */
   virtual ~ossimNitfImageBandV2_0();
   
   virtual void parseStream(std::istream& in);
   virtual void writeStream(std::ostream& out);

   /**
    * @param bandNumber Zero based band number.
    */
   virtual std::ostream& print(std::ostream& out,
                               ossim_uint32 band=0)const;
   
   virtual ossim_uint32 getNumberOfLuts()const;
   virtual const ossimRefPtr<ossimNitfImageLut> getLut(ossim_uint32 idx)const;
   virtual ossimRefPtr<ossimNitfImageLut> getLut(ossim_uint32 idx);

   /** @return The band representation as an ossimString. */
   virtual ossimString getBandRepresentation()const;

   /**
    * Sets the band representation.
    *
    * @param rep The band representation.
    */
   virtual void setBandRepresentation(const ossimString& rep);

   /** @return The band significance as an ossimString. */
   virtual ossimString getBandSignificance()const;

   /**
    * Sets the band significance.
    *
    * @param rep The band significance.
    */
   virtual void setBandSignificance(const ossimString& rep);

protected:
   void clearFields();
   void printLookupTables(std::ostream& out)const;
   /*!
    * FIELD: IREPBAND,,
    * Is a required 2 byte field.  When theNumberOfBands is
    * 1 this field will contain all spaces.
    */
   char theBandRepresentation[3];

   /*!
    * FIELD:  ISUBCATnn
    * Is a required 6 byte field.
    */
   char theBandSignificance[7];

   /*!
    * FIELD:  IFCnn
    * Is an required 1 byte field.  Will be N
    */
   char theBandImageFilterCondition[2];

   /*!
    * FIELD:  IMFLTnn
    * is a required 3 byte field.  This is a reserved field
    */
   char theBandStandardImageFilterCode[4];

   /*!
    * FIELD:  NLUTSnn
    * This is a required 1 byte field.  Has value 0-4
    */
   char theBandNumberOfLuts[2];

   /*!
    * NELUTnn
    * This is a conditional field.
    */
   char theBandNumberOfLutEntries[6];

   std::vector<ossimRefPtr<ossimNitfImageLut> > theLookupTables;
};

#endif
