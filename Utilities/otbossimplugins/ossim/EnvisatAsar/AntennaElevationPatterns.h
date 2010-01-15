
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

#ifndef AntennaElevationPatterns_h
#define AntennaElevationPatterns_h

#include <iostream>
#include <EnvisatAsar/EnvisatAsarRecord.h>

namespace ossimplugins
{
/**
 * @ingroup AntennaElevationPatternsRecord
 * @brief This class is able to read the ASAR AntennaElevationPatterns record
 */
class AntennaElevationPatterns : public EnvisatAsarRecord
{
public:
  /**
   * @brief Constructor
   */
  AntennaElevationPatterns();

  /**
   * @brief Destructor
   */
  virtual ~AntennaElevationPatterns();

  /**
   * @brief This function writes the AntennaElevationPatterns in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const AntennaElevationPatterns& data);

  /**
   * @brief This function reads a AntennaElevationPatterns from a stream
   */
  friend std::istream& operator>>(std::istream& is, AntennaElevationPatterns& data);

  /**
   * @brief Copy constructor
   */
  AntennaElevationPatterns(const AntennaElevationPatterns& rhs);

  /**
   * @brief Copy operator
   */
  AntennaElevationPatterns& operator=(const AntennaElevationPatterns& rhs);

  /**
   * @brief This function is able to create a new instance of the class
   */
  EnvisatAsarRecord* Instanciate()
  {
    return new AntennaElevationPatterns();
  };

  /**
   * @brief This function is able to create a new instance of the class initialised with the data of the calling instance
   */
  EnvisatAsarRecord* Clone()
  {
    return new AntennaElevationPatterns(*this);
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
  * @brief zero_doppler_time
  */
  std::string   get_zero_doppler_time()
  {
    return _zero_doppler_time;
  };
  /**
  * @brief attach_flag
  */
  bool   get_attach_flag()
  {
    return _attach_flag;
  };
  /**
  * @brief swath
  */
  std::string    get_swath()
  {
    return _swath;
  };
  /**
  * @brief Elevation Pattern
  */
  float *    get_elevation_pattern()
  {
    return _elevation_pattern;
  };


  /**
  * @brief zero_doppler_time
  */
  std::string   _zero_doppler_time;
  /**
  * @brief attach_flag
  */
  bool   _attach_flag;
  /**
  * @brief swath
  */
  std::string   _swath;
  /**
  * @brief Elevation Pattern
  */
  float  _elevation_pattern[33];


private:

};
}
#endif


