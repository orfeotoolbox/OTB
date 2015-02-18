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

#include <EnvisatAsar/SQ_ADSR.h>

namespace ossimplugins
{
   SQ_ADSR::SQ_ADSR() : EnvisatAsarRecord("sq_adsr_rec")
   {
   }

   SQ_ADSR::~SQ_ADSR()
   {
   }



   std::ostream& operator<<(std::ostream& os, const SQ_ADSR& data)
   {
      os<<"zero_doppler_time:"<<data._zero_doppler_time.c_str()<<std::endl;

      os<<"attach_flag:"<<data._attach_flag<<std::endl;

      os<<"input_mean_flag:"<<data._input_mean_flag<<std::endl;

      os<<"input_std_dev_flag:"<<data._input_std_dev_flag<<std::endl;

      os<<"input_gaps_flag:"<<data._input_gaps_flag<<std::endl;

      os<<"input_missing_lines_flag:"<<data._input_missing_lines_flag<<std::endl;

      os<<"dop_cen_flag:"<<data._dop_cen_flag<<std::endl;

      os<<"dop_amb_flag:"<<data._dop_amb_flag<<std::endl;

      os<<"output_mean_flag:"<<data._output_mean_flag<<std::endl;

      os<<"output_std_dev_flag:"<<data._output_std_dev_flag<<std::endl;

      os<<"chirp_flag:"<<data._chirp_flag<<std::endl;

      os<<"missing_data_set_flag:"<<data._missing_data_set_flag<<std::endl;

      os<<"invalid_downlink_flag:"<<data._invalid_downlink_flag<<std::endl;

      os<<"thres_chirp_broadening:"<<data._thres_chirp_broadening<<std::endl;

      os<<"thresh_chirp_sidelobe:"<<data._thresh_chirp_sidelobe<<std::endl;

      os<<"thresh_chirp_islr:"<<data._thresh_chirp_islr<<std::endl;

      os<<"thresh_input_mean:"<<data._thresh_input_mean<<std::endl;

      os<<"exp_input_mean:"<<data._exp_input_mean<<std::endl;

      os<<"thresh_input_std_dev:"<<data._thresh_input_std_dev<<std::endl;

      os<<"exp_input_std_dev:"<<data._exp_input_std_dev<<std::endl;

      os<<"thresh_dop_cen:"<<data._thresh_dop_cen<<std::endl;

      os<<"thresh_dop_amb:"<<data._thresh_dop_amb<<std::endl;

      os<<"thresh_output_mean:"<<data._thresh_output_mean<<std::endl;

      os<<"exp_output_mean:"<<data._exp_output_mean<<std::endl;

      os<<"thresh_output_std_dev:"<<data._thresh_output_std_dev<<std::endl;

      os<<"exp_output_std_dev:"<<data._exp_output_std_dev<<std::endl;

      os<<"thresh_input_missing_lines:"<<data._thresh_input_missing_lines<<std::endl;

      os<<"thresh_input_gaps:"<<data._thresh_input_gaps<<std::endl;

      os<<"lines_per_gaps:"<<data._lines_per_gaps<<std::endl;

      os<<"input_mean[0]:"<<data._input_mean[0]<<std::endl;
      os<<"input_mean[1]:"<<data._input_mean[1]<<std::endl;

      os<<"input_std_dev[0]:"<<data._input_std_dev[0]<<std::endl;
      os<<"input_std_dev[1]:"<<data._input_std_dev[1]<<std::endl;

      os<<"num_gaps:"<<data._num_gaps<<std::endl;

      os<<"num_missing_lines:"<<data._num_missing_lines<<std::endl;

      os<<"output_mean[0]:"<<data._output_mean[0]<<std::endl;
      os<<"output_mean[1]:"<<data._output_mean[1]<<std::endl;

      os<<"output_std_dev[0]:"<<data._output_std_dev[0]<<std::endl;
      os<<"output_std_dev[1]:"<<data._output_std_dev[1]<<std::endl;

      os<<"tot_errors:"<<data._tot_errors<<std::endl;

      return os;

   }

   std::istream& operator>>(std::istream& is, SQ_ADSR& data)
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
      char buf15[16];
      buf15[15] = '\0';
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
      // char buf4[5];
      // buf4[4] = '\0';
      // char buf3[4];
      // buf3[3] = '\0';
      // char buf2[3];
      // buf2[2] = '\0';
//	char buf1[1]; // not used avoid warning

      is.read(buf12,12);
      data._zero_doppler_time = buf12;

      is.read((char*)&(data._attach_flag),1);

      is.read((char*)&(data._input_mean_flag),1);

      is.read((char*)&(data._input_std_dev_flag),1);

      is.read((char*)&(data._input_gaps_flag),1);

      is.read((char*)&(data._input_missing_lines_flag),1);

      is.read((char*)&(data._dop_cen_flag),1);

      is.read((char*)&(data._dop_amb_flag),1);

      is.read((char*)&(data._output_mean_flag),1);

      is.read((char*)&(data._output_std_dev_flag),1);

      is.read((char*)&(data._chirp_flag),1);

      is.read((char*)&(data._missing_data_set_flag),1);

      is.read((char*)&(data._invalid_downlink_flag),1);

      is.read(buf7,7);

      is.read((char*)&(data._thres_chirp_broadening),4);
      data.SwitchEndian(data._thres_chirp_broadening);

      is.read((char*)&(data._thresh_chirp_sidelobe),4);
      data.SwitchEndian(data._thresh_chirp_sidelobe);

      is.read((char*)&(data._thresh_chirp_islr),4);
      data.SwitchEndian(data._thresh_chirp_islr);

      is.read((char*)&(data._thresh_input_mean),4);
      data.SwitchEndian(data._thresh_input_mean);

      is.read((char*)&(data._exp_input_mean),4);
      data.SwitchEndian(data._exp_input_mean);

      is.read((char*)&(data._thresh_input_std_dev),4);
      data.SwitchEndian(data._thresh_input_std_dev);

      is.read((char*)&(data._exp_input_std_dev),4);
      data.SwitchEndian(data._exp_input_std_dev);

      is.read((char*)&(data._thresh_dop_cen),4);
      data.SwitchEndian(data._thresh_dop_cen);

      is.read((char*)&(data._thresh_dop_amb),4);
      data.SwitchEndian(data._thresh_dop_amb);

      is.read((char*)&(data._thresh_output_mean),4);
      data.SwitchEndian(data._thresh_output_mean);

      is.read((char*)&(data._exp_output_mean),4);
      data.SwitchEndian(data._exp_output_mean);

      is.read((char*)&(data._thresh_output_std_dev),4);
      data.SwitchEndian(data._thresh_output_std_dev);

      is.read((char*)&(data._exp_output_std_dev),4);
      data.SwitchEndian(data._exp_output_std_dev);

      is.read((char*)&(data._thresh_input_missing_lines),4);
      data.SwitchEndian(data._thresh_input_missing_lines);

      is.read((char*)&(data._thresh_input_gaps),4);
      data.SwitchEndian(data._thresh_input_gaps);

      is.read((char*)&(data._lines_per_gaps),4);
      data.SwitchEndian(data._lines_per_gaps);

      is.read(buf15,15);

      is.read((char*)&(data._input_mean[0]),4);
      data.SwitchEndian(data._input_mean[0]);

      is.read((char*)&(data._input_mean[1]),4);
      data.SwitchEndian(data._input_mean[1]);

      is.read((char*)&(data._input_std_dev[0]),4);
      data.SwitchEndian(data._input_std_dev[0]);
      is.read((char*)&(data._input_std_dev[1]),4);
      data.SwitchEndian(data._input_std_dev[1]);

      is.read((char*)&(data._num_gaps),4);
      data.SwitchEndian(data._num_gaps);

      is.read((char*)&(data._num_missing_lines),4);
      data.SwitchEndian(data._num_missing_lines);

      is.read((char*)&(data._output_mean[0]),4);
      data.SwitchEndian(data._output_mean[0]);
      is.read((char*)&(data._output_mean[1]),4);
      data.SwitchEndian(data._output_mean[1]);

      is.read((char*)&(data._output_std_dev[0]),4);
      data.SwitchEndian(data._output_std_dev[0]);

      is.read((char*)&(data._output_std_dev[1]),4);
      data.SwitchEndian(data._output_std_dev[1]);

      is.read((char*)&(data._tot_errors),4);
      data.SwitchEndian(data._tot_errors);

      is.read(buf16,16);

      return is;

   }

   SQ_ADSR::SQ_ADSR(const SQ_ADSR& rhs):
      EnvisatAsarRecord(rhs),
      _zero_doppler_time(rhs._zero_doppler_time),
      _attach_flag(rhs._attach_flag),
      _input_mean_flag(rhs._input_mean_flag),
      _input_std_dev_flag(rhs._input_std_dev_flag),
      _input_gaps_flag(rhs._input_gaps_flag),
      _input_missing_lines_flag(rhs._input_missing_lines_flag),
      _dop_cen_flag(rhs._dop_cen_flag),
      _dop_amb_flag(rhs._dop_amb_flag),
      _output_mean_flag(rhs._output_mean_flag),
      _output_std_dev_flag(rhs._output_std_dev_flag),
      _chirp_flag(rhs._chirp_flag),
      _missing_data_set_flag(rhs._missing_data_set_flag),
      _invalid_downlink_flag(rhs._invalid_downlink_flag),
      _thres_chirp_broadening(rhs._thres_chirp_broadening),
      _thresh_chirp_sidelobe(rhs._thresh_chirp_sidelobe),
      _thresh_chirp_islr(rhs._thresh_chirp_islr),
      _thresh_input_mean(rhs._thresh_input_mean),
      _exp_input_mean(rhs._exp_input_mean),
      _thresh_input_std_dev(rhs._thresh_input_std_dev),
      _exp_input_std_dev(rhs._exp_input_std_dev),
      _thresh_dop_cen(rhs._thresh_dop_cen),
      _thresh_dop_amb(rhs._thresh_dop_amb),
      _thresh_output_mean(rhs._thresh_output_mean),
      _exp_output_mean(rhs._exp_output_mean),
      _thresh_output_std_dev(rhs._thresh_output_std_dev),
      _exp_output_std_dev(rhs._exp_output_std_dev),
      _thresh_input_missing_lines(rhs._thresh_input_missing_lines),
      _thresh_input_gaps(rhs._thresh_input_gaps),
      _lines_per_gaps(rhs._lines_per_gaps),
      _num_gaps(rhs._num_gaps),
      _num_missing_lines(rhs._num_missing_lines),
      _tot_errors(rhs._tot_errors)
   {
      _input_mean[0] = rhs._input_mean[0];
      _input_mean[1] = rhs._input_mean[1];

      _input_std_dev[0] = rhs._input_std_dev[0];
      _input_std_dev[1] = rhs._input_std_dev[1];

      _output_mean[0] = rhs._output_mean[0];
      _output_mean[1] = rhs._output_mean[1];

      _output_std_dev[0] = rhs._output_std_dev[0];
      _output_std_dev[1] = rhs._output_std_dev[1];

   }

   SQ_ADSR& SQ_ADSR::operator=(const SQ_ADSR& rhs)
   {
      _zero_doppler_time = rhs._zero_doppler_time;
      _attach_flag = rhs._attach_flag;
      _input_mean_flag = rhs._input_mean_flag;
      _input_std_dev_flag = rhs._input_std_dev_flag;
      _input_gaps_flag = rhs._input_gaps_flag;
      _input_missing_lines_flag = rhs._input_missing_lines_flag;
      _dop_cen_flag = rhs._dop_cen_flag;
      _dop_amb_flag = rhs._dop_amb_flag;
      _output_mean_flag = rhs._output_mean_flag;
      _output_std_dev_flag = rhs._output_std_dev_flag;
      _chirp_flag = rhs._chirp_flag;
      _missing_data_set_flag = rhs._missing_data_set_flag;
      _invalid_downlink_flag = rhs._invalid_downlink_flag;
      _thres_chirp_broadening = rhs._thres_chirp_broadening;
      _thresh_chirp_sidelobe = rhs._thresh_chirp_sidelobe;
      _thresh_chirp_islr = rhs._thresh_chirp_islr;
      _thresh_input_mean = rhs._thresh_input_mean;
      _exp_input_mean = rhs._exp_input_mean;
      _thresh_input_std_dev = rhs._thresh_input_std_dev;
      _exp_input_std_dev = rhs._exp_input_std_dev;
      _thresh_dop_cen = rhs._thresh_dop_cen;
      _thresh_dop_amb = rhs._thresh_dop_amb;
      _thresh_output_mean = rhs._thresh_output_mean;
      _exp_output_mean = rhs._exp_output_mean;
      _thresh_output_std_dev = rhs._thresh_output_std_dev;
      _exp_output_std_dev = rhs._exp_output_std_dev;
      _thresh_input_missing_lines = rhs._thresh_input_missing_lines;
      _thresh_input_gaps = rhs._thresh_input_gaps;
      _lines_per_gaps = rhs._lines_per_gaps;
      _num_gaps = rhs._num_gaps;
      _num_missing_lines = rhs._num_missing_lines;
      _tot_errors = rhs._tot_errors;

      _input_mean[0] = rhs._input_mean[0];
      _input_mean[1] = rhs._input_mean[1];

      _input_std_dev[0] = rhs._input_std_dev[0];
      _input_std_dev[1] = rhs._input_std_dev[1];

      _output_mean[0] = rhs._output_mean[0];
      _output_mean[1] = rhs._output_mean[1];

      _output_std_dev[0] = rhs._output_std_dev[0];
      _output_std_dev[1] = rhs._output_std_dev[1];

      return *this;
   }
}
