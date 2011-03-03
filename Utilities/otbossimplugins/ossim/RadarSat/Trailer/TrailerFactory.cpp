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

#include <RadarSat/Trailer/TrailerFactory.h>

#include <RadarSat/CommonRecord/DataHistogramSignalData.h>
#include <RadarSat/CommonRecord/DataQuality.h>
#include <RadarSat/CommonRecord/DataSetSummary.h>
#include <RadarSat/CommonRecord/FileDescriptor.h>
#include <RadarSat/Leader/DataHistogramProcessedData.h>
#include <RadarSat/CommonRecord/ProcessingParameters.h>
#include <RadarSat/CommonRecord/AttitudeData.h>
#include <RadarSat/CommonRecord/RadiometricData.h>
#include <RadarSat/CommonRecord/RadiometricCompensationData.h>

namespace ossimplugins
{
TrailerFactory::TrailerFactory()
{
  RegisterRecord(8, new RadiometricData());
  RegisterRecord(9, new RadiometricCompensationData());
  RegisterRecord(7, new AttitudeData());
  RegisterRecord(6, new ProcessingParameters());
  RegisterRecord(5, new DataHistogramProcessedData8());
  RegisterRecord(4, new DataHistogramSignalData());
  RegisterRecord(3, new DataQuality());
  RegisterRecord(2, new DataSetSummary());
  RegisterRecord(1, new FileDescriptor());
}

TrailerFactory::~TrailerFactory()
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
