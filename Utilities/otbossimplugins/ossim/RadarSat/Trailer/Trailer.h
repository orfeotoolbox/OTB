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

#ifndef Trailer_h
#define Trailer_h

#include <iostream>
#include <RadarSat/CommonRecord/FileDescriptor.h>
#include <RadarSat/CommonRecord/DataSetSummary.h>
#include <RadarSat/CommonRecord/DataQuality.h>
#include <RadarSat/CommonRecord/DataHistogramSignalData.h>
#include <map>

namespace ossimplugins
{
class RadiometricData;
class RadiometricCompensationData;
class AttitudeData;
class ProcessingParameters;
class DataHistogramSignalData;
class DataQuality;
class DataSetSummary;
class FileDescriptor;

/**
 * @ingroup TrailerFile
 * @brief This class is able to read the trailer file of the RadarSat file structure
 * @author CS, Mickaël Savinaud
 * @version 1.0
 * @date 25-02-2011
 */
class Trailer
{
public:
  /**
   * @brief Constructor
   */
  Trailer();

  /**
   * @brief Destructor
   */
  ~Trailer();

  /**
   * @brief This function writes the Trailer in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const Trailer& data);

  /**
   * @brief This function reads a Trailer from a stream
   */
  friend std::istream& operator>>(std::istream& is, Trailer& data);

  /**
   * @brief Copy constructor
   */
  Trailer(const Trailer& rhs);

  /**
   * @brief Copy operator
   */
  Trailer& operator=(const Trailer& rhs);

  /**
   * @brief Removes all the previous records from the Trailer
   */
  void ClearRecords();

  RadiometricData * get_RadiometricData();
  RadiometricCompensationData * get_RadiometricCompensationData();
  AttitudeData * get_AttitudeData();
  ProcessingParameters * get_ProcessingParameters();
  DataHistogramProcessedData8 * get_DataHistogramProcessedData8();
  DataHistogramSignalData * get_DataHistogramSignalData();
  DataQuality * get_DataQuality();
  DataSetSummary * get_DataSetSummary();
  FileDescriptor * get_FileDescriptor();
protected:
  std::map<int, RadarSatRecord*> _records;

  static const int RadiometricDataID;
  static const int RadiometricCompensationDataID;
  static const int AttitudeDataID;
  static const int ProcessingParametersID;
  static const int DataHistogramProcessedData8ID;
  static const int DataHistogramSignalDataID;
  static const int DataQualityID;
  static const int DataSetSummaryID;
  static const int FileDescriptorID;
private:

};
}
#endif
