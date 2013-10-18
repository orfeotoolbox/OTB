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

#include "FilePointerRecord.h"

namespace ossimplugins
{
FilePointerRecord::FilePointerRecord() : RadarSatRecord("file_pntr_rec")
{
}

FilePointerRecord::~FilePointerRecord()
{
}

std::ostream& operator<<(std::ostream& os, const FilePointerRecord& data)
{
  os<<"ascii_flag:"<<data._ascii_flag.c_str()<<std::endl;
  os<<"file_num:"<<data._file_num<<std::endl;
  os<<"file_name:"<<data._file_name.c_str()<<std::endl;
  os<<"file_class:"<<data._file_class.c_str()<<std::endl;
  os<<"file_code:"<<data._file_code.c_str()<<std::endl;
  os<<"data_type:"<<data._data_type.c_str()<<std::endl;
  os<<"data_code:"<<data._data_code.c_str()<<std::endl;
  os<<"nrec:"<<data._nrec<<std::endl;
  os<<"first_len:"<<data._first_len<<std::endl;
  os<<"max_len:"<<data._max_len<<std::endl;
  os<<"len_type:"<<data._len_type.c_str()<<std::endl;
  os<<"len_code:"<<data._len_code.c_str()<<std::endl;
  os<<"first_phyvol:"<<data._first_phyvol<<std::endl;
  os<<"last_phyvol:"<<data._last_phyvol<<std::endl;
  os<<"first_rec:"<<data._first_rec<<std::endl;
  os<<"last_rec:"<<data._last_rec<<std::endl;
  return os;
}

std::istream& operator>>(std::istream& is, FilePointerRecord& data)
{
  char buf[349]; // 361-12 (size of header)
  buf[348] = '\0';

  is.read(buf, 2);
  buf[2] = '\0';
  data._ascii_flag = buf;

  is.read(buf, 2); // spare1

  is.read(buf, 4);
  buf[4] = '\0';
  data._file_num = atoi(buf);

  is.read(buf, 16);
  buf[16] = '\0';
  data._file_name = buf;

  is.read(buf, 28);
  buf[28] = '\0';
  data._file_class = buf;

  is.read(buf, 4);
  buf[4] = '\0';
  data._file_code = buf;

  is.read(buf, 28);
  buf[28] = '\0';
  data._data_type = buf;

  is.read(buf, 4);
  buf[4] = '\0';
  data._data_code = buf;

  is.read(buf, 8);
  buf[8] = '\0';
  data._nrec = atoi(buf);

  is.read(buf, 8);
  buf[8] = '\0';
  data._first_len = atoi(buf);

  is.read(buf, 8);
  buf[8] = '\0';
  data._max_len = atoi(buf);

  is.read(buf, 12);
  buf[12] = '\0';
  data._len_type = buf;

  is.read(buf, 4);
  buf[4] = '\0';
  data._len_code = buf;

  is.read(buf, 2);
  buf[2] = '\0';
  data._first_phyvol = atoi(buf);

  is.read(buf, 2);
  buf[2] = '\0';
  data._last_phyvol = atoi(buf);

  is.read(buf, 8);
  buf[8] = '\0';
  data._first_rec = atoi(buf);

  is.read(buf, 8);
  buf[8] = '\0';
  data._last_rec = atoi(buf);

  is.read(buf, 100); // spare2

  is.read(buf, 100); // spare3
  return is;
}

FilePointerRecord::FilePointerRecord(const FilePointerRecord& rhs):
  RadarSatRecord(rhs),
  _ascii_flag(rhs._ascii_flag),
  _file_num(rhs._file_num),
  _file_name(rhs._file_name),
  _file_class(rhs._file_class),
  _file_code(rhs._file_code),
  _data_type(rhs._data_type),
  _data_code(rhs._data_code),
  _nrec(rhs._nrec),
  _first_len(rhs._first_len),
  _max_len(rhs._max_len),
  _len_type(rhs._len_type),
  _len_code(rhs._len_code),
  _first_phyvol(rhs._first_phyvol),
  _last_phyvol(rhs._last_phyvol),
  _first_rec(rhs._first_rec),
  _last_rec(rhs._last_rec)
{
}

FilePointerRecord& FilePointerRecord::operator=(const FilePointerRecord& rhs)
{
  _ascii_flag = rhs._ascii_flag;
  _file_num = rhs._file_num;
  _file_name = rhs._file_name;
  _file_class = rhs._file_class;
  _file_code = rhs._file_code;
  _data_type = rhs._data_type;
  _data_code = rhs._data_code;
  _nrec = rhs._nrec;
  _first_len = rhs._first_len;
  _max_len = rhs._max_len;
  _len_type = rhs._len_type;
  _len_code = rhs._len_code;
  _first_phyvol = rhs._first_phyvol;
  _last_phyvol = rhs._last_phyvol;
  _first_rec = rhs._first_rec;
  _last_rec = rhs._last_rec;
  return *this;
}
}
