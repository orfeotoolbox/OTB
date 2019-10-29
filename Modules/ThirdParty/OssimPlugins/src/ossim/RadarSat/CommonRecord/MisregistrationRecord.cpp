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


#include "MisregistrationRecord.h"

namespace ossimplugins
{
MisregistrationRecord::MisregistrationRecord()
{
}

MisregistrationRecord::~MisregistrationRecord()
{
}

std::ostream& operator<<(std::ostream& os, const MisregistrationRecord& data)
{
	os<<"alt_m:"<<data._alt_m<<std::endl;
    os<<"crt_m:"<<data._crt_m<<std::endl;
	return os;
}

std::istream& operator>>(std::istream& is, MisregistrationRecord& data)
{
	char buf16[17];
	buf16[16] = '\0';

	is.read(buf16,16);
	data._alt_m = atof(buf16);

	is.read(buf16,16);
	data._crt_m = atof(buf16);
	return is;
}

MisregistrationRecord::MisregistrationRecord(const MisregistrationRecord& rhs):
	_alt_m(rhs._alt_m),
	_crt_m(rhs._crt_m)
{
}

MisregistrationRecord& MisregistrationRecord::operator=(const MisregistrationRecord& rhs)
{
	_alt_m = rhs._alt_m;
	_crt_m = rhs._crt_m;
	return *this;
}
}
