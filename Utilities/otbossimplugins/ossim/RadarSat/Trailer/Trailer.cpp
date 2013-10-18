//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$

#include <RadarSat/Trailer/Trailer.h>
#include <RadarSat/Trailer/TrailerFactory.h>
#include <RadarSat/RadarSatRecordHeader.h>

#include <RadarSat/CommonRecord/DataHistogramSignalData.h>
#include <RadarSat/CommonRecord/DataQuality.h>
#include <RadarSat/CommonRecord/DataSetSummary.h>
#include <RadarSat/CommonRecord/FileDescriptor.h>
#include <RadarSat/CommonRecord/ProcessingParameters.h>
#include <RadarSat/CommonRecord/AttitudeData.h>
#include <RadarSat/CommonRecord/RadiometricData.h>
#include <RadarSat/CommonRecord/RadiometricCompensationData.h>

namespace ossimplugins
{

const int Trailer::RadiometricDataID = 8;
const int Trailer::RadiometricCompensationDataID = 9;
const int Trailer::AttitudeDataID = 7;
const int Trailer::ProcessingParametersID = 6;
const int Trailer::DataHistogramProcessedData8ID = 5;
const int Trailer::DataHistogramSignalDataID = 4;
const int Trailer::DataQualityID = 3;
const int Trailer::DataSetSummaryID = 2;
const int Trailer::FileDescriptorID = 1;

Trailer::Trailer()
{
}

Trailer::~Trailer()
{
  ClearRecords();
}

std::ostream& operator<<(std::ostream& os, const Trailer& data)
{
  std::map<int, RadarSatRecord*>::const_iterator it = data._records.begin();
  while(it != data._records.end())
  {
    (*it).second->Write(os);
    ++it;
  }
  return os;

}

std::istream& operator>>(std::istream& is, Trailer& data)
{
  TrailerFactory factory;

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
      RadarSatRecord* record = factory.Instanciate(header.get_rec_seq());
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


Trailer::Trailer(const Trailer& rhs)
{
  std::map<int, RadarSatRecord*>::const_iterator it = rhs._records.begin();
  while(it != rhs._records.end())
  {
    _records[(*it).first] = (*it).second->Clone();
    ++it;
  }
}

Trailer& Trailer::operator=(const Trailer& rhs)
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

void Trailer::ClearRecords()
{
  std::map<int, RadarSatRecord*>::const_iterator it = _records.begin();
  while(it != _records.end())
  {
    delete (*it).second;
    ++it;
  }
  _records.clear();
}

RadiometricData * Trailer::get_RadiometricData()
{
  return (RadiometricData*)_records[RadiometricDataID];
}

RadiometricCompensationData * Trailer::get_RadiometricCompensationData()
{
  return (RadiometricCompensationData*)_records[RadiometricCompensationDataID];
}

AttitudeData * Trailer::get_AttitudeData()
{
  return (AttitudeData*)_records[AttitudeDataID];
}

ProcessingParameters * Trailer::get_ProcessingParameters()
{
  return (ProcessingParameters*)_records[ProcessingParametersID];
}

DataHistogramProcessedData8 * Trailer::get_DataHistogramProcessedData8()
{
  return (DataHistogramProcessedData8*)_records[DataHistogramProcessedData8ID];
}

DataHistogramSignalData * Trailer::get_DataHistogramSignalData()
{
  return (DataHistogramSignalData*)_records[DataHistogramSignalDataID];
}

DataQuality * Trailer::get_DataQuality()
{
  return (DataQuality*)_records[DataQualityID];
}

DataSetSummary * Trailer::get_DataSetSummary()
{
  return (DataSetSummary*)_records[DataSetSummaryID];
}

FileDescriptor * Trailer::get_FileDescriptor()
{
  return (FileDescriptor*)_records[FileDescriptorID];
}
}
