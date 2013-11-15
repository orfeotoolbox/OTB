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
#include "otbSavitzkyGolayInterpolationFunctor.h"
#include "otbTimeSeries.h"

int otbSavitzkyGolayInterpolationFunctorNew(int argc, char* argv[])
{

  typedef float PixelType;
  typedef unsigned int DoYType;
  typedef itk::FixedArray< PixelType > SeriesType;
  typedef itk::FixedArray< DoYType > DatesType;
  const unsigned int Radius = 2;

  typedef otb::Functor::SavitzkyGolayInterpolationFunctor<Radius, SeriesType, DatesType> FunctorType;

  FunctorType f;

  return EXIT_SUCCESS;
}
