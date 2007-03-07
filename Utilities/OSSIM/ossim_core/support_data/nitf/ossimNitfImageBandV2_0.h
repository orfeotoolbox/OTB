//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
// 
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfImageBandV2_0.h,v 1.5 2005/09/27 19:05:09 dburken Exp $
#ifndef ossimNitfImageBandV2_0_HEADER
#define ossimNitfImageBandV2_0_HEADER
#include <iostream>
#include <iterator>

#include <support_data/nitf/ossimNitfImageBand.h>
#include <support_data/nitf/ossimNitfImageLutV2_0.h>

class ossimNitfImageBandV2_0 : public ossimNitfImageBand
{
public:
   ossimNitfImageBandV2_0();
   ~ossimNitfImageBandV2_0();
   virtual void parseStream(std::istream& in);
   virtual void writeStream(std::ostream& out);
   
   virtual std::ostream& print(std::ostream& out)const;
   virtual ossim_uint32 getNumberOfLuts()const;
   virtual const ossimRefPtr<ossimNitfImageLut> getLut(ossim_uint32 idx)const;
   virtual ossimRefPtr<ossimNitfImageLut> getLut(ossim_uint32 idx);
  
protected:
   void clearFields();
   void printLookupTables(std::ostream& out)const;
   /*!
    * Is a required 2 byte field.  When theNumberOfBands is
    * 1 this field will contain all spaces.
    */
   char theBandRepresentation[3];

   /*!
    * Is a required 6 byte field.
    */
   char theBandSignificance[7];

   /*!
    * Is an required 1 byte field.  Will be N
    */
   char theBandImageFilterCondition[2];

   /*!
    * is a required 3 byte field.  This is a reserved field
    */
   char theBandStandardImageFilterCode[4];

   /*!
    * This is a required 1 byte field.  Has value 0-4
    */
   char theBandNumberOfLuts[2];

   /*!
    * This is a conditional field.
    */
   char theBandNumberOfLutEntries[6];

   vector<ossimRefPtr<ossimNitfImageLut> > theLookupTables;
};

#endif
