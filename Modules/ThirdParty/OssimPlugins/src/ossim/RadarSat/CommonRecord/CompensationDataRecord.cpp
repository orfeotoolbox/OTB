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


#include "CompensationDataRecord.h"

namespace ossimplugins
{
CompensationDataRecord::CompensationDataRecord()
{
}

CompensationDataRecord::~CompensationDataRecord()
{
}

std::ostream& operator<<(std::ostream& os, const CompensationDataRecord& data)
{
	os<<"comp_desig:"<<data._comp_desig <<std::endl;

	os<<"comp_descr:"<<data._comp_descr <<std::endl;

    os<<"n_comp_rec:"<<data._n_comp_rec<<std::endl;

    os<<"comp_seq_no:"<<data._comp_seq_no<<std::endl;

    os<<"beam_tab_size:"<<data._beam_tab_size<<std::endl;

	for (int i=0;i<256;i++)
	{
		os<<"beam_tab["<<i<<"]:"<<data._beam_tab[i]<<std::endl;
	}

    os<<"beam_type:"<<data._beam_type <<std::endl;

    os<<"look_angle:"<<data._look_angle<<std::endl;

    os<<"beam_tab_inc:"<<data._beam_tab_inc<<std::endl;
	return os;
}

std::istream& operator>>(std::istream& is, CompensationDataRecord& data)
{
	char buf[33];
	buf[32] = '\0';


	is.read(buf,8);
	buf[8] = '\0';
	data._comp_desig = buf;

	is.read(buf,32);
	buf[32] = '\0';
	data._comp_descr = buf;

    is.read(buf,4);
	buf[4] = '\0';
	data._n_comp_rec = atoi(buf);

    is.read(buf,4);
	buf[4] = '\0';
	data._comp_seq_no = atoi(buf);

    is.read(buf,8);
	buf[8] = '\0';
	data._beam_tab_size = atoi(buf);

	for (int i=0;i<256;i++)
	{
		is.read(buf,16);
		buf[16] = '\0';
		data._beam_tab[i] = atof(buf);
	}

    is.read(buf,16);
	buf[16] = '\0';
	data._beam_type = buf;

    is.read(buf,16);
	buf[16] = '\0';
	data._look_angle = atof(buf);

    is.read(buf,16);
	buf[16] = '\0';
	data._beam_tab_inc = atof(buf);

	return is;
}

CompensationDataRecord::CompensationDataRecord(const CompensationDataRecord& rhs)
{
	_comp_desig = rhs._comp_desig;

	_comp_descr = rhs._comp_descr;

    _n_comp_rec = rhs._n_comp_rec;

    _comp_seq_no = rhs._comp_seq_no;

    _beam_tab_size = rhs._beam_tab_size;

	for (int i=0;i<256;i++)
	{
		_beam_tab[i] = rhs._beam_tab[i];
	}

    _beam_type = rhs._beam_type;

    _look_angle = rhs._look_angle;

    _beam_tab_inc = rhs._beam_tab_inc;
}

CompensationDataRecord& CompensationDataRecord::operator=(const CompensationDataRecord& rhs)
{
	_comp_desig = rhs._comp_desig;

	_comp_descr = rhs._comp_descr;

    _n_comp_rec = rhs._n_comp_rec;

    _comp_seq_no = rhs._comp_seq_no;

    _beam_tab_size = rhs._beam_tab_size;

	for (int i=0;i<256;i++)
	{
		_beam_tab[i] = rhs._beam_tab[i];
	}

    _beam_type = rhs._beam_type;

    _look_angle = rhs._look_angle;

    _beam_tab_inc = rhs._beam_tab_inc;

	return *this;
}
}
