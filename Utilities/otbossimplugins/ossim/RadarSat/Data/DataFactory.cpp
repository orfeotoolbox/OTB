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

#include <RadarSat/Data/DataFactory.h>

#include <RadarSat/Data/ImageOptionsFileDescriptor.h>
#include <RadarSat/Data/ProcessedDataRecord.h>

namespace ossimplugins
{

DataFactory::DataFactory()
{
	RegisterRecord(1, new ImageOptionsFileDescriptor());
	RegisterRecord(2, new ProcessedDataRecord());
}

DataFactory::~DataFactory()
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
