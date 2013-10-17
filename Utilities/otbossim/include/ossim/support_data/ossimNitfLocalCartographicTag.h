#ifndef ossimNitfLocalCartographicTag_HEADER
#define ossimNitfLocalCartographicTag_HEADER
#include <ossim/support_data/ossimNitfRegisteredTag.h>

class ossimNitfLocalCartographicTag : public ossimNitfRegisteredTag
{
public:
   ossimNitfLocalCartographicTag();
   virtual ~ossimNitfLocalCartographicTag();

   virtual void parseStream(std::istream& in);
   virtual void writeStream(std::ostream& out);

   virtual void clearFields();


   void setLengthUnits(const ossimString& units);
   void setEastingInterval(ossim_uint32 eastingInterval);
   void setNorthingInterval(ossim_uint32 northingInterval);
   void setEastingReferenceOrigin(ossim_float64 east);
   void setNorthingReferenceOrigin(ossim_float64 north);
   
protected:
   /**
    * FIELD: UNILOA
    *
    * required 3 byte field
    *
    * Length units
    *
    * This field shall contain the unit of measure used for
    * easting (LOD) and northing (LAD) intervals. The default value is M.
    * 3BCS-A See Part 3- 7 R 
    */
   char theLengthUnits[4];

   /**
    * FIELD: LOD
    *
    * required 5 byte field
    * 
    * Easting interval
    *
    * This field shall contain the data density in E-W direction
    * that is the column width of an image pixel.
    * 5 BCS-N positive integer 00001 to 99999 R
    */
   char theEastingInterval[6];

   /**
    * FIELD: LAD
    *
    * required 5 byte field
    * 
    * Northing interval
    *
    * This field shall contain the data density in E-W direction
    * that is the column width of an image pixel.
    * 5 BCS-N positive integer 00001 to 99999 R
    */
   char theNorthingInterval[6];

   /**
    * FIELD: LSO
    *
    * required 15 byte field
    * 
    * Easting of Reference Origin
    *
    * This field shall contain the easting of the
    * origin pixel (row number 0, column number 0) in the absolute coordinate system.
    * 15 BCS-N ±mmmmmmmmmmmm.m
    */
   char theEastingReferenceOrigin[16];
   
   /**
    * FIELD: PSO
    *
    * required 15 byte field
    * 
    * Northing of Reference Origin
    *
    * This field shall contain the northing of the
    * origin pixel (row number 0, column number 0) in the absolute coordinate system.
    * 15 BCS-N ±mmmmmmmmmmmm.m
    */
   char theNorthingReferenceOrigin[16];

TYPE_DATA   
};

#endif
