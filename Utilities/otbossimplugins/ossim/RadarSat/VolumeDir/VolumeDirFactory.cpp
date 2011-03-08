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

#include <RadarSat/VolumeDir/VolumeDirFactory.h>

#include <RadarSat/VolumeDir/FilePointerRecord.h>
#include <RadarSat/VolumeDir/VolumeDescriptorRecord.h>
#include <RadarSat/VolumeDir/TextRecord.h>


namespace ossimplugins
{
VolumeDirFactory::VolumeDirFactory()
{
  RegisterRecord(1, new VolumeDescriptorRecord());
  RegisterRecord(2, new SARLeaderFilePointerRecord());
  RegisterRecord(3, new ImageOptionsFilePointerRecord());
  RegisterRecord(4, new SARTrailerFilePointerRecord());
  RegisterRecord(5, new TextRecord());
}

VolumeDirFactory::~VolumeDirFactory()
{

  std::map<int, RadarSatRecord*>::iterator i = _availableRecords.begin();

  while (i != _availableRecords.end())
  {
     if ( (*i).second )
     {
        delete (*i).second;
        (*i).second = 0;
     }
     ++i;
  }
  _availableRecords.clear();

}
}
