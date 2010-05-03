/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkNumericTraitsFixedArrayPixel2.cxx,v $
  Language:  C++
  Date:      $Date: 2010-04-10 16:22:29 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkNumericTraitsFixedArrayPixel.h"

namespace itk
{

//
//    Previous Types can be found in the file
//
//                itkNumericTraitsFixedArrayPixel.cxx
//
//    This was split in two files in order to help the Sun CC 5.6 compiler to
//    manage the size of the compilation unit.
//

itkStaticNumericTraitsGenericArrayDimensionsMacro( FixedArray, float );
itkStaticNumericTraitsGenericArrayDimensionsMacro( FixedArray, double );
itkStaticNumericTraitsGenericArrayDimensionsMacro( FixedArray, long double );
#ifdef ITK_TYPE_USE_LONG_LONG
itkStaticNumericTraitsGenericArrayDimensionsMacro( FixedArray, long long );
itkStaticNumericTraitsGenericArrayDimensionsMacro( FixedArray, unsigned long long );
#endif // ITK_TYPE_USE_LONG_LONG

} // end namespace itk
