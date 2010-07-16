/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%                        ZZZZZ   OOO    OOO   M   M                           %
%                           ZZ  O   O  O   O  MM MM                           %
%                         ZZZ   O   O  O   O  M M M                           %
%                        ZZ     O   O  O   O  M   M                           %
%                        ZZZZZ   OOO    OOO   M   M                           %
%                                                                             %
%                                                                             %
%                      ImageMagick Image Zoom Methods                         %
%                                                                             %
%                                                                             %
%                              Software Design                                %
%                                John Cristy                                  %
%                                 July 1992                                   %
%                                                                             %
%                                                                             %
%  Copyright (C) 2000 ImageMagick Studio, a non-profit organization dedicated %
%  to making software imaging solutions freely available.                     %
%                                                                             %
%  Permission is hereby granted, free of charge, to any person obtaining a    %
%  copy of this software and associated documentation files ("ImageMagick"),  %
%  to deal in ImageMagick without restriction, including without limitation   %
%  the rights to use, copy, modify, merge, publish, distribute, sublicense,   %
%  and/or sell copies of ImageMagick, and to permit persons to whom the       %
%  ImageMagick is furnished to do so, subject to the following conditions:    %
%                                                                             %
%  The above copyright notice and this permission notice shall be included in %
%  all copies or substantial portions of ImageMagick.                         %
%                                                                             %
%  The software is provided "as is", without warranty of any kind, express or %
%  implied, including but not limited to the warranties of merchantability,   %
%  fitness for a particular purpose and noninfringement.  In no event shall   %
%  ImageMagick Studio be liable for any claim, damages or other liability,    %
%  whether in an action of contract, tort or otherwise, arising from, out of  %
%  or in connection with ImageMagick or the use or other dealings in          %
%  ImageMagick.                                                               %
%                                                                             %
%  Except as contained in this notice, the name of the ImageMagick Studio     %
%  shall not be used in advertising or otherwise to promote the sale, use or  %
%  other dealings in ImageMagick without prior written authorization from the %
%  ImageMagick Studio.                                                        %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%
%
*/
#include <ossim/base/ossimConstants.h>
#include <ossim/imaging/ossimFilter.h>
#include <ossim/matrix/newmat.h>
#include <iostream>


RTTI_DEF(ossimFilter, "ossimFilter");
RTTI_DEF1(ossimBoxFilter, "ossimBoxFilter",ossimFilter);
RTTI_DEF1(ossimSincFilter, "ossimSincFilter",ossimFilter);
RTTI_DEF1(ossimBellFilter, "ossimBellFilter",ossimFilter);
RTTI_DEF1(ossimNearestNeighborFilter, "ossimNearestNeighborFilter",ossimFilter);
RTTI_DEF1(ossimBesselFilter, "ossimBesselFilter",ossimFilter);
RTTI_DEF1(ossimBesselOrderOneFilter, "ossimBesselOrderOneFilter",ossimFilter);
RTTI_DEF1(ossimBlackmanFilter, "ossimBlackmanFilter",ossimFilter);
RTTI_DEF1(ossimBlackmanSincFilter, "ossimBlackmanSincFilter",ossimFilter);
RTTI_DEF1(ossimBlackmanBesselFilter, "ossimBlackmanBesselFilter",ossimFilter);
RTTI_DEF1(ossimCatromFilter, "ossimCatromFilter",ossimFilter);
RTTI_DEF1(ossimCubicFilter, "ossimCubicFilter",ossimFilter);
RTTI_DEF1(ossimBSplineFilter, "ossimBSplineFilter",ossimFilter);
RTTI_DEF1(ossimGaussianFilter, "ossimGaussianFilter",ossimFilter);
RTTI_DEF1(ossimHanningFilter, "ossimHanningFilter",ossimFilter);
RTTI_DEF1(ossimHammingFilter, "ossimHammingFilter",ossimFilter);
RTTI_DEF1(ossimHermiteFilter, "ossimHermiteFilter",ossimFilter);
RTTI_DEF1(ossimLanczosFilter, "ossimLanczosFilter",ossimFilter);
RTTI_DEF1(ossimMitchellFilter, "ossimMitchellFilter",ossimFilter);
RTTI_DEF1(ossimQuadraticFilter, "ossimQuadraticFilter",ossimFilter);
RTTI_DEF1(ossimTriangleFilter, "ossimTriangleFilter",ossimFilter);
/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
+   B e s s e l O r d e r O n e                                               %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Method BesselOrderOne computes the Bessel function of x of the first kind
%  of order 0:
%
%    Reduce x to |x| since j1(x)= -j1(-x), and for x in (0,8]
%
%       j1(x) = x*j1(x);
%
%    For x in (8,inf)
%
%       j1(x) = sqrt(2/(pi*x))*(p1(x)*cos(x1)-q1(x)*sin(x1))
%
%    where x1 = x-3*pi/4. Compute sin(x1) and cos(x1) as follow:
%
%       cos(x1) =  cos(x)cos(3pi/4)+sin(x)sin(3pi/4)
%               =  1/sqrt(2) * (sin(x) - cos(x))
%       sin(x1) =  sin(x)cos(3pi/4)-cos(x)sin(3pi/4)
%               = -1/sqrt(2) * (sin(x) + cos(x))
%
%  The format of the BesselOrderOne method is:
%
%      double BesselOrderOne(double x)
%
%  A description of each parameter follows:
%
%    o value: Method BesselOrderOne returns the Bessel function of x of the
%      first kind of orders 1.
%
%    o x: double value.
%
%
*/

static double J1(double x)
{
  double
    p,
    q;

  register int
    i;

  static const double
    Pone[] =
    {
       0.581199354001606143928050809e+21,
      -0.6672106568924916298020941484e+20,
       0.2316433580634002297931815435e+19,
      -0.3588817569910106050743641413e+17,
       0.2908795263834775409737601689e+15,
      -0.1322983480332126453125473247e+13,
       0.3413234182301700539091292655e+10,
      -0.4695753530642995859767162166e+7,
       0.270112271089232341485679099e+4
    },
    Qone[] =
    {
      0.11623987080032122878585294e+22,
      0.1185770712190320999837113348e+20,
      0.6092061398917521746105196863e+17,
      0.2081661221307607351240184229e+15,
      0.5243710262167649715406728642e+12,
      0.1013863514358673989967045588e+10,
      0.1501793594998585505921097578e+7,
      0.1606931573481487801970916749e+4,
      0.1e+1
    };

  p=Pone[8];
  q=Qone[8];
  for (i=7; i >= 0; i--)
  {
    p=p*x*x+Pone[i];
    q=q*x*x+Qone[i];
  }
  return(p/q);
}

static double P1(double x)
{
  double
    p,
    q;

  register int
    i;

  static const double
    Pone[] =
    {
      0.352246649133679798341724373e+5,
      0.62758845247161281269005675e+5,
      0.313539631109159574238669888e+5,
      0.49854832060594338434500455e+4,
      0.2111529182853962382105718e+3,
      0.12571716929145341558495e+1
    },
    Qone[] =
    {
      0.352246649133679798068390431e+5,
      0.626943469593560511888833731e+5,
      0.312404063819041039923015703e+5,
      0.4930396490181088979386097e+4,
      0.2030775189134759322293574e+3,
      0.1e+1
    };

  p=Pone[5];
  q=Qone[5];
  for (i=4; i >= 0; i--)
  {
    p=p*(8.0/x)*(8.0/x)+Pone[i];
    q=q*(8.0/x)*(8.0/x)+Qone[i];
  }
  return(p/q);
}

static double Q1(double x)
{
   double
      p,
      q;
   
   register int
      i;
   
   static const double
      Pone[] =
   {
      0.3511751914303552822533318e+3,
      0.7210391804904475039280863e+3,
      0.4259873011654442389886993e+3,
      0.831898957673850827325226e+2,
      0.45681716295512267064405e+1,
      0.3532840052740123642735e-1
   },
      Qone[] =
      {
         0.74917374171809127714519505e+4,
         0.154141773392650970499848051e+5,
         0.91522317015169922705904727e+4,
         0.18111867005523513506724158e+4,
         0.1038187585462133728776636e+3,
         0.1e+1
      };
      
      p=Pone[5];
      q=Qone[5];
      for (i=4; i >= 0; i--)
      {
         p=p*(8.0/x)*(8.0/x)+Pone[i];
         q=q*(8.0/x)*(8.0/x)+Qone[i];
      }
      return(p/q);
}

double ossimBesselOrderOneFilter::filter(double x, double /* support */)const
{
   double
      p,
      q;
   
   if (x == 0.0)
      return(0.0);
   p=x;
   if (x < 0.0)
      x=(-x);
   if (x < 8.0)
      return(p*J1(x));
   q=sqrt(2.0/(M_PI*x))*(P1(x)*(1.0/sqrt(2.0)*(sin(x)-cos(x)))-8.0/x*Q1(x)*
                         (-1.0/sqrt(2.0)*(sin(x)+cos(x))));
   if (p < 0.0)
      q=(-q);
   return(q);

}

void ossimFilter::createMatrix(NEWMAT::Matrix& m,
                               long width,
                               double middle,
                               double scale)const
{
   NEWMAT::ColumnVector colVec(width);
   NEWMAT::RowVector    rowVec(width);

   double t = 0.0;
   double val = 0.0;
   if(width == 1)
   {
      t = 0;
      val = filter(t, getSupport());
      colVec[0] = val;
      rowVec[0] = val;
   }
   else
   {
      for(long index = 0; index < width; index++)
      {
         t = (double)index/(double)(width-1);
         t = middle + (t - .5)*scale;
         val = filter(t, getSupport());
         colVec[index] = val;
         rowVec[index] = val;
      }
   }

   // do the outer product to construct the
   // filter matrix
   m = colVec * rowVec;
}


NEWMAT::Matrix *ossimFilter::newMatrix(long width,
                                       double middle,
                                       double scale)const
{
   NEWMAT::Matrix *result = new NEWMAT::Matrix(width, width);

   createMatrix(*result,
                width,
                middle,
                scale);

   return result;
}

NEWMAT::RowVector *ossimFilter::newVector(long width,
                                          double middle,
                                          double scale)const
{
   NEWMAT::RowVector *result = new NEWMAT::RowVector(width);


   double t = 0.0;
   double val = 0.0;
   for(long index = 0; index < width; index++)
   {
      t = (double)index/(double)(width-1);
      t = middle + (t- .5)*scale;
      val = filter(t, getSupport());
      (*result)[index] = val;
   }

   return result;
}
