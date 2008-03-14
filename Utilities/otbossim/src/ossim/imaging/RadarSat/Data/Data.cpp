#include <ossim/imaging/RadarSat/Data/Data.h>
#include <ossim/imaging/RadarSat/Data/DataFactory.h>
#include <ossim/imaging/RadarSat/RadarSatRecordHeader.h>

const int Data::ImageOptionsFileDescriptorID = 1;

Data::Data()
{
}

Data::~Data()
{
	ClearRecords();
}

std::ostream& operator<<(std::ostream& os, const Data& data)
{
	std::map<int, RadarSatRecord*>::const_iterator it = data._records.begin();
	while(it != data._records.end())
	{
		(*it).second->Write(os);
		++it;
	}
	return os;

}

std::istream& operator>>(std::istream& is, Data& data)
{
	DataFactory factory;

	data.ClearRecords();

	RadarSatRecordHeader header;
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
			RadarSatRecord* record = factory.Instanciate(header.get_rec_seq());
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


Data::Data(const Data& rhs)
{
	std::map<int, RadarSatRecord*>::const_iterator it = rhs._records.begin();
	while(it != rhs._records.end())
	{
		_records[(*it).first] = (*it).second->Clone();
		++it;
	}
}

Data& Data::operator=(const Data& rhs)
{
	std::map<int, RadarSatRecord*>::const_iterator it = _records.begin();
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

void Data::ClearRecords()
{
	std::map<int, RadarSatRecord*>::const_iterator it = _records.begin();
	while(it != _records.end())
	{
		delete (*it).second;
		++it;
	}
	_records.clear();
}

void Data::InsertRecord(int id, RadarSatRecord* record)
{
	_records[id] = record;
}

ImageOptionsFileDescriptor* Data::get_ImageOptionsFileDescriptor()
{
	return (ImageOptionsFileDescriptor*)_records[ImageOptionsFileDescriptorID];
}