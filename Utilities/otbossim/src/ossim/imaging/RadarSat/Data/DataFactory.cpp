#include <ossim/imaging/RadarSat/Data/DataFactory.h>

#include <ossim/imaging/RadarSat/Data/ImageOptionsFileDescriptor.h>


DataFactory::DataFactory()
{
	RegisterRecord(1, new ImageOptionsFileDescriptor());
}

DataFactory::~DataFactory()
{

}