#ifndef ossimFilter_HEADER
#define ossimFilter_HEADER
#include <math.h>
#include <ossim/matrix/newmat.h>
#include <ossim/base/ossimRtti.h>


class ossimFilter
{
public:
   ossimFilter()
      {}
   virtual ~ossimFilter() {}
   
   virtual double filter (double x, double support)const = 0;
   /*!
    * Will call the filter method to create a
    * convolution matrix.  This matrix will not
    * have normalized weights.
    *
    * Will generate a matrix by creating an outer
    * product of the 1-D filter array.  This matrix
    * will be square width-by-width.
    *
    * note:  the matrix is a new matrix and needs to
    *        be destroyed by the caller
    */
   virtual NEWMAT::Matrix *newMatrix(long width=3,
                                     double middle=0.0,
                                     double scale=0.0)const;
   virtual void createMatrix(NEWMAT::Matrix& m,
                             long width=3,
                             double middle=0.0,
                             double scale=0.0)const;
   
   virtual NEWMAT::RowVector *newVector(long width,
                                        double middle=0.0,
                                        double scale = 1.0)const;
   static double Sinc(double x)
      {
         if (x != 0.0)
         {
            x*=M_PI;
            return(sin(x)/x);
         }
         
         return(1.0);
      }
   static double Sinc(double x, double /* support */ )
      {
         if (x != 0.0)
         {
            x*=M_PI;
            return(sin(x)/x);
         }
         
         return(1.0);
      }

   virtual double getSupport()const=0;
   
TYPE_DATA
};

class ossimBoxFilter : public ossimFilter
{
public:
   ossimBoxFilter (){}
   virtual ~ossimBoxFilter() {}
   
   virtual double filter (double x, double /* support */ )const
      {
         if (x < -0.5)
            return(0.0);
         if (x < 0.5)
            return(1.0);
         return(0.0);
      }
   virtual double getSupport()const
      {
         return .5;
      }
TYPE_DATA
};

class ossimSincFilter : public ossimFilter
{
public:
   ossimSincFilter(){}
   virtual ~ossimSincFilter(){}
   double filter(double x, double /* support */ )const
      {
         x*=M_PI;
         if (x != 0.0)
            return(sin(x)/x);
         return(1.0);
      }
   virtual double getSupport()const
      {
         return 1.0;
      }
TYPE_DATA
};

class ossimBellFilter : public ossimFilter
{
public:
   ossimBellFilter(){}
   virtual ~ossimBellFilter(){}
   virtual double getSupport()const
      {
         return 1.5;
      }
   virtual double filter(double value, double /* support */ )const
      {
         if(value < 0) value = -value;
         if(value < .5) return(.75 - (value * value));
         if(value < 1.5) {
            value = (value - 1.5);
            return(.5 * (value * value));
         }
         return(0.0);
      }
TYPE_DATA
};

class ossimNearestNeighborFilter: public ossimBoxFilter
{
public:
   ossimNearestNeighborFilter (){}
   virtual ~ossimNearestNeighborFilter() {}
   
   virtual double getSupport()const
      {
         return 0.0;
      }
TYPE_DATA
};

   
class ossimBesselOrderOneFilter : public ossimFilter
{
public:
   ossimBesselOrderOneFilter(){}
   virtual ~ossimBesselOrderOneFilter(){}
   virtual double filter(double value, double /* support */ )const;
   virtual double getSupport()const
      {
         return 1.0;
      }
TYPE_DATA
};
class ossimBesselFilter : public ossimFilter
{
public:
   ossimBesselFilter(){}
   virtual ~ossimBesselFilter(){}
   virtual double filter(double x, double support)const
      {
         if (x == 0.0)
         {
            return(M_PI/4.0);
         }
         
         return(ossimBesselOrderOneFilter().filter((M_PI*x)/(2.0*x), support));
      }
   virtual double getSupport()const
      {
         return 1.0;
      }
   
TYPE_DATA
};

class ossimBlackmanFilter : public ossimFilter
{
public:
    ossimBlackmanFilter (){}
    virtual ~ossimBlackmanFilter() {}

    virtual double filter (double x, double /* support */ ) const
        {
           return(0.42+0.50*cos(M_PI*x)+0.08*cos(2.0*M_PI*x));
        }
   virtual double getSupport()const
      {
         return 1.0;
      }
TYPE_DATA
};

class ossimBlackmanSincFilter : public ossimBlackmanFilter
{
public:
    ossimBlackmanSincFilter (){}
    virtual ~ossimBlackmanSincFilter() {}

    virtual double filter (double x, double support) const
        {
           return ((ossimBlackmanFilter::filter(x/support,support))*
                   (ossimSincFilter().filter(x, support)));
        }
   virtual double getSupport()const
      {
         return 4.0;
      }
TYPE_DATA
};

class ossimBlackmanBesselFilter : public ossimBlackmanFilter
{
public:
    ossimBlackmanBesselFilter(){}
    virtual ~ossimBlackmanBesselFilter() {}

    virtual double filter (double x, double support) const
        {
           return ((ossimBlackmanFilter::filter(x/support,support))*
                   (ossimBesselFilter().filter(x, support)));
        }
   virtual double getSupport()const
      {
         return 3.2383;
      }
TYPE_DATA
};


class ossimCatromFilter : public ossimFilter
{
public:
   ossimCatromFilter(){}
   virtual ~ossimCatromFilter(){}
   double filter(double x, double /* support */ )const
      {
         if (x < -2.0)
            return(0.0);
         if (x < -1.0)
            return(0.5*(4.0+x*(8.0+x*(5.0+x))));
         if (x < 0.0)
            return(0.5*(2.0+x*x*(-5.0-3.0*x)));
         if (x < 1.0)
            return(0.5*(2.0+x*x*(-5.0+3.0*x)));
         if (x < 2.0)
            return(0.5*(4.0+x*(-8.0+x*(5.0-x))));
         return(0.0);
      }
   virtual double getSupport()const
      {
         return 2.0;
      }
TYPE_DATA
};

class ossimCubicFilter : public ossimFilter
{
public:
   ossimCubicFilter(){}
   virtual ~ossimCubicFilter(){}
   double filter(double x, double /* support */ )const
      {
         if (x < -2.0)
            return(0.0);
         if (x < -1.0)
            return((2.0+x)*(2.0+x)*(2.0+x)/6.0);
         if (x < 0.0)
            return((4.0+x*x*(-6.0-3.0*x))/6.0);
         if (x < 1.0)
            return((4.0+x*x*(-6.0+3.0*x))/6.0);
         if (x < 2.0)
            return((2.0-x)*(2.0-x)*(2.0-x)/6.0);
         return(0.0);
      }
   virtual double getSupport()const
      {
         return 2.0;
      }
TYPE_DATA
};

class ossimBSplineFilter : public ossimFilter
{
public:
   ossimBSplineFilter(){}
   virtual ~ossimBSplineFilter(){}
   virtual double filter(double value, double /* support */ )const
      {
	double tt;

	if(value < 0) value = -value;
	if(value < 1) {
		tt = value * value;
		return((.5 * tt * value) - tt + (2.0 / 3.0));
	} else if(value < 2) {
		value = 2 - value;
		return((1.0 / 6.0) * (value * value * value));
	}
	return(0.0);
      }
   virtual double getSupport()const
      {
         return 2.0;
      }
TYPE_DATA
};

class ossimGaussianFilter : public ossimFilter
{
public:
    ossimGaussianFilter (){}
    virtual ~ossimGaussianFilter() {}

    virtual double filter (double x, double /* support */ ) const
        {
           return(exp(-2.0*x*x)*sqrt(2.0/M_PI));
        }
   virtual double getSupport()const
      {
         return 1.25;
      }
TYPE_DATA
};

class ossimHanningFilter : public ossimFilter
{
public:
   ossimHanningFilter(){}
   virtual ~ossimHanningFilter(){}
   double filter(double x, double /* support */ )const
      {
         return(0.5+0.5*cos(M_PI*x));
      }
   virtual double getSupport()const
      {
         return 1.0;
      }
TYPE_DATA
};

class ossimHammingFilter : public ossimFilter
{
public:
    ossimHammingFilter (){}
    virtual ~ossimHammingFilter() {}

    virtual double filter (double x, double /* support */ ) const
        {
           return(0.54+0.46*cos(M_PI*x));
        }
   virtual double getSupport()const
      {
         return 1.0;
      }
TYPE_DATA
};

class ossimHermiteFilter : public ossimFilter
{
public:
   ossimHermiteFilter(){}
   virtual ~ossimHermiteFilter(){}
   double filter(double x, double /* support */ )const
      {
         if (x < -1.0)
            return(0.0);
         if (x < 0.0)
            return((2.0*(-x)-3.0)*(-x)*(-x)+1.0);
         if (x < 1.0)
            return((2.0*x-3.0)*x*x+1.0);
         return(0.0);
      }
   virtual double getSupport()const
      {
         return 1.0;
      }
TYPE_DATA
};


class ossimLanczosFilter : public ossimFilter
{
public:
   ossimLanczosFilter(){}
   virtual ~ossimLanczosFilter(){}
   double filter(double x, double /* support */ )const
      {
         if (x < -3.0)
            return(0.0);
         if (x < 0.0)
            return(Sinc(-x,getSupport())*Sinc(-x/3.0,getSupport()));
         if (x < 3.0)
            return(Sinc(x,getSupport())*Sinc(x/3.0,getSupport()));
         return(0.0);
      }
   virtual double getSupport()const
      {
         return 3.0;
      }
TYPE_DATA
};

class ossimMitchellFilter : public ossimFilter
{
public:
   ossimMitchellFilter(){}
   virtual ~ossimMitchellFilter(){}
   double filter(double x, double /* support */ )const
      {
#define MITCHELL_B   (1.0/3.0)
#define MITCHELL_C   (1.0/3.0)
#define MITCHELL_P0  ((  6.0- 2.0*MITCHELL_B       )/6.0)
#define MITCHELL_P2  ((-18.0+12.0*MITCHELL_B+ 6.0*MITCHELL_C)/6.0)
#define MITCHELL_P3  (( 12.0- 9.0*MITCHELL_B- 6.0*MITCHELL_C)/6.0)
#define MITCHELL_Q0  ((       8.0*MITCHELL_B+24.0*MITCHELL_C)/6.0)
#define MITCHELL_Q1  ((     -12.0*MITCHELL_B-48.0*MITCHELL_C)/6.0)
#define MITCHELL_Q2  ((       6.0*MITCHELL_B+30.0*MITCHELL_C)/6.0)
#define MITCHELL_Q3  ((     - 1.0*MITCHELL_B- 6.0*MITCHELL_C)/6.0)
         
         if (x < -2.0)
            return(0.0);
         if (x < -1.0)
            return(MITCHELL_Q0-x*(MITCHELL_Q1-x*(MITCHELL_Q2-x*MITCHELL_Q3)));
         if (x < 0.0)
            return(MITCHELL_P0+x*x*(MITCHELL_P2-x*MITCHELL_P3));
         if (x < 1.0)
            return(MITCHELL_P0+x*x*(MITCHELL_P2+x*MITCHELL_P3));
         if (x < 2.0)
            return(MITCHELL_Q0+x*(MITCHELL_Q1+x*(MITCHELL_Q2+x*MITCHELL_Q3)));
         return(0.0);
      }
   virtual double getSupport()const
      {
         return 2.0;
      }
TYPE_DATA
};

class ossimQuadraticFilter : public ossimFilter
{
public:
   ossimQuadraticFilter(){}
   virtual ~ossimQuadraticFilter(){}
   double filter(double x, double /* support */ )const
      {
         if (x < -1.5)
            return(0.0);
         if (x < -0.5)
            return(0.5*(x+1.5)*(x+1.5));
         if (x < 0.5)
            return(0.75-x*x);
         if (x < 1.5)
            return(0.5*(x-1.5)*(x-1.5));
         return(0.0);
      }
   double getSupport()const
      {
         return 1.5;
      }
TYPE_DATA
};

class ossimTriangleFilter : public ossimFilter
{
public:
   ossimTriangleFilter(){}
   virtual ~ossimTriangleFilter() {}
   
   virtual double filter (double x, double /* support */ )const
      {
         if (x < -1.0)
            return(0.0);
         if (x < 0.0)
            return(1.0+x);
         if (x < 1.0)
            return(1.0-x);
         return(0.0);
      }
   virtual double getSupport()const
      {
         return 1.0;
      }
TYPE_DATA
};


#endif
