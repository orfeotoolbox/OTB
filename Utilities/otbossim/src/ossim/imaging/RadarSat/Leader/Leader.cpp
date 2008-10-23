#include <ossim/imaging/RadarSat/Leader/Leader.h>
#include <ossim/imaging/RadarSat/Leader/LeaderFactory.h>
#include <ossim/imaging/RadarSat/RadarSatRecordHeader.h>

#include <ossim/imaging/RadarSat/Leader/DataHistogramSignalData.h>
#include <ossim/imaging/RadarSat/Leader/DataQuality.h>
#include <ossim/imaging/RadarSat/Leader/DataSetSummary.h>
#include <ossim/imaging/RadarSat/Leader/FileDescriptor.h>
#include <ossim/imaging/RadarSat/Leader/DataHistogramProcessedData.h>
#include <ossim/imaging/RadarSat/Leader/ProcessingParameters.h>
#include <ossim/imaging/RadarSat/Leader/PlatformPositionData.h>
#include <ossim/imaging/RadarSat/Leader/AttitudeData.h>
#include <ossim/imaging/RadarSat/Leader/RadiometricData.h>
#include <ossim/imaging/RadarSat/Leader/RadiometricCompensationData.h>

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
				delete buff;
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
