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
  REGISTER_TEST(otbRemoveIsolatedByDirectionFilterNew);
  REGISTER_TEST(otbRemoveIsolatedByDirectionFilter);
  REGISTER_TEST(otbRemoveWrongDirectionFilterNew);
  REGISTER_TEST(otbRemoveWrongDirectionFilter);
  REGISTER_TEST(otbNonMaxRemovalByDirectionFilterNew);
  REGISTER_TEST(otbNonMaxRemovalByDirectionFilter);
  REGISTER_TEST(otbVectorizationPathListFilterNew);
  REGISTER_TEST(otbVectorizationPathListFilter);
  REGISTER_TEST(otbSimplifyPathListFilterNew);
  REGISTER_TEST(otbSimplifyPathListFilter);
  REGISTER_TEST(otbBreakAngularPathListFilterNew);
  REGISTER_TEST(otbBreakAngularPathListFilter);
}
