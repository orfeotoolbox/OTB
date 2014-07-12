/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#include "itkFixedArray.h"
#include "otbTimeSeriesLeastSquareFittingFunctor.h"
#include "otbTimeSeries.h"

int otbTimeSeriesLeastSquareFittingFunctorTest(int itkNotUsed(argc), char* argv[])
{

  const unsigned int Degree = 2;
  typedef double CoefficientPrecisionType;
  typedef otb::PolynomialTimeSeries< Degree, CoefficientPrecisionType > FunctionType;
  const unsigned int nbDates = 100;
  typedef float PixelType;
  typedef unsigned int DoYType;
  typedef itk::FixedArray< PixelType, nbDates > SeriesType;
  typedef itk::FixedArray< DoYType, nbDates > DatesType;

  typedef otb::Functor::TimeSeriesLeastSquareFittingFunctor<SeriesType, FunctionType, DatesType> FunctorType;

  DatesType doySeries;
  // one acquisition every 2 days
  for(unsigned int i = 0; i<nbDates; ++i)
    doySeries[i] = 2*i;


  SeriesType inSeries;
  FunctorType::CoefficientsType inCoefs;
  inCoefs[0] = ::atof(argv[1]);
  inCoefs[1] = ::atof(argv[2]);
  inCoefs[2] = ::atof(argv[3]);

  // x = a + b * t + c * t^2
  for(unsigned int i = 0; i<nbDates; ++i)
    inSeries[i] = inCoefs[0]+inCoefs[1]*doySeries[i]+inCoefs[2]*vcl_pow(doySeries[i], 2.0);


  FunctorType f;
  f.SetDates( doySeries );

  FunctorType::CoefficientsType outCoefs = f.GetCoefficients(inSeries);

  for(unsigned int i=0; i<= Degree; ++i)
    if(fabs((outCoefs[i]-inCoefs[i])/inCoefs[i])>0.01)
      {
      std::cout << outCoefs[i] << " != " << inCoefs[i] << std::endl;
      return EXIT_FAILURE;
      }

  return EXIT_SUCCESS;
}
