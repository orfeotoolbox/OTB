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
#ifndef __otbRAndGAndNIRVegetationIndexImageFilter_h
#define __otbRAndGAndNIRVegetationIndexImageFilter_h

#include "vcl_deprecated_header.h"

#include "otbRAndGAndNIRIndexImageFilter.h"

namespace otb
{

/** \class RAndGAndNIRVegetationIndexImageFilter
 * \brief DEPRECATED : use otbRAndGAndNIRIndexImageFilter
 *
 * \deprecated use otbRAndGAndNIRIndexImageFilter
 * \ingroup Radiometry
 *
 *
 */
template <class TInputImageR, class TInputImageG, class TInputImageNIR, class TOutputImage,
    class TFunction = Functor::AVI<typename TInputImageR::PixelType,
        typename TInputImageG::PixelType,
        typename TInputImageNIR::PixelType,
        typename TOutputImage::PixelType> >
class ITK_EXPORT RAndGAndNIRVegetationIndexImageFilter :  public RAndGandNIRIndexImageFilter<TInputImageR, TInputImageG, TInputImageNIR,
      TOutputImage, TFunction>
{
};
} // end namespace otb


#endif
