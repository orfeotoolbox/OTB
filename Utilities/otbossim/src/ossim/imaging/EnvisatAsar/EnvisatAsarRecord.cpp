#include <ossim/imaging/EnvisatAsar/EnvisatAsarRecord.h>
#include <memory.h>

EnvisatAsarRecord::EnvisatAsarRecord(std::string mnemonic):
	_mnemonic(mnemonic)
{
}

EnvisatAsarRecord::~EnvisatAsarRecord()
{
}

EnvisatAsarRecord::EnvisatAsarRecord(const EnvisatAsarRecord& rhs):
	_mnemonic(rhs._mnemonic)
{
}
