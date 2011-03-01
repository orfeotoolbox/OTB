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


#include "AttitudeData.h"

namespace ossimplugins
{

AttitudeData::AttitudeData() : RadarSatRecord("att_data_rec")
{
}

AttitudeData::~AttitudeData()
{
}

std::ostream& operator<<(std::ostream& os, const AttitudeData& data)
{
	os<<"npoint:"<<data._npoint<<std::endl;
	for (int i=0;i<20;i++)
	{
		os<<"att_vect["<<i<<"]:"<<data._att_vect[i]<<std::endl;
	}
    os<<"pitch_bias:"<<data._pitch_bias<<std::endl;
    os<<"roll_bias:"<<data._roll_bias<<std::endl;
    os<<"yaw_bias:"<<data._yaw_bias<<std::endl;
	return os;
}

std::istream& operator>>(std::istream& is, AttitudeData& data)
{
	char buf[6503];
	buf[6502] = '\0';

	is.read(buf,4);
	buf[4] = '\0';
	data._npoint = atoi(buf);

	for (int i=0;i<20;i++)
	{
		is>>data._att_vect[i];
	}
    is.read(buf,14);
	buf[14] = '\0';
	data._pitch_bias = atof(buf);

    is.read(buf,14);
	buf[14] = '\0';
	data._roll_bias = atof(buf);

    is.read(buf,14);
	buf[14] = '\0';
	data._yaw_bias = atof(buf);

	is.read(buf,6502);

	return is;
}

AttitudeData::AttitudeData(const AttitudeData& rhs):
	RadarSatRecord(rhs)
{
	_npoint = rhs._npoint;
	for (int i=0;i<20;i++)
	{
		_att_vect[i] = rhs._att_vect[i];
	}
    _pitch_bias = rhs._pitch_bias;
    _roll_bias = rhs._roll_bias;
    _yaw_bias = rhs._yaw_bias;
}

AttitudeData& AttitudeData::operator=(const AttitudeData& rhs)
{
	_npoint = rhs._npoint;
	for (int i=0;i<20;i++)
	{
		_att_vect[i] = rhs._att_vect[i];
	}
    _pitch_bias = rhs._pitch_bias;
    _roll_bias = rhs._roll_bias;
    _yaw_bias = rhs._yaw_bias;
	return *this;
}
}
