#include "ers/ErsSar/ErsSarRecord.h"
#include <memory.h>

namespace ossimplugins
{

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
}
