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

#ifndef Data_h
#define Data_h

#include <iostream>
#include <map>
#include <RadarSat/RadarSatRecord.h>

namespace ossimplugins
{
class ImageOptionsFileDescriptor;
class ProcessedDataRecord;

/**
 * @ingroup DataFile
 * @brief This class is able to read the data file of the RadarSat file structure
 * @author Magellium, Pacome Dentraygues
 * @version 1.0
 * @date 23-11-07
 */
class Data
{
public:
  /**
   * @brief Constructor
   */
  Data();

  /**
   * @brief Destructor
   */
  ~Data();

  /**
   * @brief This function writes the Data in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const Data& data);

  /**
   * @brief This function reads a Data from a stream
   */
  friend std::istream& operator>>(std::istream& is, Data& data);

  /**
   * @brief Copy constructor
   */
  Data(const Data& rhs);

  /**
   * @brief Copy operator
   */
  Data& operator=(const Data& rhs);

  /**
   * @brief Removes all the previous records from the Data
   */
  void ClearRecords();

  /**
   * @brief Inserts an existing record in the Data
   */
  void InsertRecord(int id, RadarSatRecord* record);

  ImageOptionsFileDescriptor* get_ImageOptionsFileDescriptor();
  ProcessedDataRecord* get_FirstProcessedDataRecord();
  ProcessedDataRecord* get_LastProcessedDataRecord();
protected:
  std::map<int, RadarSatRecord*> _records;
  static const int ImageOptionsFileDescriptorID;
  static const int FirstProcessedDataRecordID;
  static const int LastProcessedDataRecordID;
private:

};
}
#endif
