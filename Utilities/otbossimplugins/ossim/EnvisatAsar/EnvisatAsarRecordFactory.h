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

#ifndef EnvisatAsarRecordFactory_h
#define EnvisatAsarRecordFactory_h

#include <EnvisatAsar/EnvisatAsarRecord.h>
#include <map>
#include <string>

namespace ossimplugins
{

/**
 * @ingroup EnvisatAsar
 * @brief This class is a factory able to construct Record base classes given the id of the record wanted
 */
class EnvisatAsarRecordFactory
{
public:
  /**
   * @brief Constructor
   */
  EnvisatAsarRecordFactory();
  /**
   * @brief Destructor
   */
  ~EnvisatAsarRecordFactory();

  /**
   * @brief Adds a new Record type available in this factory
   * @param record Record to add in the factory
   * @param id Id of the new avalaible Record
   */
  void RegisterRecord(std::string id, EnvisatAsarRecord * record);

  /**
   * @brief Instanciatse a new Record
   * @param id Id of the Record we want to instanciate
   */
  EnvisatAsarRecord* Instanciate(std::string id);

protected:
  /**
   * @brief Contains all the available Records for the factory
   */
  std::map<std::string, EnvisatAsarRecord*> _availableRecords;

private:
};
}
#endif
