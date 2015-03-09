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

#include "TemperatureSettingsRecord.h"

namespace ossimplugins
{
TemperatureSettingsRecord::TemperatureSettingsRecord()
{
}

TemperatureSettingsRecord::~TemperatureSettingsRecord()
{
}

std::ostream& operator<<(std::ostream& os, const TemperatureSettingsRecord& data)
{
	for (int i=0;i<4;i++)
	{
		os<<"temp_set["<<i<<"]:"<<data._temp_set[i]<<std::endl;
	}

	return os;
}

std::istream& operator>>(std::istream& is, TemperatureSettingsRecord& data)
{
	char buf[5];
	buf[4] = '\0';

	for (int i=0;i<4;i++)
	{
		is.read(buf,4);
		buf[4] = '\0';
		data._temp_set[i] = atoi(buf);
	}
	return is;
}

TemperatureSettingsRecord::TemperatureSettingsRecord(const TemperatureSettingsRecord& rhs)
{
	for (int i=0;i<4;i++)
	{
		_temp_set[i] = rhs._temp_set[i];
	}
}

TemperatureSettingsRecord& TemperatureSettingsRecord::operator=(const TemperatureSettingsRecord& rhs)
{
	for (int i=0;i<4;i++)
	{
		_temp_set[i] = rhs._temp_set[i];
	}
	return *this;
}
}
