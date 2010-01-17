/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkCostFunction.cxx,v $
  Language:  C++
  Date:      $Date: 2003-09-10 14:29:37 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkCostFunction.h"

namespace itk
{
void
CostFunction
::PrintSelf(std::ostream& os, Indent indent) const
{ 
  Superclass::PrintSelf(os,indent);
}

} // end namespace itk
