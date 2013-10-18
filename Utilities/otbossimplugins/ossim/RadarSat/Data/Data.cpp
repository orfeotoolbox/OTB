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

#include <RadarSat/Data/Data.h>
#include <RadarSat/Data/DataFactory.h>
#include <RadarSat/RadarSatRecordHeader.h>
#include <RadarSat/Data/ImageOptionsFileDescriptor.h>

#include <ossim/base/ossimTrace.h>

namespace ossimplugins
{
// Static trace for debugging
static ossimTrace traceDebug("Data:debug");

const int Data::ImageOptionsFileDescriptorID = 1;
const int Data::FirstProcessedDataRecordID = 2;
const int Data::LastProcessedDataRecordID = 3;

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

	if (sizeof(int)!=4) ossimNotify(ossimNotifyLevel_WARN)  << "RadarSat Data WARNING : (int) not coded over 32 bits, metadata might not be byte swapped correctly"<< std::endl ;
	if (sizeof(float)!=4) ossimNotify(ossimNotifyLevel_WARN)  << "RadarSat Data WARNING : (float) not coded over 32 bits, metadata might not be byte swapped correctly"<< std::endl ;
	if (sizeof(double)!=8) ossimNotify(ossimNotifyLevel_WARN)  << "RadarSat Data WARNING : (double) not coded over 64 bits, metadata might not be byte swapped correctly"<< std::endl ;

	RadarSatRecordHeader header;
	bool eof = false;

	int nbLin = 0 ; // number of image lines
	int lineLength = 0 ; // size of any ProcessedDataRecord

  is.seekg(0, std::ios::end) ;
  int lengthOfFile = is.tellg(); // total length of file
  is.seekg(0, std::ios::beg);
	while(!eof)
	  {
	  is>>header;
		if(is.eof())
		  {
		  eof = true;
		  }
		else
		  {
			if (header.get_rec_seq() == 1)
			  { // ImageOptionsFileDescriptor
			  RadarSatRecord* record = factory.Instanciate(header.get_rec_seq());
			  if (record != NULL)
				  {
			    record->Read(is);
					data._records[Data::ImageOptionsFileDescriptorID] = record;

					nbLin  = ((ImageOptionsFileDescriptor *) record)->get_nlin() ;
					if (nbLin == -1)
					  {
					  ossimNotify(ossimNotifyLevel_DEBUG) << "WARNING: nbLin is not read in the file !" << std::endl;
					  }
				  }
				else
				  {
					char* buff = new char[header.get_length()-12];
					is.read(buff, header.get_length()-12);
					delete[] buff;
				  }
			  }
			else if ((header.get_rec_seq() == 2))
			  { // First line ProcessedDataRecord
				lineLength = header.get_length() ;
				RadarSatRecord* record = factory.Instanciate(2);
				if (record != NULL)
			  	{
				  record->Read(is);
					data._records[Data::FirstProcessedDataRecordID] = record;

					char* buff = new char[header.get_length()-192];
					is.read(buff, header.get_length()-192);	// Reads the rest of the line
					delete[] buff;
				  }
				else
				  {
				  char* buff = new char[header.get_length()-12];
					is.read(buff, header.get_length()-12);
					delete[] buff;
				  }
			  }
			else if ((header.get_rec_seq() == (1+nbLin)))
			  { // Last line ProcessedDataRecord
			  RadarSatRecord* record = factory.Instanciate(2);
				if (record != NULL)
				  {
				  record->Read(is);
					data._records[Data::LastProcessedDataRecordID] = record;

					char* buff = new char[header.get_length()-192];
					is.read(buff, header.get_length()-192);	// Reads the rest of the line
					delete[] buff;
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
				// all lines between the first and last ones are skipped
				if (lineLength != 0)
				  {
				  if (nbLin == -1)
				    {
				    // Compute the number of line per dataset from the size of file and current position
				    int nbLines = ( ( lengthOfFile - is.tellg() ) / lineLength ) + 2;
				    ossimNotify(ossimNotifyLevel_DEBUG) << "To move in the dat file we compute the nb of lines = " << nbLines << std::endl;

				    // We move in the file to the last line
				    is.seekg((nbLines - 2) * lineLength - 12, std::ios::cur);

				    // We save the nbLines computed in data
				    nbLin = nbLines;
				    ImageOptionsFileDescriptor* record = data.get_ImageOptionsFileDescriptor();
				    record->set_nlin(nbLin);
				    data._records[Data::ImageOptionsFileDescriptorID] = record;
				    }
				  else
            {
            // We move in the file
            is.seekg((nbLin - 2) * lineLength - 12, std::ios::cur);
            }
				  }
				else
				  {
				  // We move to the end of the file
					is.seekg(0, std::ios::end) ;
				  }
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

ProcessedDataRecord* Data::get_FirstProcessedDataRecord()
{
	return (ProcessedDataRecord*)_records[FirstProcessedDataRecordID];
}

ProcessedDataRecord* Data::get_LastProcessedDataRecord()
{
	return (ProcessedDataRecord*)_records[LastProcessedDataRecordID];
}
}
