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


#ifndef FilePointerRecord_h
#define FilePointerRecord_h


#include<iostream>
#include <RadarSat/RadarSatRecordHeader.h>
#include <RadarSat/RadarSatRecord.h>
#include <string>

namespace ossimplugins
{
/**
 * @ingroup SARLeaderFilePointerRecordRecord
 * @brief This class is able to read the SAR leader file descriptor record of the leader file
 */
class FilePointerRecord : public RadarSatRecord
{
public:
  /**
   * @brief Constructor
   */
  FilePointerRecord();
  /**
   * @brief Destructor
   */
  virtual ~FilePointerRecord();

  /**
   * @brief This function writes the FilePointerRecord in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const FilePointerRecord& data);

  /**
   * @brief This function reads a FilePointerRecord from a stream
   */
  friend std::istream& operator>>(std::istream& is, FilePointerRecord& data);

  /**
   * @brief Copy constructor
   */
  FilePointerRecord(const FilePointerRecord& rhs);

  /**
   * @brief Copy operator
   */
  FilePointerRecord& operator=(const FilePointerRecord& rhs);

  /**
   * @brief This function is able to create a new instance of the class
   */
  RadarSatRecord* Instantiate()
  {
    return new FilePointerRecord();
  };

  /**
   * @brief This function is able to create a new instance of the class initialised with the data of the calling instance
   */
  RadarSatRecord* Clone()
  {
    return new FilePointerRecord(*this);
  };

  /**
   * @brief Reads the class data from a stream
   */
  void Read(std::istream& is)
  {
    is>>*this;
  };

  /**
   * @brief Writes the class to a stream
   */
  void Write(std::ostream& os)
  {
    os<<*this;
  };

  /**
   * @brief ASCII flag
   */
  std::string get_ascii_flag()
  {
    return _ascii_flag;
  };

  /**
   * @brief Referenced file number
   */
  int get_file_num()
  {
    return _file_num;
  };

  /**
   * @brief   Referenced file name
   */
  std::string get_file_name()
  {
    return _file_name;
  };

  /**
   * @brief   Referenced file class
   */
  std::string get_file_class()
  {
    return _file_class;
  };

  /**
   * @brief   Referenced file class code
   */
  std::string get_file_code()
  {
    return _file_code;
  };

  /**
   * @brief   Referenced file data type
   */
  std::string get_data_type()
  {
    return _data_type;
  };

  /**
   * @brief   Referenced file data type code
   */
  std::string get_data_code()
  {
    return _data_code;
  };

  /**
   * @brief   Referenced file record count
   */
  int get_nrec()
  {
    return _nrec;
  };

  /**
   * @brief  First record length, bytes
   */
  int get_first_len()
  {
    return _first_len;
  };

  /**
   * @brief   Maximum record length, bytes
   */
  int get_max_len()
  {
    return _max_len;
  };

  /**
   * @brief   Record length type
   */
  std::string get_len_type()
  {
    return _len_type;
  };

  /**
   * @brief   Record length type code
   */
  std::string get_len_code()
  {
    return _len_code;
  };

  /**
   * @brief   First physical volume
   */
  int get_first_phyvol()
  {
    return _first_phyvol;
  };

  /**
   * @brief   Last physical volume
   */
  int get_last_phyvol()
  {
    return _last_phyvol;
  };

  /**
   * @brief   First physical volume record
   */
  int get_first_rec()
  {
    return _first_rec;
  };

  /**
   * @brief   Last physical volume record
   */
  int get_last_rec()
  {
    return _last_rec;
  };

protected:
  /**
   * @brief ASCII flag
   */
  std::string _ascii_flag;

  /**
   * @brief Referenced file number
   */
  int _file_num;

  /**
   * @brief   Referenced file name
   */
  std::string _file_name;

  /**
   * @brief   Referenced file class
   */
  std::string _file_class;

  /**
   * @brief   Referenced file class code
   */
  std::string _file_code;

  /**
   * @brief   Referenced file data type
   */
  std::string _data_type;

  /**
   * @brief   Referenced file data type code
   */
  std::string _data_code;

  /**
   * @brief   Referenced file record count
   */
  int _nrec;

  /**
   * @brief  First record length, bytes
   */
  int _first_len;

  /**
   * @brief   Maximum record length, bytes
   */
  int _max_len;

  /**
   * @brief   Record length type
   */
  std::string _len_type;

  /**
   * @brief   Record length type code
   */
  std::string _len_code;

  /**
   * @brief   First physical volume
   */
  int _first_phyvol;

  /**
   * @brief   Last physical volume
   */
  int _last_phyvol;

  /**
   * @brief   First physical volume record
   */
  int _first_rec;

  /**
   * @brief   Last physical volume record
   */
  int _last_rec;

private:
};
typedef FilePointerRecord SARLeaderFilePointerRecord;
typedef FilePointerRecord ImageOptionsFilePointerRecord;
typedef FilePointerRecord SARTrailerFilePointerRecord;
}
#endif
