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

#include <EnvisatAsar/sph.h>
#include <EnvisatAsar/mph.h>

namespace ossimplugins
{
   sph::sph() : EnvisatAsarRecord("sph_rec")
   {
      this->_num_dsd = 18;
      this->_size_dsd = 280;
   }

   sph::sph(const mph& data) : EnvisatAsarRecord("sph_rec")
   {
      this->_num_dsd = data._num_dsd ;
      this->_size_dsd = data._dsd_size;
   }

   sph::~sph()
   {
   }


   std::ostream& operator<<(std::ostream& os, const sph& data)
   {
      os<<"sph_desc_title:"<<data._sph_desc_title.c_str()<<std::endl;

      os<<"sph_desc:"<<data._sph_desc.c_str()<<std::endl;

      os<<"strip_cont_ind_title:"<<data._strip_cont_ind_title.c_str()<<std::endl;

      os<<"strip_cont_ind	std::string:"<<data._strip_cont_ind.c_str()<<std::endl;

      os<<"slice_pos_title:"<<data._slice_pos_title.c_str()<<std::endl;

      os<<"slice_position:"<<data._slice_position.c_str()<<std::endl;

      os<<"num_slices_title:"<<data._num_slices_title.c_str()<<std::endl;

      os<<"num_slices:"<<data._num_slices.c_str()<<std::endl;

      os<<"first_line_time_title:"<<data._first_line_time_title.c_str()<<std::endl;

      os<<"first_line_time:"<<data._first_line_time.c_str()<<std::endl;

      os<<"last_line_time_title:"<<data._last_line_time_title.c_str()<<std::endl;

      os<<"last_line_time:"<<data._last_line_time.c_str()<<std::endl;

      os<<"first_near_lat_title:"<<data._first_near_lat_title.c_str()<<std::endl;

      os<<"first_near_lat:"<<data._first_near_lat.c_str()<<std::endl;

      os<<"first_near_lat_units:"<<data._first_near_lat_units.c_str()<<std::endl;

      os<<"first_near_long_title:"<<data._first_near_long_title.c_str()<<std::endl;

      os<<"first_near_long:"<<data._first_near_long.c_str()<<std::endl;

      os<<"first_near_long_units:"<<data._first_near_long_units.c_str()<<std::endl;

      os<<"first_mid_lat_title:"<<data._first_mid_lat_title.c_str()<<std::endl;

      os<<"first_mid_lat:"<<data._first_mid_lat.c_str()<<std::endl;

      os<<"first_mid_lat_units:"<<data._first_mid_lat_units.c_str()<<std::endl;

      os<<"first_mid_long_title:"<<data._first_mid_long_title.c_str()<<std::endl;

      os<<"first_mid_long:"<<data._first_mid_long.c_str()<<std::endl;

      os<<"first_mid_long_units:"<<data._first_mid_long_units.c_str()<<std::endl;

      os<<"first_far_lat_title:"<<data._first_far_lat_title.c_str()<<std::endl;

      os<<"first_far_lat:"<<data._first_far_lat.c_str()<<std::endl;

      os<<"first_far_lat_units:"<<data._first_far_lat_units.c_str()<<std::endl;

      os<<"first_far_long_title:"<<data._first_far_long_title.c_str()<<std::endl;

      os<<"first_far_long:"<<data._first_far_long.c_str()<<std::endl;

      os<<"first_far_long_units:"<<data._first_far_long_units.c_str()<<std::endl;

      os<<"last_near_lat_title:"<<data._last_near_lat_title.c_str()<<std::endl;

      os<<"last_near_lat:"<<data._last_near_lat.c_str()<<std::endl;

      os<<"last_near_lat_units:"<<data._last_near_lat_units.c_str()<<std::endl;

      os<<"last_near_long_title:"<<data._last_near_long_title.c_str()<<std::endl;

      os<<"last_near_long:"<<data._last_near_long.c_str()<<std::endl;

      os<<"last_near_long_units:"<<data._last_near_long_units.c_str()<<std::endl;

      os<<"last_mid_lat_title:"<<data._last_mid_lat_title.c_str()<<std::endl;

      os<<"last_mid_lat:"<<data._last_mid_lat.c_str()<<std::endl;

      os<<"last_mid_lat_units:"<<data._last_mid_lat_units.c_str()<<std::endl;

      os<<"last_mid_long_title:"<<data._last_mid_long_title.c_str()<<std::endl;

      os<<"last_mid_long:"<<data._last_mid_long.c_str()<<std::endl;

      os<<"last_mid_long_units:"<<data._last_mid_long_units.c_str()<<std::endl;

      os<<"last_far_lat_title:"<<data._last_far_lat_title.c_str()<<std::endl;

      os<<"last_far_lat:"<<data._last_far_lat.c_str()<<std::endl;

      os<<"last_far_lat_units:"<<data._last_far_lat_units.c_str()<<std::endl;

      os<<"last_far_long_title:"<<data._last_far_long_title.c_str()<<std::endl;

      os<<"last_far_long:"<<data._last_far_long.c_str()<<std::endl;

      os<<"last_far_long_units:"<<data._last_far_long_units.c_str()<<std::endl;

      os<<"swath_title:"<<data._swath_title.c_str()<<std::endl;

      os<<"swath:"<<data._swath.c_str()<<std::endl;

      os<<"pass_title:"<<data._pass_title.c_str()<<std::endl;

      os<<"pass:"<<data._pass.c_str()<<std::endl;

      os<<"sample_type_title:"<<data._sample_type_title.c_str()<<std::endl;

      os<<"sample_type:"<<data._sample_type.c_str()<<std::endl;

      os<<"algo_title:"<<data._algo_title.c_str()<<std::endl;

      os<<"algorithm:"<<data._algorithm.c_str()<<std::endl;

      os<<"mds1_tx_rx_polar_title:"<<data._mds1_tx_rx_polar_title.c_str()<<std::endl;

      os<<"mds1_tx_rx_polar:"<<data._mds1_tx_rx_polar.c_str()<<std::endl;

      os<<"mds2_tx_rx_polar_title:"<<data._mds2_tx_rx_polar_title.c_str()<<std::endl;

      os<<"mds2_tx_rx_polar:"<<data._mds2_tx_rx_polar.c_str()<<std::endl;

      os<<"compression_title:"<<data._compression_title.c_str()<<std::endl;

      os<<"compression:"<<data._compression.c_str()<<std::endl;

      os<<"azimut_looks_title:"<<data._azimut_looks_title.c_str()<<std::endl;

      os<<"azimut_looks:"<<data._azimut_looks.c_str()<<std::endl;

      os<<"range_looks_title:"<<data._range_looks_title.c_str()<<std::endl;

      os<<"range_looks:"<<data._range_looks.c_str()<<std::endl;

      os<<"range_spacing_title:"<<data._range_spacing_title.c_str()<<std::endl;

      os<<"range_spacing:"<<data._range_spacing.c_str()<<std::endl;

      os<<"range_spacing_unit:"<<data._range_spacing_unit.c_str()<<std::endl;

      os<<"azimut_spacing_title:"<<data._azimut_spacing_title.c_str()<<std::endl;

      os<<"azimut_spacing:"<<data._azimut_spacing.c_str()<<std::endl;

      os<<"azimut_spacing_unit:"<<data._azimut_spacing_unit.c_str()<<std::endl;

      os<<"line_time_interval_title:"<<data._line_time_interval_title.c_str()<<std::endl;

      os<<"line_time_interval:"<<data._line_time_interval.c_str()<<std::endl;

      os<<"line_time_interv_units:"<<data._line_time_interv_units.c_str()<<std::endl;

      os<<"line_length_title:"<<data._line_length_title.c_str()<<std::endl;

      os<<"line_length:"<<data._line_length.c_str()<<std::endl;

      os<<"line_length_units:"<<data._line_length_units.c_str()<<std::endl;

      os<<"data_type_title:"<<data._data_type_title.c_str()<<std::endl;

      os<<"data_type:"<<data._data_type.c_str()<<std::endl;

      return os;

   }

   std::istream& operator>>(std::istream& is, sph& data)
   {
      char buf64[65];
      buf64[64] = '\0';
      char buf32[33];
      buf32[32] = '\0';
      char buf27[28];
      buf27[27] = '\0';
      char buf16[17];
      buf16[16]='\0';
      char buf15[16];
      buf15[15] = '\0';
      char buf14[15];
      buf14[14] = '\0';
      char buf13[14];
      buf13[13] = '\0';
      char buf12[13];
      buf12[12] = '\0';
      char buf11[12];
      buf11[11] = '\0';
      char buf10[11];
      buf10[10] = '\0';
      char buf9[10];
      buf9[9] = '\0';
      char buf8[9];
      buf8[8] = '\0';
      char buf7[8];
      buf7[7] = '\0';
      char buf6[7];
      buf6[6] = '\0';
      char buf5[6];
      buf5[5] = '\0';
      char buf4[5];
      buf4[4] = '\0';
      char buf3[4];
      buf3[3] = '\0';
      // char buf2[3];
      // buf2[2] = '\0';
      char buf1[1];


      is.read(buf15,15);
      data._sph_desc_title = buf15;

      is.read(buf1,1);

      is.read(buf32,28);
      buf32[28] = '\0';
      data._sph_desc = buf32;

      is.read(buf1,1);

      is.read(buf1,1);

      is.read(buf32,31);
      buf32[31] = '\0';
      data._strip_cont_ind_title = buf32;

      is.read(buf4,4);
      data._strip_cont_ind =  buf4;

      is.read(buf1,1);

      is.read(buf15,15);
      data._slice_pos_title = buf15;

      is.read(buf4,4);
      data._slice_position = buf4;

      is.read(buf1,1);

      is.read(buf11,11);
      data._num_slices_title = buf11;

      is.read(buf4,4);
      data._num_slices = buf4;

      is.read(buf1,1);

      is.read(buf16,16);
      data._first_line_time_title = buf16;

      is.read(buf1,1);

      is.read(buf27,27);
      data._first_line_time = buf27;

      is.read(buf1,1);

      is.read(buf1,1);

      is.read(buf15,15);
      data._last_line_time_title = buf15;

      is.read(buf1,1);

      is.read(buf27,27);
      data._last_line_time = buf27;

      is.read(buf1,1);

      is.read(buf1,1);

      is.read(buf15,15);
      data._first_near_lat_title = buf15;

      is.read(buf11,11);
      data._first_near_lat = buf11;

      is.read(buf10,10);
      data._first_near_lat_units = buf10;

      is.read(buf1,1);

      is.read(buf16,16);
      data._first_near_long_title = buf16;

      is.read(buf11,11);
      data._first_near_long = buf11;

      is.read(buf10,10);
      data._first_near_long_units = buf10;

      is.read(buf1,1);

      is.read(buf14,14);
      data._first_mid_lat_title = buf14;

      is.read(buf11,11);
      data._first_mid_lat = buf11;

      is.read(buf10,10);
      data._first_mid_lat_units = buf10;

      is.read(buf1,1);

      is.read(buf15,15);
      data._first_mid_long_title = buf15;

      is.read(buf11,11);
      data._first_mid_long = buf11;

      is.read(buf10,10);
      data._first_mid_long_units = buf10;

      is.read(buf1,1);

      is.read(buf14,14);
      data._first_far_lat_title = buf14;

      is.read(buf11,11);
      data._first_far_lat = buf11;

      is.read(buf10,10);
      data._first_far_lat_units = buf10;

      is.read(buf1,1);

      is.read(buf15,15);
      data._first_far_long_title = buf15;

      is.read(buf11,11);
      data._first_far_long = buf11;

      is.read(buf10,10);
      data._first_far_long_units = buf10;

      is.read(buf1,1);

      is.read(buf14,14);
      data._last_near_lat_title = buf14;

      is.read(buf11,11);
      data._last_near_lat = buf11;

      is.read(buf10,10);
      data._last_near_lat_units = buf10;

      is.read(buf1,1);

      is.read(buf15,15);
      data._last_near_long_title = buf15;

      is.read(buf11,11);
      data._last_near_long = buf11;

      is.read(buf10,10);
      data._last_near_long_units = buf10;

      is.read(buf1,1);

      is.read(buf13,13);
      data._last_mid_lat_title = buf13;

      is.read(buf11,11);
      data._last_mid_lat = buf11;

      is.read(buf10,10);
      data._last_mid_lat_units = buf10;

      is.read(buf1,1);

      is.read(buf14,14);
      data._last_mid_long_title = buf14;

      is.read(buf11,11);
      data._last_mid_long = buf11;

      is.read(buf10,10);
      data._last_mid_long_units = buf10;

      is.read(buf1,1);

      is.read(buf13,13);
      data._last_far_lat_title = buf13;

      is.read(buf11,11);
      data._last_far_lat = buf11;

      is.read(buf10,10);
      data._last_far_lat_units = buf10;

      is.read(buf1,1);

      is.read(buf14,14);
      data._last_far_long_title = buf14;

      is.read(buf11,11);
      data._last_far_long = buf11;

      is.read(buf10,10);
      data._last_far_long_units = buf10;

      is.read(buf1,1);

      is.read(buf64,35);

      is.read(buf1,1);

      is.read(buf6,6);
      data._swath_title = buf6;

      is.read(buf1,1);

      is.read(buf3,3);
      data._swath = buf3;

      is.read(buf1,1);

      is.read(buf1,1);

      is.read(buf5,5);
      data._pass_title = buf5;

      is.read(buf1,1);

      is.read(buf10,10);
      data._pass = buf10;

      is.read(buf1,1);

      is.read(buf1,1);

      is.read(buf12,12);
      data._sample_type_title = buf12;

      is.read(buf1,1);

      is.read(buf8,8);
      data._sample_type = buf8;

      is.read(buf1,1);

      is.read(buf1,1);

      is.read(buf10,10);
      data._algo_title = buf10;

      is.read(buf1,1);

      is.read(buf7,7);
      data._algorithm = buf7;

      is.read(buf1,1);

      is.read(buf1,1);

      is.read(buf32,17);
      buf32[17] = '\0';
      data._mds1_tx_rx_polar_title = buf32;

      is.read(buf1,1);

      is.read(buf3,3);
      data._mds1_tx_rx_polar = buf3;

      is.read(buf1,1);

      is.read(buf1,1);

      is.read(buf32,17);
      buf32[17] = '\0';
      data._mds2_tx_rx_polar_title = buf32;

      is.read(buf1,1);

      is.read(buf3,3);
      data._mds2_tx_rx_polar = buf3;

      is.read(buf1,1);

      is.read(buf1,1);

      is.read(buf12,12);
      data._compression_title = buf12;

      is.read(buf1,1);

      is.read(buf5,5);
      data._compression = buf5;

      is.read(buf1,1);

      is.read(buf1,1);

      is.read(buf14,14);
      data._azimut_looks_title = buf14;

      is.read(buf4,4);
      data._azimut_looks = buf4;

      is.read(buf1,1);

      is.read(buf12,12);
      data._range_looks_title = buf12;

      is.read(buf4,4);
      data._range_looks = buf4;

      is.read(buf1,1);

      is.read(buf14,14);
      data._range_spacing_title = buf14;

      is.read(buf15,15);
      data._range_spacing = buf15;

      is.read(buf3,3);
      data._range_spacing_unit = buf3;

      is.read(buf1,1);

      is.read(buf16,16);
      data._azimut_spacing_title = buf16;

      is.read(buf15,15);
      data._azimut_spacing = buf15;

      is.read(buf3,3);
      data._azimut_spacing_unit = buf3;

      is.read(buf1,1);

      is.read(buf32,19);
      buf32[19] = '\0';
      data._line_time_interval_title = buf32;

      is.read(buf15,15);
      data._line_time_interval = buf15;

      is.read(buf3,3);
      data._line_time_interv_units = buf3;

      is.read(buf1,1);

      is.read(buf12,12);
      data._line_length_title = buf12;

      is.read(buf6,6);
      data._line_length = buf6;

      is.read(buf9,9);
      data._line_length_units = buf9;

      is.read(buf1,1);

      is.read(buf10,10);
      data._data_type_title = buf10;

      is.read(buf1,1);

      is.read(buf5,5);
      data._data_type = buf5;

      is.read(buf1,1);

      is.read(buf1,1);

      is.read(buf64,50);

      is.read(buf1,1);

      for (int i = 0; i<data._num_dsd; i++)
      {
         dsd dsd_rec;
         is>>dsd_rec;
         data._dsd_vector.push_back(dsd_rec);
      }

      //char *buffer = (char* )malloc(data._num_dsd*data._size_dsd);
      //is.read(buffer, data._num_dsd*data._size_dsd);

      return is;
   }


   sph::sph(const sph& rhs):
      EnvisatAsarRecord(rhs),
      _sph_desc_title(rhs._sph_desc_title),
      _sph_desc(rhs._sph_desc),
      _strip_cont_ind_title(rhs._strip_cont_ind_title),
      _strip_cont_ind(rhs._strip_cont_ind),
      _slice_pos_title(rhs._slice_pos_title),
      _slice_position(rhs._slice_position),
      _num_slices_title(rhs._num_slices_title),
      _num_slices(rhs._num_slices),
      _first_line_time_title(rhs._first_line_time_title),
      _first_line_time(rhs._first_line_time),
      _last_line_time_title(rhs._last_line_time_title),
      _last_line_time(rhs._last_line_time),
      _first_near_lat_title(rhs._first_near_lat_title),
      _first_near_lat(rhs._first_near_lat),
      _first_near_lat_units(rhs._first_near_lat_units),
      _first_near_long_title(rhs._first_near_long_title),
      _first_near_long(rhs._first_near_long),
      _first_near_long_units(rhs._first_near_long_units),
      _first_mid_lat_title(rhs._first_mid_lat_title),
      _first_mid_lat(rhs._first_mid_lat),
      _first_mid_lat_units(rhs._first_mid_lat_units),
      _first_mid_long_title(rhs._first_mid_long_title),
      _first_mid_long(rhs._first_mid_long),
      _first_mid_long_units(rhs._first_mid_long_units),
      _first_far_lat_title(rhs._first_far_lat_title),
      _first_far_lat(rhs._first_far_lat),
      _first_far_lat_units(rhs._first_far_lat_units),
      _first_far_long_title(rhs._first_far_long_title),
      _first_far_long(rhs._first_far_long),
      _first_far_long_units(rhs._first_far_long_units),
      _last_near_lat_title(rhs._last_near_lat_title),
      _last_near_lat(rhs._last_near_lat),
      _last_near_lat_units(rhs._last_near_lat_units),
      _last_near_long_title(rhs._last_near_long_title),
      _last_near_long(rhs._last_near_long),
      _last_near_long_units(rhs._last_near_long_units),
      _last_mid_lat_title(rhs._last_mid_lat_title),
      _last_mid_lat(rhs._last_mid_lat),
      _last_mid_lat_units(rhs._last_mid_lat_units),
      _last_mid_long_title(rhs._last_mid_long_title),
      _last_mid_long(rhs._last_mid_long),
      _last_mid_long_units(rhs._last_mid_long_units),
      _last_far_lat_title(rhs._last_far_lat_title),
      _last_far_lat(rhs._last_far_lat),
      _last_far_lat_units(rhs._last_far_lat_units),
      _last_far_long_title(rhs._last_far_long_title),
      _last_far_long(rhs._last_far_long),
      _last_far_long_units(rhs._last_far_long_units),
      _swath_title(rhs._swath_title),
      _swath(rhs._swath),
      _pass_title(rhs._pass_title),
      _pass(rhs._pass),
      _sample_type_title(rhs._sample_type_title),
      _sample_type(rhs._sample_type),
      _algo_title(rhs._algo_title),
      _algorithm(rhs._algorithm),
      _mds1_tx_rx_polar_title(rhs._mds1_tx_rx_polar_title),
      _mds1_tx_rx_polar(rhs._mds1_tx_rx_polar),
      _mds2_tx_rx_polar_title(rhs._mds2_tx_rx_polar_title),
      _mds2_tx_rx_polar(rhs._mds2_tx_rx_polar),
      _compression_title(rhs._compression_title),
      _compression(rhs._compression),
      _azimut_looks_title(rhs._azimut_looks_title),
      _azimut_looks(rhs._azimut_looks),
      _range_looks_title(rhs._range_looks_title),
      _range_looks(rhs._range_looks),
      _range_spacing_title(rhs._range_spacing_title),
      _range_spacing(rhs._range_spacing),
      _range_spacing_unit(rhs._range_spacing_unit),
      _azimut_spacing_title(rhs._azimut_spacing_title),
      _azimut_spacing(rhs._azimut_spacing),
      _azimut_spacing_unit(rhs._azimut_spacing_unit),
      _line_time_interval_title(rhs._line_time_interval_title),
      _line_time_interval(rhs._line_time_interval),
      _line_time_interv_units(rhs._line_time_interv_units),
      _line_length_title(rhs._line_length_title),
      _line_length(rhs._line_length),
      _line_length_units(rhs._line_length_units),
      _data_type_title(rhs._data_type_title),
      _data_type(rhs._data_type)
   {}


   sph& sph::operator=(const sph& rhs)
   {
      _sph_desc_title = rhs._sph_desc_title;
      _sph_desc = rhs._sph_desc;
      _strip_cont_ind_title = rhs._strip_cont_ind_title;
      _strip_cont_ind	= rhs._strip_cont_ind;
      _slice_pos_title = rhs._slice_pos_title;
      _slice_position = rhs._slice_position;
      _num_slices_title = rhs._num_slices_title;
      _num_slices = rhs._num_slices;
      _first_line_time_title = rhs._first_line_time_title;
      _first_line_time = rhs._first_line_time;
      _last_line_time_title = rhs._last_line_time_title;
      _last_line_time = rhs._last_line_time;
      _first_near_lat_title = rhs._first_near_lat_title;
      _first_near_lat = rhs._first_near_lat;
      _first_near_lat_units = rhs._first_near_lat_units;
      _first_near_long_title = rhs._first_near_long_title;
      _first_near_long = rhs._first_near_long;
      _first_near_long_units = rhs._first_near_long_units;
      _first_mid_lat_title = rhs._first_mid_lat_title;
      _first_mid_lat = rhs._first_mid_lat;
      _first_mid_lat_units = rhs._first_mid_lat_units;
      _first_mid_long_title = rhs._first_mid_long_title;
      _first_mid_long = rhs._first_mid_long;
      _first_mid_long_units = rhs._first_mid_long_units;
      _first_far_lat_title = rhs._first_far_lat_title;
      _first_far_lat = rhs._first_far_lat;
      _first_far_lat_units = rhs._first_far_lat_units;
      _first_far_long_title = rhs._first_far_long_title;
      _first_far_long = rhs._first_far_long;
      _first_far_long_units = rhs._first_far_long_units;
      _last_near_lat_title = rhs._last_near_lat_title;
      _last_near_lat = rhs._last_near_lat;
      _last_near_lat_units = rhs._last_near_lat_units;
      _last_near_long_title = rhs._last_near_long_title;
      _last_near_long = rhs._last_near_long;
      _last_near_long_units = rhs._last_near_long_units;
      _last_mid_lat_title = rhs._last_mid_lat_title;
      _last_mid_lat = rhs._last_mid_lat;
      _last_mid_lat_units = rhs._last_mid_lat_units;
      _last_mid_long_title = rhs._last_mid_long_title;
      _last_mid_long = rhs._last_mid_long;
      _last_mid_long_units = rhs._last_mid_long_units;
      _last_far_lat_title = rhs._last_far_lat_title;
      _last_far_lat = rhs._last_far_lat;
      _last_far_lat_units = rhs._last_far_lat_units;
      _last_far_long_title = rhs._last_far_long_title;
      _last_far_long = rhs._last_far_long;
      _last_far_long_units = rhs._last_far_long_units;
      _swath_title = rhs._swath_title;
      _swath = rhs._swath;
      _pass_title = rhs._pass_title;
      _pass = rhs._pass;
      _sample_type_title = rhs._sample_type_title;
      _sample_type = rhs._sample_type;
      _algo_title = rhs._algo_title;
      _algorithm = rhs._algorithm;
      _mds1_tx_rx_polar_title = rhs._mds1_tx_rx_polar_title;
      _mds1_tx_rx_polar = rhs._mds1_tx_rx_polar;
      _mds2_tx_rx_polar_title = rhs._mds2_tx_rx_polar_title;
      _mds2_tx_rx_polar = rhs._mds2_tx_rx_polar;
      _compression_title = rhs._compression_title;
      _compression = rhs._compression;
      _azimut_looks_title = rhs._azimut_looks_title;
      _azimut_looks = rhs._azimut_looks;
      _range_looks_title = rhs._range_looks_title;
      _range_looks = rhs._range_looks;
      _range_spacing_title = rhs._range_spacing_title;
      _range_spacing = rhs._range_spacing;
      _range_spacing_unit = rhs._range_spacing_unit;
      _azimut_spacing_title = rhs._azimut_spacing_title;
      _azimut_spacing = rhs._azimut_spacing;
      _azimut_spacing_unit = rhs._azimut_spacing_unit;
      _line_time_interval_title = rhs._line_time_interval_title;
      _line_time_interval = rhs._line_time_interval;
      _line_time_interv_units = rhs._line_time_interv_units;
      _line_length_title = rhs._line_length_title;
      _line_length = rhs._line_length;
      _line_length_units = rhs._line_length_units;
      _data_type_title = rhs._data_type_title;
      _data_type = rhs._data_type;

      return *this;
   }
}
