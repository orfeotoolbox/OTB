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

#ifndef CompensationDataRecord_h
#define CompensationDataRecord_h

#include <iostream>

#include <RadarSat/RadarSatRecord.h>
#include <RadarSat/RadarSatRecordHeader.h>

namespace ossimplugins
{
/**
 * @ingroup RadiometricCompensationDataRecord
 * @brief This class is able to read a Compensation data record
 */
class CompensationDataRecord
{
public:
  /**
   * @brief Constructor
   */
  CompensationDataRecord();

  /**
   * @brief Destructor
   */
  ~CompensationDataRecord();

  /**
   * @brief Copy constructor
   */
  CompensationDataRecord(const CompensationDataRecord& rhs);

  /**
   * @brief Copy operator
   */
  CompensationDataRecord& operator=(const CompensationDataRecord& rhs);
  /**
   * @brief This function writes the CompensationDataRecord in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const CompensationDataRecord& data);

  /**
   * @brief This function reads a CompensationDataRecord from a stream
   */
  friend std::istream& operator>>(std::istream& is, CompensationDataRecord& data);

  /**
   * @brief Compensation data designator
   */
  std::string   get_comp_desig()
  {
    return _comp_desig;
  };
    /**
   * @brief Compensation data descriptor
   */
  std::string   get_comp_descr()
  {
    return _comp_descr;
  };
    /**
   * @brief Number of compensation records
   */
  int   get_n_comp_rec()
  {
    return _n_comp_rec;
  };
    /**
   * @brief Compensation record sequence number
   */
  int   get_comp_seq_no()
  {
    return _comp_seq_no;
  };
    /**
   * @brief Number of beam table entries
   */
  int   get_beam_tab_size()
  {
    return _beam_tab_size;
  };
    /**
   * @brief Elevation gain beam profile
   */
  double*  get_beam_tab()
  {
    return _beam_tab;
  };
    /**
   * @brief Beam type
   */
  std::string   get_beam_type()
  {
    return _beam_type;
  };
    /**
   * @brief Look angle of beam table centre
   */
  double   get_look_angle()
  {
    return _look_angle;
  };
    /**
   * @brief Increment between beam table entries
   */
  double   get_beam_tab_inc()
  {
    return _beam_tab_inc;
  };

protected:
  /**
   * @brief Compensation data designator
   */
  std::string   _comp_desig;
    /**
   * @brief Compensation data descriptor
   */
  std::string   _comp_descr;
    /**
   * @brief Number of compensation records
   */
  int   _n_comp_rec;
    /**
   * @brief Compensation record sequence number
   */
  int   _comp_seq_no;
    /**
   * @brief Number of beam table entries
   */
  int   _beam_tab_size;
    /**
   * @brief Elevation gain beam profile
   */
  double  _beam_tab[256];
    /**
   * @brief Beam type
   */
  std::string   _beam_type;
    /**
   * @brief Look angle of beam table centre
   */
  double   _look_angle;
    /**
   * @brief Increment between beam table entries
   */
  double   _beam_tab_inc;
private:
};
}
#endif
