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
#ifndef __otbRAndNIRVegetationIndexImageFilter_h
#define __otbRAndNIRVegetationIndexImageFilter_h

#include "otbRAndNIRIndexImageFilter.h"

namespace otb
{

/** \class RAndNIRVegetationIndexImageFilter
 * \brief DEPRECATED : use otbRAndNIRIndexImageFilter
 *
 * \deprecated use otbRAndNIRIndexImageFilter
 * \ingroup Radiometry
 */

template <class TInputImageR, class TInputImageNIR, class TOutputImage,
class TFunction = Functor::NDVI<              typename TInputImageR::PixelType,
typename TInputImageNIR::PixelType,
typename TOutputImage::PixelType > >
class ITK_EXPORT RAndNIRVegetationIndexImageFilter :  public RAndNIRIndexImageFilter< TInputImageR, TInputImageNIR, TOutputImage, TFunction >
{
};
} // end namespace otb


#endif
