/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBloxBoundaryPointPixel.txx,v $
  Language:  C++
  Date:      $Date: 2003-09-10 14:29:01 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkBloxBoundaryPointPixel_txx
#define __itkBloxBoundaryPointPixel_txx

#include "itkBloxBoundaryPointPixel.h"

namespace itk
{

template <unsigned int NDimensions>
BloxBoundaryPointPixel<NDimensions>
::BloxBoundaryPointPixel()
{

}

template <unsigned int NDimensions>
BloxBoundaryPointPixel<NDimensions>
::~BloxBoundaryPointPixel()
{
  // The default destructor walks the pixel and deletes all bloxitems
}

} // end namespace itk

#endif
