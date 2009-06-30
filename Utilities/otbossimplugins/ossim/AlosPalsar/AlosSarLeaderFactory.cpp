#include <AlosSarLeaderFactory.h>

#include <AlosSarDataSetSummary.h>
#include <AlosSarFileDescriptor.h>
//#include <AlosSarMapProjectionData.h>//
#include <AlosSarPlatformPositionData.h>
#include <AlosSarFacilityData.h>

AlosSarLeaderFactory::AlosSarLeaderFactory()
{
	RegisterRecord(17, new AlosSarFacilityData());
	RegisterRecord(3, new AlosSarPlatformPositionData());
//	RegisterRecord(3, new AlosSarMapProjectionData());//
	RegisterRecord(2, new AlosSarDataSetSummary());
	RegisterRecord(1, new AlosSarFileDescriptor());
}

AlosSarLeaderFactory::~AlosSarLeaderFactory()
{

}