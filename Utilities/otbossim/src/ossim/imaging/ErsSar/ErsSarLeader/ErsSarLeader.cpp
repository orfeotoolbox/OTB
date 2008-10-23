#include <ossim/imaging/ErsSar/ErsSarLeader/ErsSarLeader.h>
#include <ossim/imaging/ErsSar/ErsSarLeader/ErsSarLeaderFactory.h>
#include <ossim/imaging/ErsSar/ErsSarRecordHeader.h>

#include <ossim/imaging/ErsSar/ErsSarLeader/ErsSarFileDescriptor.h>
#include <ossim/imaging/ErsSar/ErsSarLeader/ErsSarDataSetSummary.h>
#include <ossim/imaging/ErsSar/ErsSarLeader/ErsSarMapProjectionData.h>
#include <ossim/imaging/ErsSar/ErsSarLeader/ErsSarFacilityData.h>

const int ErsSarLeader::ErsSarFacilityDataID = 5;
const int ErsSarLeader::ErsSarPlatformPositionDataID = 4;
const int ErsSarLeader::ErsSarMapProjectionDataID = 3;
const int ErsSarLeader::ErsSarDataSetSummaryID = 2;
const int ErsSarLeader::ErsSarFileDescriptorID = 1;

ErsSarLeader::ErsSarLeader()
{
}

ErsSarLeader::~ErsSarLeader()
{
	ClearRecords();
}

std::ostream& operator<<(std::ostream& os, const ErsSarLeader& data)
{
	std::map<int, ErsSarRecord*>::const_iterator it = data._records.begin();
	while(it != data._records.end())
	{
		(*it).second->Write(os);
		++it;
	}
	return os;

}

std::istream& operator>>(std::istream& is, ErsSarLeader& data)
{
	ErsSarLeaderFactory factory;

	data.ClearRecords();

	ErsSarRecordHeader header;
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
			ErsSarRecord* record = factory.Instanciate(header.get_rec_seq());
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


ErsSarLeader::ErsSarLeader(const ErsSarLeader& rhs)
{
	std::map<int, ErsSarRecord*>::const_iterator it = rhs._records.begin();
	while(it != rhs._records.end())
	{
		_records[(*it).first] = (*it).second->Clone();
		++it;
	}
}

ErsSarLeader& ErsSarLeader::operator=(const ErsSarLeader& rhs)
{
	ClearRecords();
	std::map<int, ErsSarRecord*>::const_iterator it = rhs._records.begin();
	while(it != rhs._records.end())
	{
		_records[(*it).first] = (*it).second->Clone();
		++it;
	}

	return *this;
}

void ErsSarLeader::ClearRecords()
{
	std::map<int, ErsSarRecord*>::const_iterator it = _records.begin();
	while(it != _records.end())
	{
		delete (*it).second;
		++it;
	}
	_records.clear();
}



ErsSarFacilityData * ErsSarLeader::get_ErsSarFacilityData()
{
	return (ErsSarFacilityData*)_records[ErsSarFacilityDataID];
}
ErsSarPlatformPositionData * ErsSarLeader::get_ErsSarPlatformPositionData()
{
	return (ErsSarPlatformPositionData*)_records[ErsSarPlatformPositionDataID];
}
ErsSarMapProjectionData * ErsSarLeader::get_ErsSarMapProjectionData()
{
	return (ErsSarMapProjectionData*)_records[ErsSarMapProjectionDataID];
}

ErsSarDataSetSummary * ErsSarLeader::get_ErsSarDataSetSummary()
{
	return (ErsSarDataSetSummary*)_records[ErsSarDataSetSummaryID];
}

ErsSarFileDescriptor * ErsSarLeader::get_ErsSarFileDescriptor()
{
	return (ErsSarFileDescriptor*)_records[ErsSarFileDescriptorID];
}
