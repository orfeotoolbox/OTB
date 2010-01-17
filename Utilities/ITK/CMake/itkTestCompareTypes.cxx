/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkTestCompareTypes.cxx,v $
  Language:  C++
  Date:      $Date: 2005-01-22 21:02:11 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
typedef ITK_TEST_COMPARE_TYPE_1 Type1;
typedef ITK_TEST_COMPARE_TYPE_2 Type2;

void function(Type1**) {}

int main()
{
  Type2** p = 0;
  function(p);
  return 0;
}
