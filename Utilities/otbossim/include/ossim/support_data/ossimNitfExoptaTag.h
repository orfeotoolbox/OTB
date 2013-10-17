//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
// Description: EXOPTA tag header definition
//
//----------------------------------------------------------------------------
// $Id$
#include <ossim/support_data/ossimNitfRegisteredTag.h>

/**
*
* EXOPTA - Exploitation Usability Optical Information Extension Format
*
*/
class OSSIM_DLL ossimNitfExoptaTag : public ossimNitfRegisteredTag
{
public:
   enum{
      ANGLE_TO_NORTH_SIZE   = 3,
      MEAN_GSD_SIZE         = 5,
      EXOPTA_RESERVED1_SIZE = 1,
      DYNAMIC_RANGE_SIZE    = 5,
      EXOPTA_RESERVED2_SIZE = 7,
      OBL_ANG_SIZE          = 5,
      ROLL_ANG_SIZE         = 6,
      PRIME_ID_SIZE         = 12,
      PRIME_BE_SIZE         = 15,
      EXOPTA_RESERVED3_SIZE = 5,
      N_SEC_SIZE            = 3,
      EXOPTA_RESERVED4_SIZE = 2,
      EXOPTA_RESERVED5_SIZE = 7,
      N_SEG_SIZE            = 3,
      MAX_LP_SEG_SIZE       = 6,
      EXOPTA_RESERVED6_SIZE = 12,
      SUN_EL_SIZE           = 5,
      SUN_AZ_SIZE           = 5,
      CEL_SIZE              = 107 // total
   };

   ossimNitfExoptaTag();

   virtual void parseStream(std::istream& in);
   virtual void writeStream(std::ostream& out);

   virtual std::ostream& print(std::ostream& out,
                               const std::string& prefix=std::string()) const;
   virtual void clearFields();

protected:
   /**
   * Field: ANGLE_TO_NORTH
   *  
   *  Angle to True North. Angle,
   *  measured clockwise, from the
   *  first-row vector (pointing from
   *  the first column to the last) to
   *  a vector pointing to True
   *  North.
   */
   char m_angleToNorth[ANGLE_TO_NORTH_SIZE+1];

   /**
   * Field: MEAN_GSD
   *
   * Mean Ground Sample
   * Distance. The geometric
   * mean of the cross and along
   * scan center-to-center distance
   * between contiguous ground
   * samples. Accuracy = ±10%
   * Note: Systems requiring an
   * extended range shall insert a
   * default value of “000.0” for this
   * field and utilize the PIAMC
   * tag.
   */
   char m_meanGsd[MEAN_GSD_SIZE+1];

   char m_reserved1[EXOPTA_RESERVED1_SIZE+1];

   /**
   * Field: DYNAMIC_RANGE
   *
   * Dynamic Range of image pixels.
   */
   char m_dynamicRange[DYNAMIC_RANGE_SIZE+1];
   char m_reserved2[EXOPTA_RESERVED2_SIZE+1];

   /**
   * Field: OBL_ANG
   *
   * Obliquity Angle. Angle between the local NED
   * horizontal and the optical axis of the image.
   */
   char m_oblAng[OBL_ANG_SIZE+1];

   /**
   * Field: ROLL_ANG
   *
   * Roll Angle of the platform body
   */
   char m_rollAng[ROLL_ANG_SIZE+1];

   /**
   * Field: PRIME_ID
   *
   * Primary Target ID
   */
   char m_primeId[PRIME_ID_SIZE+1];

   /**
   * Field: PRIME_BE
   *
   * Primary Target BE / OSUFFIX (target designator)
   */
   char m_primeBe[PRIME_BE_SIZE+1];
   char m_reserved3[EXOPTA_RESERVED3_SIZE+1];
   
   /**
   * Field: N_SEC
   *
   * Number Of Secondary Targets in Image. Determines the number of SECTG
   * extension present in the image subheader.
   */
   char m_nSec[N_SEC_SIZE+1];
   char m_reserved4[EXOPTA_RESERVED4_SIZE+1];
   char m_reserved5[EXOPTA_RESERVED5_SIZE+1];

   /**
   * Field: N_SEG
   *
   *
   * Number of Segments. Segments are separate imagery pieces within an 
   * imaging operation. Range 001 to 999
   */
   char m_nSeg[N_SEG_SIZE+1];

   /**
   * Field: MAX_LP_SEG
   *
   * Maximum Number of Lines Per Segment. Includes overlap lines.
   *
   * Range: 000001 to 199999
   */
   char m_maxLpSeg[MAX_LP_SEG_SIZE+1];

   char m_reserved6[EXOPTA_RESERVED6_SIZE+1];

  /**
   * Field: SUN_EL
   *
   * Sun Elevation. Angle in degrees, measured from the target plane at intersection of
   * the optical line of sight with the earth’s surface at the time
   * of the first image line (NITF row 1). 999.9 indicates data is
   * not available.
   * 
   * range: +-90.0, 999.9
   */
   char m_sunEl[SUN_EL_SIZE+1];

  /**
   * Field: SUN_AZ
   *
   * Sun Azimuth. Angle in degrees, from True North
   * clockwise (as viewed from space) at the time of the first
   * image line. 999.9 indicates data is not available.
   *
   * Range: 000.0 to 359.9, 999.9
   */
   char m_sunAz[SUN_AZ_SIZE+1];
};
