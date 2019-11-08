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


#include <RadarSat/Leader/Leader.h>
#include <RadarSat/Leader/LeaderFactory.h>
#include <RadarSat/RadarSatRecordHeader.h>

#include <RadarSat/Leader/DataHistogramProcessedData.h>
#include <RadarSat/Leader/PlatformPositionData.h>

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

const int Leader::RadiometricDataID = 9;
const int Leader::RadiometricCompensationDataID = 10;
const int Leader::AttitudeDataID = 8;
const int Leader::PlatformPositionDataID = 7;
const int Leader::ProcessingParametersID = 6;
const int Leader::DataHistogramProcessedDataID = 5;
const int Leader::DataHistogramSignalDataID = 4;
const int Leader::DataQualityID = 3;
const int Leader::DataSetSummaryID = 2;
const int Leader::FileDescriptorID = 1;

Leader::Leader()
{
}

Leader::~Leader()
{
	ClearRecords();
}

std::ostream& operator<<(std::ostream& os, const Leader& data)
{
	std::map<int, RadarSatRecord*>::const_iterator it = data._records.begin();
	while(it != data._records.end())
	{
		(*it).second->Write(os);
		++it;
	}
	return os;

}

std::istream& operator>>(std::istream& is, Leader& data)
{
	LeaderFactory factory;

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
		  RadarSatRecord* record;
		  if ( (header.get_rec_seq() == 2) && (header.get_length() == 8960) )
		    {
		    record = factory.Instantiate(header.get_rec_seq() + 5); // case of SCN, SCW
        if (record != NULL)
          {
          record->Read(is);
          data._records[header.get_rec_seq() + 5] = record;
          }
        else
          {
          char* buff = new char[header.get_length()-12];
          is.read(buff, header.get_length()-12);
          delete[] buff;
          }
		    }
		  else
		    {
        record = factory.Instantiate(header.get_rec_seq());
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
	  }
	return is;
}

Leader::Leader(const Leader& rhs)
{
	std::map<int, RadarSatRecord*>::const_iterator it = rhs._records.begin();
	while(it != rhs._records.end())
	{
		_records[(*it).first] = (*it).second->Clone();
		++it;
	}
}

Leader& Leader::operator=(const Leader& rhs)
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

void Leader::ClearRecords()
{
	std::map<int, RadarSatRecord*>::const_iterator it = _records.begin();
	while(it != _records.end())
	{
		delete (*it).second;
		++it;
	}
	_records.clear();
}

RadiometricData * Leader::get_RadiometricData()
{
	return (RadiometricData*)_records[RadiometricDataID];
}

RadiometricCompensationData * Leader::get_RadiometricCompensationData()
{
	return (RadiometricCompensationData*)_records[RadiometricCompensationDataID];
}

AttitudeData * Leader::get_AttitudeData()
{
	return (AttitudeData*)_records[AttitudeDataID];
}

PlatformPositionData * Leader::get_PlatformPositionData()
{
	return (PlatformPositionData*)_records[PlatformPositionDataID];
}

ProcessingParameters * Leader::get_ProcessingParameters()
{
	return (ProcessingParameters*)_records[ProcessingParametersID];
}

DataHistogramProcessedData * Leader::get_DataHistogramProcessedData()
{
	return (DataHistogramProcessedData*)_records[DataHistogramProcessedDataID];
}

DataHistogramSignalData * Leader::get_DataHistogramSignalData()
{
	return (DataHistogramSignalData*)_records[DataHistogramSignalDataID];
}

DataQuality * Leader::get_DataQuality()
{
	return (DataQuality*)_records[DataQualityID];
}

DataSetSummary * Leader::get_DataSetSummary()
{
	return (DataSetSummary*)_records[DataSetSummaryID];
}

FileDescriptor * Leader::get_FileDescriptor()
{
	return (FileDescriptor*)_records[FileDescriptorID];
}
}
