#include <ossim/imaging/ErsSar/ErsSarData/ErsSarData.h>
#include <ossim/imaging/ErsSar/ErsSarData/ErsSarDataFactory.h>
#include <ossim/imaging/ErsSar/ErsSarRecordHeader.h>

const int ErsSarData::ErsSarImageOptionsFileDescriptorID = 1;

ErsSarData::ErsSarData()
{
}

ErsSarData::~ErsSarData()
{
	ClearRecords();
}

std::ostream& operator<<(std::ostream& os, const ErsSarData& data)
{
	std::map<int, ErsSarRecord*>::const_iterator it = data._records.begin();
	while(it != data._records.end())
	{
		(*it).second->Write(os);
		++it;
	}
	return os;

}

std::istream& operator>>(std::istream& is, ErsSarData& data)
{
	ErsSarDataFactory factory;

	data.ClearRecords();

	ErsSarRecordHeader header;
	bool eof = false;
	//while(!eof)
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
				char* buff = new char[header.get_length()];
				is.read(buff, header.get_length());
				delete buff;
			}
		}
	}
	return is;
}


ErsSarData::ErsSarData(const ErsSarData& rhs)
{
	std::map<int, ErsSarRecord*>::const_iterator it = rhs._records.begin();
	while(it != rhs._records.end())
	{
		_records[(*it).first] = (*it).second->Clone();
		++it;
	}
}

ErsSarData& ErsSarData::operator=(const ErsSarData& rhs)
{
	std::map<int, ErsSarRecord*>::const_iterator it = _records.begin();
	while(it != _records.end())
	{
		delete (*it).second;
		++it;
	}
	_records.clear();

	it = rhs._records.begin();
	while(it != rhs._records.end())
	{
		_records[(*it).first] = (*it).second->Clone();
		++it;
	}

	return *this;
}

void ErsSarData::ClearRecords()
{
	std::map<int, ErsSarRecord*>::const_iterator it = _records.begin();
	while(it != _records.end())
	{
		delete (*it).second;
		++it;
	}
	_records.clear();
}

void ErsSarData::InsertRecord(int id, ErsSarRecord* record)
{
	_records[id] = record;
}

ErsSarImageOptionsFileDescriptor* ErsSarData::get_ErsSarImageOptionsFileDescriptor()
{
	return (ErsSarImageOptionsFileDescriptor*)_records[ErsSarImageOptionsFileDescriptorID];
}