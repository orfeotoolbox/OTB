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


#include "BeamPixelCountRecord.h"

namespace ossimplugins
{
BeamPixelCountRecord::BeamPixelCountRecord()
{
}

BeamPixelCountRecord::~BeamPixelCountRecord()
{
}

std::ostream& operator<<(std::ostream& os, const BeamPixelCountRecord& data)
{
	os<<"pix_update:"<<data._pix_update <<std::endl;

	for (int i=0;i<4;i++)
	{
		os<<"crt_m["<<i<<":"<<data._n_pix[i]<<std::endl;
	}
	return os;
}

std::istream& operator>>(std::istream& is, BeamPixelCountRecord& data)
{
	char buf[23];
	buf[22] = '\0';

	is.read(buf,21);
	data._pix_update = buf;

	for (int i=0;i<4;i++)
	{
		is.read(buf,8);
		buf[8] = '\0';
		data._n_pix[i] = atoi(buf);
	}
	return is;
}

BeamPixelCountRecord::BeamPixelCountRecord(const BeamPixelCountRecord& rhs):
	_pix_update(rhs._pix_update)
{
	for (int i=0;i<4;i++)
	{
		_n_pix[i] = rhs._n_pix[i];
	}
}

BeamPixelCountRecord& BeamPixelCountRecord::operator=(const BeamPixelCountRecord& rhs)
{
	_pix_update = rhs._pix_update;
	for (int i=0;i<4;i++)
	{
		_n_pix[i] = rhs._n_pix[i];
	}
	return *this;
}
}
