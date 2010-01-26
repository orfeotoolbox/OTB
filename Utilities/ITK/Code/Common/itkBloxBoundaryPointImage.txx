/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBloxBoundaryPointImage.txx,v $
  Language:  C++
  Date:      $Date: 2006-03-18 18:06:14 $
  Version:   $Revision: 1.23 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkBloxBoundaryPointImage_txx
#define __itkBloxBoundaryPointImage_txx

#include "itkImageRegionIterator.h"
#include "itkBloxBoundaryPointImage.h"

namespace itk
{

template<unsigned int VImageDimension>
BloxBoundaryPointImage<VImageDimension>
::BloxBoundaryPointImage()
{
  m_NumBoundaryPoints = 0;
}

template<unsigned int VImageDimension>
BloxBoundaryPointImage<VImageDimension>
::~BloxBoundaryPointImage()
{

}

template<unsigned int VImageDimension>
void
BloxBoundaryPointImage<VImageDimension>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os << indent << "Total number of boundary points: "
     << m_NumBoundaryPoints << std::endl;
}

} // end namespace itk

#endif
