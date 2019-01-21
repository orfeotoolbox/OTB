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


#include "erssar/ErsSarRecordHeader.h"


namespace ossimplugins
{

ErsSarRecordHeader::ErsSarRecordHeader()
{
}

ErsSarRecordHeader::~ErsSarRecordHeader()
{
}

std::ostream& operator<<(std::ostream& os, const ErsSarRecordHeader& data)
{
  os << "record_sequence_number:" << data._rec_seq << std::endl;
  os << "first_record_sub-type:" << (int)data._rec_sub1 << std::endl;
  os << "record_type_code:" << (int)data._rec_type << std::endl;
  os << "second_record_sub-type:" << (int)data._rec_sub2 << std::endl;
  os << "third_record_sub-type:" << (int)data._rec_sub3 << std::endl;
  os << "length:" << data._length << std::endl;
  return os;
}

std::istream& operator>>(std::istream& is, ErsSarRecordHeader& data)
{
  is.read((char*)&(data._rec_seq), 4);
  data.SwitchEndian(data._rec_seq);
  is.read((char*)&(data._rec_sub1), 1);
  is.read((char*)&(data._rec_type), 1);
  is.read((char*)&(data._rec_sub2), 1);
  is.read((char*)&(data._rec_sub3), 1);
  is.read((char*)&(data._length), 4);
  data.SwitchEndian(data._length);
  return is;
}

ErsSarRecordHeader::ErsSarRecordHeader(const ErsSarRecordHeader& rhs):
    _rec_seq(rhs._rec_seq),
    _rec_sub1(rhs._rec_sub1),
    _rec_type(rhs._rec_type),
    _rec_sub2(rhs._rec_sub2),
    _rec_sub3(rhs._rec_sub3)
{
}

ErsSarRecordHeader& ErsSarRecordHeader::operator=(const ErsSarRecordHeader& rhs)
{
  _rec_seq = rhs._rec_seq;
  _rec_sub1 = rhs._rec_sub1;
  _rec_type = rhs._rec_type;
  _rec_sub2 = rhs._rec_sub2;
  _rec_sub3 = rhs._rec_sub3;
  return *this;
}

void ErsSarRecordHeader::SwitchEndian(unsigned int& value)
{
  char buffer[4];
  char res[4];

  memcpy(buffer, &value, 4);
  res[0] = buffer[3];
  res[1] = buffer[2];
  res[2] = buffer[1];
  res[3] = buffer[0];

  memcpy(&value, res, 4);
}
}
