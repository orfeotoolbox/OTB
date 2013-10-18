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

#include "MisregistrationRecord.h"

namespace ossimplugins
{
MisregistrationRecord::MisregistrationRecord()
{
}

MisregistrationRecord::~MisregistrationRecord()
{
}

std::ostream& operator<<(std::ostream& os, const MisregistrationRecord& data)
{
	os<<"alt_m:"<<data._alt_m<<std::endl;
    os<<"crt_m:"<<data._crt_m<<std::endl;
	return os;
}

std::istream& operator>>(std::istream& is, MisregistrationRecord& data)
{
	char buf16[17];
	buf16[16] = '\0';

	is.read(buf16,16);
	data._alt_m = atof(buf16);

	is.read(buf16,16);
	data._crt_m = atof(buf16);
	return is;
}

MisregistrationRecord::MisregistrationRecord(const MisregistrationRecord& rhs):
	_alt_m(rhs._alt_m),
	_crt_m(rhs._crt_m)
{
}

MisregistrationRecord& MisregistrationRecord::operator=(const MisregistrationRecord& rhs)
{
	_alt_m = rhs._alt_m;
	_crt_m = rhs._crt_m;
	return *this;
}
}
