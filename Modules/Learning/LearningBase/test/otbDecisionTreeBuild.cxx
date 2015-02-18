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

int otbDecisionTreeBuild(int itkNotUsed(argc), char* itkNotUsed(argv) [])
{
/** We build the following decision tree

                                         -------------
                  ( DOY LT 100  )
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
  typedef bool AttributeValueType;

  typedef otb::DecisionTree< AttributeValueType, WheatTypes > DecisionTreeType;

  DecisionTreeType::Pointer decisionTree = DecisionTreeType::New();
  DecisionTreeType::Pointer doyLT100Tree = DecisionTreeType::New();
  DecisionTreeType::Pointer doyGT100Tree = DecisionTreeType::New();


  decisionTree->SetAttribute( 0 );
  decisionTree->AddBranch( false, doyGT100Tree );
  decisionTree->AddBranch( true, doyLT100Tree );

  doyLT100Tree->SetAttribute( 1 );
  doyLT100Tree->AddBranch( false, SummerWheat );
  doyLT100Tree->AddBranch( true, WinterWheat );

  doyGT100Tree->SetAttribute( 1 );
  doyGT100Tree->AddBranch( false, WinterWheat );
  doyGT100Tree->AddBranch( true, SummerWheat );

  // Build some examples for testing

  DecisionTreeType::ExampleType ww_in_summer; //(DoY<100), (NDVI>0.7)
  ww_in_summer.push_back(false);
  ww_in_summer.push_back(false);

  DecisionTreeType::ExampleType ww_in_winter;
  ww_in_winter.push_back(true);
  ww_in_winter.push_back(true);

  DecisionTreeType::ExampleType sw_in_winter;
  sw_in_winter.push_back(true);
  sw_in_winter.push_back(false);

  DecisionTreeType::ExampleType sw_in_summer;
  sw_in_summer.push_back(false);
  sw_in_summer.push_back(true);

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
