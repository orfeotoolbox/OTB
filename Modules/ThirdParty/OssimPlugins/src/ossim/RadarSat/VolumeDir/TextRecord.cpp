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
  os<<"ascii_flag:"<<data._ascii_flag <<std::endl;
  os<<"cont_flag:"<<data._cont_flag <<std::endl;
  os<<"product_type:"<<data._product_type <<std::endl;
  os<<"product_create:"<<data._product_create <<std::endl;
  os<<"phyvol_id:"<<data._phyvol_id <<std::endl;
  os<<"scene_id:"<<data._scene_id <<std::endl;
  os<<"scene_loc:"<<data._scene_loc <<std::endl;
  os<<"copyright_info:"<<data._copyright_info <<std::endl;
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
