/*
 * Copyright (C) 2005-2017 by Centre National d'Etudes Spatiales (CNES)
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


#include "RadiometricData.h"

namespace ossimplugins
{

RadiometricData::RadiometricData() : RadarSatRecord("radi_data_rec")
{
}

RadiometricData::~RadiometricData()
{
}

std::ostream& operator<<(std::ostream& os, const RadiometricData& data)
{
	os<<"seq_num:"<<data._seq_num<<std::endl;

    os<<"n_data:"<<data._n_data<<std::endl;

    os<<"field_size:"<<data._field_size<<std::endl;

	os<<"chan_ind:"<<data._chan_ind <<std::endl;

    os<<"table_desig:"<<data._table_desig <<std::endl;

    os<<"n_samp:"<<data._n_samp<<std::endl;

    os<<"samp_type:"<<data._samp_type <<std::endl;

    os<<"samp_inc:"<<data._samp_inc<<std::endl;

	for (int i=0;i<512;i++)
	{
		os<<"lookup_tab["<<i<<"]:"<<data._lookup_tab[i]<<std::endl;
	}

    os<<"noise_scale:"<<data._noise_scale<<std::endl;

    os<<"offset:"<<data._offset<<std::endl;

    os<<"calib_const:"<<data._calib_const<<std::endl;

	return os;
}

std::istream& operator>>(std::istream& is, RadiometricData& data)
{
	char buf[1513];
	buf[1512] = '\0';


	is.read(buf,4);
	buf[4] = '\0';
	data._seq_num = atoi(buf);

    is.read(buf,4);
	buf[4] = '\0';
	data._n_data = atoi(buf);

    is.read(buf,8);
	buf[8] = '\0';
	data._field_size = atoi(buf);

	is.read(buf,4);
	buf[4] = '\0';
	data._chan_ind = buf;

    is.read(buf,4);

    is.read(buf,24);
	buf[24] = '\0';
	data._table_desig = buf;

    is.read(buf,8);
	buf[8] = '\0';
	data._n_samp = atoi(buf);

    is.read(buf,16);
	buf[16] = '\0';
	data._samp_type = buf;

    is.read(buf,4);
	buf[4] = '\0';
	data._samp_inc = atoi(buf);

	for (int i=0;i<512;i++)
	{
		is.read(buf,16);
		buf[16] = '\0';
		data._lookup_tab[i] = atof(buf);
	}

    is.read(buf,4);

    is.read(buf,16);
	buf[16] = '\0';
	data._noise_scale = atof(buf);

    is.read(buf,16);

    is.read(buf,16);
	buf[16] = '\0';
	data._offset = atof(buf);

    is.read(buf,16);
	buf[16] = '\0';
	data._calib_const = atof(buf);

	is.read(buf,1512);

	return is;
}

RadiometricData::RadiometricData(const RadiometricData& rhs):
	RadarSatRecord(rhs)
{

	_seq_num = rhs._seq_num;

    _n_data = rhs._n_data;

    _field_size = rhs._field_size;

	_chan_ind = rhs._chan_ind;

    _table_desig = rhs._table_desig;

    _n_samp = rhs._n_samp;

    _samp_type = rhs._samp_type;

    _samp_inc = rhs._samp_inc;

	for (int i=0;i<512;i++)
	{
		_lookup_tab[i] = rhs._lookup_tab[i];
	}

    _noise_scale = rhs._noise_scale;

    _offset = rhs._offset;

    _calib_const = rhs._calib_const;
}

RadiometricData& RadiometricData::operator=(const RadiometricData& rhs)
{
	_seq_num = rhs._seq_num;

    _n_data = rhs._n_data;

    _field_size = _field_size;

	_chan_ind = rhs._chan_ind;

    _table_desig = rhs._table_desig;

    _n_samp = rhs._n_samp;

    _samp_type = rhs._samp_type;

    _samp_inc = rhs._samp_inc;

	for (int i=0;i<512;i++)
	{
		_lookup_tab[i] = rhs._lookup_tab[i];
	}

    _noise_scale = rhs._noise_scale;

    _offset = rhs._offset;

    _calib_const = rhs._calib_const;

	return *this;
}
}
