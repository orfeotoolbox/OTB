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
#include "itkExceptionObject.h"

#include "otbGreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculator.h"

int otbGreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculatorNew(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef float                                                  MeasurementType;
  typedef itk::Statistics::Histogram<MeasurementType, Dimension> HistogramType;
  typedef otb::GreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculator<
      HistogramType> FilterType;

  FilterType::Pointer filter = FilterType::New();

  return EXIT_SUCCESS;
}
