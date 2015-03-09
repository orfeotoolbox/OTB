//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
// "Copyright Centre for Remote Imaging, Sensing and Processing"
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$

#ifndef AlosPalsarRadiometricData_h
#define AlosPalsarRadiometricData_h

#include <iostream>
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
  AlosPalsarRecord* Instanciate()
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
