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
#include "otbDecisionTree.h"

enum WheatTypes { WinterWheat, SummerWheat };

int otbDecisionTreeBuild(int argc, char* argv[])
{
/** We build the following decision tree

         	       	       	       	   -------------      
				          ( DOY LT 100  )     
				           -------------      
				    	Yes  -/	  --   No     
				    	  --/	    \--	      
			     -------------	------------- 
			    ( NDVI GT 0.7 )    ( NDVI GT 0.7 )
			     -------------	------------- 
				 / --		       /\     
			  Yes  -/    \-	No     	  Yes-/	 \- No
			     -/	       \	   -/	   \- 
			   WW  	       SW      	   SW  	    WW

  */
  typedef bool AttributeValueType;

  typedef otb::DecisionTree< AttributeValueType, WheatTypes > DecisionTreeType;

  DecisionTreeType::Pointer decisionTree = DecisionTreeType::New();
  DecisionTreeType::Pointer ndviGT07Tree = DecisionTreeType::New();
  DecisionTreeType::Pointer ndviLT07Tree = DecisionTreeType::New();


  // which atribute is tested at the root node
  decisionTree->SetAttribute( 0 );
  decisionTree->AddBranch( true, ndviGT07Tree );
  decisionTree->AddBranch( false, ndviLT07Tree );

  ndviGT07Tree->SetAttribute( 1 );
  ndviGT07Tree->AddBranch( true, WinterWheat );
  ndviGT07Tree->AddBranch( false, SummerWheat );

  ndviLT07Tree->SetAttribute( 1 );
  ndviLT07Tree->AddBranch( true, SummerWheat );
  ndviLT07Tree->AddBranch( false, WinterWheat );

  
  return EXIT_SUCCESS;
}

