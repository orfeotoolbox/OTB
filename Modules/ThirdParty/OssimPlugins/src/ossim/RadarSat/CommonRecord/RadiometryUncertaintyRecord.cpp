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


#include "RadiometryUncertaintyRecord.h"

namespace ossimplugins
{
RadiometryUncertaintyRecord::RadiometryUncertaintyRecord()
{
}

RadiometryUncertaintyRecord::~RadiometryUncertaintyRecord()
{
}

std::ostream& operator<<(std::ostream& os, const RadiometryUncertaintyRecord& data)
{
	os<<"db:"<<data._db<<std::endl;
    os<<"deg:"<<data._deg<<std::endl;
	return os;
}

std::istream& operator>>(std::istream& is, RadiometryUncertaintyRecord& data)
{
	char buf16[17];
	buf16[16] = '\0';

	is.read(buf16,16);
	data._db = atof(buf16);

	is.read(buf16,16);
	data._deg = atof(buf16);
	return is;
}

RadiometryUncertaintyRecord::RadiometryUncertaintyRecord(const RadiometryUncertaintyRecord& rhs):
	_db(rhs._db),
	_deg(rhs._deg)
{
}

RadiometryUncertaintyRecord& RadiometryUncertaintyRecord::operator=(const RadiometryUncertaintyRecord& rhs)
{
	_db = rhs._db;
	_deg = rhs._deg;
	return *this;
}
}
