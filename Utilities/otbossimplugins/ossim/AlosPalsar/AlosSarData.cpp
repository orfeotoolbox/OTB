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

#include <AlosPalsar/AlosSarData.h>
#include <AlosPalsar/AlosSarRecordHeader.h>

#include <AlosPalsar/AlosSarDataFileDescriptor.h>
#include <AlosPalsar/AlosSarSignalData.h>

#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>

// Static trace for debugging
static ossimTrace traceDebug("ossimAlosSarData:debug");

namespace ossimplugins
{

const int AlosSarData::AlosSarDataFileDescriptorID = 1;
const int AlosSarData::AlosSarSignalDataID = 2;

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

  is>>header;

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

  std::streampos filePosition;

  filePosition = is.tellg();
  is>>header;

  record = new AlosSarSignalData;

  if (record != NULL)
  {
    record->Read(is);
    data._records[header.get_rec_seq()] = record;
    std::cout << "Record sequence number = " << header.get_rec_seq() << std::endl;
  }
  is.seekg(filePosition); // Rewind file pointer to start of record
  // Then, advance pointer to next record
  is.seekg(static_cast<std::streamoff>(header.get_length()), std::ios::cur);

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

  /*
   * Adding metadata necessary to the sensor model in the keywordlist
   */
  const AlosSarDataFileDescriptor *datafiledesc = get_AlosSarDataFileDescriptor();
  if (datafiledesc != NULL)
  {
    kwl.add(prefix, "num_lines", datafiledesc->get_num_lines(),true);
    kwl.add(prefix, "num_pix_in_line", datafiledesc->get_num_pix_in_line(),true);
    // FIXME debug
    std::cout << std::endl << "num_lines = " << datafiledesc->get_num_lines() << std::endl;
    std::cout << std::endl << "num_pix_in_line = " << datafiledesc->get_num_pix_in_line() << std::endl;
  }
  else
  {
    result = false;
  }

  const AlosSarSignalData *signalData = get_AlosSarSignalData();
  if (datafiledesc != NULL)
  {
    kwl.add(prefix, "pulse_repetition_frequency", signalData->get_pulse_repetition_frequency(),true);
    // slant range to 1st data sample in metres
    kwl.add(prefix, "slant_range_to_1st_data_sample", signalData->get_slant_range_to_1st_data_sample(),true);
    // FIXME debug
    std::cout << std::endl << "pulse_repetition_frequency = " << signalData->get_pulse_repetition_frequency() << std::endl;
    std::cout << std::endl << "slant_range_to_1st_data_sample = " << signalData->get_slant_range_to_1st_data_sample() << std::endl;
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

const AlosSarSignalData * AlosSarData::get_AlosSarSignalData() const
{
  // TODO: Check if _records[AlosSarSignalDataID] works
  return dynamic_cast<const AlosSarSignalData*>(_records.find(AlosSarSignalDataID)->second);
}

}
