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


#include <RadarSat/Leader/DataHistogramProcessedData.h>

namespace ossimplugins
{
DataHistogramProcessedData::DataHistogramProcessedData() : RadarSatRecord("pdr16_hist_rec")
{
}

DataHistogramProcessedData::~DataHistogramProcessedData()
{
}

std::ostream& operator<<(std::ostream& os, const DataHistogramProcessedData& data)
{
	os<<"rec_seq:"<<data._rec_seq<<std::endl;

    os<<"sar_chn:"<<data._sar_chn<<std::endl;

	os<<"ntab:"<<data._ntab<<std::endl;

	os<<"ltab:"<<data._ltab<<std::endl;

	os<<data._histogram1<<std::endl;

	os<<data._histogram2<<std::endl;

	return os;
}

std::istream& operator>>(std::istream& is, DataHistogramProcessedData& data)
{
	char buff[33];
	buff[32] = '\0';

	is.read(buff,4);
	buff[4] = '\0';
	data._rec_seq = atoi(buff);

    is.read(buff,4);
	buff[4] = '\0';
	data._sar_chn = atoi(buff);

	is.read(buff,8);
	buff[8] = '\0';
	data._ntab = atoi(buff);

	is.read(buff,8);
	buff[8] = '\0';
	data._ltab = atoi(buff);

	is>>data._histogram1;

	is>>data._histogram2;

	char tmp[5];
	tmp[4] = '\0';
	is.read(tmp, 4);
	return is;
}

DataHistogramProcessedData::DataHistogramProcessedData(const DataHistogramProcessedData& rhs) :
		RadarSatRecord(rhs),
		_rec_seq(rhs._rec_seq),
		_sar_chn(rhs._sar_chn),
		_ntab(rhs._ntab),
		_ltab(rhs._ltab),
		_histogram1(rhs._histogram1),
		_histogram2(rhs._histogram2)
{

}

DataHistogramProcessedData& DataHistogramProcessedData::operator=(const DataHistogramProcessedData& rhs)
{
	_histogram1 = rhs._histogram1;
	_histogram2 = rhs._histogram2;
	_sar_chn = rhs._sar_chn;
	_rec_seq = rhs._rec_seq;
	_ntab = rhs._ntab;
	_ltab = rhs._ltab;
	return *this;
}
}
