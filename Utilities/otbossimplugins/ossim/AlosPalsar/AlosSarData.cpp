//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
// "Copyright Centre for Remote Imaging, Sensing and Processing"
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$

#include <AlosSarData.h>
#include <AlosSarRecordHeader.h>

#include <AlosSarDataFileDescriptor.h>

#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>

// Static trace for debugging
static ossimTrace traceDebug("ossimAlosSarData:debug");

namespace ossimplugins
{

const int AlosSarData::AlosSarDataFileDescriptorID = 1;

AlosSarData::AlosSarData()
{

}

AlosSarData::~AlosSarData()
{
  ClearRecords();
}

std::ostream& operator<<(std::ostream& os, const AlosSarData& data)
{
  std::map<int, AlosSarRecord*>::const_iterator it = data._records.begin();
  while(it != data._records.end())
  {
    (*it).second->Write(os);
    ++it;
  }
  return os;

}

std::istream& operator>>(std::istream& is, AlosSarData& data)
{

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
      AlosSarRecord* record = new AlosSarDataFileDescriptor;
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


AlosSarData::AlosSarData(const AlosSarData& rhs)
{
  std::map<int, AlosSarRecord*>::const_iterator it = rhs._records.begin();
  while(it != rhs._records.end())
  {
    _records[(*it).first] = (*it).second->Clone();
    ++it;
  }
}

AlosSarData& AlosSarData::operator=(const AlosSarData& rhs)
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

void AlosSarData::ClearRecords()
{
  std::map<int, AlosSarRecord*>::const_iterator it = _records.begin();
  while(it != _records.end())
  {
    delete (*it).second;
    ++it;
  }
  _records.clear();
}

bool AlosSarData::saveState(ossimKeywordlist& kwl,
                             const char* prefix) const
{

  static const char MODULE[] = "AlosSarData::saveState";

  if (traceDebug())
  {
    ossimNotify(ossimNotifyLevel_DEBUG)<< MODULE << " entered...\n";
  }

  bool result = true;

  char name[64];

  kwl.add(prefix, ossimKeywordNames::TYPE_KW, "ossimAlosSarModel", true);
  /*
   * Adding metadata necessary to the sensor model in the keywordlist
   */
  const AlosSarDataFileDescriptor *datafiledesc = get_AlosSarDataFileDescriptor();
  if (datafiledesc != NULL)
  {
    kwl.add(prefix, "num_lines", datafiledesc->get_num_lines(),true);
    kwl.add(prefix, "num_pix_in_line", datafiledesc->get_num_pix_in_line(),true);
  }
  else
  {
    result = false;
  }

  return result;
}


const AlosSarDataFileDescriptor * AlosSarData::get_AlosSarDataFileDescriptor() const
{
  return dynamic_cast<const AlosSarDataFileDescriptor*>(_records.find(AlosSarDataFileDescriptorID)->second);
}

}
