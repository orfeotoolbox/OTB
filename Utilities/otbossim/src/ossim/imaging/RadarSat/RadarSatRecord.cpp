#include <ossim/imaging/RadarSat/RadarSatRecord.h>
#include <memory.h>

RadarSatRecord::RadarSatRecord(std::string mnemonic):
	_mnemonic(mnemonic)
{
}

RadarSatRecord::~RadarSatRecord()
{
}

RadarSatRecord::RadarSatRecord(const RadarSatRecord& rhs):
	_mnemonic(rhs._mnemonic)
{
}