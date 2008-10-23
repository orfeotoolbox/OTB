#ifndef ossimRpfAttributeOffsetRecord_HEADER
#define ossimRpfAttributeOffsetRecord_HEADER
#include <iostream>
using namespace std;

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimErrorContext.h>

class ossimRpfAttributeOffsetRecord
{
public:
   friend ostream& operator <<(ostream& out,
                               const ossimRpfAttributeOffsetRecord& data);

   ossimRpfAttributeOffsetRecord();
   virtual ~ossimRpfAttributeOffsetRecord(){}

   ossimErrorCode parseStream(istream& in, ossimByteOrder byteOrder);
   void print(ostream& out)const;
   ossim_uint16  getAttributeId()const{return theAttributeId;}
   ossim_uint8   getParamterId()const{return theParameterId;}
   ossim_uint32  getAttributeRecordOffset()const{return theAttributeRecordOffset;}
   
private:
   void clearFields();
   /*!
    * The attribute Id.  The Id and descriptions are taken from
    * MIL-STD-2411-1 and can have one of the following values:
    *
    * ID     Description
    * _________________________________________________________________
    * 1      Currency Date, 1 parameter:
    *          1. Date of most recent revision to the RPF
    *             YYYYMMDD
    *             8 byte ascii value
    *
    * 2      Production Date, 1 parameter:
    *          1. Date the source data was transformed to RPF format
    *             YYYYMMDD
    *             8 byte ascii value
    *  
    * 3      Significant Date, 1 parameter:
    *          1. Most accurately describes the date of
    *             the source data.
    *             YYYYMMDD
    *             8 byte ascii value
    *
    * 4      Map/Chart source, 4 parameters:
    *          1. Short title for the id of a group of
    *             products.  ex: JOG 1501A ...
    *             10 byte ascii value
    *          2. The designation of the hardcopy source
    *             8 byte ascii value
    *          3. Old horizontal datum code.  Original
    *             horizontal datum of the hardcopy product
    *             4 byte ascii value
    *          4. Edition number of the source graphic
    *             7 byte ascii value.
    *
    * 5      Projection Systemd, 5 parameters: p. 38 of MIL-STD-2411-1
    *          1. Projection Code. 2 byte ascii value
    *             AC    Albers Equal area
    *                   Requires Parameters A B C D
    *             AL    Azimuthal Equal Dist.
    *                   Requires Parameters A B
    *             RB    Hotline Oblique Mercator
    *                   Requires Parameters A B C 
    *             LE    Lambert Conformal Conic
    *                   Requires Parameters A B C D
    *             MC    Mercator
    *                   Requires Parameters A B
    *             OC    Oblique Mercator
    *                   Requires Parameters A B C
    *             OD    Orthographic
    *                   Requires Parameters A B
    *             PG    Polar Stereo-graphic
    *                   Requires Parameters A B C D
    *             PH    Poly conic
    *                   Requires Parameters A B
    *             TC    Transverse Mercator
    *                   Requires Parameters A B C
    *             UT    Universal Transverse Mercator
    *                   Requires Parameters A
    *          2. Projection Param A: real 4 byte value
    *          3. Projeciton Param B: real 4 byte value
    *          4. Projection Param C: real 4 byte value
    *          5. Projection Param D: real 4 byte value
    * 6      Vertical Datum (1 parameter) p. 37 MIL-STD-2411-1
    *         4 byte ascii
    *
    *          MSL     Mean Sea Level.  All elevations in th data set
    *                  are referenced to the geoid of the specified datum
    *          GEOD    Geodetic (All elevations in the data set
    *                  are referenced to the ellipsoid of the specified
    *                  datum
    * 7      Horizontal Datum (1 parameter)
    *        4 byte ascii
    *
    *            Look on p.33 of MIL-STD-2411-1.  Too big to list
    *            we will need a translation table for these
    *            codes
    * 8      Vertical Absolute Accurracy (2 parameters)
    *        1. vertical absolute accuracy: 4 byte unsigned int
    *        2. Accuracy units of measure:  2 byte value 
    *             0   unknown
    *             1   meters (Length area Volume)
    *             2   Kilopounds
    *             3   Seconds (of time)
    *             4   Meters/Second (speed)
    *             5   Cubic Meters per second (Flow)
    *             6   Volt (Electrical potential)
    *             7   Watt (Ten to 6 electirical power)
    *             8   Hertz
    *             9   +- DD MM SS.S (Angular)
    *             10  +- HH MM SS.S (Time)
    *             11  International Nautical Mile (1852 meters)
    *             12  knot
    *             13  Nautical mile per day
    *             14  Feet
    *             15  Fathom
    *             16  Micrometers (microns)
    *             17  Mils
    *             18  Seconds of Arc
    *             19  Minutes of Arc
    *             20  Degrees of arc
    *             999 Other
    *
    * 9      Horizontal Absolute Accuracy (2 parameters)
    *        1. Horizontal absolute accuracy: 4 byte unsigned int
    *        2. Accuracy units of measure:  2 byte value 
    *             0   unknown
    *             1   meters (Length area Volume)
    *             2   Kilopounds
    *             3   Seconds (of time)
    *             4   Meters/Second (speed)
    *             5   Cubic Meters per second (Flow)
    *             6   Volt (Electrical potential)
    *             7   Watt (Ten to 6 electirical power)
    *             8   Hertz
    *             9   +- DD MM SS.S (Angular)
    *             10  +- HH MM SS.S (Time)
    *             11  International Nautical Mile (1852 meters)
    *             12  knot
    *             13  Nautical mile per day
    *             14  Feet
    *             15  Fathom
    *             16  Micrometers (microns)
    *             17  Mils
    *             18  Seconds of Arc
    *             19  Minutes of Arc
    *             20  Degrees of arc
    *             999 Other
    *
    * 10     Vertical Relative Accuracy (2 parameters)
    *        1. Vertical relative accuracy: 4 byte unsigned int
    *        2. Accuracy units of measure:  2 byte value 
    *             0   unknown
    *             1   meters (Length area Volume)
    *             2   Kilopounds
    *             3   Seconds (of time)
    *             4   Meters/Second (speed)
    *             5   Cubic Meters per second (Flow)
    *             6   Volt (Electrical potential)
    *             7   Watt (Ten to 6 electirical power)
    *             8   Hertz
    *             9   +- DD MM SS.S (Angular)
    *             10  +- HH MM SS.S (Time)
    *             11  International Nautical Mile (1852 meters)
    *             12  knot
    *             13  Nautical mile per day
    *             14  Feet
    *             15  Fathom
    *             16  Micrometers (microns)
    *             17  Mils
    *             18  Seconds of Arc
    *             19  Minutes of Arc
    *             20  Degrees of arc
    *             999 Other
    *
    * 11     Horizontal Relative Accuracy (2 parameters)
    *        1. Horizontal relative accuracy: 4 byte unsigned int
    *        2. Accuracy units of measure:  2 byte value 
    *             0   unknown
    *             1   meters (Length area Volume)
    *             2   Kilopounds
    *             3   Seconds (of time)
    *             4   Meters/Second (speed)
    *             5   Cubic Meters per second (Flow)
    *             6   Volt (Electrical potential)
    *             7   Watt (Ten to 6 electirical power)
    *             8   Hertz
    *             9   +- DD MM SS.S (Angular)
    *             10  +- HH MM SS.S (Time)
    *             11  International Nautical Mile (1852 meters)
    *             12  knot
    *             13  Nautical mile per day
    *             14  Feet
    *             15  Fathom
    *             16  Micrometers (microns)
    *             17  Mils
    *             18  Seconds of Arc
    *             19  Minutes of Arc
    *             20  Degrees of arc
    *             999 Other
    *
    * 12     Ellipsoid, 1 parameter:
    *        1. Ellipsoid code.  3 byte ascii
    *           See p. 40 of MIL-STD-2411-1
    * 13     Sounding Datumm, 1 parameter: 4 byte ascii
    *        See p. 33 MIL-STD-2411-1 5.3.2.2
    * 14     Navigation System, 1 parameter: 
    *        1. Navigation code. 2 byte unsigned int
    *           See MIL-STD-2411-1 5.3.2.4
    * 15     Grid, 1 parameter:
    *        1. Grid code. 2 byte ascii
    *           See MIL-STD-2411-1 5.3.2.7
    * 16     Easterly Annual Magnetic Change, 2 parameters:
    *        1. Easterly annual magnetic change. 4 byte real
    *        2. Units of the change.  See 5.3.2.6 of MIL-STD-2411-1
    * 17     Westerly Annual Magnetic Change, 2 parameters:
    *        1. Westerly annual magnetic change. 4 byte real
    *        2. Units of the change.  See 5.3.2.6 of MIL-STD-2411-1
    * 18     Grid North Magnetic North ANgle, 2 paramters:
    *        1. Grid North - Magnetic North.  4 byte real
    *        2. Units of angle. 2 byte unsigned int
    *           see 5.3.2.6
    * 19     Grid convergence angle, 2 parameters:
    *        1. Grid convergence angle.  4 byte Real
    *        2. Units of angle. 2 byte unsigned int 5.3.2.6
    * 20     Highest known elevation (4 parameters)
    *        1. Highest known elevation: 8 byte real
    *        2. Units of elevation: See 5.3.2.6 of MIL-STD-2411-1
    *        3. Lat of elevation: 8 byte real in decimal degrees
    *        4. Lon of elevation: 8 byte real in decimal degrees
    * 21     Multiple legend, 1 paraemter:
    *        1. Name of legend file that applies to this
    *           12 byte ascii
    * 22     Image source, 2 parameters:
    *        1. Source of data from which this
    *           RPF was derived. ex: SPOT. Landsat7, Landsat4.
    *           for CDTED: "imagery, "carto"
    *           12 byte ascii
    *        2. GSD.  Ground sample or post distance.
    *           4 byte unsigned int
    * 23     Data Level, 1 parameter:
    *        1. The level of the source.  For example:
    *           if this is a DTED or CDTED it would
    *           distinguish between level 1 and 2
    */
   ossim_uint16 theAttributeId;

   /*!
    * This defines which parameter number for theAttrributeId
    */
   ossim_uint8  theParameterId;

   /*!
    * This is a 1 byte field.  If the value is 0 then
    * the attribute applies to the entire geographic coverage
    * of this frame.  If it's > 0 then the areal coverage
    * where the given attribute applies is defined in the corresponding
    * explicit areal coverage record in the explicit areal
    * coverage table.
    */
    ossim_uint8 theArealCoverageSequenceNumber;

   /*!
    * a 4-byte unsigned integer.  displacement in bytes measured
    * from the beginning of the [attribute subsection] and the first
    * byte of the [attribute record].
    */
   ossim_uint32 theAttributeRecordOffset;
};

#endif
