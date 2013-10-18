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

#ifndef BeamInformationRecord_h
#define BeamInformationRecord_h

#include <iostream>

#include <RadarSat/RadarSatRecord.h>

namespace ossimplugins
{
/**
 * @ingroup ProcessingParametersRecord
 * @brief This class is able to read a Beam information record
 */
class BeamInformationRecord
{
public:
  /**
   * @brief Constructor
   */
  BeamInformationRecord();

  /**
   * @brief Destructor
   */
  ~BeamInformationRecord();

  /**
   * @brief Copy constructor
   */
  BeamInformationRecord(const BeamInformationRecord& rhs);

  /**
   * @brief Copy operator
   */
  BeamInformationRecord& operator=(const BeamInformationRecord& rhs);

  /**
   * @brief This function writes the BeamInformationRecord in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const BeamInformationRecord& data);

  /**
   * @brief This function reads a BeamInformationRecord from a stream
   */
  friend std::istream& operator>>(std::istream& is, BeamInformationRecord& data);

  /**
   * @brief Beam type
   */
  std::string   get_beam_type()
  {
    return _beam_type;
  };

  /**
   * @brief Elevation beam look angle source
   */
  std::string   get_beam_look_src()
  {
    return _beam_look_src;
  };

    /**
   * @brief Applied elevation beam look angle (deg)
   */
  double   get_beam_look_ang()
  {
    return _beam_look_ang;
  };

  /**
   * @brief Actual PRF (Hz)
   */
  double   get_prf()
  {
    return _prf;
  };
protected:

  /**
   * @brief Beam type
   */
  std::string  _beam_type;

  /**
   * @brief Elevation beam look angle source
   */
  std::string  _beam_look_src;

    /**
   * @brief Applied elevation beam look angle (deg)
   */
  double   _beam_look_ang;

  /**
   * @brief Actual PRF (Hz)
   */
  double   _prf;
private:

};
}
#endif
