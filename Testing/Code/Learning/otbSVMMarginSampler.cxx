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

#include "itkVariableLengthVector.h"
#include "itkListSample.h"
#include "otbSVMMarginSampler.h"
#include "otbSVMModel.h"

int otbSVMMarginSamplerNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef itk::VariableLengthVector<double>                 SampleType;
  typedef itk::Statistics::ListSample<SampleType>           SampleListType;
  typedef otb::SVMModel<SampleListType::MeasurementType,
    unsigned int>                                           SVMModelType;
  typedef otb::SVMMarginSampler<SampleListType,SVMModelType>     MarginSamplerType;

  MarginSamplerType::Pointer  marginSampler = MarginSamplerType::New();

  std::cout << marginSampler << std::endl;

  return EXIT_SUCCESS;
}
