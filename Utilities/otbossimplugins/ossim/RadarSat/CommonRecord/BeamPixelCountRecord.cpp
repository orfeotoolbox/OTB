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

#include "BeamPixelCountRecord.h"

namespace ossimplugins
{
BeamPixelCountRecord::BeamPixelCountRecord()
{
}

BeamPixelCountRecord::~BeamPixelCountRecord()
{
}

std::ostream& operator<<(std::ostream& os, const BeamPixelCountRecord& data)
{
	os<<"pix_update:"<<data._pix_update.c_str()<<std::endl;

	for (int i=0;i<4;i++)
	{
		os<<"crt_m["<<i<<":"<<data._n_pix[i]<<std::endl;
	}
	return os;
}

std::istream& operator>>(std::istream& is, BeamPixelCountRecord& data)
{
	char buf[23];
	buf[22] = '\0';

	is.read(buf,21);
	data._pix_update = buf;

	for (int i=0;i<4;i++)
	{
		is.read(buf,8);
		buf[8] = '\0';
		data._n_pix[i] = atoi(buf);
	}
	return is;
}

BeamPixelCountRecord::BeamPixelCountRecord(const BeamPixelCountRecord& rhs):
	_pix_update(rhs._pix_update)
{
	for (int i=0;i<4;i++)
	{
		_n_pix[i] = rhs._n_pix[i];
	}
}

BeamPixelCountRecord& BeamPixelCountRecord::operator=(const BeamPixelCountRecord& rhs)
{
	_pix_update = rhs._pix_update;
	for (int i=0;i<4;i++)
	{
		_n_pix[i] = rhs._n_pix[i];
	}
	return *this;
}
}
