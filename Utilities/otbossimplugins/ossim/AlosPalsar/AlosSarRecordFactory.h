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

#ifndef AlosSarRecordFactory_h
#define AlosSarRecordFactory_h


#include <AlosPalsar/AlosSarRecordHeader.h>
#include <AlosPalsar/AlosSarRecord.h>
#include <map>

namespace ossimplugins
{

/**
 * @ingroup AlosSar
 * @brief This class is a facory able to construct Record base classes given the id of the record wanted
 */
class AlosSarRecordFactory
{
public:
  /**
   * @brief Constructor
   */
  AlosSarRecordFactory();
  /**
   * @brief Destructor
   */
  ~AlosSarRecordFactory();

  /**
   * @brief Add a new Record type available in this factory
   * @param record Record to add in the factory
   * @param id Id of the new avalaible Record
   */
  void RegisterRecord(int id, AlosSarRecord * record);

  /**
   * @brief Instanciate a new Record
   * @param id Id of the Record we want to instanciate
   */
  AlosSarRecord* Instanciate(int id) ;
protected:

  /**
   * @brief Contain all the available Records for the factory
   */
  std::map<int, AlosSarRecord*> _availableRecords;

private:
};
}
#endif
