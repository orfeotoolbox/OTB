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


#include "SRGRCoefficientSetRecord.h"

namespace ossimplugins
{
SRGRCoefficientSetRecord::SRGRCoefficientSetRecord()
{
}

SRGRCoefficientSetRecord::~SRGRCoefficientSetRecord()
{
}

std::ostream& operator<<(std::ostream& os, const SRGRCoefficientSetRecord& data)
{
	os<<"srgr_update:"<<data._srgr_update <<std::endl;
	for (int i=0;i<6;i++)
	{
		os<<"srgr_coef["<<i<<"]:"<<data._srgr_coef[i]<<std::endl;
	}

	return os;
}

std::istream& operator>>(std::istream& is, SRGRCoefficientSetRecord& data)
{
	char buf[22];
	buf[21] = '\0';

	is.read(buf,21);
	buf[21] = '\0';
	data._srgr_update = buf;

	for (int i=0;i<6;i++)
	{
		is.read(buf,16);
		buf[16] = '\0';
		data._srgr_coef[i] = atof(buf);
	}
	return is;
}

SRGRCoefficientSetRecord::SRGRCoefficientSetRecord(const SRGRCoefficientSetRecord& rhs):
	_srgr_update(rhs._srgr_update)
{
	for (int i=0;i<6;i++)
	{
		_srgr_coef[i] = rhs._srgr_coef[i];
	}
}

SRGRCoefficientSetRecord& SRGRCoefficientSetRecord::operator=(const SRGRCoefficientSetRecord& rhs)
{
	_srgr_update = rhs._srgr_update;
	for (int i=0;i<6;i++)
	{
		_srgr_coef[i] = rhs._srgr_coef[i];
	}
	return *this;
}
}
