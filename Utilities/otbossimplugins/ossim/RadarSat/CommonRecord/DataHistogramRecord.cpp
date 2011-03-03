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

#include "DataHistogramRecord.h"

namespace ossimplugins
{

DataHistogramRecord::DataHistogramRecord():
	_hist(NULL)
{
}

DataHistogramRecord::~DataHistogramRecord()
{
	if (_hist != NULL)
		delete[] _hist;
}

DataHistogramRecord::DataHistogramRecord(const DataHistogramRecord& rhs):
	_hist_desc(rhs._hist_desc),
	_nrec(rhs._nrec),
	_tab_seq(rhs._tab_seq),
	_nbin(rhs._nbin),
	_ns_lin(rhs._ns_lin),
	_ns_pix(rhs._ns_pix),
	_ngrp_lin(rhs._ngrp_lin),
	_ngrp_pix(rhs._ngrp_pix),
	_nsamp_lin(rhs._nsamp_lin),
	_nsamp_pix(rhs._nsamp_pix),
	_min_smp(rhs._min_smp),
	_max_smp(rhs._max_smp),
	_mean_smp(rhs._mean_smp),
	_std_smp(rhs._std_smp),
	_smp_inc(rhs._smp_inc),
	_min_hist(rhs._min_hist),
	_max_hist(rhs._max_hist),
	_mean_hist(rhs._mean_hist),
	_std_hist(rhs._std_hist),
	_nhist(rhs._nhist)
{
	_hist = new int[rhs._nhist];
	for (int i=0;i<rhs._nhist;i++)
	{
		_hist[i] = rhs._hist[i];
	}
}

DataHistogramRecord& DataHistogramRecord::operator=(const DataHistogramRecord& rhs)
{
	_hist_desc = rhs._hist_desc;
	_nrec = rhs._nrec;
	_tab_seq = rhs._tab_seq;
	_nbin = rhs._nbin;
	_ns_lin = rhs._ns_lin;
	_ns_pix = rhs._ns_pix;
	_ngrp_lin = rhs._ngrp_lin;
	_ngrp_pix = rhs._ngrp_pix;
	_nsamp_lin = rhs._nsamp_lin;
	_nsamp_pix = rhs._nsamp_pix;
	_min_smp = rhs._min_smp;
	_max_smp = rhs._max_smp;
	_mean_smp = rhs._mean_smp;
	_std_smp = rhs._std_smp;
	_smp_inc = rhs._smp_inc;
	_min_hist = rhs._min_hist;
	_max_hist = rhs._max_hist;
	_mean_hist = rhs._mean_hist;
	_std_hist = rhs._std_hist;
	_nhist = rhs._nhist;
	return *this;
}

std::ostream& operator<<(std::ostream& os, const DataHistogramRecord& data)
{
	os<<"hist_desc:"<<data._hist_desc.c_str()<<std::endl;

    os<<"nrec:"<<data._nrec<<std::endl;

    os<<"tab_seq:"<<data._tab_seq<<std::endl;

    os<<"nbin:"<<data._nbin<<std::endl;

    os<<"ns_lin:"<<data._ns_lin<<std::endl;

    os<<"ns_pix:"<<data._ns_pix<<std::endl;

    os<<"ngrp_lin:"<<data._ngrp_lin<<std::endl;

    os<<"ngrp_pix:"<<data._ngrp_pix<<std::endl;

    os<<"nsamp_lin:"<<data._nsamp_lin<<std::endl;

    os<<"nsamp_pix:"<<data._nsamp_pix<<std::endl;

    os<<"min_smp:"<<data._min_smp<<std::endl;

    os<<"max_smp:"<<data._max_smp<<std::endl;

    os<<"mean_smp:"<<data._mean_smp<<std::endl;

    os<<"std_smp:"<<data._std_smp<<std::endl;

    os<<"smp_inc:"<<data._smp_inc<<std::endl;

    os<<"min_hist:"<<data._min_hist<<std::endl;

    os<<"max_histd:"<<data._max_hist<<std::endl;

    os<<"mean_hist:"<<data._mean_hist<<std::endl;

    os<<"std_hist:"<<data._std_hist<<std::endl;

    os<<"nhist:"<<data._nhist<<std::endl;

	for(int i=0;i<data._nhist;i++)
	{
		os<<"his["<<i<<"]:"<<data._hist[i]<<std::endl;
	}
	return os;
}

std::istream& operator>>(std::istream& is, DataHistogramRecord& data)
{
	char buff[33];
	buff[32] = '\0';

	is.read(buff,32);
	data._hist_desc = buff;

    is.read(buff,4);
	buff[4] = '\0';
	data._nrec = atoi(buff);

    is.read(buff,4);
	buff[4] = '\0';
	data._tab_seq = atoi(buff);

    is.read(buff,8);
	buff[8] = '\0';
	data._nbin = atoi(buff);

    is.read(buff,8);
	buff[8] = '\0';
	data._ns_lin = atoi(buff);

    is.read(buff,8);
	buff[8] = '\0';
	data._ns_pix = atoi(buff);

    is.read(buff,8);
	buff[8] = '\0';
	data._ngrp_lin = atoi(buff);

    is.read(buff,8);
	buff[8] = '\0';
	data._ngrp_pix = atoi(buff);

    is.read(buff,8);
	buff[8] = '\0';
	data._nsamp_lin = atoi(buff);

    is.read(buff,8);
	buff[8] = '\0';
	data._nsamp_pix = atoi(buff);

    is.read(buff,16);
	buff[16] = '\0';
	data._min_smp = atof(buff);

    is.read(buff,16);
	buff[16] = '\0';
	data._max_smp = atof(buff);

    is.read(buff,16);
	buff[16] = '\0';
	data._mean_smp = atof(buff);

    is.read(buff,16);
	buff[16] = '\0';
	data._std_smp = atof(buff);

    is.read(buff,16);
	buff[16] = '\0';
	data._smp_inc = atof(buff);

    is.read(buff,16);
	buff[16] = '\0';
	data._min_hist = atof(buff);

    is.read(buff,16);
	buff[16] = '\0';
	data._max_hist = atof(buff);

    is.read(buff,16);
	buff[16] = '\0';
	data._mean_hist = atof(buff);

    is.read(buff,16);
	buff[16] = '\0';
	data._std_hist = atof(buff);

    is.read(buff,8);
	buff[8] = '\0';
	data._nhist = atoi(buff);

	if(data._hist != NULL)
	{
		delete[] data._hist;
	}

	//for (int i=0;i<data._nhist;i++)
	int nhist ;
	if (data._nhist == 256)
		{  nhist = 256 ; } // Signal Data
	else {nhist = 1024 ; } // Processed Data

	data._hist = new int[nhist];
	for (int i=0;i<nhist;i++)
	{
		is.read(buff,8);
		buff[8] = '\0';
		data._hist[i] = atoi(buff);
	}

	return is;
}
}
