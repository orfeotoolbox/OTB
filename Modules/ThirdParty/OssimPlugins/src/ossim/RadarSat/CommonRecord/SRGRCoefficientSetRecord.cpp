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

#include "SRGRCoefficientSetRecord.h"

namespace ossimplugins
{
SRGRCoefficientSetRecord::SRGRCoefficientSetRecord()
{
}

SRGRCoefficientSetRecord::~SRGRCoefficientSetRecord()
{
}

std::ostream& operator<<(std::ostream& os, const SRGRCoefficientSetRecord& data)
{
	os<<"srgr_update:"<<data._srgr_update.c_str()<<std::endl;
	for (int i=0;i<6;i++)
	{
		os<<"srgr_coef["<<i<<"]:"<<data._srgr_coef[i]<<std::endl;
	}

	return os;
}

std::istream& operator>>(std::istream& is, SRGRCoefficientSetRecord& data)
{
	char buf[22];
	buf[21] = '\0';

	is.read(buf,21);
	buf[21] = '\0';
	data._srgr_update = buf;

	for (int i=0;i<6;i++)
	{
		is.read(buf,16);
		buf[16] = '\0';
		data._srgr_coef[i] = atof(buf);
	}
	return is;
}

SRGRCoefficientSetRecord::SRGRCoefficientSetRecord(const SRGRCoefficientSetRecord& rhs):
	_srgr_update(rhs._srgr_update)
{
	for (int i=0;i<6;i++)
	{
		_srgr_coef[i] = rhs._srgr_coef[i];
	}
}

SRGRCoefficientSetRecord& SRGRCoefficientSetRecord::operator=(const SRGRCoefficientSetRecord& rhs)
{
	_srgr_update = rhs._srgr_update;
	for (int i=0;i<6;i++)
	{
		_srgr_coef[i] = rhs._srgr_coef[i];
	}
	return *this;
}
}
