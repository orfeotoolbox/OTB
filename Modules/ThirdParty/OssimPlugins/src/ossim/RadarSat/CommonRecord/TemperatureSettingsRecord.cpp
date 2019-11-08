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


#include "TemperatureSettingsRecord.h"

namespace ossimplugins
{
TemperatureSettingsRecord::TemperatureSettingsRecord()
{
}

TemperatureSettingsRecord::~TemperatureSettingsRecord()
{
}

std::ostream& operator<<(std::ostream& os, const TemperatureSettingsRecord& data)
{
	for (int i=0;i<4;i++)
	{
		os<<"temp_set["<<i<<"]:"<<data._temp_set[i]<<std::endl;
	}

	return os;
}

std::istream& operator>>(std::istream& is, TemperatureSettingsRecord& data)
{
	char buf[5];
	buf[4] = '\0';

	for (int i=0;i<4;i++)
	{
		is.read(buf,4);
		buf[4] = '\0';
		data._temp_set[i] = atoi(buf);
	}
	return is;
}

TemperatureSettingsRecord::TemperatureSettingsRecord(const TemperatureSettingsRecord& rhs)
{
	for (int i=0;i<4;i++)
	{
		_temp_set[i] = rhs._temp_set[i];
	}
}

TemperatureSettingsRecord& TemperatureSettingsRecord::operator=(const TemperatureSettingsRecord& rhs)
{
	for (int i=0;i<4;i++)
	{
		_temp_set[i] = rhs._temp_set[i];
	}
	return *this;
}
}
