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

#ifndef AlosPalsarLeader_h
#define AlosPalsarLeader_h

#include <iostream>
#include "AlosPalsar/AlosPalsarFileDescriptor.h"
#include "AlosPalsar/AlosPalsarDataSetSummary.h"
#include "AlosPalsar/AlosPalsarMapProjectionData.h"
#include "AlosPalsar/AlosPalsarPlatformPositionData.h"
#include "AlosPalsar/AlosPalsarRadiometricData.h"
#include "AlosPalsar/AlosPalsarFacilityData.h"
#include <map>

class ossimKeywordlist;

namespace ossimplugins
{

class AlosPalsarPlatformPositionData;
class AlosPalsarMapProjectionData;
class AlosPalsarDataSetSummary;
class AlosPalsarFileDescriptor;
class AlosPalsarFacilityData;
class AlosPalsarRadiometricData;

/**
 * @ingroup AlosPalsarLeaderFile
 * @brief This class is able to read the Leader file of the AlosPalsar file structure
 */
class AlosPalsarLeader
{
public:
  /**
   * @brief Constructor
   */
  AlosPalsarLeader();

  /**
   * @brief Destructor
   */
  virtual ~AlosPalsarLeader();

  /**
   * @brief This function write the AlosPalsarLeader in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const AlosPalsarLeader& data);

  /**
   * @brief This function read a AlosPalsarLeader from a stream
   */
  friend std::istream& operator>>(std::istream& is, AlosPalsarLeader& data);

  /**
   * @brief Copy constructor
   */
  AlosPalsarLeader(const AlosPalsarLeader& rhs);

  /**
   * @brief Copy operator
   */
  AlosPalsarLeader& operator=(const AlosPalsarLeader& rhs);

  /**
   * @brief Remove all the previous records from the AlosPalsarLeader
   */
  void ClearRecords();


  /**
   * @brief Method to save object state to a keyword list.
   * @param kwl Keyword list to save to.
   * @param prefix added to keys when saved.
   * @return true on success, false on error.
   */
  virtual bool saveState(ossimKeywordlist& kwl,
                         const char* prefix = 0) const;


  const AlosPalsarFacilityData * get_AlosPalsarFacilityData() const;
  const AlosPalsarRadiometricData * get_AlosPalsarRadiometricData() const;
  const AlosPalsarPlatformPositionData * get_AlosPalsarPlatformPositionData() const;
//  const AlosPalsarMapProjectionData * get_AlosPalsarMapProjectionData() const;
  const AlosPalsarDataSetSummary * get_AlosPalsarDataSetSummary() const;
  const AlosPalsarFileDescriptor * get_AlosPalsarFileDescriptor() const;

protected:
  typedef std::map<int, AlosPalsarRecord*> RecordType;
  RecordType _records;

  static const int AlosPalsarFacilityDataID;
  static const int AlosPalsarRadiometricDataID;
  static const int AlosPalsarPlatformPositionDataID;
//  static const int AlosPalsarMapProjectionDataID;
  static const int AlosPalsarDataSetSummaryID;
  static const int AlosPalsarFileDescriptorID;
private:

};
}
#endif
