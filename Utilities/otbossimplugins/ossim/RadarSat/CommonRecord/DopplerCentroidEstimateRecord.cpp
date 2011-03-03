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

#include "DopplerCentroidEstimateRecord.h"

namespace ossimplugins
{
DopplerCentroidEstimateRecord::DopplerCentroidEstimateRecord()
{
}

DopplerCentroidEstimateRecord::~DopplerCentroidEstimateRecord()
{
}

std::ostream& operator<<(std::ostream& os, const DopplerCentroidEstimateRecord& data)
{
	os<<"dopcen_conf:"<<data._dopcen_conf<<std::endl;
	os<<"dopcen_ref_tim:"<<data._dopcen_ref_tim<<std::endl;
	for (int i=0;i<4;i++)
	{
		os<<"dopcen_coef["<<i<<"]:"<<data._dopcen_coef[i]<<std::endl;
	}

	return os;
}

std::istream& operator>>(std::istream& is, DopplerCentroidEstimateRecord& data)
{
	char buf[17];
	buf[16] = '\0';

	is.read(buf,16);
	buf[16] = '\0';
	data._dopcen_conf = atof(buf);

	is.read(buf,16);
	buf[16] = '\0';
	data._dopcen_ref_tim = atof(buf);

	for (int i=0;i<4;i++)
	{
		is.read(buf,16);
		buf[16] = '\0';
		data._dopcen_coef[i] = atof(buf);
	}
	return is;
}

DopplerCentroidEstimateRecord::DopplerCentroidEstimateRecord(const DopplerCentroidEstimateRecord& rhs):
	_dopcen_conf(rhs._dopcen_conf),
	_dopcen_ref_tim(rhs._dopcen_ref_tim)
{
	for (int i=0;i<4;i++)
	{
		_dopcen_coef[i] = rhs._dopcen_coef[i];
	}
}

DopplerCentroidEstimateRecord& DopplerCentroidEstimateRecord::operator=(const DopplerCentroidEstimateRecord& rhs)
{
	_dopcen_conf = rhs._dopcen_conf;
	_dopcen_ref_tim = rhs._dopcen_ref_tim;
	for (int i=0;i<4;i++)
	{
		_dopcen_coef[i] = rhs._dopcen_coef[i];
	}
	return *this;
}
}
