//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$

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
	os<<"comp_desig:"<<data._comp_desig.c_str()<<std::endl;

	os<<"comp_descr:"<<data._comp_descr.c_str()<<std::endl;

    os<<"n_comp_rec:"<<data._n_comp_rec<<std::endl;

    os<<"comp_seq_no:"<<data._comp_seq_no<<std::endl;

    os<<"beam_tab_size:"<<data._beam_tab_size<<std::endl;

	for (int i=0;i<256;i++)
	{
		os<<"beam_tab["<<i<<"]:"<<data._beam_tab[i]<<std::endl;
	}

    os<<"beam_type:"<<data._beam_type.c_str()<<std::endl;

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
