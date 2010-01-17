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

#ifndef RadarSatRecord_h
#define RadarSatRecord_h


#include <RadarSat/RadarSatRecordHeader.h>
#include <iostream>
#include <cstdlib>

namespace ossimplugins
{
/**
 * @ingroup RadarSat
 * @brief This class is the base class of all the record classes
 */
class RadarSatRecord
{
public:
  /**
   * @brief Constructor
   * @param mnemonic Name of the record
   */
  RadarSatRecord(std::string mnemonic);
  /**
   * @brief Destructor
   */
  virtual ~RadarSatRecord();

  /**
   * @brief Copy constructor
   */
  RadarSatRecord(const RadarSatRecord& rhs);

  /**
   * @brief This function is able to create a new instance of the class
   */
  virtual RadarSatRecord* Instanciate() =0;

  /**
   * @brief This function is able to create a new instance of the class initialised with the data of the calling instance
   */
  virtual RadarSatRecord* Clone()=0;

  /**
   * @brief Reads the class data from a stream
   */
  virtual void Read(std::istream& is) =0;

  /**
   * @brief Writes the class to a stream
   */
  virtual void Write(std::ostream& os)=0;

  std::string get_mnemonic()
  {
    return _mnemonic;
  };

protected:

  std::string _mnemonic;

private:
};
}
#endif
