#include <ossim/imaging/RadarSat/Data/DataFactory.h>

#include <ossim/imaging/RadarSat/Data/ImageOptionsFileDescriptor.h>
#include <ossim/imaging/RadarSat/Data/ProcessedDataRecord.h>


DataFactory::DataFactory()
{
	RegisterRecord(1, new ImageOptionsFileDescriptor());
	RegisterRecord(2, new ProcessedDataRecord());
}

DataFactory::~DataFactory()
{

}