/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbVectorDataToImageFilter_h
#define __otbVectorDataToImageFilter_h

#include "itkImageSource.h"

namespace otb
{
/** \class VectorDataToImageFilter
   *   \brief Render a vector data into an image
   *
 */


template <class TVectorData, class TImage>
  class VectorDataToImageFilter
    : public itk::ImageSource
{




}; // end class
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorDataToImageFilter.txx"
#endif

#endif