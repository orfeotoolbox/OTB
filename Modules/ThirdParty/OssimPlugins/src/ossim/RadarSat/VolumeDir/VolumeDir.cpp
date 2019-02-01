/*
 * Copyright (C) 2005-2019 by Centre National d'Etudes Spatiales (CNES)
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


#include <RadarSat/VolumeDir/VolumeDir.h>
#include <RadarSat/VolumeDir/VolumeDirFactory.h>
#include <RadarSat/RadarSatRecordHeader.h>


namespace ossimplugins
{
const int VolumeDir::TextRecordID = 1;
const int VolumeDir::SARLeaderFilePointerRecordID = 2;
const int VolumeDir::ImageOptionsFilePointerRecordID = 3;
const int VolumeDir::SARTrailerFilePointerRecordID = 4;
const int VolumeDir::VolumeDescriptorRecordID = 5;

VolumeDir::VolumeDir()
{
}

VolumeDir::~VolumeDir()
{
  ClearRecords();
}

std::ostream& operator<<(std::ostream& os, const VolumeDir& data)
{
  std::map<int, RadarSatRecord*>::const_iterator it = data._records.begin();
  while(it != data._records.end())
  {
    (*it).second->Write(os);
    ++it;
  }
  return os;

}

std::istream& operator>>(std::istream& is, VolumeDir& data)
{
  VolumeDirFactory factory;

  data.ClearRecords();

  RadarSatRecordHeader header;
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
      RadarSatRecord* record = factory.Instantiate(header.get_rec_seq());
      if (record != NULL)
      {
        record->Read(is);
        data._records[header.get_rec_seq()] = record;
      }
      else
      {
        char* buff = new char[header.get_length()-12];
        is.read(buff, header.get_length()-12);
        delete[] buff;
      }
    }
  }
  return is;
}


VolumeDir::VolumeDir(const VolumeDir& rhs)
{
  std::map<int, RadarSatRecord*>::const_iterator it = rhs._records.begin();
  while(it != rhs._records.end())
  {
    _records[(*it).first] = (*it).second->Clone();
    ++it;
  }
}

VolumeDir& VolumeDir::operator=(const VolumeDir& rhs)
{
  ClearRecords();
  std::map<int, RadarSatRecord*>::const_iterator it = rhs._records.begin();
  while(it != rhs._records.end())
  {
    _records[(*it).first] = (*it).second->Clone();
    ++it;
  }

  return *this;
}

void VolumeDir::ClearRecords()
{
  std::map<int, RadarSatRecord*>::const_iterator it = _records.begin();
  while(it != _records.end())
  {
    delete (*it).second;
    ++it;
  }
  _records.clear();
}

SARLeaderFilePointerRecord * VolumeDir::get_SARLeaderFilePointerRecord()
{
  return (SARLeaderFilePointerRecord*)_records[SARLeaderFilePointerRecordID];
}

ImageOptionsFilePointerRecord * VolumeDir::get_ImageOptionsFilePointerRecord()
{
  return (ImageOptionsFilePointerRecord*)_records[ImageOptionsFilePointerRecordID];
}

SARTrailerFilePointerRecord * VolumeDir::get_SARTrailerFilePointerRecord()
{
  return (SARTrailerFilePointerRecord*)_records[SARTrailerFilePointerRecordID];
}

TextRecord * VolumeDir::get_TextRecord()
{
  return (TextRecord*)_records[TextRecordID];
}

VolumeDescriptorRecord * VolumeDir::get_VolumeDescriptorRecord()
{
  return (VolumeDescriptorRecord*)_records[VolumeDescriptorRecordID];
}
}
