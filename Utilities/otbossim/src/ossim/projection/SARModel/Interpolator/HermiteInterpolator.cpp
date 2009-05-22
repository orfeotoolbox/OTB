#include <ossim/projection/SARModel/Interpolator/HermiteInterpolator.h>

#include <string>

HermiteInterpolator::HermiteInterpolator():
	_nbrPoints(0),
	_x(NULL),
	_y(NULL),
	_dy(NULL)
{
}
	
HermiteInterpolator::HermiteInterpolator(int nbrPoints, double* x, double* y, double* dy):
	_nbrPoints(nbrPoints)
{
	if(x != NULL)
	{
		_x = new double[_nbrPoints];
		for (int i=0;i<_nbrPoints;i++)
		{
			_x[i] = x[i];
		}
	}
	else
	{
		_x = NULL;
	}
	
	if(y != NULL)
	{
		_y = new double[_nbrPoints];
		for (int i=0;i<_nbrPoints;i++)
		{
			_y[i] = y[i];
		}
	}
	else
	{
		_y = NULL;
	}
	
	if(dy != NULL)
	{
		_dy = new double[_nbrPoints];
		for (int i=0;i<_nbrPoints;i++)
		{
			_dy[i] = dy[i];
		}
	}
	else
	{
		_dy = NULL;
	}
}
	
HermiteInterpolator::~HermiteInterpolator()
{
	Clear();
}
	
HermiteInterpolator::HermiteInterpolator(const HermiteInterpolator& rhs):
	_nbrPoints(rhs._nbrPoints)
{
	if(rhs._x != NULL)
	{
		_x = new double[_nbrPoints];
		for (int i=0;i<_nbrPoints;i++)
		{
			_x[i] = rhs._x[i];
		}
	}
	else
	{
		_x = NULL;
	}
	
	if(rhs._y != NULL)
	{
		_y = new double[_nbrPoints];
		for (int i=0;i<_nbrPoints;i++)
		{
			_y[i] = rhs._y[i];
		}
	}
	else
	{
		_y = NULL;
	}
	
	if(rhs._dy != NULL)
	{
		_dy = new double[_nbrPoints];
		for (int i=0;i<_nbrPoints;i++)
		{
			_dy[i] = rhs._dy[i];
		}
	}
	else
	{
		_dy = NULL;
	}
}
	
HermiteInterpolator& HermiteInterpolator::operator =(const HermiteInterpolator& rhs)
{
	Clear();
	_nbrPoints = rhs._nbrPoints;
	if(rhs._x != NULL)
	{
		_x = new double[_nbrPoints];
		for (int i=0;i<_nbrPoints;i++)
		{
			_x[i] = rhs._x[i];
		}
	}
	else
	{
		_x = NULL;
	}
	
	if(rhs._y != NULL)
	{
		_y = new double[_nbrPoints];
		for (int i=0;i<_nbrPoints;i++)
		{
			_y[i] = rhs._y[i];
		}
	}
	else
	{
		_y = NULL;
	}
	
	if(rhs._dy != NULL)
	{
		_dy = new double[_nbrPoints];
		for (int i=0;i<_nbrPoints;i++)
		{
			_dy[i] = rhs._dy[i];
		}
	}
	else
	{
		_dy = NULL;
	}

	return *this;
}


int HermiteInterpolator::Interpolate(const double x, double& y, double& dy)
{
	int k1, k2 ;
	double f, d, p, q, r, s, t, p2 ;
	/*
	 * Il n'y a pas assez de points pour faire l'interpolation
	 */
	if (_nbrPoints < 2)
		return -1;
	else
	{
		for (int i = 1 ; i < _nbrPoints ; i++)
		{
			/**
			 * @todo Verifier que l'interpolateur n'ai pas besoin ques les abscisses soitent strictement croissantes
			 */
			 
			/*
			 * Les abscisses ne sont pas croissantes
			 */
			if (_x[i] <= _x[i-1])
				return -2;
		}
		
		y = 0.0 ;
		dy = 0.0 ;
		for (int i = 0 ; i < _nbrPoints ; i++)
		{
			p = 1.0 ;
			q = 0.0 ;
			s = 0.0 ;
			for (int j = 0 ; j < i ; j++)
			{
				r  = _x[i] - _x[j] ;
				p  = p * ((x - _x[j]) / r) ;
				q  = q + 1.0 / r ;
				k1 = (i<j) ? i : j ;
				k2 = (i<j) ? j : i ;
				t  = 1.0 ;
				for (int k = 0 ; k < k1 ; k++)
					t = t * (x - _x[k]) / (_x[i] - _x[k]) ;
				for (int k = k1+1 ; k < k2 ; k++)
					t = t * (x - _x[k]) / (_x[i] - _x[k]) ;
				for (int k = k2+1 ; k < _nbrPoints ; k++)
					t = t * (x-_x[k]) / (_x[i]-_x[k]) ;
				s = s + t / r ;
			}
      
			for (int j = i+1 ; j < _nbrPoints ; j++)
			{
				r  = _x[i] - _x[j] ;
				p  = p * ((x - _x[j]) / r) ;
				q  = q + 1.0 / r ;
				k1 = (i < j) ? i : j ;
				k2 = (i < j) ? j : i ;
				t  = 1.0 ;
				for (int k = 0 ; k < k1 ; k++)
					t = t * (x - _x[k]) / (_x[i] - _x[k]) ;
				for (int k = k1+1 ; k < k2 ; k++)
					t = t * (x - _x[k]) / (_x[i] - _x[k]) ;
				for (int k = k2+1 ; k < _nbrPoints ; k++)
					t = t * (x - _x[k]) / (_x[i] - _x[k]) ;
				s = s + t / r ;
			}
      
			r  = x - _x[i] ;
			p2 = p * p ;
			f  = p2 * (1.0 - 2. * q * r) ;
			d  = p2 * r ;
			y = y + f * _y[i] + d * _dy[i] ;
			f  = 2.0 * p * (s * (1.0 - 2.0 * q * r) - p * q) ;
			d  = p * (p + 2.0 * r * s) ;
			dy = dy + f * _y[i] + d * _dy[i] ;
		}
	
	}
	return 0;
}

void HermiteInterpolator::Clear()
{
	if (_x != NULL)
	{
		delete[] _x;
		_x = NULL;
	}
	
	if (_y != NULL)
	{
		delete[] _y;
		_y = NULL;
	}
	
	if (_dy != NULL)
	{
		delete[] _dy;
		_dy = NULL;
	}
	
	_nbrPoints = 0;
}