#include <ossim/imaging/ErsSar/ErsSarLeader/ErsSarLeaderFactory.h>

#include <ossim/imaging/ErsSar/ErsSarLeader/ErsSarDataSetSummary.h>
#include <ossim/imaging/ErsSar/ErsSarLeader/ErsSarFileDescriptor.h>
#include <ossim/imaging/ErsSar/ErsSarLeader/ErsSarMapProjectionData.h>
#include <ossim/imaging/ErsSar/ErsSarLeader/ErsSarPlatformPositionData.h>
#include <ossim/imaging/ErsSar/ErsSarLeader/ErsSarFacilityData.h>

ErsSarLeaderFactory::ErsSarLeaderFactory()
{
	RegisterRecord(5, new ErsSarFacilityData());
	RegisterRecord(4, new ErsSarPlatformPositionData());
	RegisterRecord(3, new ErsSarMapProjectionData());
	RegisterRecord(2, new ErsSarDataSetSummary());
	RegisterRecord(1, new ErsSarFileDescriptor());
}

ErsSarLeaderFactory::~ErsSarLeaderFactory()
{

}