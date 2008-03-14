#include <ossim/projection/SARModel/Ephemeris/GeographicEphemeris.h>
#include <ossim/projection/SARModel/Ephemeris/GalileanEphemeris.h>

#include <ossim/projection/SARModel/DateTime/GMSTDateTime.h>

#include <math.h>

GeographicEphemeris::GeographicEphemeris() : Ephemeris()
{
}

GeographicEphemeris::~GeographicEphemeris()
{
}

GeographicEphemeris::GeographicEphemeris(JSDDateTime date, double pos[3], double vitesse[3]) : Ephemeris(date, pos, vitesse)
{
}

GeographicEphemeris::GeographicEphemeris(const GeographicEphemeris& rhs) : Ephemeris(rhs)
{
}
	
GeographicEphemeris& GeographicEphemeris::operator=(const GeographicEphemeris& rhs)
{
	((Ephemeris)*this) = ((Ephemeris)rhs);
	return *this;
}


void GeographicEphemeris::ToGalilean(GalileanEphemeris* vGal)
{
	const double RDR_OMEGATERRE  = 6.28318530717958647693 / 86164.09054 ;
	GMSTDateTime h;
	h.set_origine(GMSTDateTime::AN1950);
	
	double s,c;
	_date.AsGMSTDateTime(&h) ;
	
    c = cos (h.get_tms()) ;
    s = sin (h.get_tms()) ;
	vGal->set_date(_date);
	
	double pos[3];
	double vitesse[3];
	
    pos[0] = _position[0] * c - _position[1] * s ;
    pos[1] = _position[0] * s + _position[1] * c ;
    pos[2] = _position[2] ;
	
    vitesse[0]  =  _vitesse[0]  * c -  _vitesse[1] * s - RDR_OMEGATERRE * (_position[0] * s + _position[1] * c) ;
    vitesse[1]  =  _vitesse[0]  * s +  _vitesse[1] * c + RDR_OMEGATERRE * (_position[0] * c - _position[1] * s) ;
    vitesse[2]  = _vitesse[2] ;
	
	vGal->set_position(pos);
    vGal->set_vitesse(vitesse);
}

GeographicEphemeris::operator GalileanEphemeris()
{
	GalileanEphemeris rhs;
	ToGalilean(&rhs);
	return rhs;
}

GeographicEphemeris::GeographicEphemeris(GalileanEphemeris& rhs)
{
	rhs.ToGeographic(this);
}
