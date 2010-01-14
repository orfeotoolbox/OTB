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

#ifndef BeamPixelCountRecord_h
#define BeamPixelCountRecord_h

#include <iostream>

#include <RadarSat/RadarSatRecord.h>

namespace ossimplugins
{
/**
 * @ingroup ProcessingParametersRecord
 * @brief This class is able to read a Beam pixel count record
 */
class BeamPixelCountRecord
{
public:
  /**
   * @brief Constructor
   */
  BeamPixelCountRecord();

  /**
   * @brief Destructor
   */
  ~BeamPixelCountRecord();

  /**
   * @brief Copy constructor
   */
  BeamPixelCountRecord(const BeamPixelCountRecord& rhs);

  /**
   * @brief Copy operator
   */
  BeamPixelCountRecord& operator=(const BeamPixelCountRecord& rhs);

  /**
   * @brief This function writes the BeamPixelCountRecord in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const BeamPixelCountRecord& data);

  /**
   * @brief This function reads a BeamPixelCountRecord from a stream
   */
  friend std::istream& operator>>(std::istream& is, BeamPixelCountRecord& data);

  /**
   * @brief Pixel count update date/time
   */
  std::string   get_pix_update()
  {
    return _pix_update;
  };

    /**
   * @brief Count of image pixels in beams
   */
  int*   get_n_pix()
  {
    return _n_pix;
  };
protected:

  /**
   * @brief Pixel count update date/time
   */
  std::string  _pix_update;
    /**
   * @brief Count of image pixels in beams
   */
  int   _n_pix[4];
private:

};
}
#endif
