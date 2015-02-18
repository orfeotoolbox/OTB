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

#include <EnvisatAsar/ChirpParameters.h>

namespace ossimplugins
{
   ChirpParameters::ChirpParameters() : EnvisatAsarRecord("ChirpParameters_rec")
   {
   }

   ChirpParameters::~ChirpParameters()
   {
   }


   std::ostream& operator<<(std::ostream& os, const ChirpParameters& data)
   {
      os<<"zero_doppler_time:"<<data._zero_doppler_time.c_str()<<std::endl;

      os<<"attach_flag:"<<data._attach_flag<<std::endl;

      os<<"swath:"<<data._swath.c_str()<<std::endl;

      os<<"polar:"<<data._polar.c_str()<<std::endl;

      os<<"chirp_width:"<<data._chirp_width<<std::endl;

      os<<"chirp_sidelobe:"<<data._chirp_sidelobe<<std::endl;

      os<<"chirp_islr:"<<data._chirp_islr<<std::endl;

      os<<"chirp_peak_loc:"<<data._chirp_peak_loc<<std::endl;

      os<<"re_chirp_power:"<<data._re_chirp_power<<std::endl;

      os<<"elev_chirp_power:"<<data._elev_chirp_power<<std::endl;

      os<<"chirp_quality_flag:"<<data._chirp_quality_flag<<std::endl;

      os<<"ref_chirp_power:"<<data._ref_chirp_power<<std::endl;

      os<<"normalisation_source:"<<data._normalisation_source.c_str()<<std::endl;

      for (int i = 0 ; i<352; i++) {
         os<<"cal_pulse_info["<<i<<"]:"<<data._cal_pulse_info[i]<<std::endl;
      }

      return os;

   }

   std::istream& operator>>(std::istream& is, ChirpParameters& data)
   {
      // char buf64[65];
      // buf64[64] = '\0';
      // char buf32[33];
      // buf32[32] = '\0';
      // char buf27[28];
      // buf27[27] = '\0';
      // char buf20[21];
      // buf20[20] = '\0';
      char buf16[17];
      buf16[16]='\0';
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
      char buf7[8];
      buf7[7] = '\0';
      // char buf6[7];
      // buf6[6] = '\0';
      // char buf5[6];
      // buf5[5] = '\0';
      char buf4[5];
      buf4[4] = '\0';
      char buf3[4];
      buf3[3] = '\0';
      // char buf2[3];
      // buf2[2] = '\0';
//	char buf1[1]; // not used avoid warning

      is.read(buf12,12);
      data._zero_doppler_time = buf12;

      is.read((char*)&(data._attach_flag),1);

      is.read(buf3,3);
      data._swath = buf3;

      is.read(buf3,3);
      data._polar = buf3;

      is.read((char*)&(data._chirp_width),4);
      data.SwitchEndian(data._chirp_width);

      is.read((char*)&(data._chirp_sidelobe),4);
      data.SwitchEndian(data._chirp_sidelobe);

      is.read((char*)&(data._chirp_islr),4);
      data.SwitchEndian(data._chirp_islr);

      is.read((char*)&(data._chirp_peak_loc),4);
      data.SwitchEndian(data._chirp_peak_loc);

      is.read((char*)&(data._re_chirp_power),4);
      data.SwitchEndian(data._re_chirp_power);

      is.read((char*)&(data._elev_chirp_power),4);
      data.SwitchEndian(data._elev_chirp_power);

      is.read((char*)&(data._chirp_quality_flag),1);
      data.SwitchEndian(data._chirp_quality_flag);

      is.read((char*)&(data._ref_chirp_power),4);
      data.SwitchEndian(data._ref_chirp_power);

      is.read(buf7,7);
      data._normalisation_source = buf7;

      is.read(buf4,4);

      for (int i = 0; i<352; i++) {
         is.read((char*)&(data._cal_pulse_info[i]),4);
         data.SwitchEndian(data._cal_pulse_info[i]);
      }

      is.read(buf16,16);

      return is;

   }

   ChirpParameters::ChirpParameters(const ChirpParameters& rhs):
      EnvisatAsarRecord(rhs),
      _zero_doppler_time(rhs._zero_doppler_time),
      _attach_flag(rhs._attach_flag),
      _swath(rhs._swath),
      _polar(rhs._polar),
      _chirp_width(rhs._chirp_width),
      _chirp_sidelobe(rhs._chirp_sidelobe),
      _chirp_islr(rhs._chirp_islr),
      _chirp_peak_loc(rhs._chirp_peak_loc),
      _re_chirp_power(rhs._re_chirp_power),
      _elev_chirp_power(rhs._elev_chirp_power),
      _chirp_quality_flag(rhs._chirp_quality_flag),
      _ref_chirp_power(rhs._ref_chirp_power),
      _normalisation_source(rhs._normalisation_source)
   {
      for (int i = 0; i<352; i++) {
         _cal_pulse_info[i] = rhs._cal_pulse_info[i];
      }
   }


   ChirpParameters& ChirpParameters::operator=(const ChirpParameters& rhs)
   {
      _zero_doppler_time = rhs._zero_doppler_time;
      _attach_flag = rhs._attach_flag;
      _swath = rhs._swath;
      _polar = rhs._polar;
      _chirp_width = rhs._chirp_width;
      _chirp_sidelobe = rhs._chirp_sidelobe;
      _chirp_islr = rhs._chirp_islr;
      _chirp_peak_loc = rhs._chirp_peak_loc;
      _re_chirp_power = rhs._re_chirp_power;
      _elev_chirp_power = rhs._elev_chirp_power;
      _chirp_quality_flag = rhs._chirp_quality_flag;
      _ref_chirp_power = rhs._ref_chirp_power;
      _normalisation_source = rhs._normalisation_source;

      for (int i = 0; i<352; i++) {
         _cal_pulse_info[i] = rhs._cal_pulse_info[i];
      }

      return *this;
   }
}

