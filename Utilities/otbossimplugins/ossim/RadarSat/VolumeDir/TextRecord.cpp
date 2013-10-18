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

#include "TextRecord.h"

namespace ossimplugins
{
TextRecord::TextRecord() : RadarSatRecord("text_rec")
{
}

TextRecord::~TextRecord()
{
}

std::ostream& operator<<(std::ostream& os, const TextRecord& data)
{
  os<<"ascii_flag:"<<data._ascii_flag.c_str()<<std::endl;
  os<<"cont_flag:"<<data._cont_flag.c_str()<<std::endl;
  os<<"product_type:"<<data._product_type.c_str()<<std::endl;
  os<<"product_create:"<<data._product_create.c_str()<<std::endl;
  os<<"phyvol_id:"<<data._phyvol_id.c_str()<<std::endl;
  os<<"scene_id:"<<data._scene_id.c_str()<<std::endl;
  os<<"scene_loc:"<<data._scene_loc.c_str()<<std::endl;
  os<<"copyright_info:"<<data._copyright_info.c_str()<<std::endl;
  return os;
}

std::istream& operator>>(std::istream& is, TextRecord& data)
{
  char buf[349]; // 361-12
  buf[348] = '\0';

  is.read(buf, 2);
  buf[2] = '\0';
  data._ascii_flag = buf;

  is.read(buf, 2);
  buf[2] = '\0';
  data._cont_flag = buf;

  is.read(buf, 40);
  buf[40] = '\0';
  data._product_type = buf;

  is.read(buf, 60);
  buf[60] = '\0';
  data._product_create = buf;

  is.read(buf, 40);
  buf[40] = '\0';
  data._phyvol_id = buf;

  is.read(buf, 40);
  buf[40] = '\0';
  data._scene_id = buf;

  is.read(buf, 40);
  buf[40] = '\0';
  data._scene_loc = buf;

  is.read(buf, 20);
  buf[20] = '\0';
  data._copyright_info = buf;

  is.read(buf, 104); // spare1

  return is;
}

TextRecord::TextRecord(const TextRecord& rhs):
  RadarSatRecord(rhs),
  _ascii_flag(rhs._ascii_flag),
  _cont_flag(rhs._cont_flag),
  _product_type(rhs._product_type),
  _product_create(rhs._product_create),
  _phyvol_id(rhs._phyvol_id),
  _scene_id(rhs._scene_id),
  _scene_loc(rhs._scene_loc),
  _copyright_info(rhs._copyright_info)
{
}

TextRecord& TextRecord::operator=(const TextRecord& rhs)
{
  _ascii_flag = rhs._ascii_flag;
  _cont_flag = rhs._cont_flag;
  _product_type = rhs._product_type;
  _product_create = rhs._product_create;
  _phyvol_id = rhs._phyvol_id;
  _scene_id = rhs._scene_id;
  _scene_loc = rhs._scene_loc;
  _copyright_info = rhs._copyright_info;
  return *this;
}
}
