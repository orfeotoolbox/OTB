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
#include "otbContingencyTableCalculator.h"

int otbContingencyTableCalculatorNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{

  typedef int                                             ClassLabelType;
  typedef otb::ContingencyTableCalculator<ClassLabelType> CalculatorType;

  CalculatorType::Pointer calculator = CalculatorType::New();

  return EXIT_SUCCESS;
}



int otbContingencyTableCalculatorSetListSamples(int argc, char* argv[])
{

  if (argc != 3)
    {
    std::cerr << "Usage: " << argv[0] << " nbSamples nbClasses " << std::endl;
    return EXIT_FAILURE;
    }

  typedef int                                             ClassLabelType;
  typedef itk::VariableLengthVector<ClassLabelType>       PLabelType;
  typedef itk::Statistics::ListSample<PLabelType>         PListLabelType;
  typedef itk::FixedArray<ClassLabelType, 1>              RLabelType;
  typedef itk::Statistics::ListSample<RLabelType>         RListLabelType;
  typedef otb::ContingencyTableCalculator<ClassLabelType> CalculatorType;

  CalculatorType::Pointer calculator = CalculatorType::New();

  RListLabelType::Pointer refLabels = RListLabelType::New();
  PListLabelType::Pointer prodLabels = PListLabelType::New();

  // Set the measurement vector size for the list sample labels
  refLabels->SetMeasurementVectorSize(1);
  prodLabels->SetMeasurementVectorSize(1);

  int nbSamples = atoi(argv[1]);
  int nbClasses = atoi(argv[2]);

  for (int i = 0; i < nbSamples; ++i)
    {
    ClassLabelType label = (i % nbClasses) + 1;
    PLabelType plab;
    plab.SetSize(1);
    plab[0] = label;
    refLabels->PushBack(label);
    prodLabels->PushBack(plab);
    }

  calculator->Compute( refLabels->Begin(), refLabels->End(), prodLabels->Begin(), prodLabels->End() );

  return EXIT_SUCCESS;
}

int otbContingencyTableCalculatorCompute(int argc, char* argv[])
{

  if (argc != 3)
    {
    std::cerr << "Usage: " << argv[0] << " nbSamples nbRefClasses " << std::endl;
    return EXIT_FAILURE;
    }


  typedef int                                             ClassLabelType;
  typedef itk::VariableLengthVector<ClassLabelType>       PLabelType;
  typedef itk::Statistics::ListSample<PLabelType>         PListLabelType;
  typedef itk::FixedArray<ClassLabelType, 1>              RLabelType;
  typedef itk::Statistics::ListSample<RLabelType>         RListLabelType;
  typedef otb::ContingencyTableCalculator<ClassLabelType> CalculatorType;
  typedef CalculatorType::ContingencyTableType          ContingencyTableType;

  CalculatorType::Pointer calculator = CalculatorType::New();

  RListLabelType::Pointer refLabels = RListLabelType::New();
  PListLabelType::Pointer prodLabels = PListLabelType::New();

  // Set the measurement vector size for the list sample labels
  refLabels->SetMeasurementVectorSize(1);
  prodLabels->SetMeasurementVectorSize(1);

  int nbSamples = atoi(argv[1]);
  int nbRefClasses = atoi(argv[2]);
  int nbProdClasses = nbSamples;

  for (int i = 0; i < nbSamples; ++i)
    {
    int label;

    label = (i % nbProdClasses) + 1;

    PLabelType plab;
    RLabelType rlab;
    plab.SetSize(1);

    plab[0] = label > nbProdClasses ? nbProdClasses : label;
    rlab[0] = label > nbRefClasses ? nbRefClasses : label;

    refLabels->PushBack(rlab);
    prodLabels->PushBack(plab);
    }

  calculator->Compute( refLabels->Begin(), refLabels->End(), prodLabels->Begin(), prodLabels->End() );
  ContingencyTableType confmat = calculator->GetContingencyTable();


  if( static_cast<int>(calculator->GetNumberOfRefClasses()) != nbRefClasses )
    {
    std::cerr << "Wrong number of reference classes " << calculator->GetNumberOfRefClasses() << " != " << nbRefClasses
              << std::endl;
    return EXIT_FAILURE;
    }
  if( static_cast<int>(calculator->GetNumberOfProdClasses()) != nbProdClasses )
    {
    std::cerr << "Wrong number of produced classes " << calculator->GetNumberOfProdClasses() << " != " << nbProdClasses
              << std::endl;
    return EXIT_FAILURE;
    }
  if( static_cast<int>(calculator->GetNumberOfSamples()) != nbSamples )
    {
    std::cerr << "Wrong number of samples" << std::endl;
    return EXIT_FAILURE;
    }


  std::cout << "contingency table" << std::endl  << confmat << std::endl;

  return EXIT_SUCCESS;
}



int otbContingencyTableCalculatorComputeWithBaseline(int itkNotUsed(argc), char* itkNotUsed(argv) [])
{
  typedef int ClassLabelType;
  typedef itk::VariableLengthVector<ClassLabelType> PLabelType;
  typedef itk::Statistics::ListSample<PLabelType> PListLabelType;
  typedef itk::FixedArray<ClassLabelType, 1> RLabelType;
  typedef itk::Statistics::ListSample<RLabelType> RListLabelType;
  typedef otb::ContingencyTableCalculator<ClassLabelType> CalculatorType;

  CalculatorType::Pointer calculator = CalculatorType::New();

  RListLabelType::Pointer refLabels = RListLabelType::New();
  PListLabelType::Pointer prodLabels = PListLabelType::New();

  unsigned int nbSamples = 12;

  // Reference samples: a b c d a b c d a b c d
  // Classified reference samples: a c c d d b c d d d d c
  std::vector<ClassLabelType> labelsUniverse, labelsClassified;

  labelsUniverse.push_back( 'a' );
  labelsUniverse.push_back( 'b' );
  labelsUniverse.push_back( 'b' );
  labelsUniverse.push_back( 'b' );
  labelsUniverse.push_back( 'c' );
  labelsUniverse.push_back( 'd' );
  labelsUniverse.push_back( 'd' );
  labelsUniverse.push_back( 'd' );
  labelsUniverse.push_back( 'd' );
  labelsUniverse.push_back( 'c' );
  labelsUniverse.push_back( 'c' );
  labelsUniverse.push_back( 'z' );

  labelsClassified.push_back( 'a' );
  labelsClassified.push_back( 'b' );
  labelsClassified.push_back( 'c' );
  labelsClassified.push_back( 'd' );
  labelsClassified.push_back( 'd' );
  labelsClassified.push_back( 'y' );
  labelsClassified.push_back( 'c' );
  labelsClassified.push_back( 'u' );
  labelsClassified.push_back( 'd' );
  labelsClassified.push_back( 'k' );
  labelsClassified.push_back( 'd' );
  labelsClassified.push_back( 'c' );

  for( unsigned int i = 0; i < nbSamples; ++i )
    {
    ClassLabelType label = labelsUniverse[i];
    PLabelType plab;
    plab.SetSize( 1 );
    plab[0] = labelsClassified[i];
    if( i == 0 )
      {
      prodLabels->SetMeasurementVectorSize( itk::NumericTraits<PLabelType>::GetLength( plab ) );
      }
    refLabels->PushBack( label );
    prodLabels->PushBack( plab );
    }

  calculator->Compute( refLabels->Begin(), refLabels->End(), prodLabels->Begin(), prodLabels->End() );
  CalculatorType::ContingencyTableType confmat = calculator->GetContingencyTable();

  std::cout << std::endl;
  std::cout << "contingency Table" << std::endl << confmat << std::endl;
  unsigned int nbRefClasses = 5;
  unsigned int nbProdClasses = 7;

  if( calculator->GetNumberOfRefClasses() != nbRefClasses )
    {
    std::cerr << "Wrong number of reference classes " << calculator->GetNumberOfRefClasses() << " != " << nbRefClasses
              << std::endl;
    return EXIT_FAILURE;
    }
  if( calculator->GetNumberOfProdClasses() != nbProdClasses )
    {
    std::cerr << "Wrong number of produced classes " << calculator->GetNumberOfProdClasses() << " != " << nbProdClasses
              << std::endl;
    return EXIT_FAILURE;
    }
  if( calculator->GetNumberOfSamples() != nbSamples )
    {
    std::cerr << "Wrong number of samples" << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}

