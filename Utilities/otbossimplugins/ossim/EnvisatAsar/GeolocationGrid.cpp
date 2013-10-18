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

#include <EnvisatAsar/GeolocationGrid.h>

namespace ossimplugins
{
   GeolocationGrid::GeolocationGrid() : EnvisatAsarRecord("GeolocationGrid_rec")
   {
   }

   GeolocationGrid::~GeolocationGrid()
   {
   }


   std::ostream& operator<<(std::ostream& os, const GeolocationGrid& data)
   {
      os<<"first_zero_doppler_time_day:"<<data._first_zero_doppler_time_day<<std::endl;

      os<<"first_zero_doppler_time_sec:"<<data._first_zero_doppler_time_sec<<std::endl;

      os<<"first_zero_doppler_time_microsec:"<<data._first_zero_doppler_time_microsec<<std::endl;

      os<<"attach_flag:"<<data._attach_flag<<std::endl;

      os<<"line_num:"<<data._line_num<<std::endl;

      os<<"num_lines:"<<data._num_lines<<std::endl;

      os<<"sub_sat_track:"<<data._sub_sat_track<<std::endl;

      for (int i = 0; i<11; i++) {
         os<<"samp_numbers["<<i<<"]:"<<data._samp_numbers[i]<<std::endl;
      }

      for (int i = 0; i<11; i++) {
         os<<"slant_range_times["<<i<<"]:"<<data._slant_range_times[i]<<std::endl;
      }

      for (int i = 0; i<11; i++) {
         os<<"angles["<<i<<"]:"<<data._angles[i]<<std::endl;
      }

      for (int i = 0; i<11; i++) {
         os<<"lats["<<i<<"]:"<<data._lats[i]<<std::endl;
      }

      for (int i = 0; i<11; i++) {
         os<<"longs["<<i<<"]:"<<data._longs[i]<<std::endl;
      }

      os<<"last_zero_doppler_time:"<<data._last_zero_doppler_time.c_str()<<std::endl;

      os<<"last_line_tie_points:"<<data._last_line_tie_points.c_str()<<std::endl;

      return os;

   }

   std::istream& operator>>(std::istream& is, GeolocationGrid& data)
   {
      char buf220[221];
      buf220[220] = '\0';
      char buf32[33];
      buf32[32] = '\0';
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
      // char buf3[4];
      // buf3[3] = '\0';
      // char buf2[3];
      // buf2[2] = '\0';
//	char buf1[1]; // not used avoid warning

      is.read((char*)&(data._first_zero_doppler_time_day),4);
      data.SwitchEndian(data._first_zero_doppler_time_day);

      is.read((char*)&(data._first_zero_doppler_time_sec),4);
      data.SwitchEndian(data._first_zero_doppler_time_sec);

      is.read((char*)&(data._first_zero_doppler_time_microsec),4);
      data.SwitchEndian(data._first_zero_doppler_time_microsec);

      is.read((char*)&(data._attach_flag),1);

      is.read((char*)&(data._line_num),4);
      data.SwitchEndian(data._line_num);

      is.read((char*)&(data._num_lines),4);
      data.SwitchEndian(data._num_lines);

      is.read((char*)&(data._sub_sat_track),4);
      data.SwitchEndian(data._sub_sat_track);

      for (int i = 0; i<11; i++) {
         is.read((char*)&(data._samp_numbers[i]),4);
         data.SwitchEndian(data._samp_numbers[i]);
      }

      for (int i = 0; i<11; i++) {
         is.read((char*)&(data._slant_range_times[i]),4);
         data.SwitchEndian(data._slant_range_times[i]);
      }

      for (int i = 0; i<11; i++) {
         is.read((char*)&(data._angles[i]),4);
         data.SwitchEndian(data._angles[i]);
      }

      for (int i = 0; i<11; i++) {
         is.read((char*)&(data._lats[i]),4);
         data.SwitchEndian(data._lats[i]);
      }

      for (int i = 0; i<11; i++) {
         is.read((char*)&(data._longs[i]),4);
         data.SwitchEndian(data._longs[i]);
      }

      is.read(buf32,22);

      is.read(buf12,12);
      data._last_zero_doppler_time = buf12;

      is.read(buf220,220);
      data._last_line_tie_points = buf220;

      is.read(buf32,22);

      return is;

   }

   GeolocationGrid::GeolocationGrid(const GeolocationGrid& rhs):
      EnvisatAsarRecord(rhs),
      _first_zero_doppler_time_day(rhs._first_zero_doppler_time_day),
      _first_zero_doppler_time_sec(rhs._first_zero_doppler_time_sec),
      _first_zero_doppler_time_microsec(rhs._first_zero_doppler_time_microsec),
      _attach_flag(rhs._attach_flag),
      _line_num(rhs._line_num),
      _num_lines(rhs._num_lines),
      _sub_sat_track(rhs._sub_sat_track),
      _last_zero_doppler_time(rhs._last_zero_doppler_time),
      _last_line_tie_points(rhs._last_line_tie_points)
   {

      for (int i = 0; i<11; i++) {
         _samp_numbers[i] = rhs._samp_numbers[i];
         _slant_range_times[i] = rhs._slant_range_times[i];
         _angles[i] = rhs._angles[i];
         _lats[i] = rhs._lats[i];
         _longs[i] = rhs._longs[i];
      }


   }


   GeolocationGrid& GeolocationGrid::operator=(const GeolocationGrid& rhs)
   {
      _first_zero_doppler_time_day = rhs._first_zero_doppler_time_day;
      _first_zero_doppler_time_sec = rhs._first_zero_doppler_time_sec;
      _first_zero_doppler_time_microsec = rhs._first_zero_doppler_time_microsec;
      _attach_flag = rhs._attach_flag;
      _line_num = rhs._line_num;
      _num_lines = rhs._num_lines;
      _sub_sat_track = rhs._sub_sat_track;
      _last_zero_doppler_time = rhs._last_zero_doppler_time;
      _last_line_tie_points = rhs._last_line_tie_points;

      for (int i = 0; i<11; i++) {
         _samp_numbers[i] = rhs._samp_numbers[i];
         _slant_range_times[i] = rhs._slant_range_times[i];
         _angles[i] = rhs._angles[i];
         _lats[i] = rhs._lats[i];
         _longs[i] = rhs._longs[i];
      }

      return *this;
   }
}
