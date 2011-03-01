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

#include "DataPointDataRecord.h"

namespace ossimplugins
{
DataPointDataRecord::DataPointDataRecord()
{
}

DataPointDataRecord::~DataPointDataRecord()
{
}

std::ostream& operator<<(std::ostream& os, const DataPointDataRecord& data)
{
	os<<"gmt_day:"<<data._gmt_day<<std::endl;
    os<<"gmt_sec:"<<data._gmt_sec<<std::endl;
    os<<"pitch_flag:"<<data._pitch_flag<<std::endl;
    os<<"roll_flag:"<<data._roll_flag<<std::endl;
    os<<"yaw_flag:"<<data._yaw_flag<<std::endl;
    os<<"pitch:"<<data._pitch<<std::endl;
    os<<"roll:"<<data._roll<<std::endl;
    os<<"yaw:"<<data._yaw<<std::endl;
    os<<"pitch_rate_flag:"<<data._pitch_rate_flag<<std::endl;
    os<<"roll_rate_flag:"<<data._roll_rate_flag<<std::endl;
    os<<"yaw_rate_flag:"<<data._yaw_rate_flag<<std::endl;
    os<<"pitch_rate:"<<data._pitch_rate<<std::endl;
    os<<"roll_rate:"<<data._roll_rate<<std::endl;
	os<<"yaw_rate:"<<data._yaw_rate<<std::endl;
	return os;
}

std::istream& operator>>(std::istream& is, DataPointDataRecord& data)
{
	char buf[15];
	buf[14] = '\0';

	is.read(buf,4);
	buf[4] = '\0';
	data._gmt_day = atoi(buf);

	is.read(buf,8);
	buf[8] = '\0';
	data._gmt_sec = atoi(buf);

    is.read(buf,4);
	buf[4] = '\0';
	data._pitch_flag = atoi(buf);

    is.read(buf,4);
	buf[4] = '\0';
	data._roll_flag = atoi(buf);

    is.read(buf,4);
	buf[4] = '\0';
	data._yaw_flag = atoi(buf);

    is.read(buf,14);
	buf[14] = '\0';
	data._pitch = atof(buf);

    is.read(buf,14);
	buf[14] = '\0';
	data._roll = atof(buf);

    is.read(buf,14);
	buf[14] = '\0';
	data._yaw = atof(buf);

    is.read(buf,4);
	buf[4] = '\0';
	data._pitch_rate_flag = atoi(buf);

    is.read(buf,4);
	buf[4] = '\0';
	data._roll_rate_flag = atoi(buf);

    is.read(buf,4);
	buf[4] = '\0';
	data._yaw_rate_flag = atoi(buf);

    is.read(buf,14);
	buf[14] = '\0';
	data._pitch_rate = atof(buf);

    is.read(buf,14);
	buf[14] = '\0';
	data._roll_rate = atof(buf);

    is.read(buf,14);
	buf[14] = '\0';
	data._yaw_rate = atof(buf);

	return is;
}

DataPointDataRecord::DataPointDataRecord(const DataPointDataRecord& rhs)
{
	_gmt_day = rhs._gmt_day;
    _gmt_sec = rhs._gmt_sec;
    _pitch_flag = rhs._pitch_flag;
    _roll_flag = rhs._roll_flag;
    _yaw_flag = rhs._yaw_flag;
    _pitch = rhs._pitch;
    _roll = rhs._roll;
    _yaw = rhs._yaw;
    _pitch_rate_flag = rhs._pitch_rate_flag;
    _roll_rate_flag = rhs._roll_rate_flag;
    _yaw_rate_flag = rhs._yaw_rate_flag;
    _pitch_rate = rhs._pitch_rate;
    _roll_rate = rhs._roll_rate;
	_yaw_rate = rhs._yaw_rate;
}

DataPointDataRecord& DataPointDataRecord::operator=(const DataPointDataRecord& rhs)
{
	_gmt_day = rhs._gmt_day;
    _gmt_sec = rhs._gmt_sec;
    _pitch_flag = rhs._pitch_flag;
    _roll_flag = rhs._roll_flag;
    _yaw_flag = rhs._yaw_flag;
    _pitch = rhs._pitch;
    _roll = rhs._roll;
    _yaw = rhs._yaw;
    _pitch_rate_flag = rhs._pitch_rate_flag;
    _roll_rate_flag = rhs._roll_rate_flag;
    _yaw_rate_flag = rhs._yaw_rate_flag;
    _pitch_rate = rhs._pitch_rate;
    _roll_rate = rhs._roll_rate;
	_yaw_rate = rhs._yaw_rate;
	return *this;
}
}
