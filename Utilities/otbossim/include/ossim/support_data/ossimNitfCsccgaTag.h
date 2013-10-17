//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
// Description: CSCCGA tag class definition
//
//----------------------------------------------------------------------------
// $Id$
#include <ossim/support_data/ossimNitfRegisteredTag.h>

/**
* All comments in this document were taken from the online resource found at:
*
* http://jitc.fhu.disa.mil/nitf/tag_reg
*
* The Cloud Cover Grid Data TRE (CSCCGA) provides support data that identifies 
* which image segment and sensors were used to create the cloud grid. CSCCGA 
* also geometrically registers the cloud grid to the pixel grid of one of the image segments.
* 1. The format and content of the CSCCGA TRE shall be as detailed in Table 3.1-1.
* 2. When cloud cover information is included in the dataset (see Section 2.1.4.2 for an exception) 
*    both the CSCCGA TRE and the Cloud Cover Shapefile DES (Section 2.6.2) shall be 
*    included in each dataset even though they both express similar information.
*/
class OSSIM_DLL ossimNitfCsccgaTag : public ossimNitfRegisteredTag
{
public:
   enum
   {
      CCG_SOURCE_SIZE    = 18,
      REG_SENSOR_SIZE    = 6,
      ORIGIN_LINE_SIZE   = 7,
      ORIGIN_SAMPLE_SIZE = 5,
      AS_CELL_SIZE_SIZE  = 7,
      CS_CELL_SIZE_SIZE  = 5,
      CCG_MAX_LINE_SIZE  = 7,
      CCG_MAX_SAMPLE_SIZE= 5,
      CEL_SIZE           = 60 

   };
   ossimNitfCsccgaTag();

   virtual void parseStream(std::istream& in);
   virtual void writeStream(std::ostream& out);

   virtual std::ostream& print(std::ostream& out,
                               const std::string& prefix=std::string()) const;
   virtual void clearFields();

protected:
   /**
    * Field: CCG_SOURCE
    *
    * BCS-A 18 byte field.
    *
    * Values: PAN or MS or PAN, MS
    *
    * Source of Grid Concatenation of all sensors used to create 
    * cloud cover grid separated by commas
    */
   char m_ccgSource[CCG_SOURCE_SIZE+1];
	
   /**
    * Field: REG_SENSOR
    *
    * BCS-A 6 bytes.
    *
    Values: PAN or MS
    *
    * Image Segment Sensor to which Cloud Cover Grid is registered 
    * (CCG is always registered to the synthetic array)
    */
   char m_regSensor[REG_SENSOR_SIZE+1];

   /**
    * Field: ORIGIN_LINE
    *
    * BCS-N 7 bytes.
    *
    * Values: 0000001
    *
    * Cloud Cover Grid Origin – Line 
    * (Corresponding line in registered image segment)
    */
   char m_originLine[ORIGIN_LINE_SIZE+1];

   /**
    * Field: ORIGIN_SAMPLE
    *
    * BCS-N 5 bytes
    *
    * Values: 00001
    *
    * Cloud Cover Grid Origin – Sample 
    * (Corresponding sample in registered image segment)
    */
   char m_originSample[ORIGIN_SAMPLE_SIZE+1];

   /**
    * Field: AS_CELL_SIZE
    *
    * BCS-N 7 bytes
    *
    * Value Range: 0000001 - 9999999
    *
    * Along Scan Cell Size – Lines (Cloud Cover Grid spacing in 
    * registered image segment lines)
    */
   char m_asCellSize[AS_CELL_SIZE_SIZE+1];

   /**
    * Field: CS_CELL_SIZE
    *
    * BCS-N 5 bytes
    *
    * Value Range: 00001 - 99999
    *
    * Cross Scan Cell Size – Samples 
    * (Cloud Cover Grid spacing in registered image segment samples)
    */
   char m_csCellSize[CS_CELL_SIZE_SIZE+1];
	
   /**
    * Field: CCG_MAX_LINE
    *
    * BCS-N 7 bytes
    *
    * Value Range: 0000001 - 9999999
    *
    * Number of Rows in CC Grid (Number of Cells in "lines" direction)
    */
   char m_ccgMaxLine[CCG_MAX_LINE_SIZE+1];

   /**
    * Field: CCG_MAX_SAMPLE
    *
    * BCS-N 5 bytes
    *
    * Value Range: 00001 - 99999
    *
    * Number of Columns in CC Grid (Number of Cells in "sample" direction)
    */
   char m_ccgMaxSample[CCG_MAX_SAMPLE_SIZE+1];
};
