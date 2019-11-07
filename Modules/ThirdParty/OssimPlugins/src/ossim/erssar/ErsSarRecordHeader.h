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


#ifndef ErsSarRecordHeader_h
#define ErsSarRecordHeader_h

#include<iostream>
#include<cstring>

namespace ossimplugins
{

/**
 * @ingroup ErsSar
 * @brief This class is able to read a record header
 *
 */
class ErsSarRecordHeader
{
public:
  /**
   * @brief Constructor
   */
  ErsSarRecordHeader();

  /**
   * @brief Destructor
   */
  virtual ~ErsSarRecordHeader();

  /**
   * @brief Copy constructor
   */
  ErsSarRecordHeader(const ErsSarRecordHeader& rhs);

  /**
   * @brief Copy operator
   */
  ErsSarRecordHeader& operator=(const ErsSarRecordHeader& rhs);
  /**
   * @brief This function write the RecordHeader in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const ErsSarRecordHeader& data);

  /**
   * @brief This function read a RecordHeader from a stream
   */
  friend std::istream& operator>>(std::istream& is, ErsSarRecordHeader& data);

  /**
   * @brief Record sequence number
   */
  unsigned int  get_rec_seq()
  {
    return _rec_seq;
  };

  /**
   * @brief First record sub-type code
   */
  unsigned char   get_rec_sub1()
  {
    return _rec_sub1;
  };

  /**
   * @brief Record type code
   */
  unsigned char   get_rec_type()
  {
    return _rec_type;
  };

  /**
   * @brief Second record sub-type code
   */
  unsigned char   get_rec_sub2()
  {
    return _rec_sub2;
  };

  /**
   * @brief Third record sub-type code
   */
  unsigned char   get_rec_sub3()
  {
    return _rec_sub3;
  };

  /**
   * @brief Length of this record (in bytes)
   */
  unsigned int get_length()
  {
    return _length;
  };
protected:

  /**
   * @brief This function switch the LSB value and the MSB value of the parameter
   */
  void SwitchEndian(unsigned int& value);

  /**
   * @brief Record sequence number
   */
  unsigned int  _rec_seq;
  /**
   * @brief First record sub-type code
   */
  unsigned char   _rec_sub1;
  /**
   * @brief Record type code
   */
  unsigned char   _rec_type;
  /**
   * @brief Second record sub-type code
   */
  unsigned char   _rec_sub2;
  /**
   * @brief Third record sub-type code
   */
  unsigned char   _rec_sub3;
  /**
   * @brief Length of this record (in bytes)
   */
  unsigned int  _length;
private:


};
}

#endif
