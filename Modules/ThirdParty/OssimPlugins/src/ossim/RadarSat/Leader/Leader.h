/*
 * Copyright (C) 2005-2019 by Centre National d'Etudes Spatiales (CNES)
 *
 * This file is licensed under MIT license:
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#ifndef Leader_h
#define Leader_h

#include <RadarSat/CommonRecord/FileDescriptor.h>
#include <RadarSat/CommonRecord/DataSetSummary.h>
#include <RadarSat/CommonRecord/DataQuality.h>
#include <RadarSat/CommonRecord/DataHistogramSignalData.h>
#include "DataHistogramProcessedData.h"
#include <map>

namespace ossimplugins
{
class RadiometricData;
class RadiometricCompensationData;
class AttitudeData;
class PlatformPositionData;
class ProcessingParameters;
class DataHistogramProcessedData;
class DataHistogramSignalData;
class DataQuality;
class DataSetSummary;
class FileDescriptor;

/**
 * @ingroup LeaderFile
 * @brief This class is able to read the leader file of the RadarSat file structure
 * @author Magellium, Pacome Dentraygues
 * @version 1.0
 * @date 23-11-07
 */
class Leader
{
public:
  /**
   * @brief Constructor
   */
  Leader();

  /**
   * @brief Destructor
   */
  ~Leader();

  /**
   * @brief This function writes the Leader in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const Leader& data);

  /**
   * @brief This function reads a Leader from a stream
   */
  friend std::istream& operator>>(std::istream& is, Leader& data);

  /**
   * @brief Copy constructor
   */
  Leader(const Leader& rhs);

  /**
   * @brief Copy operator
   */
  Leader& operator=(const Leader& rhs);

  /**
   * @brief Removes all the previous records from the Leader
   */
  void ClearRecords();

  RadiometricData * get_RadiometricData();
  RadiometricCompensationData * get_RadiometricCompensationData();
  AttitudeData * get_AttitudeData();
  PlatformPositionData * get_PlatformPositionData();
  ProcessingParameters * get_ProcessingParameters();
  DataHistogramProcessedData * get_DataHistogramProcessedData();
  DataHistogramSignalData * get_DataHistogramSignalData();
  DataQuality * get_DataQuality();
  DataSetSummary * get_DataSetSummary();
  FileDescriptor * get_FileDescriptor();
protected:
  std::map<int, RadarSatRecord*> _records;

  static const int RadiometricDataID;
  static const int RadiometricCompensationDataID;
  static const int AttitudeDataID;
  static const int PlatformPositionDataID;
  static const int ProcessingParametersID;
  static const int DataHistogramProcessedDataID;
  static const int DataHistogramSignalDataID;
  static const int DataQualityID;
  static const int DataSetSummaryID;
  static const int FileDescriptorID;
private:

};
}
#endif
