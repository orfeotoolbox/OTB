#include <ossim/imaging/RadarSat/Leader/LeaderFactory.h>

#include <ossim/imaging/RadarSat/Leader/DataHistogramSignalData.h>
#include <ossim/imaging/RadarSat/Leader/DataQuality.h>
#include <ossim/imaging/RadarSat/Leader/DataSetSummary.h>
#include <ossim/imaging/RadarSat/Leader/FileDescriptor.h>
#include <ossim/imaging/RadarSat/Leader/DataHistogramProcessedData.h>
#include <ossim/imaging/RadarSat/Leader/ProcessingParameters.h>
#include <ossim/imaging/RadarSat/Leader/PlatformPositionData.h>
#include <ossim/imaging/RadarSat/Leader/AttitudeData.h>
#include <ossim/imaging/RadarSat/Leader/RadiometricData.h>
#include <ossim/imaging/RadarSat/Leader/RadiometricCompensationData.h>

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

}