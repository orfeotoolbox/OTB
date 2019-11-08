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

#include <EnvisatAsar/SRGRConversionParameters.h>

namespace ossimplugins
{

   SRGRConversionParameters::SRGRConversionParameters() : EnvisatAsarRecord("SRGRConversionParameters_rec")
   {
   }

   SRGRConversionParameters::~SRGRConversionParameters()
   {
   }

   std::ostream& operator<<(std::ostream& os, const SRGRConversionParameters& data)
   {
      os<<"first_zero_doppler_time_day:"<<data._first_zero_doppler_time_day<<std::endl;

      os<<"first_zero_doppler_time_sec:"<<data._first_zero_doppler_time_sec<<std::endl;

      os<<"first_zero_doppler_time_microsec:"<<data._first_zero_doppler_time_microsec<<std::endl;

      os<<"attach_flag:"<<data._attach_flag<<std::endl;

      os<<"_slant_range_time:"<<data._slant_range_time<<std::endl;

      os<<"_ground_range_origin:"<<data._ground_range_origin<<std::endl;

      for (int i = 0; i<5; i++) {
         os<<"_srgr_coef[i]:"<<data._srgr_coef[i]<<std::endl;
      }

      return os;

   }

   std::istream& operator>>(std::istream& is, SRGRConversionParameters& data)
   {
      char buf14[15];
      buf14[14] = '\0';
      // char buf12[13];
      // buf12[12] = '\0';


      is.read((char*)&(data._first_zero_doppler_time_day),4);
      data.SwitchEndian(data._first_zero_doppler_time_day);

      is.read((char*)&(data._first_zero_doppler_time_sec),4);
      data.SwitchEndian(data._first_zero_doppler_time_sec);

      is.read((char*)&(data._first_zero_doppler_time_microsec),4);
      data.SwitchEndian(data._first_zero_doppler_time_microsec);

      is.read((char*)&(data._attach_flag),1);

      is.read((char*)&(data._slant_range_time),4);
      data.SwitchEndian(data._slant_range_time);

      is.read((char*)&(data._ground_range_origin),4);
      data.SwitchEndian(data._ground_range_origin);

      for (int i = 0; i<5; i++) {
         is.read((char*)&(data._srgr_coef[i]),4);
         data.SwitchEndian(data._srgr_coef[i]);
      }

      is.read(buf14,14);

      return is;
   }


   SRGRConversionParameters::SRGRConversionParameters(const SRGRConversionParameters& rhs):
      EnvisatAsarRecord(rhs),
      _first_zero_doppler_time_day(rhs._first_zero_doppler_time_day),
      _first_zero_doppler_time_sec(rhs._first_zero_doppler_time_sec),
      _first_zero_doppler_time_microsec(rhs._first_zero_doppler_time_microsec),
      _attach_flag(rhs._attach_flag),
      _slant_range_time(rhs._slant_range_time),
      _ground_range_origin(rhs._ground_range_origin)
   {
      for (int i = 0; i<5; i++) {
         _srgr_coef[i] = rhs._srgr_coef[i];
      }
   }

   SRGRConversionParameters& SRGRConversionParameters::operator=(const SRGRConversionParameters& rhs)
   {
      _first_zero_doppler_time_day = rhs._first_zero_doppler_time_day;
      _first_zero_doppler_time_sec = rhs._first_zero_doppler_time_sec;
      _first_zero_doppler_time_microsec = rhs._first_zero_doppler_time_microsec;
      _attach_flag = rhs._attach_flag;
      _slant_range_time = rhs._slant_range_time;

      for (int i = 0; i<5; i++) {
         _srgr_coef[i] = rhs._srgr_coef[i];
      }

      return *this;
   }
}
