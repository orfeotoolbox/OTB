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

#include <EnvisatAsar/EnvisatAsarData.h>
#include <EnvisatAsar/EnvisatAsarDataFactory.h>

#include <EnvisatAsar/sph.h>
#include <EnvisatAsar/mph.h>
#include <EnvisatAsar/SQ_ADSR.h>
#include <EnvisatAsar/MainProcessingParameters.h>
#include <EnvisatAsar/SRGRConversionParameters.h>
#include <EnvisatAsar/DopplerCentroidParameters.h>
#include <EnvisatAsar/ChirpParameters.h>
#include <EnvisatAsar/AntennaElevationPatterns.h>
#include <EnvisatAsar/GeolocationGrid.h>

namespace ossimplugins
{

EnvisatAsarData::EnvisatAsarData()
{
}

EnvisatAsarData::~EnvisatAsarData()
{
	ClearRecords();
}

std::ostream& operator<<(std::ostream& os, const EnvisatAsarData& data)
{
	std::list<EnvisatAsarRecord*>::const_iterator it = data._records.begin();
	while(it != data._records.end())
	{
		(*it)->Write(os);
		++it;
	}
	return os;

}

std::istream& operator>>(std::istream& is, EnvisatAsarData& data)
{
	EnvisatAsarDataFactory factory;

	if (sizeof(int)!=4) std::cout << "EnvisatAsarData WARNING : (int) not coded over 32 bits, metadata might not be byte swapped correctly"<< std::endl ;
	if (sizeof(float)!=4) std::cout << "EnvisatAsarData WARNING : (float) not coded over 32 bits, metadata might not be byte swapped correctly"<< std::endl ;
	if (sizeof(double)!=8) std::cout << "EnvisatAsarData WARNING : (double) not coded over 64 bits, metadata might not be byte swapped correctly"<< std::endl ;

	data.ClearRecords();

	// read the first record (MPH)
	EnvisatAsarRecord* mph_record = factory.Instanciate("MPH");
	mph_record->Read(is);
	data._records.push_back(mph_record);

	// read the second record (sph) and update it with the mph (number of dsd)
	EnvisatAsarRecord* sph_record = factory.Instanciate("SPH");
	((sph*)sph_record)->update_sph_from_mph(*((mph *)mph_record));
	sph_record->Read(is);
	data._records.push_back(sph_record);

	std::vector<dsd> dsd_vector = ((sph*)sph_record)->get_dsd_vector();

	// For each dsd, check if present in the file and in the record factory.
	// If true, instanciate it and add to the EnvisatAsarRecord list.
	std::vector<dsd>::iterator it = dsd_vector.begin();
	while(it != dsd_vector.end())
	{
		std::string str = (it->get_ds_name());

		if ( (it->get_ds_size()) != 0)
		{
			for (int i = 0; i<(it->get_num_dsr()); i++)
			{
				EnvisatAsarRecord* record = factory.Instanciate(str);
				if (record != NULL)
				{
					is.seekg((std::streampos)(it->get_ds_offset())+ (std::streampos)(i*(it->get_dsr_size())));
					record->Read(is);
					data._records.push_back(record);
				}
			}
		}
		++it;
	}

	return is;
}


EnvisatAsarData::EnvisatAsarData(const EnvisatAsarData& rhs)
{
	std::list<EnvisatAsarRecord*>::const_iterator it = rhs._records.begin();
	while(it != rhs._records.end())
	{
		_records.push_back((*it)->Clone());
		++it;
	}
}

EnvisatAsarData& EnvisatAsarData::operator=(const EnvisatAsarData& rhs)
{
	ClearRecords();
	std::list<EnvisatAsarRecord*>::const_iterator it = rhs._records.begin();
	while(it != rhs._records.end())
	{
		_records.push_back((*it)->Clone());
		++it;
	}

	return *this;
}

void EnvisatAsarData::ClearRecords()
{
	std::list<EnvisatAsarRecord*>::const_iterator it = _records.begin();
	while(it != _records.end())
	{
		delete (*it);
		++it;
	}
	_records.clear();
}
mph * EnvisatAsarData::get_mph()
{
	std::list<EnvisatAsarRecord*>::const_iterator it = _records.begin();
	while(it != _records.end())
	{
		if ((*it)->get_mnemonic().compare("mph_rec") == 0)	return (mph*)(*it);
		++it;
	}
	return NULL;
}
sph * EnvisatAsarData::get_sph()
{
	std::list<EnvisatAsarRecord*>::const_iterator it = _records.begin();
	while(it != _records.end())
	{
		if ((*it)->get_mnemonic().compare("sph_rec") == 0)	return (sph*)(*it);
		++it;
	}
	return NULL;
}

SQ_ADSR * EnvisatAsarData::get_SQ_ADSR()
{
	std::list<EnvisatAsarRecord*>::const_iterator it = _records.begin();
	while(it != _records.end())
	{
		if ((*it)->get_mnemonic().compare("SQ_ADSR_rec") == 0)	return (SQ_ADSR*)(*it);
		++it;
	}
	return NULL;
}
MainProcessingParameters * EnvisatAsarData::get_MainProcessingParameters()
{
	std::list<EnvisatAsarRecord*>::const_iterator it = _records.begin();
	while(it != _records.end())
	{
		if ((*it)->get_mnemonic().compare("MainProcessingParameters_rec") == 0)	return (MainProcessingParameters*)(*it);
		++it;
	}
	return NULL;
}
DopplerCentroidParameters * EnvisatAsarData::get_DopplerCentroidParameters()
{
	std::list<EnvisatAsarRecord*>::const_iterator it = _records.begin();
	while(it != _records.end())
	{
		if ((*it)->get_mnemonic().compare("DopplerCentroidParameters_rec") == 0)	return (DopplerCentroidParameters*)(*it);
		++it;
	}
	return NULL;
}
SRGRConversionParameters * EnvisatAsarData::get_SRGRConversionParameters(int pos)
{
	std::list<EnvisatAsarRecord*>::const_iterator it = _records.begin();
	int cpt = 0;
	while(it != _records.end())
	{
		if ((*it)->get_mnemonic().compare("SRGRConversionParameters_rec") == 0)
		{
			if (cpt == pos) {
				return (SRGRConversionParameters*)(*it);
			}
			else cpt++;
		}
		++it;
	}
	return NULL;
}
ChirpParameters * EnvisatAsarData::get_ChirpParameters()
{
	std::list<EnvisatAsarRecord*>::const_iterator it = _records.begin();
	while(it != _records.end())
	{
		if ((*it)->get_mnemonic().compare("ChirpParameters_rec") == 0)	return (ChirpParameters*)(*it);
		++it;
	}
	return NULL;
}
AntennaElevationPatterns * EnvisatAsarData::get_AntennaElevationPatterns()
{
	std::list<EnvisatAsarRecord*>::const_iterator it = _records.begin();
	while(it != _records.end())
	{
		if ((*it)->get_mnemonic().compare("AntennaElevationPatterns_rec") == 0)	return (AntennaElevationPatterns*)(*it);
		++it;
	}
	return NULL;
}

// Return the first Geolocation Grid Record
//GeolocationGrid * EnvisatAsarData::get_GeolocationGrid()
//{
//	std::list<EnvisatAsarRecord*>::const_iterator it = _records.begin();
//	while(it != _records.end())
//	{
//		std::string str = (*it)->get_mnemonic();
//		if ((*it)->get_mnemonic().compare("GeolocationGrid_rec") == 0)
//		{
//			return (GeolocationGrid*)(*it);
//		}
//		++it;
//	}
//	return NULL;
//}

// Return the num_ds Geolocation Grid Record
GeolocationGrid * EnvisatAsarData::get_GeolocationGrid(int num_ds)
{
	std::list<EnvisatAsarRecord*>::const_iterator it = _records.begin();

	int total_num_ds = 0;
	while(it != _records.end())
	{
		std::string str = (*it)->get_mnemonic();
		if ((*it)->get_mnemonic().compare("GeolocationGrid_rec") == 0)	total_num_ds += 1;
		++it;
	}

	int cpt = 0;
	it = _records.begin();
	while(it != _records.end())
	{
		std::string str = (*it)->get_mnemonic();
		if ((*it)->get_mnemonic().compare("GeolocationGrid_rec") == 0)
		{
			if ((cpt == num_ds) || (cpt == total_num_ds-1))	return (GeolocationGrid*)(*it);
			cpt +=1;

		}
		++it;
	}
	return NULL;
}


int EnvisatAsarData::get_num_ds(EnvisatAsarRecord * record)
{
	if (record == NULL) return 0;
	std::list<EnvisatAsarRecord*>::const_iterator it = _records.begin();
	int cpt = 0;
	while(it != _records.end())
	{
		std::string str = (*it)->get_mnemonic();
		if ((*it)->get_mnemonic().compare(record->get_mnemonic()) == 0)
		{
			cpt++;
		}
		++it;
	}
	return cpt;
}
}
