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
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkFixedArray.h"
#include "otbTimeSeriesLeastSquareFittingFunctor.h"
#include "otbTimeSeries.h"

int otbTimeSeriesLeastSquareFittingFunctorTest(int argc, char* argv[])
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

  FunctorType f;

  SeriesType inSeries;


  SeriesType outSeries = f(inSeries);

  FunctorType::CoefficientsType outCoefs = f.GetCoefficients();
  
  return EXIT_SUCCESS;
}
