#include <ossim/projection/SARModel/PlatformPosition.h>
#include <ossim/projection/SARModel/Ephemeris/Ephemeris.h>
#include <ossim/projection/SARModel/Interpolator/HermiteInterpolator.h>

#include <string>
#include <math.h>

PlatformPosition::PlatformPosition():
	_nbrData(0),
	_data(NULL)
{
}

PlatformPosition::~PlatformPosition()
{
	Clear();
}

void PlatformPosition::Clear()
{
	if(_data != NULL)
	{
		for (int i=0;i<_nbrData;i++)
		{
			delete _data[i];
		}
		delete _data;
	}
	_data = NULL;
	_nbrData = 0;
}

PlatformPosition::PlatformPosition(const PlatformPosition& rhs)
{
	_data = new Ephemeris*[rhs._nbrData];
	_nbrData = rhs._nbrData;
	for (int i=0;i<rhs._nbrData;i++)
	{
		_data[i] = rhs._data[i]->Clone();
	}
}
	
PlatformPosition& PlatformPosition::operator=(const PlatformPosition& rhs)
{
	Clear();
	_data = new Ephemeris*[rhs._nbrData];
	_nbrData = rhs._nbrData;
	for (int i=0;i<rhs._nbrData;i++)
	{
		_data[i] = rhs._data[i]->Clone();
	}
	
	return *this;
}

PlatformPosition::PlatformPosition(Ephemeris** data, int nbrData):
	_nbrData(nbrData)
{
	_data = new Ephemeris*[_nbrData];
	for (int i=0;i<_nbrData;i++)
	{
		_data[i] = data[i]->Clone();
	}
}

Ephemeris* PlatformPosition::Interpolate(JSDDateTime date)
{
	const double RDR_JOURCIVIL = 86400.0 ;
	Ephemeris* ephem = NULL;
	if (_nbrData<=1)
	{
		ephem = NULL;
	}
	else
	{
		/*
		 * On clone le premier element de la liste pour etre sure que l'ephemeride de sortie a le même système de coordonnées que les ephemerides d'entré
		 */
		ephem = _data[0]->Clone();

		/* CAS NORMAL */
		/*------------*/
		double * x = new double[_nbrData];
		double * y = new double[_nbrData];
		double * yd = new double[_nbrData];
		double dt = 0.0;
		bool echIrreg = false;
		double d;

		x[0] = 0.0 ;
		for (int i = 1 ; i < _nbrData ; i++)
		{
			x[i] =   (_data[i]->get_date().get_day0hTU().get_julianDate() - _data[0]->get_date().get_day0hTU().get_julianDate())
             * RDR_JOURCIVIL
			 + _data[i]->get_date().get_second()   - _data[0]->get_date().get_second()
             + _data[i]->get_date().get_decimal()     - _data[0]->get_date().get_decimal();
			d = x[i] - x[i-1] ;

			/*
			 * Temps non croissant
			 */
			if (d <= 0.0)/* Temps non croissants */
			{
				delete ephem;
				ephem = NULL;
			}
			else if (i == 1)
			{
				 dt = d ;
			}
			else if (fabs (d-dt) >= 1.0e-9) /* Echentillonnage irregulier*/
			{
				echIrreg = true; 
			}
		}
      
		if (ephem != NULL)
		{
			ephem->set_date(date);

			dt =  (date.get_day0hTU().get_julianDate() - _data[0]->get_date().get_day0hTU().get_julianDate()) * RDR_JOURCIVIL
			+ date.get_second()   - _data[0]->get_date().get_second()
			+ date.get_decimal()     - _data[0]->get_date().get_decimal();
            
			/* Si nPts impair ou si la date cherchee n'est situee */
			/* au milieu de la liste d'ephemerides  ->  Lagrange  */
			d = (dt - x[_nbrData/2-1]) / (x[_nbrData/2] - x[_nbrData/2-1]) ;

    
			/* Calcul par Everett  */
			/*---------------------*/
			double pos[3];
			double vit[3];
			for (int j = 0 ; j < 3 ; j++)
			{
				for (int i = 0 ; i < _nbrData ; i++)
				{ 
					y[i] = _data[i]->get_position()[j] ;
					yd[i] = _data[i]->get_vitesse()[j] ;
				}
				HermiteInterpolator interpolator(_nbrData,x,y,yd);
				interpolator.Interpolate(dt, pos[j], vit[j]);
				
			}
			ephem->set_position(pos);
			ephem->set_vitesse(vit);
    
		}

		delete[] x;
		delete[] y;
		delete[] yd;

	}
	return ephem;
}