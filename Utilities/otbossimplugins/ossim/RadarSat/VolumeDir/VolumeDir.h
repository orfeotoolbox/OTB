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

#ifndef VolumeDir_h
#define VolumeDir_h

#include <iostream>
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
