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

}
}
