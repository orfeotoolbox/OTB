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


#ifndef VolumeDir_h
#define VolumeDir_h

#include <RadarSat/VolumeDir/FilePointerRecord.h>
#include <RadarSat/VolumeDir/VolumeDescriptorRecord.h>
#include <RadarSat/VolumeDir/TextRecord.h>
#include <map>

namespace ossimplugins
{
class FilePointerRecord;
class TextRecord;
class VolumeDescriptorRecord;

/**
 * @ingroup VolumeDirFile
 * @brief This class is able to read the volume directory file of the RadarSat file structure
 * @author CS, Mickaël Savinaud
 * @version 1.0
 * @date 25-02-2011
 */
class VolumeDir
{
public:
  /**
   * @brief Constructor
   */
  VolumeDir();

  /**
   * @brief Destructor
   */
  ~VolumeDir();

  /**
   * @brief This function writes the VolumeDir in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const VolumeDir& data);

  /**
   * @brief This function reads a VolumeDir from a stream
   */
  friend std::istream& operator>>(std::istream& is, VolumeDir& data);

  /**
   * @brief Copy constructor
   */
  VolumeDir(const VolumeDir& rhs);

  /**
   * @brief Copy operator
   */
  VolumeDir& operator=(const VolumeDir& rhs);

  /**
   * @brief Removes all the previous records from the VolumeDir
   */
  void ClearRecords();

  VolumeDescriptorRecord * get_VolumeDescriptorRecord();
  SARLeaderFilePointerRecord * get_SARLeaderFilePointerRecord();
  ImageOptionsFilePointerRecord * get_ImageOptionsFilePointerRecord();
  SARTrailerFilePointerRecord * get_SARTrailerFilePointerRecord();
  TextRecord * get_TextRecord();

protected:
  std::map<int, RadarSatRecord*> _records;

  static const int VolumeDescriptorRecordID;
  static const int SARLeaderFilePointerRecordID;
  static const int ImageOptionsFilePointerRecordID;
  static const int SARTrailerFilePointerRecordID;
  static const int TextRecordID;

private:

};
}
#endif
