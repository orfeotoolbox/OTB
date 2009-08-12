#include "ers/ErsSar/ErsSarLeader/ErsSarLeaderFactory.h"

#include "ers/ErsSar/ErsSarLeader/ErsSarDataSetSummary.h"
#include "ers/ErsSar/ErsSarLeader/ErsSarFileDescriptor.h"
#include "ers/ErsSar/ErsSarLeader/ErsSarMapProjectionData.h"
#include "ers/ErsSar/ErsSarLeader/ErsSarPlatformPositionData.h"
#include "ers/ErsSar/ErsSarLeader/ErsSarFacilityData.h"

namespace ossimplugins
{

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
}
