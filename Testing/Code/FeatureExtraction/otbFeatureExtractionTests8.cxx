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

// this file defines the otbCommonTest for the test driver
// and all it expects is that you have a function called RegisterTests
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <iostream>
#include "otbTestMain.h" 

void RegisterTests()
{
REGISTER_TEST(otbRemoveTortuousPathListFilterNew);
REGISTER_TEST(otbRemoveTortuousPathListFilter);
REGISTER_TEST(otbLinkPathListFilterNew);
REGISTER_TEST(otbLinkPathListFilter);
REGISTER_TEST(otbLikelihoodPathListFilterNew);
REGISTER_TEST(otbLikelihoodPathListFilter);
REGISTER_TEST(otbParallelLinePathListFilterNew);
REGISTER_TEST(otbParallelLinePathListFilter);  
REGISTER_TEST(otbRoadExtractionFilterNew);
REGISTER_TEST(otbRoadExtractionFilter);
REGISTER_TEST(otbAddCarvingPathFilterNew);
REGISTER_TEST(otbRemoveCarvingPathFilterNew);
REGISTER_TEST(otbImageToCarvingPathFilterNew);
}
