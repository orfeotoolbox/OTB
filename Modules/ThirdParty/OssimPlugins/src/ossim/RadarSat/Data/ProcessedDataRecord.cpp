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

#include <RadarSat/Data/ProcessedDataRecord.h>

namespace ossimplugins
{

   ProcessedDataRecord::ProcessedDataRecord() : RadarSatRecord("proc_data_rec")
   {
   }

   ProcessedDataRecord::~ProcessedDataRecord()
   {
   }

   std::ostream& operator<<(std::ostream& os, const ProcessedDataRecord& data)
   {
      os<<"line_num:"<<data._line_num<<std::endl;
      os<<"rec_num:"<<data._rec_num<<std::endl;
      os<<"n_left_pixel:"<<data._n_left_pixel<<std::endl;
      os<<"n_data_pixel:"<<data._n_data_pixel<<std::endl;
      os<<"n_right_pixel:"<<data._n_right_pixel<<std::endl;
      os<<"sensor_updf:"<<data._sensor_updf<<std::endl;
      os<<"acq_year:"<<data._acq_year<<std::endl;
      os<<"acq_day:"<<data._acq_day<<std::endl;
      os<<"acq_msec:"<<data._acq_msec<<std::endl;
      os<<"sar_chan_ind:"<<data._sar_chan_ind<<std::endl;
      os<<"sar_chan_code:"<<data._sar_chan_code<<std::endl;
      os<<"tran_polar:"<<data._tran_polar<<std::endl;
      os<<"recv_polar:"<<data._recv_polar<<std::endl;
      os<<"prf:"<<data._prf<<std::endl;
      os<<"sr_first:"<<data._sr_first<<std::endl;
      os<<"sr_last:"<<data._sr_last<<std::endl;
      os<<"fdc_first:"<<data._fdc_first<<std::endl;
      os<<"fdc_mid:"<<data._fdc_mid<<std::endl;
      os<<"fdc_last:"<<data._fdc_last<<std::endl;
      os<<"ka_first:"<<data._ka_first<<std::endl;
      os<<"ka_mid:"<<data._ka_mid<<std::endl;
      os<<"ka_last:"<<data._ka_last<<std::endl;
      os<<"nadir_ang:"<<data._nadir_ang<<std::endl;
      os<<"squint_ang:"<<data._squint_ang<<std::endl;
      os<<"geo_updf:"<<data._geo_updf<<std::endl;
      os<<"lat_first:"<<data._lat_first<<std::endl;
      os<<"lat_mid:"<<data._lat_mid<<std::endl;
      os<<"lat_last:"<<data._lat_last<<std::endl;
      os<<"lon_first:"<<data._lon_first<<std::endl;
      os<<"lon_mid:"<<data._lon_mid<<std::endl;
      os<<"lon_last:"<<data._lon_last<<std::endl;
      os<<"north_first:"<<data._north_first<<std::endl;
      os<<"north_last:"<<data._north_last<<std::endl;
      os<<"east_first:"<<data._east_first<<std::endl;
      os<<"east_last:"<<data._east_last<<std::endl;
      os<<"heading:"<<data._heading<<std::endl;

      return os;
   }

   void SwitchEndian(int& value)
   {
      char buffer[4];
      char res[4];

      memcpy(buffer,&value,4);
      res[0] = buffer[3];
      res[1] = buffer[2];
      res[2] = buffer[1];
      res[3] = buffer[0];

      memcpy(&value,res,4);
   }

   std::istream& operator>>(std::istream& is, ProcessedDataRecord& data)
   {
      char buf[193];
      buf[192] = '\0';

      is.read((char*)&(data._line_num),4);
      data.SwitchEndian(data._line_num);

      is.read((char*)&(data._rec_num),4);
      data.SwitchEndian(data._rec_num);

      is.read((char*)&(data._n_left_pixel),4);
      data.SwitchEndian(data._n_left_pixel);

      is.read((char*)&(data._n_data_pixel),4);
      data.SwitchEndian(data._n_data_pixel);

      is.read((char*)&(data._n_right_pixel),4);
      data.SwitchEndian(data._n_right_pixel);

      is.read((char*)&(data._sensor_updf),4);
      data.SwitchEndian(data._sensor_updf);

      is.read((char*)&(data._acq_year),4);
      data.SwitchEndian(data._acq_year);

      is.read((char*)&(data._acq_day),4);
      data.SwitchEndian(data._acq_day);

      is.read((char*)&(data._acq_msec),4);
      data.SwitchEndian(data._acq_msec);

      is.read((char*)&(data._sar_chan_ind),2);
      data.SwitchEndian(data._sar_chan_ind);

      is.read((char*)&(data._sar_chan_code),2);
      data.SwitchEndian(data._sar_chan_code);

      is.read((char*)&(data._tran_polar),2);
      data.SwitchEndian(data._tran_polar );

      is.read((char*)&(data._recv_polar),2);
      data.SwitchEndian(data._recv_polar);

      is.read((char*)&(data._prf),4);
      data.SwitchEndian(data._prf );

      is.read(buf,4);

      is.read((char*)&(data._sr_first),4);
      data.SwitchEndian(data._sr_first);

      is.read((char*)&(data._sr_mid),4);
      data.SwitchEndian(data._sr_mid);

      is.read((char*)&(data._sr_last),4);
      data.SwitchEndian(data._sr_last);

      is.read((char*)&(data._fdc_first),4);
      data.SwitchEndian(data._fdc_first);

      is.read((char*)&(data._fdc_mid),4);
      data.SwitchEndian(data._fdc_mid);

      is.read((char*)&(data._fdc_last),4);
      data.SwitchEndian(data._fdc_last);

      is.read((char*)&(data._ka_first),4);
      data.SwitchEndian(data._ka_first);

      is.read((char*)&(data._ka_mid),4);
      data.SwitchEndian(data._ka_mid);

      is.read((char*)&(data._ka_last),4);
      data.SwitchEndian(data._ka_last);

      is.read((char*)&(data._nadir_ang),4);
      data.SwitchEndian(data._nadir_ang);

      is.read((char*)&(data._squint_ang),4);
      data.SwitchEndian(data._squint_ang);

      is.read(buf,4);
      is.read(buf,16);

      is.read((char*)&(data._geo_updf),4);
      data.SwitchEndian(data._geo_updf);

      is.read((char*)&(data._lat_first),4);
      data.SwitchEndian(data._lat_first);

      is.read((char*)&(data._lat_mid),4);
      data.SwitchEndian(data._lat_mid);

      is.read((char*)&(data._lat_last),4);
      data.SwitchEndian(data._lat_last);

      is.read((char*)&(data._lon_first),4);
      data.SwitchEndian(data._lon_first);

      is.read((char*)&(data._lon_mid),4);
      data.SwitchEndian(data._lon_mid);

      is.read((char*)&(data._lon_last),4);
      data.SwitchEndian(data._lon_last);

      is.read((char*)&(data._north_first),4);
      data.SwitchEndian(data._north_first);

      is.read(buf,4);

      is.read((char*)&(data._north_last),4);
      data.SwitchEndian(data._north_last);

      is.read((char*)&(data._east_first),4);
      data.SwitchEndian(data._east_first);

      is.read(buf,4);

      is.read((char*)&(data._east_last),4);
      data.SwitchEndian(data._east_last);

      is.read((char*)&(data._heading),4);
      data.SwitchEndian(data._heading);

      is.read(buf,8);

      return is;
   }

   ProcessedDataRecord::ProcessedDataRecord(const ProcessedDataRecord& rhs):
      RadarSatRecord(rhs)
   {


   }

   ProcessedDataRecord& ProcessedDataRecord::operator=(const ProcessedDataRecord& /* rhs */)
   {
      return *this;
   }
}
