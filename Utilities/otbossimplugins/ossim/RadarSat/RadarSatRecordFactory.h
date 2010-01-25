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

#ifndef RadarSatRecordFactory_h
#define RadarSatRecordFactory_h


#include <RadarSat/RadarSatRecordHeader.h>
#include <RadarSat/RadarSatRecord.h>
#include <map>

namespace ossimplugins
{
/**
 * @ingroup RadarSat
 * @brief This class is a factory able to construct Record base classes given the id of the wanted record
 */
class RadarSatRecordFactory
{
public:
  /**
   * @brief Constructor
   */
  RadarSatRecordFactory();
  /**
   * @brief Destructor
   */
  ~RadarSatRecordFactory();

  /**
   * @brief Add a new Record type available in this factory
   * @param record Record to add in the factory
   * @param id Id of the new available Record
   */
  void RegisterRecord(int id, RadarSatRecord * record);

  /**
   * @brief Instanciates a new Record
   * @param id Id of the Record we want to instanciate
   */
  RadarSatRecord* Instanciate(int id) ;
protected:

  /**
   * @brief Contains all the available Records for the factory
   */
  std::map<int, RadarSatRecord*> _availableRecords;

private:
};
}
#endif
