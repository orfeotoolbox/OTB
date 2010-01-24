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

#ifndef AlosPalsarRecordFactory_h
#define AlosPalsarRecordFactory_h


#include <AlosPalsar/AlosPalsarRecordHeader.h>
#include <AlosPalsar/AlosPalsarRecord.h>
#include <map>

namespace ossimplugins
{

/**
 * @ingroup AlosPalsar
 * @brief This class is a facory able to construct Record base classes given the id of the record wanted
 */
class AlosPalsarRecordFactory
{
public:
  /**
   * @brief Constructor
   */
  AlosPalsarRecordFactory();
  /**
   * @brief Destructor
   */
  ~AlosPalsarRecordFactory();

  /**
   * @brief Add a new Record type available in this factory
   * @param record Record to add in the factory
   * @param id Id of the new avalaible Record
   */
  void RegisterRecord(int id, AlosPalsarRecord * record);

  /**
   * @brief Instanciate a new Record
   * @param id Id of the Record we want to instanciate
   */
  AlosPalsarRecord* Instanciate(int id) ;
protected:

  /**
   * @brief Contain all the available Records for the factory
   */
  std::map<int, AlosPalsarRecord*> _availableRecords;

private:
};
}
#endif
