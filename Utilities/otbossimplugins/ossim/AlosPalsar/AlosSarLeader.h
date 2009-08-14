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

#ifndef AlosSarLeader_h
#define AlosSarLeader_h

#include <iostream>
#include "AlosSarFileDescriptor.h"
#include "AlosSarDataSetSummary.h"
#include "AlosSarMapProjectionData.h"
#include "AlosSarPlatformPositionData.h"
#include "AlosSarFacilityData.h"
#include <map>

class ossimKeywordlist;

namespace ossimplugins
{

class AlosSarPlatformPositionData;
class AlosSarMapProjectionData;
class AlosSarDataSetSummary;
class AlosSarFileDescriptor;
class AlosSarFacilityData;

/**
 * @ingroup AlosSarLeaderFile
 * @brief This class is able to read the Leader file of the AlosSar file structure
 */
class AlosSarLeader
{
public:
  /**
   * @brief Constructor
   */
  AlosSarLeader();

  /**
   * @brief Destructor
   */
  ~AlosSarLeader();

  /**
   * @brief This function write the AlosSarLeader in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const AlosSarLeader& data);

  /**
   * @brief This function read a AlosSarLeader from a stream
   */
  friend std::istream& operator>>(std::istream& is, AlosSarLeader& data);

  /**
   * @brief Copy constructor
   */
  AlosSarLeader(const AlosSarLeader& rhs);

  /**
   * @brief Copy operator
   */
  AlosSarLeader& operator=(const AlosSarLeader& rhs);

  /**
   * @brief Remove all the previous records from the AlosSarLeader
   */
  void ClearRecords();


  /**
   * @brief Method to save object state to a keyword list.
   * @param kwl Keyword list to save to.
   * @param prefix added to keys when saved.
   * @return true on success, false on error.
   */
  virtual bool saveState(ossimKeywordlist& kwl,
                         const char* prefix=0) const;


  AlosSarFacilityData * get_AlosSarFacilityData() const;
  AlosSarPlatformPositionData * get_AlosSarPlatformPositionData() const;
//  AlosSarMapProjectionData * get_AlosSarMapProjectionData() const;
  AlosSarDataSetSummary * get_AlosSarDataSetSummary() const;
  AlosSarFileDescriptor * get_AlosSarFileDescriptor() const;

protected:
  std::map<int, AlosSarRecord*> _records;

  static const int AlosSarFacilityDataID;
  static const int AlosSarPlatformPositionDataID;
//  static const int AlosSarMapProjectionDataID;
  static const int AlosSarDataSetSummaryID;
  static const int AlosSarFileDescriptorID;
private:

};
}
#endif
