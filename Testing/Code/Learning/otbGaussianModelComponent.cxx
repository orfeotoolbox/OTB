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

#include "itkListSample.h"
#include "itkSubsample.h"
#include "otbGaussianModelComponent.h"
#include "itkVariableLengthVector.h"

int otbGaussianModelComponentNew(int argc, char * argv[])
{

  typedef itk::Statistics::ListSample<itk::VariableLengthVector<double> >  SampleType;
  typedef itk::Statistics::Subsample< SampleType >                   ClassSampleType  ;
  typedef otb::Statistics::GaussianModelComponent<ClassSampleType>   GaussianModelType;

  GaussianModelType::Pointer filter = GaussianModelType::New();

  return EXIT_SUCCESS;

}
