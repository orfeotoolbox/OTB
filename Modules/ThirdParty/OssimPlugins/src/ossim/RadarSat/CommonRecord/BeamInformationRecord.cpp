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


#include "BeamInformationRecord.h"

namespace ossimplugins
{
BeamInformationRecord::BeamInformationRecord()
{
}

BeamInformationRecord::~BeamInformationRecord()
{
}

std::ostream& operator<<(std::ostream& os, const BeamInformationRecord& data)
{
	os<<"beam_type:"<<data._beam_type <<std::endl;

	os<<"beam_look_src:"<<data._beam_look_src <<std::endl;

    os<<"beam_look_ang:"<<data._beam_look_ang<<std::endl;

	os<<"prf:"<<data._prf<<std::endl;

	return os;
}

std::istream& operator>>(std::istream& is, BeamInformationRecord& data)
{
	char buf[17];
	buf[16] = '\0';

	is.read(buf,3);
	buf[3] = '\0';
	data._beam_type = buf;

	is.read(buf,9);
	buf[9] = '\0';
	data._beam_look_src = buf;

    is.read(buf,16);
	buf[16] = '\0';
	data._beam_look_ang = atof(buf);

	is.read(buf,16);
	buf[16] = '\0';
	data._prf = atof(buf);

	return is;
}

BeamInformationRecord::BeamInformationRecord(const BeamInformationRecord& rhs):
	_beam_type(rhs._beam_type),
	_beam_look_src(rhs._beam_look_src),
	_beam_look_ang(rhs._beam_look_ang),
	_prf(rhs._prf)
{

}

BeamInformationRecord& BeamInformationRecord::operator=(const BeamInformationRecord& rhs)
{
	_beam_type = rhs._beam_type;
	_beam_look_src = rhs._beam_look_src;
	_beam_look_ang = rhs._beam_look_ang;
	_prf = rhs._prf;
	return *this;
}
}
