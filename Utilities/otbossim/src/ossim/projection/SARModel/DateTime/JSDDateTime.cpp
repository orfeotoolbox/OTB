#include <ossim/projection/SARModel/DateTime/JSDDateTime.h>
#include <ossim/projection/SARModel/DateTime/CivilDateTime.h>
#include <ossim/projection/SARModel/DateTime/JulianDate.h>
#include <ossim/projection/SARModel/DateTime/GMSTDateTime.h>

#include <ossim/projection/SARModel/Arithmetic.h>

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

JSDDateTime::JSDDateTime():
	_day0hTU(0),
	_second(0.0),
	_decimal(0.0)
{
}

JSDDateTime::~JSDDateTime()
{
}

JSDDateTime::JSDDateTime(const JSDDateTime& rhs):
	_day0hTU(rhs._day0hTU),
	_second(rhs._second),
	_decimal(rhs._decimal)
{
}

JSDDateTime::JSDDateTime(JulianDate day0hTU, double second, double decimal):
	_day0hTU(day0hTU),
	_second(second),
	_decimal(decimal)
{
}
	
JSDDateTime::JSDDateTime(CivilDateTime& rhs)
{
	rhs.AsJSDDateTime(this);
}

JSDDateTime::JSDDateTime(JulianDate& rhs)
{
	rhs.AsJSDDateTime(this);
}

JSDDateTime& JSDDateTime::operator=(const JSDDateTime& rhs)
{
	_day0hTU = rhs._day0hTU;
	_second = rhs._second;
	_decimal = rhs._decimal;
	
	return *this;
}

int JSDDateTime::AsJulianDate(JulianDate* julian)
{
	double jourJulien = _day0hTU.get_julianDate() + (_second + _decimal) / CivilDateTime::RDR_JOURCIVIL ;
	julian->set_julianDate(jourJulien);

	return 0 ;
}

int JSDDateTime::AsGMSTDateTime(GMSTDateTime* GMST)
{
	const double RDR_DEUXPI      = 6.28318530717958647693 ;
	const double RDR_OMEGATERRE  = 6.28318530717958647693 / 86164.09054 ;
	
	int etat ;
	JulianDate jourJulien;
	double  t , tsm0hTU, tsm ;
	JSDDateTime date0hTU ;


	date0hTU         = *this ;
	date0hTU.set_second(0);
	date0hTU.set_decimal(0.0);

	etat = date0hTU.AsJulianDate(&jourJulien ) ;

	if (etat == 0)
	{
		if (GMST->get_origine() == GMSTDateTime::AN1900)
		{
			t       = (jourJulien.get_julianDate() - 2415020.0) / 36525.0 ;
			tsm0hTU = 23925.836 + t * (8640184.542 + t * 0.0929) ;
			tsm0hTU = tsm0hTU * RDR_DEUXPI / CivilDateTime::RDR_JOURCIVIL ;
		}
		else if (GMST->get_origine() == GMSTDateTime::AN1950)
		{
			t       = jourJulien.get_julianDate() - 2433281.5 ;
			tsm0hTU = 1.72944494 + t * 6.3003880987 ;
		}
		else if (GMST->get_origine() == GMSTDateTime::AN2000)
		{
			t       = (jourJulien.get_julianDate() - 2451545.0) / 36525.0 ;
			tsm0hTU =   24110.54841 + t * (8640184.812866 
                + t * (0.093103 - t * 6.210e-6)) ;
			tsm0hTU = tsm0hTU * RDR_DEUXPI / CivilDateTime::RDR_JOURCIVIL ;
		}
		t = tsm0hTU + (_second + _decimal) * RDR_OMEGATERRE ;
		tsm = fmod (t,RDR_DEUXPI) ;
		if (tsm < 0.0)
		{
			tsm = tsm + RDR_DEUXPI ;
		}
		
		GMST->set_tms(tsm);
	}

	return etat ;
}

void JSDDateTime::NormDate()
{
	const double epsilon = 1.0e-12 ;
  
	int k, iu, iv, iw ;
	double u, v, w, ru, rv, rw ;


	w = _day0hTU.get_julianDate() - 0.5 ;
	if (fabs (w - fnint (w)) < epsilon)
	{
		iw = (int)inint(w) ;
		rw = 0.0 ;
	}
	else
	{
		iw = (int)w ;
		rw = w - (double)iw ;
		if ((iw < 0) && (rw != 0.0))
		{
		iw = iw - 1 ;
		rw = rw + 1.0 ;
		}
	}

	v = _second + rw * CivilDateTime::RDR_JOURCIVIL ;
	if (fabs(v - fnint (v)) < epsilon)
	{
		iv = (int)inint (v) ;
		rv = 0.0 ;
	}
	else
	{
		iv = (int)v ;
		rv = v - (double)iv ;
		if ((iv < 0) && (rv != 0.0))
		{
			iv = iv - 1 ;
			rv = rv + 1.0 ;
		}
	}

	u  = _decimal + rv ;
	iu = (int)u ;
	ru = u - (double)iu ;
	if ((iu < 0) && (ru != 0.0))
	{
		iu = iu - 1 ;
		ru = ru + 1.0 ;
	}

	iv = iv + iu ;
	k  = iv / ((int)inint(CivilDateTime::RDR_JOURCIVIL)) ;
	iw = iw + k ;
	iv = iv - k * ((int)inint(CivilDateTime::RDR_JOURCIVIL)) ;
	if (iv < 0)
	{
		iw = iw - 1 ;
		iv = iv + (int)inint(CivilDateTime::RDR_JOURCIVIL) ;
	}

	_decimal    = ru ;
	_second  = (double)iv ;
	_day0hTU = (double)iw + 0.5 ;
}

JSDDateTime::operator JulianDate()
{
	JulianDate date;
	this->AsJulianDate(&date);
	return date;
}

JSDDateTime::operator GMSTDateTime()
{
	GMSTDateTime date;
	this->AsGMSTDateTime(&date);
	return date;
}