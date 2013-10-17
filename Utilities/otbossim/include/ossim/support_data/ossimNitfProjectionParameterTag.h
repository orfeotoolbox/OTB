//*******************************************************************
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
// 
// Author: Garrett Potts
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfProjectionParameterTag.h 22013 2012-12-19 17:37:20Z dburken $

#ifndef ossimNitfProjectionParameterTag_HEADER
#define ossimNitfProjectionParameterTag_HEADER
#include <vector>
#include <ossim/support_data/ossimNitfRegisteredTag.h>

class OSSIM_DLL ossimNitfProjectionParameterTag : public ossimNitfRegisteredTag
{
public:
   ossimNitfProjectionParameterTag();
   virtual ~ossimNitfProjectionParameterTag();

   virtual void parseStream(std::istream& in);
   virtual void writeStream(std::ostream& out);

   /**
    * @brief Print method that outputs a key/value type format
    * adding prefix to keys.
    * @param out Stream to output to.
    * @param prefix Prefix added to key like "image0.";
    */
   virtual std::ostream& print(std::ostream& out,
                               const std::string& prefix=std::string()) const;

   virtual ossim_uint32 getSizeInBytes()const;
   virtual void clearFields();
   virtual void clearParameters();
   virtual void addParameter(const ossimString& param);
   virtual void addParameter(const ossim_float64& param);
   void setName(const ossimString& name);
   void setCode(const ossimString& code);
   ossimString getName()const;
   ossimString getCode()const;
   void setFalseX(double falseX);
   void setFalseY(double falseY);
   double getFalseX()const;
   double getFalseY()const;
   
protected:
   /**
    * FIELD: PRN
    *
    * required 80 byte field.
    * 
    * Projection Name
    *
    * This field shall contain the name of the projection to which the Image Segment refers.
    * The default value is Transverse Mercator.
    */
   char theProjectionName[81];

   /**
    *
    * FIELD: PCO
    *
    * required 2 byte field.
    * 
    * Projection Code
    *
    * This field shall contain the code of the projection to which the Image Segment refers.
    * The default value is TC.
    *
    * Code values:
    *
    * AC           Albers Equal-Area Conic
    *              Parameters:
    *                1. Longitude of origin
    *                2. std. parallel nearer to equator
    *                3. std parallel farther from equator
    *                4. latitude of origin
    *              
    * AK           (Lambert) Azimuthal Equal-Area
    *              Parameters:
    *                1. Longitude of origin
    *                2. Latitude of origin
    *              
    * AL           Azimuthal Equidistant
    *              Parameters:
    *                1. Longitude of origin
    *                2. Latitude of origin
    * 
    * BF           Bonne
    *              Parameters:
    *                1. Longitude of origin
    *                2. Latitude of origin
    *                3. Scale factor at projection origin
    *                
    * CC           Equidistant Conic with 1 Standard Parallel
    *              Parameters:
    *                1. Longitude of origin
    *                2. Latitude of origin
    *                3. Latitude of Std. Parallel
    *                
    * CP           Equirectangular
    *              Parameters:
    *                1. Longitude of central meridian 
    *                2. Latitude of true scale
    *                3. Radius of sphere
    *                
    * CS           Cassini-Soldner
    *              Parameters:
    *                1. Longitude of origin
    *                2. Latitude of origin
    *            
    * ED           Eckert VI
    *              Parameters:
    *                1. Longitude of origin
    *                2. Radius of sphere
    * 
    * EF           Eckert IV
    *              Parameters:
    *                1. Longitude of origin
    *                2. Radius of sphere
    * 
    * GN           Gnomonic
    *              Parameters:
    *                1. Longitude of origin
    *                2. Latitude of origin
    * 
    * HX           Hotine Oblique Mercator based on 2 Points 
    *              Parameters:
    *                1. Scale factor at projection origin
    *                2. latitude of projection origin
    *                3. longitude of first point defining center line
    *                4. latitude of fist point defining center line
    *                
    * KA           Equidistant Conic with 2 Standard Parallels 
    *              Parameters:
    *                1. Longitude of central meridian
    *                2. Latitude of origin
    *                3. Latitude of standard parallel Nearer to equator
    *                4. Latitude of standard parallel farther from equator
    *
    * LA           Laborde
    *              Parameters:
    *                1. Longitude of origin
    *                2. Latitude of origin
    *                3. Scale factor at proj origin
    *                4. Azimuth at origin of axis of constant scale
    * 
    * LE           Lambert Conformal Conic
    *              Parameters:
    *                1. Longitude of origin
    *                2. std. parallel nearer to equator
    *                3. std. parallel farther from equator
    *                4. Latitude of origin
    * 
    * LI           Cylindrical Equal Area
    *              Parameters:
    *                1. Longitude of central meridian
    *                2. Latitude of origin
    *                
    * LJ           Lambert Equal-Area Meridional
    *              Parameters:
    *                1. Longitude of central meridian
    *                2. Latitude of origin
    *                
    * MC           Mercator
    *              Parameters:
    *                1. Longitude of origin
    *                2. Latitude of true scale
    *                3. Latitude of reference origin
    *                
    * MH           Miller Cylindrical
    *              Parameters:
    *                1. Longitude of central meridian
    *                2. Radius of sphere
    *                
    * MJ           French Lambert
    *              Parameters:
    *                1. Longitude of origin
    *                2. Latitude of origin
    *                3. Scale factor at proj origin
    *                
    * MP           Mollweide
    *              Parameters:
    *                1. Longitude of origin
    *                2. Radius of sphere
    *                
    * NT           New Zealand Map Grid
    *              Parameters:
    *                1. Longitude of origin
    *                2. Latitude of origin
    *                
    * OC           Oblique Mercator
    *              Parameters:
    *                1. Longitude of origin
    *                2. Latitude of origin
    *                3. Azimuth
    *                4. radius of sphere
    *                
    * OD           Orthographic
    *              Parameters:
    *                1. Longitude of origin
    *                2. Latitude of origin
    *                3. Radius of sphere
    *                
    * PG           Polar Stereographic
    *              Parameters:
    *                1. Latitude of true scale
    *                
    * PH           Polyconic
    *              Parameters:
    *                1. Longitude of central meridian
    *                2. Latitude of origin
    *                
    * RS or RB     Hotine Oblique Mercator (Rectified Skew Orthomorphic)
    *              Parameters:
    *                1. Longitude of origin
    *                2. Latitude of origin
    *                3. Azimuth East of North for Central Line (Skew X-Axis at Proj. origin)
    *                4. Scale factor at proj origin
    *                
    * RX           Robinson
    *              Parameters:
    *                1. Longitude of origin
    *                2. Radius of sphere
    *                
    * SA           Sinusoidal
    *              Parameters:
    *                1. Longitude of origin
    *                2. Radius of sphere
    *               
    * SD           Oblique Stereographic
    *              Parameters:
    *                1. Longitude of origin
    *                2. Latitude of origin
    *                3. Scale factor at origin
    *                
    * SX           Space Oblique Mercator
    *              Parameters:
    *                1. Application code:
    *                     1 = Landsat, USGS equations
    *                     2 = Landsat, EOSAT equations
    *                2. Vehicle number
    *                3. Orbital path number
    *                
    * TC           Transverse Mercator
    *              Parameters:
    *                1. Longitude of central meridian
    *                2. Central Scale Factor
    *                3. Latitude of origin
    *                
    * TX           Transverse Cylindrical Equal Area
    *              Parameters:
    *                1. Longitude of central meridian
    *                2. Latitude of origin
    *                3. Scale factor along central meridian
    *                
    * VA           Van der Grinten
    *              Parameters:
    *                1. Longitude of central meridian
    *                2. Radius of sphere
    * 
    * VX           General Vertical NearSide Perspective
    *              Parameters:
    *                1. Longitude of origin
    *                2. Latitude of origin
    *                3. Heigh of perspective map above surface
    *
    * ZY           Other Known Projection
    */
   char theProjectionCode[3];

   /**
    * FIELD: NUM_PRJ
    *
    * required 1 byte field ranging from 0-9
    * 
    * Number of Projection Parameters
    *
    * This field shall contain the number of projection parameters.
    * The PRJ Field should be repeated as necessary depending on the
    * projection code (see Part 3-6).  If the number of projection parameters
    * provided is lower than specified in Part 3-6, the
    * missing parameters value is 0.
    *
    */
   char              theNumberOfParameters[2];
   
   /**
    * FIELD(S): PRJn
    *
    * Conditional 15 byte field each.
    *
    * Projection Parameter Each occurrence of this field provides an appropriate
    * parameter to accurately describe the projection.  See Part 3-6 to know the
    * kind of parameters needed for each projection code. 15 BCS-N C 
    */
   std::vector<ossimString> theProjectionParameters;
   
   /**
    * FIELD: XOR
    *
    * required  15 byte field.
    * 
    * Projection False X (Easting) Origin
    *
    * This field shall contain the projection false X (easting) origin.
    * The default value is 000000000000000, which implies that there is no projection false X origin.
    * 
    */
   char theFalseXOrigin[16];


   /**
    * FIELD: YOR
    *
    * required 15 byte field.
    *
    * Projection False Y (Northing) Origin
    *
    * This field shall contain the projection false Y (northing) origin.
    * The default value is 000000000000000, which implies that there is
    * no projection false Y origin.
    * 
    */
   char theFalseYOrigin[16];
};

#endif
