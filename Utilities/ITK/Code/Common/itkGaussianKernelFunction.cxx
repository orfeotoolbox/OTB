/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkGaussianKernelFunction.cxx,v $
  Language:  C++
  Date:      $Date: 2008-10-17 01:08:45 $
  Version:   $Revision: 1.7 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkGaussianKernelFunction.h"
#include "vnl/vnl_math.h"

namespace itk
{

/**
 * Initialize static const m_Factor
 */
const double GaussianKernelFunction::m_Factor =
1.0 / vcl_sqrt( 2.0 * vnl_math::pi );

GaussianKernelFunction::GaussianKernelFunction()
{
}

GaussianKernelFunction::~GaussianKernelFunction()
{
}

} // namespace itk
