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

#include "RadiometryUncertaintyRecord.h"

namespace ossimplugins
{
RadiometryUncertaintyRecord::RadiometryUncertaintyRecord()
{
}

RadiometryUncertaintyRecord::~RadiometryUncertaintyRecord()
{
}

std::ostream& operator<<(std::ostream& os, const RadiometryUncertaintyRecord& data)
{
	os<<"db:"<<data._db<<std::endl;
    os<<"deg:"<<data._deg<<std::endl;
	return os;
}

std::istream& operator>>(std::istream& is, RadiometryUncertaintyRecord& data)
{
	char buf16[17];
	buf16[16] = '\0';

	is.read(buf16,16);
	data._db = atof(buf16);

	is.read(buf16,16);
	data._deg = atof(buf16);
	return is;
}

RadiometryUncertaintyRecord::RadiometryUncertaintyRecord(const RadiometryUncertaintyRecord& rhs):
	_db(rhs._db),
	_deg(rhs._deg)
{
}

RadiometryUncertaintyRecord& RadiometryUncertaintyRecord::operator=(const RadiometryUncertaintyRecord& rhs)
{
	_db = rhs._db;
	_deg = rhs._deg;
	return *this;
}
}
