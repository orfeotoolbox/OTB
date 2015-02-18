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

#ifndef AlosPalsarSignalData_h
#define AlosPalsarSignalData_h


#include <iostream>
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
  AlosPalsarRecord* Instanciate()
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
