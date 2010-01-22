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

#ifndef AlosSarSignalData_h
#define AlosSarSignalData_h


#include <iostream>
#include <cstdlib>
#include <AlosPalsar/AlosSarRecordHeader.h>
#include <AlosPalsar/AlosSarRecord.h>

namespace ossimplugins
{

/**
 * @ingroup SARDataAlosSarSignalDataRecord
 * @brief This class is able to read the header of the Signal Data Records of the image file
 */
class AlosSarSignalData : public AlosSarRecord
{
public:
  /**
   * @brief Constructor
   */
  AlosSarSignalData();
  /**
   * @brief Destructor
   */
  ~AlosSarSignalData();

  /**
   * @brief This function writes AlosSarSignalData to a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const AlosSarSignalData& data);

  /**
   * @brief This function reads to AlosSarSignalData from a stream
   */
  friend std::istream& operator>>(std::istream& is, AlosSarSignalData& data);

  /**
   * @brief Copy constructor
   */
  AlosSarSignalData(const AlosSarSignalData& rhs);

  /**
   * @brief Copy operator
   */
  AlosSarSignalData& operator=(const AlosSarSignalData& rhs);

  /**
   * @brief This function is able to create a new instance of the class
   */
  AlosSarRecord* Instanciate()
  {
    return new AlosSarSignalData();
  };

  /**
   * @brief This function is able to create a new instance of the class initialised with the data of the calling instance
   */
  AlosSarRecord* Clone()
  {
    return new AlosSarSignalData(*this);
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
