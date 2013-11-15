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
#include "otbEnvelopeSavitzkyGolayInterpolationFunctor.h"
#include "otbTimeSeries.h"

int otbEnvelopeSavitzkyGolayInterpolationFunctorTest(int argc, char* argv[])
{

  typedef float PixelType;
  typedef unsigned int DoYType;
  const unsigned int nbDates = 100;
  typedef itk::FixedArray< PixelType, nbDates > SeriesType;
  typedef itk::FixedArray< DoYType, nbDates > DatesType;
  const unsigned int Radius = 2;



  // Create a time series
  SeriesType inSeries;
  SeriesType cleanSeries;
  SeriesType weightSeries;
  DatesType doySeries;

  for(unsigned int i=0; i<nbDates; ++i)
    {
    inSeries[i] = 10*vcl_cos(i/10.0);
    cleanSeries[i] = inSeries[i];
    doySeries[i] = i;
    weightSeries[i] = 1;
    }

  // Set artifacts
  inSeries[nbDates/4] = 0.0;
  inSeries[nbDates/2] = 0.0;

  weightSeries[nbDates/4] = 100000.0;
  weightSeries[nbDates/2] = 100000.0;

  typedef otb::Functor::EnvelopeSavitzkyGolayInterpolationFunctor<Radius, SeriesType, DatesType, SeriesType> FunctorType;

  FunctorType f;
  f.SetWeights( weightSeries );
  f.SetDates( doySeries );
  f.SetUpperEnvelope();
  f.SetDecreaseFactor(0.9);
  f.SetIterations(4);

  SeriesType outSeries = f(inSeries);

  double interpolError = 0.0;

  for(unsigned int i=0; i<nbDates; ++i)
    {

    interpolError += vcl_fabs(outSeries[i]-cleanSeries[i]);

    }

  interpolError/=nbDates;


  if(interpolError > 0.1)
    {
    std::cout << "Interpolation error = " << interpolError << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
