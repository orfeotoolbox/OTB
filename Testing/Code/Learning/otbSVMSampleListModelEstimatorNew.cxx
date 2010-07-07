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

#include "itkExceptionObject.h"
#include "itkListSample.h"
#include <iostream>

#include "otbSVMSampleListModelEstimator.h"

int otbSVMSampleListModelEstimatorNew(int argc, char* argv[])
{
  typedef double InputPixelType;
  typedef double LabelPixelType;

  typedef itk::VariableLengthVector<InputPixelType>       SampleType;
  typedef itk::Statistics::ListSample<SampleType>         ListSampleType;
  typedef itk::FixedArray<LabelPixelType, 1>              TrainingSampleType;
  typedef itk::Statistics::ListSample<TrainingSampleType> TrainingListSampleType;

  typedef otb::SVMSampleListModelEstimator<ListSampleType,
      TrainingListSampleType> EstimatorType;

  EstimatorType::Pointer estimator = EstimatorType::New();

  return EXIT_SUCCESS;
}
