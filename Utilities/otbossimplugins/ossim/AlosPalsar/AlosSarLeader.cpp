#include <AlosSarLeader.h>
#include <AlosSarLeaderFactory.h>
#include <AlosSarRecordHeader.h>

#include <AlosSarFileDescriptor.h>
#include <AlosSarDataSetSummary.h>
// #include <AlosSarMapProjectionData.h>//
#include <AlosSarFacilityData.h>

namespace ossimplugins
{

const int AlosSarLeader::AlosSarFacilityDataID = 17;
const int AlosSarLeader::AlosSarPlatformPositionDataID = 3;
// const int AlosSarLeader::AlosSarMapProjectionDataID = 3; //
const int AlosSarLeader::AlosSarDataSetSummaryID = 2;
const int AlosSarLeader::AlosSarFileDescriptorID = 1;

AlosSarLeader::AlosSarLeader()
{
}

AlosSarLeader::~AlosSarLeader()
{
  ClearRecords();
}

std::ostream& operator<<(std::ostream& os, const AlosSarLeader& data)
{
  std::map<int, AlosSarRecord*>::const_iterator it = data._records.begin();
  while(it != data._records.end())
  {
    (*it).second->Write(os);
    ++it;
  }
  return os;

}

std::istream& operator>>(std::istream& is, AlosSarLeader& data)
{
  AlosSarLeaderFactory factory;

  data.ClearRecords();

  AlosSarRecordHeader header;
  bool eof = false;
  while(!eof)
  {
    is>>header;
    if(is.eof())
    {
      eof = true;
    }
    else
    {
      AlosSarRecord* record = factory.Instanciate(header.get_rec_seq());
      if (record != NULL)
      {
        record->Read(is);
        data._records[header.get_rec_seq()] = record;
      }
      else
      {
        char* buff = new char[header.get_length()-12];
        is.read(buff, header.get_length()-12);
        delete buff;
      }
    }
  }
  return is;
}


AlosSarLeader::AlosSarLeader(const AlosSarLeader& rhs)
{
  std::map<int, AlosSarRecord*>::const_iterator it = rhs._records.begin();
  while(it != rhs._records.end())
  {
    _records[(*it).first] = (*it).second->Clone();
    ++it;
  }
}

AlosSarLeader& AlosSarLeader::operator=(const AlosSarLeader& rhs)
{
  ClearRecords();
  std::map<int, AlosSarRecord*>::const_iterator it = rhs._records.begin();
  while(it != rhs._records.end())
  {
    _records[(*it).first] = (*it).second->Clone();
    ++it;
  }

  return *this;
}

void AlosSarLeader::ClearRecords()
{
  std::map<int, AlosSarRecord*>::const_iterator it = _records.begin();
  while(it != _records.end())
  {
    delete (*it).second;
    ++it;
  }
  _records.clear();
}



AlosSarFacilityData * AlosSarLeader::get_AlosSarFacilityData()
{
  return (AlosSarFacilityData*)_records[AlosSarFacilityDataID];
}
AlosSarPlatformPositionData * AlosSarLeader::get_AlosSarPlatformPositionData()
{
  return (AlosSarPlatformPositionData*)_records[AlosSarPlatformPositionDataID];
}
/* AlosSarMapProjectionData * AlosSarLeader::get_AlosSarMapProjectionData()
{
  return (AlosSarMapProjectionData*)_records[AlosSarMapProjectionDataID];
}

*/

//no map projection data

AlosSarDataSetSummary * AlosSarLeader::get_AlosSarDataSetSummary()
{
  return (AlosSarDataSetSummary*)_records[AlosSarDataSetSummaryID];
}

AlosSarFileDescriptor * AlosSarLeader::get_AlosSarFileDescriptor()
{
  return (AlosSarFileDescriptor*)_records[AlosSarFileDescriptorID];
}

}
