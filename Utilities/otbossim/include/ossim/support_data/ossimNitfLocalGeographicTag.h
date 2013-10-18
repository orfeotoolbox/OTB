//*******************************************************************
// Copyright (C) 2004 Intelligence Data Systems. 
//
// LICENSE: LGPL
//
// see top level LICENSE.txt
// 
// Author: Garrett Potts
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfLocalGeographicTag.h 19682 2011-05-31 14:21:20Z dburken $
#ifndef ossimNitfLocalGeographicTag_HEADER
#define ossimNitfLocalGeographicTag_HEADER
#include <ossim/support_data/ossimNitfRegisteredTag.h>

class OSSIM_DLL ossimNitfLocalGeographicTag : public ossimNitfRegisteredTag
{
public:
   ossimNitfLocalGeographicTag();
   virtual ~ossimNitfLocalGeographicTag();
   virtual std::string getRegisterTagName()const;
   virtual void parseStream(std::istream& in);
   virtual void writeStream(std::ostream& out);
   virtual ossim_uint32 getSizeInBytes()const;

   virtual void clearFields();

   virtual void setDegreesPerPixelLat(double deltaLat);
   virtual void setDegreesPerPixelLon(double deltaLon);
   virtual void setLatOrigin(double origin);
   virtual void setLonOrigin(double origin);
   
protected:
   /**
    * FIELD: ARV
    *
    * required 9 byte field
    *
    * Longitude density
    *
    * This field shall contain the pixel ground spacing in E/W direction that is
    * the number of pixels or elements intervals in 360 degrees.
    * 9 BCS-N positive integer 000000002 to 999999999. 
    */
   char theLonDensity[10];

   /**
    * FIELD: BRV
    *
    * required 9 byte field
    *
    * Latitude density
    *
    * This field shall contain the pixel ground spacing in N/S direction that is the number of
    * pixels or elements intervals in 360 degrees. 9 BCS-N positive integer 000000002 to 999999999 R 
    */ 
   char theLatDensity[10];

   /**
    * FIELD: LSO
    * 
    * required 15 byte field
    *
    * Longitude of Reference Origin This field shall contain the longitude of the origin pixel
    * (row number 0, column number 0) in the absolute coordinate system. 15 BCS-N R 
    */
   char theLonOrigin[16];

   /**
    * FIELD: PSO
    *
    * required 15 byte field
    *
    * Latitude of Reference Origin This field shall contain the latitude of the origin
    * pixel (row number 0, column number 0) in the absolute coordinate system. 15 BCS-N R 
    */
   char theLatOrigin[15];

TYPE_DATA   
};
#endif
