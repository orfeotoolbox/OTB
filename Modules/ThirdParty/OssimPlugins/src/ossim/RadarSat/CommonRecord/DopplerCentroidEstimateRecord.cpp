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


#include "DopplerCentroidEstimateRecord.h"

namespace ossimplugins
{
DopplerCentroidEstimateRecord::DopplerCentroidEstimateRecord()
{
}

DopplerCentroidEstimateRecord::~DopplerCentroidEstimateRecord()
{
}

std::ostream& operator<<(std::ostream& os, const DopplerCentroidEstimateRecord& data)
{
	os<<"dopcen_conf:"<<data._dopcen_conf<<std::endl;
	os<<"dopcen_ref_tim:"<<data._dopcen_ref_tim<<std::endl;
	for (int i=0;i<4;i++)
	{
		os<<"dopcen_coef["<<i<<"]:"<<data._dopcen_coef[i]<<std::endl;
	}

	return os;
}

std::istream& operator>>(std::istream& is, DopplerCentroidEstimateRecord& data)
{
	char buf[17];
	buf[16] = '\0';

	is.read(buf,16);
	buf[16] = '\0';
	data._dopcen_conf = atof(buf);

	is.read(buf,16);
	buf[16] = '\0';
	data._dopcen_ref_tim = atof(buf);

	for (int i=0;i<4;i++)
	{
		is.read(buf,16);
		buf[16] = '\0';
		data._dopcen_coef[i] = atof(buf);
	}
	return is;
}

DopplerCentroidEstimateRecord::DopplerCentroidEstimateRecord(const DopplerCentroidEstimateRecord& rhs):
	_dopcen_conf(rhs._dopcen_conf),
	_dopcen_ref_tim(rhs._dopcen_ref_tim)
{
	for (int i=0;i<4;i++)
	{
		_dopcen_coef[i] = rhs._dopcen_coef[i];
	}
}

DopplerCentroidEstimateRecord& DopplerCentroidEstimateRecord::operator=(const DopplerCentroidEstimateRecord& rhs)
{
	_dopcen_conf = rhs._dopcen_conf;
	_dopcen_ref_tim = rhs._dopcen_ref_tim;
	for (int i=0;i<4;i++)
	{
		_dopcen_coef[i] = rhs._dopcen_coef[i];
	}
	return *this;
}
}
