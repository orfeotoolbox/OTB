/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBloxBoundaryPointItem.txx,v $
  Language:  C++
  Date:      $Date: 2003/09/10 14:29:01 $
  Version:   $Revision: 1.5 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkBloxBoundaryPointItem_txx
#define __itkBloxBoundaryPointItem_txx

#include "itkBloxBoundaryPointItem.h"

namespace itk
{

template <unsigned int VImageDimension>
BloxBoundaryPointItem<VImageDimension>
::BloxBoundaryPointItem()
{
  m_PhysicalPosition.Fill(0);
  m_Gradient.Fill(0);
}

template <unsigned int VImageDimension>
BloxBoundaryPointItem<VImageDimension>
::~BloxBoundaryPointItem()
{

}

} // end namespace itk

#endif
