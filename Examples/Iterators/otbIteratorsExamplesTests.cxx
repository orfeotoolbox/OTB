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

// this file defines the otbMultiScaleTest for the test driver
// and all it expects is that you have a function called RegisterTests
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif
#include <iostream>
#include "otbTestMain.h"

void RegisterTests()
{
  REGISTER_TEST(ImageRegionIteratorWithIndexTest);
  REGISTER_TEST(NeighborhoodIterators1Test);
  REGISTER_TEST(NeighborhoodIterators4Test);
  REGISTER_TEST(NeighborhoodIterators6Test);
  REGISTER_TEST(ImageRegionIteratorTest);
  REGISTER_TEST(ShapedNeighborhoodIterators2Test);
}

#undef main
#define main ImageRegionIteratorWithIndexTest
#include "ImageRegionIteratorWithIndex.cxx"

#undef main
#define main NeighborhoodIterators1Test
#include "NeighborhoodIterators1.cxx"

#undef main
#define main NeighborhoodIterators4Test
#include "NeighborhoodIterators4.cxx"

#undef main
#define main NeighborhoodIterators6Test
#include "NeighborhoodIterators6.cxx"

#undef main
#define main ImageRegionIteratorTest
#include "ImageRegionIterator.cxx"

#undef main
#define main ShapedNeighborhoodIterators2Test
#include "ShapedNeighborhoodIterators2.cxx"

