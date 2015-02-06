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

#include "erssar/ErsSarRecordHeader.h"


namespace ossimplugins
{

ErsSarRecordHeader::ErsSarRecordHeader():
  _rec_seq(0),
  _rec_sub1(0),
  _rec_type(0),
  _rec_sub2(0),
  _rec_sub3(0),
  _length(0)
  
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
    _rec_sub3(rhs._rec_sub3),
    _length(rhs._length)
{
}

ErsSarRecordHeader& ErsSarRecordHeader::operator=(const ErsSarRecordHeader& rhs)
{
  _rec_seq = rhs._rec_seq;
  _rec_sub1 = rhs._rec_sub1;
  _rec_type = rhs._rec_type;
  _rec_sub2 = rhs._rec_sub2;
  _rec_sub3 = rhs._rec_sub3;
  _length   = rhs._length;
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
