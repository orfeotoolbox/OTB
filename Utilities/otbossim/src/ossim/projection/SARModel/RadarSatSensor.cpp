#include <ossim/projection/SARModel/RadarSatSensor.h>
#include <ossim/projection/SARModel/DateTime/JSDDateTime.h>
#include <ossim/projection/SARModel/Ephemeris/Ephemeris.h>
#include <ossim/projection/SARModel/Ephemeris/GeographicEphemeris.h>
#include <ossim/projection/SARModel/Sensor.h>
#include <ossim/projection/SARModel/SensorParams.h>
#include <ossim/projection/SARModel/RefPoint.h>
#include <ossim/projection/SARModel/PlatformPosition.h>
#include <ossim/projection/SARModel/Equation.h>
#include <complex>
#include <ossim/projection/SARModel/Coordinates/RectangularCoordinate.h>
#include <ossim/projection/SARModel/Coordinates/GeodesicCoordinate.h>

RadarSatSensor::RadarSatSensor(SensorParams* params, PlatformPosition* position, RefPoint* refpoint) : Sensor(params, position, refpoint)
{	
}

RadarSatSensor::~RadarSatSensor()
{
}

double RadarSatSensor::getDistance(double col)
{
	const double RDR_CLUM        = 2.99792458e+8 ;
	return  _refpoint->get_distance() + _params->get_col_direction() * (col - (_refpoint->get_pix_col())) * ((RDR_CLUM / 2.0) / _params->get_sf()) ;
}

JSDDateTime RadarSatSensor::getTime(double line)
{
	double dt = _params->get_lin_direction() * (line - _refpoint->get_pix_line()) / _params->get_prf() ;
	JSDDateTime time = _refpoint->get_ephemeris()->get_date();
	time.set_second(time.get_second() + dt);
	time.NormDate();
	return time;
}

void RadarSatSensor::ImageToWorld(double imX, double imY, double& lon, double& lat)
{
	const double RDR_A_WGS84     = 6378137.0 ;
	const double RDR_B_WGS84     = 6356752.3141 ;

	double lambda = _params->get_rwl();
	double sensVisee = +1.0;

	double distance = getDistance(imX);
	JSDDateTime time = getTime(imY);

	Ephemeris* satPosition = _position->Interpolate(time);

	GeographicEphemeris *geoEph = (GeographicEphemeris*)satPosition;

	RectangularCoordinate cart;
	localisationApriori(*geoEph, lambda, distance, 0.0, sensVisee, RDR_A_WGS84 , RDR_B_WGS84 , 0, &cart);

	GeodesicCoordinate geo;
	cart.AsGeodesicCoordinates(RDR_A_WGS84 , RDR_B_WGS84, &geo);
	lon = geo.get_x();
	lat = geo.get_y();
}

int RadarSatSensor::localisationApriori ( GeographicEphemeris PosVit , double lambda ,
                        double dist , double fDop , int sensVisee ,
                        double rayonEqu , double rayonPol ,
                        double h , RectangularCoordinate* cart )
{
	const double KILO = 1.0e-3 ;
	const double MEGA = 1.0e-6 ;
	const double EPSILON = 1.0e-12 ;
	double coordCart[3];
  
	int* ordre;
	int etat , fin , n , i , i1 , i2 , nRac , indice[4] ;
	double xSat , ySat , zSat , vxSat , vySat , vzSat ;
	double dist1 , fDop1 , he , hp ;
	double rho2 , k , a , b , c , d , u , v , w ;
	double x[4] , y[4] , z[4] , r[4] ;
	std::complex<double> aa[5];
	std::complex<double> *racine;
	GeographicEphemeris PosVitGeo ;
  


	/* Initialisation des grandeurs caracteristiques            */
	/* remarque :  pour des raisons de precisions des calculs : */
	/*           - les distances sont traitees en Mm            */
	/*           - les vitesses sont traitees en km/s           */
	/*           - les frequences sont traitees en kHz          */
  
	etat  = 0 ;
  
	dist1 = dist * MEGA ;
  
	fDop1 = fDop * KILO ;
  
	he    = (rayonEqu + h) * MEGA ;       /* Rayon equatorial augmente de h */
	hp    = (rayonPol + h) * MEGA ;       /* Rayon polaire augmente de h    */

	xSat  = PosVit.get_position()[0] * MEGA ;
    ySat  = PosVit.get_position()[1] * MEGA ;
    zSat  = PosVit.get_position()[2] * MEGA ;
    vxSat = - PosVit.get_vitesse()[0] * KILO ;
    vySat = - PosVit.get_vitesse()[1] * KILO ;
    vzSat = - PosVit.get_vitesse()[2] * KILO ;


	/* Calcul des coefficients et resolution de l'equation */
	if (etat == 0)
	{
		rho2 = xSat * xSat + ySat * ySat + zSat * zSat ;
		k    = xSat * vxSat + ySat * vySat + zSat * vzSat - 
           lambda * dist1 * fDop1 / 2.0 ;

		u = vxSat * ySat - vySat * xSat ;
		a = (vxSat / u) * (1.0 - (he / hp) * (he / hp)) / 2.0 ;
		b = (vxSat * zSat - vzSat * xSat) / u ;
		c = (vxSat * (he * he + rho2 - dist1 * dist1) - 2.0 * k * xSat) / (2.0 * u);

		u     = vzSat - b * vySat ;
		v     = c * vySat - k ;
		w     = v * v - (vxSat * vxSat) * (he * he - c * c) ;
		aa[0] = std::complex<double>(w,0.0);                     /* Coefficient constant     */
		w     = 2.0 * (u * v - (b * c) * (vxSat * vxSat)) ;
		aa[1] = std::complex<double>(w,0.0) ;                     /* Coefficient 1er degre    */
		w     = u * u + 2.0 * a * v * vySat + 
            (vxSat * vxSat) * ((he / hp) * (he / hp) + b * b + 2.0 * a * c) ;
		aa[2] = std::complex<double>(w,0.0) ;                     /* Coefficient 2eme degre   */
		w     = 2.0 * a * (u * vySat - b * vxSat * vxSat) ;
		aa[3] = std::complex<double>(w,0.0) ;                     /* Coefficient 3eme degre   */
		w     = (vxSat * vxSat + vySat * vySat) * a * a ;
		aa[4] = std::complex<double>(w,0.0) ;                     /* Coefficient 4eme degre   */

		/**
		 * @todo Remplacer cette fonction par la classe Equation
		 */
		Equation eq(4,aa);
		eq.Solve();

		//equal ( 4 , aa , &n , racine , ordre ) ;     /* Resolution de l'equation */
		n = eq.get_nbrSol();
		racine = eq.get_solutions();
		ordre = eq.get_order();

		nRac = 0 ;
		for (i = 0 ; i < n ; i++)               /* Selection des racines reelles */
		{
			d = fabs(racine[i].imag()) ;
			if (d < EPSILON)
			{
				z[nRac] = racine[i].real();
				y[nRac] = (a * z[nRac] - b) * z[nRac] + c ;
				x[nRac] = (k - (vySat * y[nRac] + vzSat * z[nRac])) / vxSat ;
				nRac    = nRac + 1 ;
			}
		}
		if (nRac == 0)
			etat = 2 ;  /* Pas de racine */
	}

	/* Tri des racines calculees */
	if (etat == 0)
	{
		for (i = 0 ; i < nRac ; i++)
		{
			/* Calcul de la "distance" entre les images des racines */
			/* et les valeurs des equations                         */
			u = x[i] - xSat ;
			v = y[i] - ySat ;
			w = z[i] - zSat ;
			r[i] = fabs ((u * u + v * v + w * w) / (dist1 * dist1) - 1.0 ) ;
			u = u * vxSat ;
			v = v * vySat ;
			w = w * vzSat ;
			if (fabs (fDop) > EPSILON)
				r[i] = r[i] + fabs (1.0 + 2.0 * (u + v + w) / (lambda * dist1 * fDop1));
			else
				r[i] = r[i] + fabs (2.0 * (u + v + w) / (lambda * dist1)) ;
			u = x[i] / he ;
			v = y[i] / he ;
			w = z[i] / hp ;
			r[i] = r[i] + fabs (u * u + v * v + w * w - 1.0) ;
			indice[i] = i ;
		}
    
		/* Tri des racines par ecarts croissants  (tri "a bulles") */
		fin = 0 ;
		while (fin == 0)
		{
			fin = 1 ;
			for (i = 0 ; i < (nRac - 1) ; i++)
			{
				i1 = indice[i] ;
				i2 = indice[i+1] ;
				if (r[i2] < r[i1])
				{
					indice[i] = i2 ;
					indice[i+1] = i1 ;
					fin = 0 ;
				}
			}
		}
    
		/* Selection de la racine correspondant a la direction de visee */
		fin = 0 ;
		i = 0 ;
		while ((fin == 0) && (i < nRac))
		{
			i1 = indice[i] ;
			u  = ySat * vzSat - zSat * vySat ;
			v  = zSat * vxSat - xSat * vzSat ;
			w  = xSat * vySat - ySat * vxSat ;
			a  = x[i1] - xSat ;
			b  = y[i1] - ySat ;
			c  = z[i1] - zSat ;
			d  = (a * u + b * v + c * w) * sensVisee;  /* (double)sensVisee ; */
			if (d >= 0.0)
			{
				coordCart[0] = x[i1] / MEGA ;   /* Coordonnees dans le repere */
				coordCart[1] = y[i1] / MEGA ;   /* geographique,              */
				coordCart[2] = z[i1] / MEGA ;   /* en unites legales (m)      */
				fin = 1 ;
			}
			i++ ;
		}
		if (fin == 0)
			etat = 1 ;  /* Aucune racine dans la direction de visee */
	}

	cart->set_coordinates(coordCart[0], coordCart[1], coordCart[2]);
	return etat ;
}
