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
#include "otbDecisionTree.h"

enum WheatTypes { WinterWheat, SummerWheat };

int otbDecisionTreeWithRealValues(int itkNotUsed(argc), char** itkNotUsed(argv))
{
/** We build the following decision tree

                                         -------------
                  ( DOY LE 100  )
                   -------------
              Yes  -/    --   No
                --/      \--
           -------------  -------------
          ( NDVI GT 0.7 )    ( NDVI GT 0.7 )
           -------------  -------------
         / --           /\
        Yes  -/    \-  No         Yes-/   \- No
           -/         \     -/     \-
         WW           SW           SW        WW

  */
  typedef double AttributeValueType;

  typedef otb::DecisionTree< AttributeValueType, WheatTypes > DecisionTreeType;

  DecisionTreeType::Pointer decisionTree = DecisionTreeType::New();
  DecisionTreeType::Pointer doyLE100Tree = DecisionTreeType::New();
  DecisionTreeType::Pointer doyGT100Tree = DecisionTreeType::New();


  decisionTree->SetAttribute( 0 );
  decisionTree->AddBranch( 100, DecisionTreeType::GT, doyGT100Tree );
  decisionTree->AddBranch( 100, DecisionTreeType::LE, doyLE100Tree );

  doyLE100Tree->SetAttribute( 1 );
  doyLE100Tree->AddBranch( 0.7, DecisionTreeType::LT, SummerWheat );
  doyLE100Tree->AddBranch( 0.7, DecisionTreeType::GE, WinterWheat );

  doyGT100Tree->SetAttribute( 1 );
  doyGT100Tree->AddBranch( 0.7, DecisionTreeType::LE, WinterWheat );
  doyGT100Tree->AddBranch( 0.7, DecisionTreeType::GT, SummerWheat );

  // Build some examples for testing

  DecisionTreeType::ExampleType ww_in_summer; //(DoY), (NDVI)
  ww_in_summer.push_back(200);
  ww_in_summer.push_back(0.2);

  DecisionTreeType::ExampleType ww_in_winter;
  ww_in_winter.push_back(70);
  ww_in_winter.push_back(0.9);

  DecisionTreeType::ExampleType sw_in_winter;
  sw_in_winter.push_back(60);
  sw_in_winter.push_back(0.1);

  DecisionTreeType::ExampleType sw_in_summer;
  sw_in_summer.push_back(220);
  sw_in_summer.push_back(0.76);

  std::cout << "ww_in_summer" << std::endl;
  if( decisionTree->Decide( ww_in_summer ) != WinterWheat )
    {
    std::cerr << "ww_in_summer yields " << decisionTree->Decide( ww_in_summer ) << std::endl;
    return EXIT_FAILURE;
    }

  std::cout << "sw_in_winter" << std::endl;
  if( decisionTree->Decide( sw_in_winter ) != SummerWheat )
    {
    std::cerr << "sw_in_winter yields " << decisionTree->Decide( sw_in_winter ) << std::endl;
    return EXIT_FAILURE;
    }

  std::cout << "sw_in_summer" << std::endl;
  if( decisionTree->Decide( sw_in_summer ) != SummerWheat )
    {
    std::cerr << "sw_in_summer yields " << decisionTree->Decide( sw_in_summer ) << std::endl;
    return EXIT_FAILURE;
    }


  std::cout << "ww_in_winter" << std::endl;
  if( decisionTree->Decide( ww_in_winter ) != WinterWheat )
    {
    std::cerr << "ww_in_winter yields " << decisionTree->Decide( ww_in_winter ) << std::endl;
    return EXIT_FAILURE;
    }




  return EXIT_SUCCESS;
}
