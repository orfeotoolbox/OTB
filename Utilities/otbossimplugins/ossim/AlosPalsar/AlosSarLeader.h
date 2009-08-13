#ifndef AlosSarLeader_h
#define AlosSarLeader_h

#include <iostream>
#include "AlosSarFileDescriptor.h"
#include "AlosSarDataSetSummary.h"
#include "AlosSarMapProjectionData.h"
#include "AlosSarPlatformPositionData.h"
#include "AlosSarFacilityData.h"

#include <map>

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
 * @author Magellium, Pacome Dentraygues
 * @version 1.0
 * @date 23-11-07
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
  AlosSarFacilityData * get_AlosSarFacilityData();
  AlosSarPlatformPositionData * get_AlosSarPlatformPositionData();
//  AlosSarMapProjectionData * get_AlosSarMapProjectionData();
  AlosSarDataSetSummary * get_AlosSarDataSetSummary();
  AlosSarFileDescriptor * get_AlosSarFileDescriptor();

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
