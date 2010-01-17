/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBloxBoundaryPointPixel.h,v $
  Language:  C++
  Date:      $Date: 2007-02-08 16:56:10 $
  Version:   $Revision: 1.7 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkBloxBoundaryPointPixel_h
#define __itkBloxBoundaryPointPixel_h

#include "itkBloxBoundaryPointItem.h"
#include "itkBloxPixel.h"

namespace itk
{

/**
 * \class BloxBoundaryPointPixel
 * \brief Holds a linked list of itk::BloxBoundaryPointItem's.
 *
 * \ingroup ImageObjects 
 * */

template <unsigned int NDimensions>
class ITK_EXPORT BloxBoundaryPointPixel : public BloxPixel< 
                                          BloxBoundaryPointItem<NDimensions> >
{
public:
  BloxBoundaryPointPixel();
  ~BloxBoundaryPointPixel();
};

} // end namespace itk

// Define instantiation macro for this template.
#define ITK_TEMPLATE_BloxBoundaryPointPixel(_, EXPORT, x, y) namespace itk { \
  _(1(class EXPORT BloxBoundaryPointPixel< ITK_TEMPLATE_1 x >)) \
  namespace Templates { typedef BloxBoundaryPointPixel< ITK_TEMPLATE_1 x > \
                                            BloxBoundaryPointPixel##y; } \
  }

#if ITK_TEMPLATE_EXPLICIT
# include "Templates/itkBloxBoundaryPointPixel+-.h"
#endif

#if ITK_TEMPLATE_TXX
# include "itkBloxBoundaryPointPixel.txx"
#endif

#endif
