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
#include "otbHistogramCurve.h"
#include "itkHistogram.h"
#include "itkDenseFrequencyContainer2.h"

int otbHistogramCurveNew(int argc, char * argv[])
{
  typedef itk::Statistics::Histogram<double,
      itk::Statistics::DenseFrequencyContainer2>  HistogramType;
  typedef otb::HistogramCurve<HistogramType> HistogramCurveType;
  HistogramCurveType::Pointer curve = HistogramCurveType::New();

  std::cout << curve << std::endl;

  return EXIT_SUCCESS;
}
