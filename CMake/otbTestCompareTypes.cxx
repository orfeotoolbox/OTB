/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


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
