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


#ifndef AlosPalsarSignalData_h
#define AlosPalsarSignalData_h


#include <cstdlib>
#include <AlosPalsar/AlosPalsarRecordHeader.h>
#include <AlosPalsar/AlosPalsarRecord.h>

namespace ossimplugins
{

/**
 * @ingroup SARDataAlosPalsarSignalDataRecord
 * @brief This class is able to read the header of the Signal Data Records of the image file
 */
class AlosPalsarSignalData : public AlosPalsarRecord
{
public:
  /**
   * @brief Constructor
   */
  AlosPalsarSignalData();
  /**
   * @brief Destructor
   */
  virtual ~AlosPalsarSignalData();

  /**
   * @brief This function writes AlosPalsarSignalData to a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const AlosPalsarSignalData& data);

  /**
   * @brief This function reads to AlosPalsarSignalData from a stream
   */
  friend std::istream& operator>>(std::istream& is, AlosPalsarSignalData& data);

  /**
   * @brief Copy constructor
   */
  AlosPalsarSignalData(const AlosPalsarSignalData& rhs);

  /**
   * @brief Copy operator
   */
  AlosPalsarSignalData& operator=(const AlosPalsarSignalData& rhs);

  /**
   * @brief This function is able to create a new instance of the class
   */
  AlosPalsarRecord* Instantiate()
  {
    return new AlosPalsarSignalData();
  };

  /**
   * @brief This function is able to create a new instance of the class initialised with the data of the calling instance
   */
  AlosPalsarRecord* Clone()
  {
    return new AlosPalsarSignalData(*this);
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


  int get_pulse_repetition_frequency() const
  {
    return _pulse_repetition_frequency;
  };

  int get_slant_range_to_1st_data_sample() const
  {
    return _slant_range_to_1st_data_sample;
  };


protected:

  /**
  * @brief pulse_repetition_frequency
  */
  int _pulse_repetition_frequency;

  /**
  * @brief slant_range_to_1st_data_sample
  */
  int _slant_range_to_1st_data_sample;

private:
};

}
#endif
