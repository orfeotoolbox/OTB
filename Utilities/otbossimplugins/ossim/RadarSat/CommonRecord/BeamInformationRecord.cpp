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

#include "BeamInformationRecord.h"

namespace ossimplugins
{
BeamInformationRecord::BeamInformationRecord()
{
}

BeamInformationRecord::~BeamInformationRecord()
{
}

std::ostream& operator<<(std::ostream& os, const BeamInformationRecord& data)
{
	os<<"beam_type:"<<data._beam_type.c_str()<<std::endl;

	os<<"beam_look_src:"<<data._beam_look_src.c_str()<<std::endl;

    os<<"beam_look_ang:"<<data._beam_look_ang<<std::endl;

	os<<"prf:"<<data._prf<<std::endl;

	return os;
}

std::istream& operator>>(std::istream& is, BeamInformationRecord& data)
{
	char buf[17];
	buf[16] = '\0';

	is.read(buf,3);
	buf[3] = '\0';
	data._beam_type = buf;

	is.read(buf,9);
	buf[9] = '\0';
	data._beam_look_src = buf;

    is.read(buf,16);
	buf[16] = '\0';
	data._beam_look_ang = atof(buf);

	is.read(buf,16);
	buf[16] = '\0';
	data._prf = atof(buf);

	return is;
}

BeamInformationRecord::BeamInformationRecord(const BeamInformationRecord& rhs):
	_beam_type(rhs._beam_type),
	_beam_look_src(rhs._beam_look_src),
	_beam_look_ang(rhs._beam_look_ang),
	_prf(rhs._prf)
{

}

BeamInformationRecord& BeamInformationRecord::operator=(const BeamInformationRecord& rhs)
{
	_beam_type = rhs._beam_type;
	_beam_look_src = rhs._beam_look_src;
	_beam_look_ang = rhs._beam_look_ang;
	_prf = rhs._prf;
	return *this;
}
}
