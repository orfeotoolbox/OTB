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
#include "itkMacro.h"
#include "otbVegetationIndicesFunctor.h"

int otbLAIFromNDVILogarithmic(int itkNotUsed(argc), char * argv[])
{
  typedef double                           PixelType;

  typedef otb::Functor::LAIFromNDVILogarithmic<PixelType, PixelType, PixelType> FunctorType;

  FunctorType laiFunct = FunctorType();

  double redValue = (::atof(argv[1]));
  double nirValue = (::atof(argv[2]));
  double ndviSoil(::atof(argv[3]));
  double ndviInf(::atof(argv[4]));
  double extCoef(::atof(argv[5]));

  double ndvi = (nirValue-redValue)/(nirValue+redValue);
  double goodResult = -1/extCoef*vcl_log((ndvi-ndviInf)/(ndviSoil-ndviInf));

  laiFunct.SetNdviInf(ndviInf);
  laiFunct.SetNdviSoil(ndviSoil);
  laiFunct.SetExtinctionCoefficient(extCoef);

  laiFunct.SetRedIndex(1);
  laiFunct.SetNIRIndex(2);

  itk::VariableLengthVector<PixelType> pixel;
  pixel.Reserve(2);
  pixel[0] = redValue;
  pixel[1] = nirValue;

  double result = laiFunct(pixel);

  if( result!=goodResult ) return EXIT_FAILURE;

  return EXIT_SUCCESS;
}
