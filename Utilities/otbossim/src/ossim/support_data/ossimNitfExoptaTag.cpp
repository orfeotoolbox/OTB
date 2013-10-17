//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
// Description: EXOPTA tag class definition
//
//----------------------------------------------------------------------------
// $Id$
#include <ossim/support_data/ossimNitfExoptaTag.h>
#include <iomanip>
#include <iostream>

ossimNitfExoptaTag::ossimNitfExoptaTag()
   : ossimNitfRegisteredTag(std::string("EXOPTA"), CEL_SIZE) 
{
   clearFields();   
}

void ossimNitfExoptaTag::parseStream(std::istream& in)
{
   clearFields();
   in.read(m_angleToNorth,ANGLE_TO_NORTH_SIZE);
   in.read(m_meanGsd, MEAN_GSD_SIZE); 
   in.read(m_reserved1, EXOPTA_RESERVED1_SIZE);
   in.read(m_dynamicRange, DYNAMIC_RANGE_SIZE);
   in.read(m_reserved2, EXOPTA_RESERVED2_SIZE);
   in.read(m_oblAng, OBL_ANG_SIZE);
   in.read(m_rollAng, ROLL_ANG_SIZE);
   in.read(m_primeId, PRIME_ID_SIZE);
   in.read(m_primeBe, PRIME_BE_SIZE);
   in.read(m_reserved3, EXOPTA_RESERVED3_SIZE);
   in.read(m_nSec, N_SEC_SIZE);
   in.read(m_reserved4, EXOPTA_RESERVED4_SIZE);
   in.read(m_reserved5, EXOPTA_RESERVED5_SIZE);
   in.read(m_nSeg, N_SEG_SIZE);
   in.read(m_maxLpSeg, MAX_LP_SEG_SIZE);
   in.read(m_reserved6, EXOPTA_RESERVED6_SIZE);
   in.read(m_sunEl, SUN_EL_SIZE);
   in.read(m_sunAz, SUN_AZ_SIZE);
}

void ossimNitfExoptaTag::writeStream(std::ostream& out)
{
   out.write(m_angleToNorth,ANGLE_TO_NORTH_SIZE);
   out.write(m_meanGsd, MEAN_GSD_SIZE); 
   out.write(m_reserved1, EXOPTA_RESERVED1_SIZE);
   out.write(m_dynamicRange, DYNAMIC_RANGE_SIZE);
   out.write(m_reserved2, EXOPTA_RESERVED2_SIZE);
   out.write(m_oblAng, OBL_ANG_SIZE);
   out.write(m_rollAng, ROLL_ANG_SIZE);
   out.write(m_primeId, PRIME_ID_SIZE);
   out.write(m_primeBe, PRIME_BE_SIZE);
   out.write(m_reserved3, EXOPTA_RESERVED3_SIZE);
   out.write(m_nSec, N_SEC_SIZE);
   out.write(m_reserved4, EXOPTA_RESERVED4_SIZE);
   out.write(m_reserved5, EXOPTA_RESERVED5_SIZE);
   out.write(m_nSeg, N_SEG_SIZE);
   out.write(m_maxLpSeg, MAX_LP_SEG_SIZE);
   out.write(m_reserved6, EXOPTA_RESERVED6_SIZE);
   out.write(m_sunEl, SUN_EL_SIZE);
   out.write(m_sunAz, SUN_AZ_SIZE);
}

std::ostream& ossimNitfExoptaTag::print(std::ostream& out,
  const std::string& prefix )const
{
   std::string pfx = prefix;
   pfx += getTagName();
   pfx += ".";
   out << setiosflags(ios::left)
       << pfx << std::setw(24) << "CETAG:"      << getTagName() << "\n"
       << pfx << std::setw(24) << "CEL:"        << getTagLength() << "\n"
       << pfx << std::setw(24) << "ANGLE_TO_NORTH:"        << m_angleToNorth<< "\n"
       << pfx << std::setw(24) << "DYNAMIC_RANGE:"        << m_dynamicRange<< "\n"
       << pfx << std::setw(24) << "OBL_ANG:"        << m_oblAng<< "\n"
       << pfx << std::setw(24) << "ROLL_ANG:"        << m_rollAng<< "\n"
       << pfx << std::setw(24) << "PRIME_ID:"        << m_primeId<< "\n"
       << pfx << std::setw(24) << "PRIME_BE:"        << m_primeBe<< "\n"
       << pfx << std::setw(24) << "N_SEC"        << m_nSec<< "\n"
       << pfx << std::setw(24) << "N_SEG:"        << m_nSeg<< "\n"
       << pfx << std::setw(24) << "MAX_LP_SEG:"        << m_maxLpSeg<< "\n"
       << pfx << std::setw(24) << "SUN_EL:"        << m_sunEl<< "\n"
       << pfx << std::setw(24) << "SUN_AZ:"        << m_sunAz<< "\n";
   
   return out;
}

void ossimNitfExoptaTag::clearFields()
{
   memset(m_angleToNorth, '0', ANGLE_TO_NORTH_SIZE);
   memset(m_meanGsd, '0', MEAN_GSD_SIZE);
   m_meanGsd[3] = '.';
   m_reserved1[0] = '1';
   memset(m_dynamicRange, '0', DYNAMIC_RANGE_SIZE);
   memset(m_reserved2, ' ', EXOPTA_RESERVED2_SIZE);
   memset(m_oblAng, '0', OBL_ANG_SIZE);
   m_oblAng[2] = '.';
   memset(m_rollAng, '0',ROLL_ANG_SIZE);
   m_rollAng[0]='+';
   m_rollAng[3]='.';
   memset(m_primeId, ' ', PRIME_ID_SIZE);
   memset(m_primeBe, ' ', PRIME_BE_SIZE);
   memset(m_reserved3, ' ', EXOPTA_RESERVED3_SIZE);
   memset(m_nSec, '0', N_SEC_SIZE);
   memset(m_reserved4, ' ', EXOPTA_RESERVED4_SIZE);
   memset(m_reserved5, '0', EXOPTA_RESERVED5_SIZE);
   m_reserved5[6] = '1';
   memset(m_nSeg, '0', N_SEG_SIZE);
   m_nSeg[2] = '1';
   memset(m_maxLpSeg, '0', MAX_LP_SEG_SIZE);
   m_maxLpSeg[5] = '1';
   memset(m_reserved6, ' ', EXOPTA_RESERVED6_SIZE);
   memset(m_sunEl, '0', SUN_EL_SIZE);
   m_sunEl[0] = '+';
   m_sunEl[3] = '.';
   memset(m_sunAz, '0', SUN_AZ_SIZE);
   m_sunAz[3] = '.';
   
   m_angleToNorth[ANGLE_TO_NORTH_SIZE] = '\0';
   m_meanGsd[MEAN_GSD_SIZE]='\0';
   m_reserved1[EXOPTA_RESERVED1_SIZE]='\0';
   m_dynamicRange[DYNAMIC_RANGE_SIZE]='\0';
   m_reserved2[EXOPTA_RESERVED2_SIZE]='\0';
   m_oblAng[OBL_ANG_SIZE]='\0';
   m_rollAng[ROLL_ANG_SIZE]='\0';
   m_primeId[PRIME_ID_SIZE]='\0';
   m_primeBe[PRIME_BE_SIZE]='\0';
   m_reserved3[EXOPTA_RESERVED3_SIZE]='\0';
   m_nSec[N_SEC_SIZE]='\0';
   m_reserved4[EXOPTA_RESERVED4_SIZE]='\0';
   m_reserved5[EXOPTA_RESERVED5_SIZE]='\0';
   m_nSeg[N_SEG_SIZE]='\0';
   m_maxLpSeg[MAX_LP_SEG_SIZE]='\0';
   m_reserved6[EXOPTA_RESERVED6_SIZE]='\0';
   m_sunEl[SUN_EL_SIZE]='\0';
   m_sunAz[SUN_AZ_SIZE]='\0';
}
