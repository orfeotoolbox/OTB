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

  typedef itk::VariableLengthVector<int>                   PLabelType;
  typedef itk::Statistics::ListSample<PLabelType>          PListLabelType;
  typedef itk::FixedArray<int, 1>                          RLabelType;
  typedef itk::Statistics::ListSample<RLabelType>           RListLabelType;
  typedef otb::ConfusionMatrixCalculator< RListLabelType,
                                          PListLabelType > CalculatorType;

  CalculatorType::Pointer calculator = CalculatorType::New();
  
  return EXIT_SUCCESS;
}

int otbConfusionMatrixCalculatorSetListSamples(int argc, char* argv[])
{

  if( argc!= 3)
    {
    std::cerr << "Usage: " << argv[0] << " nbSamples nbClasses " << std::endl;
    return EXIT_FAILURE;
    }

  typedef itk::VariableLengthVector<int>                   PLabelType;
  typedef itk::Statistics::ListSample<PLabelType>          PListLabelType;
  typedef itk::FixedArray<int, 1>                          RLabelType;
  typedef itk::Statistics::ListSample<RLabelType>           RListLabelType;
  typedef otb::ConfusionMatrixCalculator< RListLabelType,
                                          PListLabelType > CalculatorType;
                                                  
  CalculatorType::Pointer calculator = CalculatorType::New();

  RListLabelType::Pointer         refLabels = RListLabelType::New();
  PListLabelType::Pointer         prodLabels = PListLabelType::New();

  int nbSamples = atoi(argv[1]);
  int nbClasses = atoi(argv[2]);
  

  for(int i=0; i<nbSamples; i++)
    {
    int label = (i%nbClasses)+1;
    PLabelType plab;
    plab.SetSize(1);
    plab[0] = label;
    refLabels->PushBack( label );
    prodLabels->PushBack( plab );
    }

  calculator->SetReferenceLabels( refLabels );
  calculator->SetProducedLabels( prodLabels );

  return EXIT_SUCCESS;
}

int otbConfusionMatrixCalculatorWrongSize(int argc, char* argv[])
{

    if( argc!= 3)
    {
    std::cerr << "Usage: " << argv[0] << " nbSamples nbClasses " << std::endl;
    return EXIT_FAILURE;
    }

    typedef itk::VariableLengthVector<int>                   PLabelType;
    typedef itk::Statistics::ListSample<PLabelType>          PListLabelType;
    typedef itk::FixedArray<int, 1>                          RLabelType;
    typedef itk::Statistics::ListSample<RLabelType>          RListLabelType;
    typedef otb::ConfusionMatrixCalculator< RListLabelType,
                                            PListLabelType > CalculatorType;

  CalculatorType::Pointer calculator = CalculatorType::New();

  RListLabelType::Pointer refLabels = RListLabelType::New();
  PListLabelType::Pointer         prodLabels = PListLabelType::New();

  int nbSamples = atoi(argv[1]);
  int nbClasses = atoi(argv[2]);
  

  for(int i=0; i<nbSamples; i++)
    {
    int label = (i%nbClasses)+1;
    PLabelType plab;
    plab.SetSize(1);
    plab[0] = label;
    refLabels->PushBack( label );
    prodLabels->PushBack( plab );
    }

  PLabelType plab;
  plab.SetSize(1);
  plab[0] = 0;
  prodLabels->PushBack( plab );
  
  calculator->SetReferenceLabels( refLabels );
  calculator->SetProducedLabels( prodLabels );

  try
    {
    calculator->Update();
    }
  catch (itk::ExceptionObject)
    {
    return EXIT_FAILURE;
    }
  
  return EXIT_SUCCESS;
}


int otbConfusionMatrixCalculatorUpdate(int argc, char* argv[])
{

  if( argc!= 3)
    {
    std::cerr << "Usage: " << argv[0] << " nbSamples nbClasses " << std::endl;
    return EXIT_FAILURE;
    }

  typedef itk::VariableLengthVector<int>                   PLabelType;
  typedef itk::Statistics::ListSample<PLabelType>          PListLabelType;
  typedef itk::FixedArray<int, 1>                          RLabelType;
  typedef itk::Statistics::ListSample<RLabelType>           RListLabelType;
  typedef otb::ConfusionMatrixCalculator< RListLabelType,
                                          PListLabelType > CalculatorType;

  CalculatorType::Pointer calculator = CalculatorType::New();

  RListLabelType::Pointer refLabels = RListLabelType::New();
  PListLabelType::Pointer         prodLabels = PListLabelType::New();

  int nbSamples = atoi(argv[1]);
  int nbClasses = atoi(argv[2]);
  

  for(int i=0; i<nbSamples; i++)
    {
    int label = (i%nbClasses)+1;
    PLabelType plab;
    plab.SetSize(1);
    plab[0] = label;
    refLabels->PushBack( label );
    prodLabels->PushBack( plab );
    }

    
  calculator->SetReferenceLabels( refLabels );
  calculator->SetProducedLabels( prodLabels );

  calculator->Update();

  if( calculator->GetNumberOfClasses() != nbClasses )
    {
    std::cerr << "Wrong number of classes" << std::endl;
    return EXIT_FAILURE;
    }

  CalculatorType::ConfusionMatrixType confmat = calculator->GetConfusionMatrix();

  double totalError = 0.0;

  for (int i = 0; i < nbClasses; i++)
    for (int j = 0; j < nbClasses; j++)
      {
      double goodValue = 0.0;
      if (i == j) goodValue = nbSamples / nbClasses;
      if (confmat(i, j) != goodValue) totalError += confmat(i, j);
      }

  if( totalError > 0.001 )
    {
    std::cerr << confmat << std::endl;
    std::cerr<< "Error = " << totalError << std::endl;
    return EXIT_FAILURE;
    }

  
  return EXIT_SUCCESS;
}
