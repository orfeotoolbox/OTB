//*******************************************************************
// Copyright (C) 2003 Storage Area Networks, Inc.
//
// License:  See top level LICENSE.txt file.
//
// Author:  Kenneth Melero (kmelero@sanz.com)
//
//*******************************************************************
//  $Id: ossimWorldFileWriter.h 15766 2009-10-20 12:37:09Z gpotts $

#ifndef ossimWorldFileWriter_H
#define ossimWorldFileWriter_H

#include <ossim/imaging/ossimMetadataFileWriter.h>

/** ossimWorldFileWriter */
class OSSIMDLLEXPORT ossimWorldFileWriter : public ossimMetadataFileWriter
{
public:
   
   /** default constructor */
   ossimWorldFileWriter();

   /**
    * Load state method:
    *
    * This method call base class ossimMetadataFileWriter::loadState after
    * looking for its keywords.
    *
    * @param kwl Keyword list to initialize from.
    *
    * @param prefix Usually something like: "object1."
    *
    * @return This method will alway return true as it is intended to be
    * used in conjuction with the set methods.
    *
    * Keywords picked up by loadState:
    *
    * (linear units must be "feet", "us_survey_feet", or "meters")
    * (If projection is geographic units will always be decimal degrees.)
    * 
    * linear_units: us_survey_feet
    */
   virtual bool loadState(const ossimKeywordlist& kwl, const char* prefix=0);

   /**
    * Satisfies pure virtual from ossimMetadataWriter base.
    *
    * Appends the writers image types to the "metadatatypeList".
    * 
    * @param metadatatypeList stl::vector<ossimString> list to append to.
    *
    * @note Appends to the list, does not clear it first.
    */
   virtual void getMetadatatypeList(
      std::vector<ossimString>& metadatatypeList) const;

   /**
    * Satisfies pure virtual from ossimMetadataWriter base.
    *
    * @param imageType string representing image type.
    *
    * @return true if "metadataType" is supported by writer.
    */
   virtual bool hasMetadataType(const ossimString& metadataType)const;

   /**
    * Sets the linear units of the output file to either feet,
    * us_survey_feet, or meters.
    *
    * If the projection is geographic, the call has no affect.
    *
    * If units is not feet, us_survey_feet or meters this call has no effect.
    *
    * @param units Desired output units of the world file.
    */
   void setLinearUnits(ossimUnitType units);
   
protected:
   /** default constructor */
   virtual ~ossimWorldFileWriter();
   virtual bool writeFile();

   ossimUnitType theUnits;

TYPE_DATA
};

#endif /* End of #ifndef ossimWorldFileWriter_H */
