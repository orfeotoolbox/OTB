/*
 * Copyright (C) 2005-2019 by Centre National d'Etudes Spatiales (CNES)
 * Copyright (C) 2008-2010 by Centre for Remote Imaging, Sensing and Processing (CRISP)
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


#ifndef AlosPalsarDataFileDescriptor_h
#define AlosPalsarDataFileDescriptor_h


#include<iostream>
#include <cstdlib>
#include <AlosPalsar/AlosPalsarRecordHeader.h>
#include <AlosPalsar/AlosPalsarRecord.h>

namespace ossimplugins
{

/**
 * @ingroup SARLeaderAlosPalsarDataFileDescriptorRecord
 * @brief This class is able to read the SAR leader file descriptor record of the leader file
 */
class AlosPalsarDataFileDescriptor : public AlosPalsarRecord
{
public:
  /**
   * @brief Constructor
   */
  AlosPalsarDataFileDescriptor();
  /**
   * @brief Destructor
   */
  ~AlosPalsarDataFileDescriptor();

  /**
   * @brief This function write the AlosPalsarDataFileDescriptor in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const AlosPalsarDataFileDescriptor& data);

  /**
   * @brief This function read a AlosPalsarDataFileDescriptor from a stream
   */
  friend std::istream& operator>>(std::istream& is, AlosPalsarDataFileDescriptor& data);

  /**
   * @brief Copy constructor
   */
  AlosPalsarDataFileDescriptor(const AlosPalsarDataFileDescriptor& rhs);

  /**
   * @brief Copy operator
   */
  AlosPalsarDataFileDescriptor& operator=(const AlosPalsarDataFileDescriptor& rhs);

  /**
   * @brief This function is able to create a new instance of the class
   */
  AlosPalsarRecord* Instantiate()
  {
    return new AlosPalsarDataFileDescriptor();
  };

  /**
   * @brief This function is able to create a new instance of the class initialised with the data of the calling instance
   */
  AlosPalsarRecord* Clone()
  {
    return new AlosPalsarDataFileDescriptor(*this);
  };

  /**
   * @brief Read the class data from a stream
   */
  void Read(std::istream& is)
  {
    is >> *this;
  };

  /**
   * @brief Write the class to a stream
   */
  void Write(std::ostream& os)
  {
    os << *this;
  };


  int get_num_pix_in_line() const
  {
    return _num_pix_in_line;
  };

  int get_num_lines() const
  {
    return _num_lines;
  };

protected:
  /**
  * @brief num_pix_in_line
  */
  int   _num_pix_in_line;
  /**
  * @brief num_lines
  */
  int   _num_lines;


private:
};
}
#endif
