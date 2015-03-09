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

#include <EnvisatAsar/dsd.h>

namespace ossimplugins
{
   dsd::dsd() : EnvisatAsarRecord("dsd_rec")
   {
   }

   dsd::~dsd()
   {
   }

   std::ostream& operator<<(std::ostream& os, const dsd& data)
   {
      os<<"ds_name:"<<data._ds_name.c_str()<<std::endl;
      os<<"ds_type:"<<data._ds_type.c_str()<<std::endl;
      os<<"filename:"<<data._filename.c_str()<<std::endl;
      os<<"ds_offset:"<<data._ds_offset<<std::endl;
      os<<"ds_size:"<<data._ds_size<<std::endl;
      os<<"num_dsr:"<<data._num_dsr<<std::endl;
      os<<"dsr_size:"<<data._dsr_size<<std::endl;

      return os;
   }

   std::istream& operator>>(std::istream& is, dsd& data)
   {
      char buf64[65];
      buf64[64] = '\0';
      // char buf32[33];
      // buf32[32] = '\0';
      char buf28[29];
      buf28[28] = '\0';
      char buf27[28];
      buf27[27] = '\0';
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
      char buf11[12];
      buf11[11] = '\0';
      char buf10[11];
      buf10[10] = '\0';
      char buf9[10];
      buf9[9] = '\0';
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
      char buf1[1];

      is.read(buf9,9);

      is.read(buf28,28);
      data._ds_name = buf28;

      is.read(buf10,10);

      is.read(buf1,1);
      data._ds_type = buf1;

      is.read(buf11,11);

      is.read(buf64,62);
      buf64[62] = '\0';
      data._filename = buf64;

      is.read(buf12,12);

      is.read(buf27,21);
      buf27[21] = '\0';
      data._ds_offset = atof(buf27);

      is.read(buf16,16);

      is.read(buf27,21);
      buf27[21] = '\0';
      data._ds_size = atof(buf27);

      is.read(buf16,16);

      is.read(buf11,11);
      data._num_dsr = atol(buf11);

      is.read(buf10,10);

      is.read(buf11,11);
      data._dsr_size = atol(buf11);

      is.read(buf64,41);

      return is;

   }
   std::string   _ds_name;
   std::string   _ds_type;
   std::string   _filename;
   double	_ds_offset;
   double	_ds_size;
   int	_num_dsr;
   int	_dsr_size;

   dsd::dsd(const dsd& rhs):
      EnvisatAsarRecord(rhs),
      _ds_name(rhs._ds_name),
      _ds_type(rhs._ds_type),
      _filename(rhs._filename),
      _ds_offset(rhs._ds_offset),
      _ds_size(rhs._ds_size),
      _num_dsr(rhs._num_dsr),
      _dsr_size(rhs._dsr_size)
   {
   }


   dsd& dsd::operator=(const dsd& rhs)
   {
      _ds_name = rhs._ds_name;
      _ds_type = rhs._ds_type;
      _filename = rhs._filename;
      _ds_offset = rhs._ds_offset;
      _ds_size = rhs._ds_size;
      _num_dsr = rhs._num_dsr;
      _dsr_size = rhs._dsr_size;

      return *this;
   }
}
