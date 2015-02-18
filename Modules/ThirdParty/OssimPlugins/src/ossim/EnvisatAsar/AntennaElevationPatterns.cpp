//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$

#include <EnvisatAsar/AntennaElevationPatterns.h>

namespace ossimplugins
{
   AntennaElevationPatterns::AntennaElevationPatterns() : EnvisatAsarRecord("AntennaElevationPatterns_rec")
   {
   }

   AntennaElevationPatterns::~AntennaElevationPatterns()
   {
   }


   std::ostream& operator<<(std::ostream& os, const AntennaElevationPatterns& data)
   {
      os<<"zero_doppler_time:"<<data._zero_doppler_time.c_str()<<std::endl;

      os<<"attach_flag:"<<data._attach_flag<<std::endl;

      os<<"swath:"<<data._swath.c_str()<<std::endl;

      for (int i = 0; i<33; i++) {
         os<<"elevation pattern:"<<data._elevation_pattern[i]<<std::endl;
      }

      return os;

   }

   std::istream& operator>>(std::istream& is, AntennaElevationPatterns& data)
   {
      char buf16[17];
      buf16[16]='\0';
      char buf12[13];
      buf12[12] = '\0';
      char buf3[4];
      buf3[3] = '\0';
      // char buf2[3];
      // buf2[2] = '\0';
//	char buf1[1]; // avoid not used warning

      is.read(buf12,12);
      data._zero_doppler_time = buf12;

      is.read((char*)&(data._attach_flag),1);

      is.read(buf3,3);
      data._swath = buf3;

      for (int i = 0; i<33; i++) {
         is.read((char*)&(data._elevation_pattern[i]),4);
         data.SwitchEndian(data._elevation_pattern[i]);
      }

      is.read(buf16,14);

      return is;

   }

   AntennaElevationPatterns::AntennaElevationPatterns(const AntennaElevationPatterns& rhs):
      EnvisatAsarRecord(rhs),
      _zero_doppler_time(rhs._zero_doppler_time),
      _attach_flag(rhs._attach_flag),
      _swath(rhs._swath)
   {
      for (int i = 0; i<33; i++) {
         _elevation_pattern[i] = rhs._elevation_pattern[i];
      }
   }


   AntennaElevationPatterns& AntennaElevationPatterns::operator=(const AntennaElevationPatterns& rhs)
   {
      _zero_doppler_time = rhs._zero_doppler_time;
      _attach_flag = rhs._attach_flag;
      _swath = rhs._swath;

      for (int i = 0; i<33; i++) {
         _elevation_pattern[i] = rhs._elevation_pattern[i];
      }

      return *this;
   }
}
