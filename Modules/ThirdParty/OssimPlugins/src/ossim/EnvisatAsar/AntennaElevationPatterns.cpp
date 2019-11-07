/*
 * Copyright (C) 2005-2019 by Centre National d'Etudes Spatiales (CNES)
 *
 * This file is licensed under MIT license:
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


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
      os<<"zero_doppler_time:"<<data._zero_doppler_time <<std::endl;

      os<<"attach_flag:"<<data._attach_flag<<std::endl;

      os<<"swath:"<<data._swath <<std::endl;

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
