/*
 * Copyright (C) 2005-2019 by Centre National d'Etudes Spatiales (CNES)
 * Copyright (C) 2008-2010 by Centre for Remote Imaging, Sensing and Processing (CRISP)
 *
 * This file is licensed under MIT license:
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#include <AlosPalsar/AlosPalsarData.h>
#include <AlosPalsar/AlosPalsarRecordHeader.h>

#include <AlosPalsar/AlosPalsarDataFileDescriptor.h>
#include <AlosPalsar/AlosPalsarSignalData.h>

#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>

// Static trace for debugging
static ossimTrace traceDebug("ossimAlosPalsarData:debug");

namespace ossimplugins
{

const int AlosPalsarData::AlosPalsarDataFileDescriptorID = 1;
const int AlosPalsarData::AlosPalsarSignalDataID = 2;

AlosPalsarData::AlosPalsarData()
{

}

AlosPalsarData::~AlosPalsarData()
{
  ClearRecords();
}

std::ostream& operator<<(std::ostream& os, const AlosPalsarData& data)
{
  std::map<int, AlosPalsarRecord*>::const_iterator it = data._records.begin();
  while (it != data._records.end())
  {
    (*it).second->Write(os);
    ++it;
  }
  return os;

}

std::istream& operator>>(std::istream& is, AlosPalsarData& data)
{

  data.ClearRecords();

  AlosPalsarRecordHeader header;

  is >> header;

  AlosPalsarRecord* record = new AlosPalsarDataFileDescriptor;
  if (record != NULL)
  {
    record->Read(is);
    data._records[header.get_rec_seq()] = record;
  }
  else
  {
    char* buff = new char[header.get_length()-12];
    is.read(buff, header.get_length() - 12);
    delete buff;
  }

  std::streampos filePosition;

  filePosition = is.tellg();
  is >> header;

  record = new AlosPalsarSignalData;

  if (record != NULL)
  {
    record->Read(is);
    data._records[header.get_rec_seq()] = record;
//    std::cout << "Record sequence number = " << header.get_rec_seq() << std::endl;
  }
  is.seekg(filePosition); // Rewind file pointer to start of record
  // Then, advance pointer to next record
  is.seekg(static_cast<std::streamoff>(header.get_length()), std::ios::cur);

  return is;
}


AlosPalsarData::AlosPalsarData(const AlosPalsarData& rhs)
{
  std::map<int, AlosPalsarRecord*>::const_iterator it = rhs._records.begin();
  while (it != rhs._records.end())
  {
    _records[(*it).first] = (*it).second->Clone();
    ++it;
  }
}

AlosPalsarData& AlosPalsarData::operator=(const AlosPalsarData& rhs)
{
  ClearRecords();
  std::map<int, AlosPalsarRecord*>::const_iterator it = rhs._records.begin();
  while (it != rhs._records.end())
  {
    _records[(*it).first] = (*it).second->Clone();
    ++it;
  }

  return *this;
}

void AlosPalsarData::ClearRecords()
{
  std::map<int, AlosPalsarRecord*>::const_iterator it = _records.begin();
  while (it != _records.end())
  {
    delete(*it).second;
    ++it;
  }
  _records.clear();
}

bool AlosPalsarData::saveState(ossimKeywordlist& kwl,
                               const char* prefix) const
{

  static const char MODULE[] = "AlosPalsarData::saveState";

  if (traceDebug())
  {
    ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered...\n";
  }

  bool result = true;

  /*
   * Adding metadata necessary to the sensor model in the keywordlist
   */
  const AlosPalsarDataFileDescriptor *datafiledesc = get_AlosPalsarDataFileDescriptor();
  if (datafiledesc != NULL)
  {
    kwl.add(prefix, "num_lines", datafiledesc->get_num_lines(), true);
    kwl.add(prefix, "num_pix_in_line", datafiledesc->get_num_pix_in_line(), true);
  }
  else
  {
    result = false;
  }

  const AlosPalsarSignalData *signalData = get_AlosPalsarSignalData();
  if (datafiledesc != NULL)
  {
    kwl.add(prefix, "pulse_repetition_frequency", signalData->get_pulse_repetition_frequency(), true);
    // slant range to 1st data sample in meters
    kwl.add(prefix, "slant_range_to_1st_data_sample", signalData->get_slant_range_to_1st_data_sample(), true);
  }
  else
  {
    result = false;
  }


  return result;
}


const AlosPalsarDataFileDescriptor * AlosPalsarData::get_AlosPalsarDataFileDescriptor() const
{
  return dynamic_cast<const AlosPalsarDataFileDescriptor*>(_records.find(AlosPalsarDataFileDescriptorID)->second);
}

const AlosPalsarSignalData * AlosPalsarData::get_AlosPalsarSignalData() const
{
  // TODO: Check if _records[AlosPalsarSignalDataID] works
  return dynamic_cast<const AlosPalsarSignalData*>(_records.find(AlosPalsarSignalDataID)->second);
}

}
