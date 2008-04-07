#include <ossim/imaging/ErsSar/ErsSarRecord.h>
#include <memory.h>

ErsSarRecord::ErsSarRecord(std::string mnemonic):
	_mnemonic(mnemonic)
{
}

ErsSarRecord::~ErsSarRecord()
{
}

ErsSarRecord::ErsSarRecord(const ErsSarRecord& rhs):
	_mnemonic(rhs._mnemonic)
{
}