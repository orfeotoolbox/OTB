/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkShapedNeighborhoodIterator.txx,v $
  Language:  C++
  Date:      $Date: 2009-03-03 15:09:17 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkShapedNeighborhoodIterator_txx
#define __itkShapedNeighborhoodIterator_txx
#include "itkShapedNeighborhoodIterator.h"
namespace itk {

  
template<class TImage, class TBoundaryCondition>
void
ShapedNeighborhoodIterator<TImage, TBoundaryCondition>
::PrintSelf(std::ostream &os, Indent indent) const
{
  os << indent <<  "ShapedNeighborhoodIterator = " << this
     << std::endl;
  Superclass::PrintSelf(os, indent.GetNextIndent());
}
  
} // namespace itk

#endif
