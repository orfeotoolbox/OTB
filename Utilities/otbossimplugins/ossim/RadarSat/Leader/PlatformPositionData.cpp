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

#include <RadarSat/Leader/PlatformPositionData.h>

namespace ossimplugins
{
PlatformPositionData::PlatformPositionData() : RadarSatRecord("pos_data_rec")
{
}

PlatformPositionData::~PlatformPositionData()
{
}

std::ostream& operator<<(std::ostream& os, const PlatformPositionData& data)
{
	os<<"orbit_ele_desg:"<<data._orbit_ele_desg.c_str()<<std::endl;

	for(int i=0;i<6;i++)
	{
		os<<"orbit_ele["<<i<<"]:"<<data._orbit_ele[i]<<std::endl;
	}

	os<<"ndata:"<<data._ndata<<std::endl;

    os<<"year:"<<data._year<<std::endl;

    os<<"month:"<<data._month<<std::endl;

    os<<"day:"<<data._day<<std::endl;

    os<<"gmt_day:"<<data._gmt_day<<std::endl;

    os<<"gmt_sec:"<<data._gmt_sec<<std::endl;

    os<<"data_int:"<<data._data_int<<std::endl;

    os<<"ref_coord:"<<data._ref_coord.c_str()<<std::endl;

    os<<"hr_angle:"<<data._hr_angle<<std::endl;

    os<<"alt_poserr:"<<data._alt_poserr<<std::endl;

    os<<"crt_poserr:"<<data._crt_poserr<<std::endl;

    os<<"rad_poserr:"<<data._rad_poserr<<std::endl;

	os<<"alt_velerr:"<<data._alt_velerr<<std::endl;

    os<<"crt_velerr:"<<data._crt_velerr<<std::endl;

    os<<"rad_velerr:"<<data._rad_velerr<<std::endl;

	for (int i=0;i<64;i++)
	{
		os<<"pos_vect["<<i<<"]:"<<data._pos_vect[i]<<std::endl;
	}

	return os;
}

std::istream& operator>>(std::istream& is, PlatformPositionData& data)
{
	char buf[1207];
	buf[1206] = '\0';

	is.read(buf,32);
	buf[32] = '\0';
	data._orbit_ele_desg = buf;

	for(int i=0;i<6;i++)
	{
		is.read(buf,16);
		buf[16] = '\0';
		data._orbit_ele[i] = atof(buf);
	}

	is.read(buf,4);
	buf[4] = '\0';
	data._ndata = atoi(buf);

    is.read(buf,4);
	buf[4] = '\0';
	data._year = atoi(buf);

    is.read(buf,4);
	buf[4] = '\0';
	data._month = atoi(buf);

    is.read(buf,4);
	buf[4] = '\0';
	data._day = atoi(buf);

    is.read(buf,4);
	buf[4] = '\0';
	data._gmt_day = atoi(buf);

    is.read(buf,22);
	buf[22] = '\0';
	data._gmt_sec = atof(buf);

    is.read(buf,22);
	buf[22] = '\0';
	data._data_int = atof(buf);

    is.read(buf,64);
	buf[64] = '\0';
	data._ref_coord = buf;

    is.read(buf,22);
	buf[22] = '\0';
	data._hr_angle = atof(buf);

    is.read(buf,16);
	buf[16] = '\0';
	data._alt_poserr = atof(buf);

    is.read(buf,16);
	buf[16] = '\0';
	data._crt_poserr = atof(buf);

    is.read(buf,16);
	buf[16] = '\0';
	data._rad_poserr = atof(buf);

    is.read(buf,16);
	buf[16] = '\0';
	data._alt_velerr = atof(buf);

    is.read(buf,16);
	buf[16] = '\0';
	data._crt_velerr = atof(buf);

    is.read(buf,16);
	buf[16] = '\0';
	data._rad_velerr = atof(buf);

	for (int i=0;i<64;i++)
	{
		is>>data._pos_vect[i];
	}

    is.read(buf,126);


	return is;
}

PlatformPositionData::PlatformPositionData(const PlatformPositionData& rhs):
	RadarSatRecord(rhs)
{
	_orbit_ele_desg = rhs._orbit_ele_desg;

	for(int i=0;i<6;i++)
	{
		_orbit_ele[i] = rhs._orbit_ele[i];
	}

	_ndata = rhs._ndata;

    _year = rhs._year;

    _month = rhs._month;

    _day = rhs._day;

    _gmt_day = rhs._gmt_day;

    _gmt_sec = rhs._gmt_sec;

    _data_int = rhs._data_int;

    _ref_coord = rhs._ref_coord;

    _hr_angle = rhs._hr_angle;

    _alt_poserr = rhs._alt_poserr;

    _crt_poserr = rhs._crt_poserr;

    _rad_poserr = rhs._rad_poserr;

	_alt_velerr = rhs._alt_velerr;

    _crt_velerr = rhs._crt_velerr;

    _rad_velerr = rhs._rad_velerr;

	for (int i=0;i<64;i++)
	{
		_pos_vect[i] = rhs._pos_vect[i];
	}

}

PlatformPositionData& PlatformPositionData::operator=(const PlatformPositionData& rhs)
{
		_orbit_ele_desg = rhs._orbit_ele_desg;

	for(int i=0;i<6;i++)
	{
		_orbit_ele[i] = rhs._orbit_ele[i];
	}

	_ndata = rhs._ndata;

    _year = rhs._year;

    _month = rhs._month;

    _day = rhs._day;

    _gmt_day = rhs._gmt_day;

    _gmt_sec = rhs._gmt_sec;

    _data_int = rhs._data_int;

    _ref_coord = rhs._ref_coord;

    _hr_angle = rhs._hr_angle;

    _alt_poserr = rhs._alt_poserr;

    _crt_poserr = rhs._crt_poserr;

    _rad_poserr = rhs._rad_poserr;

	_alt_velerr = rhs._alt_velerr;

    _crt_velerr = rhs._crt_velerr;

    _rad_velerr = rhs._rad_velerr;

	for (int i=0;i<64;i++)
	{
		_pos_vect[i] = rhs._pos_vect[i];
	}

	return *this;
}
}
