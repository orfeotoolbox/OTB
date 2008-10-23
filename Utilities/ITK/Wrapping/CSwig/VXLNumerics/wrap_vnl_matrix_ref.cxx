/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: wrap_vnl_matrix_ref.cxx,v $
  Language:  C++
  Date:      $Date: 2003-09-10 14:30:12 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "vnl/vnl_matrix_ref.h"

#ifdef CABLE_CONFIGURATION
#include "wrap_VXLNumerics.h"

namespace _cable_
{
  const char* const group = ITK_WRAP_GROUP(vnl_matrix_ref);
  namespace wrappers
  {
    typedef vnl_matrix_ref<double> vnl_matrix_ref_double;
    typedef vnl_matrix_ref<float>  vnl_matrix_ref_float;
  }
}

void force_instantiate()
{
  using namespace _cable_::wrappers;
  sizeof(vnl_matrix_ref_double);
  sizeof(vnl_matrix_ref_float);
}

#endif
