/*=========================================================================

  Program:   Orfeo Toolbox
  Module:    $RCSfile: otbTestCompareTypes.cxx,v $
  Language:  C++
  Date:      $Date: 2006/07/10 15:31:11 $
  Version:   $Revision: 1.1 $

  Copyright (c)  Orfeo Toolbox. All rights reserved.
  See OTBCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
typedef OTB_TEST_COMPARE_TYPE_1 Type1;
typedef OTB_TEST_COMPARE_TYPE_2 Type2;

void function(Type1**) {}

int main()
{
  Type2** p = 0;
  function(p);
  return 0;
}
