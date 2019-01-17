/*
 * Copyright (C) 2005-2019 by Centre National d'Etudes Spatiales (CNES)
 *
 * This file is licensed under MIT license:
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */



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
