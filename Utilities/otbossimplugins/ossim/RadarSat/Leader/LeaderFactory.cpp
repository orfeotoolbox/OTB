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

#include <RadarSat/Leader/LeaderFactory.h>

#include <RadarSat/Leader/DataHistogramSignalData.h>
#include <RadarSat/Leader/DataQuality.h>
#include <RadarSat/Leader/DataSetSummary.h>
#include <RadarSat/Leader/FileDescriptor.h>
#include <RadarSat/Leader/DataHistogramProcessedData.h>
#include <RadarSat/Leader/ProcessingParameters.h>
#include <RadarSat/Leader/PlatformPositionData.h>
#include <RadarSat/Leader/AttitudeData.h>
#include <RadarSat/Leader/RadiometricData.h>
#include <RadarSat/Leader/RadiometricCompensationData.h>

namespace ossimplugins
{
LeaderFactory::LeaderFactory()
{
	RegisterRecord(9, new RadiometricData());
	RegisterRecord(10, new RadiometricCompensationData());
	RegisterRecord(8, new AttitudeData());
	RegisterRecord(7, new PlatformPositionData());
	RegisterRecord(6, new ProcessingParameters());
	RegisterRecord(5, new DataHistogramProcessedData());
	RegisterRecord(4, new DataHistogramSignalData());
	RegisterRecord(3, new DataQuality());
	RegisterRecord(2, new DataSetSummary());
	RegisterRecord(1, new FileDescriptor());
}

LeaderFactory::~LeaderFactory()
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
