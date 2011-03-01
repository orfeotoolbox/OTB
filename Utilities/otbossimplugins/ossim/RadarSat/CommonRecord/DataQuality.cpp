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

#include "DataQuality.h"

namespace ossimplugins
{
DataQuality::DataQuality() : RadarSatRecord("qual_sum_rec")
{
}

DataQuality::~DataQuality()
{
}

std::ostream& operator<<(std::ostream& os, const DataQuality& data)
{

	os<<"rec_seq:"<<data._rec_seq<<std::endl;

	os<<"sar_chn:"<<data._sar_chn.c_str()<<std::endl;

    os<<"cali_date:"<<data._cali_date.c_str()<<std::endl;

    os<<"nchn:"<<data._nchn<<std::endl;

    os<<"islr:"<<data._islr<<std::endl;

    os<<"pslr:"<<data._pslr<<std::endl;

    os<<"azi_ambig:"<<data._azi_ambig<<std::endl;

    os<<"rng_ambig:"<<data._rng_ambig<<std::endl;

    os<<"snr:"<<data._snr<<std::endl;

    os<<"ber:"<<data._ber<<std::endl;

    os<<"rng_res:"<<data._rng_res<<std::endl;

    os<<"azi_res:"<<data._azi_res<<std::endl;

    os<<"rad_res:"<<data._rad_res<<std::endl;

    os<<"dyn_rng:"<<data._dyn_rng<<std::endl;

    os<<"rad_unc_db:"<<data._rad_unc_db<<std::endl;

    os<<"rad_unc_deg:"<<data._rad_unc_deg<<std::endl;

	for (int i=0;i<16;i++)
	{
		os<<"rad_unc:"<<data._rad_unc[i]<<std::endl;
	}

    os<<"alt_locerr:"<<data._alt_locerr<<std::endl;

    os<<"crt_locerr:"<<data._crt_locerr<<std::endl;

    os<<"alt_scale:"<<data._alt_scale<<std::endl;

    os<<"crt_scale:"<<data._crt_scale<<std::endl;

    os<<"dis_skew:"<<data._dis_skew<<std::endl;

    os<<"ori_err:"<<data._ori_err<<std::endl;

	for (int i=0;i<16;i++)
	{
		os<<"misreg:"<<data._misreg[i]<<std::endl;
	}

	os<<"nesz:"<<data._nesz<<std::endl;

    os<<"enl:"<<data._enl<<std::endl;

    os<<"tb_update:"<<data._tb_update.c_str()<<std::endl;


	return os;
}

std::istream& operator>>(std::istream& is, DataQuality& data)
{
	char buf[2149];
	buf[2148] = '\0';


	is.read(buf,4);
	buf[4] = '\0';
	data._rec_seq = atoi(buf);

	is.read(buf,4);
	buf[4] = '\0';
	data._sar_chn = buf;

    is.read(buf,6);
	buf[6] = '\0';
	data._cali_date = buf;

    is.read(buf,4);
	buf[4] = '\0';
	data._nchn = atoi(buf);

    is.read(buf,16);
	buf[16] = '\0';
	data._islr = atof(buf);

    is.read(buf,16);
	buf[16] = '\0';
	data._pslr = atof(buf);

    is.read(buf,16);
	buf[16] = '\0';
	data._azi_ambig = atof(buf);

    is.read(buf,16);
	buf[16] = '\0';
	data._rng_ambig = atof(buf);

    is.read(buf,16);
	buf[16] = '\0';
	data._snr = atof(buf);

    is.read(buf,16);
	buf[16] = '\0';
	data._ber = atof(buf);

    is.read(buf,16);
	buf[16] = '\0';
	data._rng_res = atof(buf);

    is.read(buf,16);
	buf[16] = '\0';
	data._azi_res = atof(buf);

    is.read(buf,16);
	buf[16] = '\0';
	data._rad_res = atof(buf);

    is.read(buf,16);
	buf[16] = '\0';
	data._dyn_rng = atof(buf);

    is.read(buf,16);
	buf[16] = '\0';
	data._rad_unc_db = atof(buf);

    is.read(buf,16);
	buf[16] = '\0';
	data._rad_unc_deg = atof(buf);

	for (int i=0;i<16;i++)
	{
		is>>data._rad_unc[i];
	}

    is.read(buf,16);
	buf[16] = '\0';
	data._alt_locerr = atof(buf);

    is.read(buf,16);
	buf[16] = '\0';
	data._crt_locerr = atof(buf);

    is.read(buf,16);
	buf[16] = '\0';
	data._alt_scale = atof(buf);

    is.read(buf,16);
	buf[16] = '\0';
	data._crt_scale = atof(buf);

    is.read(buf,16);
	buf[16] = '\0';
	data._dis_skew = atof(buf);

    is.read(buf,16);
	buf[16] = '\0';
	data._ori_err = atof(buf);

	for (int i=0;i<16;i++)
	{
		is>>data._misreg[i];
	}

	is.read(buf,16);
	buf[16] = '\0';
	data._nesz = atof(buf);

    is.read(buf,16);
	buf[16] = '\0';
	data._enl = atof(buf);

    is.read(buf,8);
	buf[8] = '\0';
	data._tb_update = buf;

    is.read(buf,238);


	return is;
}

DataQuality::DataQuality(const DataQuality& rhs):
	RadarSatRecord(rhs),
	_rec_seq(rhs._rec_seq),
	_sar_chn(rhs._sar_chn),
    _cali_date(rhs._cali_date),
    _nchn(rhs._nchn),
    _islr(rhs._islr),
    _pslr(rhs._pslr),
    _azi_ambig(rhs._azi_ambig),
    _rng_ambig(rhs._rng_ambig),
    _snr(rhs._snr),
    _ber(rhs._ber),
    _rng_res(rhs._rng_res),
    _azi_res(rhs._azi_res),
    _rad_res(rhs._rad_res),
    _dyn_rng(rhs._dyn_rng),
    _rad_unc_db(rhs._rad_unc_db),
    _rad_unc_deg(rhs._rad_unc_deg),
    _alt_locerr(rhs._alt_locerr),
    _crt_locerr(rhs._crt_locerr),
    _alt_scale(rhs._alt_scale),
    _crt_scale(rhs._crt_scale),
    _dis_skew(rhs._dis_skew),
    _ori_err(rhs._ori_err),
	_nesz(rhs._nesz),
    _enl(rhs._enl),
    _tb_update(rhs._tb_update)
{
	for (int i=0;i<16;i++)
	{
		_rad_unc[i] = rhs._rad_unc[i];
	}
	for (int i=0;i<16;i++)
	{
		_misreg[i] = rhs._misreg[i];
	}
}

DataQuality& DataQuality::operator=(const DataQuality& rhs)
{
	_rec_seq = rhs._rec_seq;
	_sar_chn = rhs._sar_chn;
    _cali_date = rhs._cali_date;
    _nchn = rhs._nchn;
    _islr = rhs._islr;
    _pslr = rhs._pslr;
    _azi_ambig = rhs._azi_ambig;
    _rng_ambig = rhs._rng_ambig;
    _snr = rhs._snr;
    _ber = rhs._ber;
    _rng_res = rhs._rng_res;
    _azi_res = rhs._azi_res;
    _rad_res = rhs._rad_res;
    _dyn_rng = rhs._dyn_rng;
    _rad_unc_db = rhs._rad_unc_db;
    _rad_unc_deg = rhs._rad_unc_deg;

	for (int i=0;i<16;i++)
	{
		_rad_unc[i] = rhs._rad_unc[i];
	}

    _alt_locerr = rhs._alt_locerr;
    _crt_locerr = rhs._crt_locerr;
    _alt_scale = rhs._alt_scale;
    _crt_scale = rhs._crt_scale;
    _dis_skew = rhs._dis_skew;
    _ori_err = rhs._ori_err;

	for (int i=0;i<16;i++)
	{
		_misreg[i] = rhs._misreg[i];
	}

	_nesz = rhs._nesz;
    _enl = rhs._enl;
    _tb_update = rhs._tb_update;

	return *this;
}
}
