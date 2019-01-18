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


#ifndef Data_h
#define Data_h

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
