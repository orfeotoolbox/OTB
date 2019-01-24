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


#ifndef dsd_h
#define dsd_h

#include <EnvisatAsar/EnvisatAsarRecord.h>
#include <string>

namespace ossimplugins
{
/**
 * @ingroup dsdRecord
 * @brief This class is able to read the ASAR dsd record
 */
class dsd : public EnvisatAsarRecord
{
public:
  /**
   * @brief Constructor
   */
  dsd();

  /**
   * @brief Destructor
   */
  virtual ~dsd();

  /**
   * @brief This function write the dsd in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const dsd& data);

  /**
   * @brief This function read a dsd from a stream
   */
  friend std::istream& operator>>(std::istream& is, dsd& data);

  /**
   * @brief Copy constructor
   */
  dsd(const dsd& rhs);

  /**
   * @brief Copy operator
   */
  dsd& operator=(const dsd& rhs);

  /**
   * @brief This function is able to create a new instance of the class
   */
  EnvisatAsarRecord* Instantiate()
  {
    return new dsd();
  };

  /**
   * @brief This function is able to create a new instance of the class initialised with the data of the calling instance
   */
  EnvisatAsarRecord* Clone()
  {
    return new dsd(*this);
  };

  /**
   * @brief Read the class data from a stream
   */
  void Read(std::istream& is)
  {
    is>>*this;
  };

  /**
   * @brief Write the class to a stream
   */
  void Write(std::ostream& os)
  {
    os<<*this;
  };


  /**
  * @brief Data set Name
  */
  std::string   get_ds_name()
  {
    return _ds_name;
  }
  /**
  * @brief Data set Type
  */
  std::string   get_ds_type()
  {
    return _ds_type;
  }
  /**
  * @brief Data set Filename
  */
  std::string   get_filename()
  {
    return _filename;
  }
  /**
  * @brief Data set Offset
  */
  double   get_ds_offset()
  {
    return _ds_offset;
  }
  /**
  * @brief Data set Size
  */
  double   get_ds_size()
  {
    return _ds_size;
  }
  /**
  * @brief Number of data set record
  */
  int   get_num_dsr()
  {
    return _num_dsr;
  }
  /**
  * @brief Data set record size
  */
  int   get_dsr_size()
  {
    return _dsr_size;
  }

  /**
  * @brief Data set Name
  */
  std::string   _ds_name;
  /**
  * @brief Data set Type
  */
  std::string   _ds_type;
  /**
  * @brief Data set Filename
  */
  std::string   _filename;
  /**
  * @brief Data set Offset
  */
  double  _ds_offset;
  /**
  * @brief Data set Size
  */
  double  _ds_size;
  /**
  * @brief Number of data set record
  */
  int  _num_dsr;
  /**
  * @brief Data set record size
  */
  int  _dsr_size;

private:

};
}
#endif


