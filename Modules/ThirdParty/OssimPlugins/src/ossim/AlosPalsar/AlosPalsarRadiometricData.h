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


#ifndef AlosPalsarRadiometricData_h
#define AlosPalsarRadiometricData_h

#include <cstdlib>
#include <AlosPalsar/AlosPalsarRecord.h>
#include <AlosPalsar/AlosPalsarRecordHeader.h>


namespace ossimplugins
{

/**
 * @ingroup RadiometricDataRecord
 * @brief This class is able to read a radiometric data record
 */
class AlosPalsarRadiometricData : public AlosPalsarRecord
{
public:
  /**
   * @brief Constructor
   */
  AlosPalsarRadiometricData();

  /**
   * @brief Destructor
   */
  virtual ~AlosPalsarRadiometricData();

  /**
   * @brief Copy constructor
   */
  AlosPalsarRadiometricData(const AlosPalsarRadiometricData& rhs);

  /**
   * @brief Copy operator
   */
  AlosPalsarRadiometricData& operator=(const AlosPalsarRadiometricData& rhs);
  /**
   * @brief This function writes the Alos Palsar radiometric data in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const AlosPalsarRadiometricData& data);

  /**
   * @brief This function reads Alos Palsar radiometric data from a stream
   */
  friend std::istream& operator>>(std::istream& is, AlosPalsarRadiometricData& data);

  /**
   * @brief This function is able to create a new instance of the class
   */
  AlosPalsarRecord* Instantiate()
  {
    return new AlosPalsarRadiometricData();
  };

  /**
   * @brief This function is able to create a new instance of the class initialised with the data of the calling instance
   */
  AlosPalsarRecord* Clone()
  {
    return new AlosPalsarRadiometricData(*this);
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

  /**
   * @brief Calibration factor
   */
  double   get_calibration_factor() const
  {
    return _calibration_factor;
  };

protected:
  /**
  * @brief Data sampling interval
  */
  double   _calibration_factor;

private:
};
}
#endif
