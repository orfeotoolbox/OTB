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

#include "DataHistogramSignalData.h"

namespace ossimplugins
{

DataHistogramSignalData::DataHistogramSignalData() : RadarSatRecord("sdr_hist_rec")
{
}

DataHistogramSignalData::~DataHistogramSignalData()
{
}

std::ostream& operator<<(std::ostream& os, const DataHistogramSignalData& data)
{

	os<<"rec_seq:"<<data._rec_seq<<std::endl;

    os<<"sar_chn:"<<data._sar_chn<<std::endl;

	os<<"ntab:"<<data._ntab<<std::endl;

	os<<"ltab:"<<data._ltab<<std::endl;

	os<<data._histogram<<std::endl;

	return os;
}

std::istream& operator>>(std::istream& is, DataHistogramSignalData& data)
{
	char buff[33];
	buff[32] = '\0';


	is.read(buff,4);
	buff[4] = '\0';
	data._rec_seq = atoi(buff);

    is.read(buff,4);
	buff[4] = '\0';
	data._sar_chn = atoi(buff);

	is.read(buff,8);
	buff[8] = '\0';
	data._ntab = atoi(buff);

	is.read(buff,8);
	buff[8] = '\0';
	data._ltab = atoi(buff);

	is>>data._histogram;

	char tmp[14589];
	tmp[14588] = '\0';
	is.read(tmp, 14588);

	return is;
}

DataHistogramSignalData::DataHistogramSignalData(const DataHistogramSignalData& rhs) :
	RadarSatRecord(rhs),
	_rec_seq(rhs._rec_seq),
	_sar_chn(rhs._sar_chn),
	_ntab(rhs._ntab),
	_ltab(rhs._ltab),
	_histogram(rhs._histogram)
{

}

DataHistogramSignalData& DataHistogramSignalData::operator=(const DataHistogramSignalData& rhs)
{
	_histogram = rhs._histogram;
	_sar_chn = rhs._sar_chn;
	_rec_seq = rhs._rec_seq;
	_ntab = rhs._ntab;
	_ltab = rhs._ltab;

	return *this;
}
}
