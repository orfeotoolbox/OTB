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

#include <EnvisatAsar/DopplerCentroidParameters.h>

namespace ossimplugins
{
   DopplerCentroidParameters::DopplerCentroidParameters() : EnvisatAsarRecord("DopplerCentroidParameters_rec")
   {
   }

   DopplerCentroidParameters::~DopplerCentroidParameters()
   {
   }


   std::ostream& operator<<(std::ostream& os, const DopplerCentroidParameters& data)
   {
      os<<"zero_doppler_time:"<<data._zero_doppler_time.c_str()<<std::endl;

      os<<"attach_flag:"<<data._attach_flag<<std::endl;

      os<<"slant_range_time:"<<data._slant_range_time<<std::endl;

      os<<"dop_coef[0]:"<<data._dop_coef[0]<<std::endl;
      os<<"dop_coef[1]:"<<data._dop_coef[1]<<std::endl;
      os<<"dop_coef[2]:"<<data._dop_coef[2]<<std::endl;
      os<<"dop_coef[3]:"<<data._dop_coef[3]<<std::endl;
      os<<"dop_coef[4]:"<<data._dop_coef[4]<<std::endl;

      os<<"dop_conf:"<<data._dop_conf<<std::endl;

      os<<"dop_conf_below_thresh_flag:"<<data._dop_conf_below_thresh_flag<<std::endl;

      os<<"delta_dopp_coeff[0]:"<<data._delta_dopp_coeff[0]<<std::endl;
      os<<"delta_dopp_coeff[1]:"<<data._delta_dopp_coeff[1]<<std::endl;
      os<<"delta_dopp_coeff[2]:"<<data._delta_dopp_coeff[2]<<std::endl;
      os<<"delta_dopp_coeff[3]:"<<data._delta_dopp_coeff[3]<<std::endl;
      os<<"delta_dopp_coeff[4]:"<<data._delta_dopp_coeff[4]<<std::endl;

      return os;

   }

   std::istream& operator>>(std::istream& is, DopplerCentroidParameters& data)
   {
      // char buf64[65];
      // buf64[64] = '\0';
      // char buf32[33];
      // buf32[32] = '\0';
      // char buf27[28];
      // buf27[27] = '\0';
      // char buf20[21];
      // buf20[20] = '\0';
      // char buf16[17];
      // buf16[16]='\0';
      // char buf15[16];
      // buf15[15] = '\0';
      // char buf14[15];
      // buf14[14] = '\0';
      // char buf13[14];
      // buf13[13] = '\0';
      char buf12[13];
      buf12[12] = '\0';
      // char buf11[12];
      // buf11[11] = '\0';
      // char buf10[11];
      // buf10[10] = '\0';
      // char buf9[10];
      // buf9[9] = '\0';
      // char buf8[9];
      // buf8[8] = '\0';
      // char buf7[8];
      // buf7[7] = '\0';
      // char buf6[7];
      // buf6[6] = '\0';
      // char buf5[6];
      // buf5[5] = '\0';
      // char buf4[5];
      // buf4[4] = '\0';
      char buf3[4];
      buf3[3] = '\0';
      // char buf2[3];
      // buf2[2] = '\0';
//	char buf1[1];  // Not used avoid warning

      is.read(buf12,12);
      data._zero_doppler_time = buf12;

      is.read((char*)&(data._attach_flag),1);

      is.read((char*)&(data._slant_range_time),4);
      data.SwitchEndian(data._slant_range_time);

      for (int i = 0; i <5; i++) {
         is.read((char*)&(data._dop_coef[i]),4);
         data.SwitchEndian(data._dop_coef[i]);
      }

      is.read((char*)&(data._dop_conf),4);
      data.SwitchEndian(data._dop_conf);

      is.read((char*)&(data._dop_conf_below_thresh_flag),1);

      for (int i = 0; i <5; i++) {
         is.read((char*)&(data._delta_dopp_coeff[i]),2);
         data.SwitchEndian(data._delta_dopp_coeff[i]);
      }

      is.read(buf3,3);

      return is;

   }

   DopplerCentroidParameters::DopplerCentroidParameters(const DopplerCentroidParameters& rhs):
      EnvisatAsarRecord(rhs),
      _zero_doppler_time(rhs._zero_doppler_time),
      _attach_flag(rhs._attach_flag),
      _slant_range_time(rhs._slant_range_time),
      _dop_conf(rhs._dop_conf),
      _dop_conf_below_thresh_flag(rhs._dop_conf_below_thresh_flag)
   {
      for (int i = 0; i <5; i++) {
         _dop_coef[i] = rhs._dop_coef[i];
      }
      for (int i = 0; i <5; i++) {
         _delta_dopp_coeff[i] = rhs._delta_dopp_coeff[i];
      }
   }


   DopplerCentroidParameters& DopplerCentroidParameters::operator=(const DopplerCentroidParameters& rhs)
   {
      _zero_doppler_time = rhs._zero_doppler_time;
      _attach_flag = rhs._attach_flag;
      _slant_range_time = rhs._slant_range_time;
      _dop_conf = rhs._dop_conf;
      _dop_conf_below_thresh_flag = rhs._dop_conf_below_thresh_flag;

      for (int i = 0; i <5; i++) {
         _dop_coef[i] = rhs._dop_coef[i];
      }
      for (int i = 0; i <5; i++) {
         _delta_dopp_coeff[i] = rhs._delta_dopp_coeff[i];
      }

      return *this;
   }
}
