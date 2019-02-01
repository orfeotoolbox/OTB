/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */



#include "otbTimeSeriesLeastSquareFittingFunctor.h"
#include "otbTimeSeries.h"

int otbTimeSeriesLeastSquareFittingFunctorWeightsTest(int itkNotUsed(argc), char* argv[])
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
  SeriesType weightSeries;
  FunctorType::CoefficientsType inCoefs;
  inCoefs[0] = ::atof(argv[1]);
  inCoefs[1] = ::atof(argv[2]);
  inCoefs[2] = ::atof(argv[3]);

  // x = a + b * t + c * t^2
  for(unsigned int i = 0; i<nbDates; ++i)
    {
    inSeries[i] = inCoefs[0]+inCoefs[1]*doySeries[i]+inCoefs[2]*std::pow(doySeries[i], 2.0);
    weightSeries[i] = 1;
    }

  // Set artifacts
  inSeries[nbDates/4] = 0.0;
  inSeries[nbDates/2] = 0.0;

  weightSeries[nbDates/4] = 1000.0;
  weightSeries[nbDates/2] = 1000.0;



  FunctorType f;
  f.SetDates( doySeries );
  f.SetWeights (weightSeries);

  FunctorType::CoefficientsType outCoefs = f.GetCoefficients(inSeries);

  for(unsigned int i=0; i<= Degree; ++i)
    if(fabs((outCoefs[i]-inCoefs[i])/inCoefs[i])>0.01)
      {
      std::cout << outCoefs[i] << " != " << inCoefs[i] << std::endl;
      return EXIT_FAILURE;
      }

  return EXIT_SUCCESS;
}
