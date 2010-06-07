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
#include "otbConfusionMatrixCalculator.h"



int otbConfusionMatrixCalculatorNew(int argc, char* argv[])
{

  typedef itk::FixedArray<int, 1>                 LabelType;
  typedef itk::Statistics::ListSample<LabelType>  ListLabelType;
  typedef otb::ConfusionMatrixCalculator< ListLabelType > CalculatorType;

  CalculatorType::Pointer calculator = CalculatorType::New();
  
  return EXIT_SUCCESS;
}

int otbConfusionMatrixCalculatorSetListSamples(int argc, char* argv[])
{

  if( argc!= 3)
    {
    std::cerr << "Usage: " << argv[0] << " nbSamples nbClasses " << std::endl;
    }
  typedef itk::FixedArray<int, 1>                 LabelType;
  typedef itk::Statistics::ListSample<LabelType>  ListLabelType;
  typedef otb::ConfusionMatrixCalculator< ListLabelType > CalculatorType;

  CalculatorType::Pointer calculator = CalculatorType::New();

  ListLabelType::Pointer refLabels = ListLabelType::New();
  ListLabelType::Pointer prodLabels = ListLabelType::New();

  unsigned int nbSamples = atoi(argv[1]);
  unsigned int nbClasses = atoi(argv[2]);
  

  for(int i=0; i<nbSamples; i++)
    {
    int label = (i%nbClasses)+1;
    refLabels->PushBack( label );
    prodLabels->PushBack( label );
    }

  calculator->GetNumberOfClasses();
  
  calculator->SetReferenceLabels( refLabels );
  calculator->SetProducedLabels( prodLabels );

  calculator->GetNumberOfClasses();

  
  
  return EXIT_SUCCESS;
}
