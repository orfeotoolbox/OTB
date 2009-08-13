#include <AlosSarRecord.h>
#include <memory.h>

namespace ossimplugins
{

AlosSarRecord::AlosSarRecord(std::string mnemonic):
  _mnemonic(mnemonic)
{
}

AlosSarRecord::~AlosSarRecord()
{
}

AlosSarRecord::AlosSarRecord(const AlosSarRecord& rhs):
  _mnemonic(rhs._mnemonic)
{
}

}
