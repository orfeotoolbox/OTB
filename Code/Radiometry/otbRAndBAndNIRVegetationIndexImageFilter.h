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
#ifndef __otbRAndBAndNIRVegetationIndexImageFilter_h
#define __otbRAndBAndNIRVegetationIndexImageFilter_h

#include "otbRAndBAndNIRIndexImageFilter.h"

namespace otb
{

/** \class RAndBAndNIRVegetationIndexImageFilter
   * \brief DEPRECATED : use otbRAndBAndNIRIndexImageFilter
   *
   * \deprecated use otbRAndBAndNIRIndexImageFilter
   * \ingroup Radiometry
   *
 */

template <class TInputImageR, class TInputImageB, class TInputImageNIR, class TOutputImage,
    class TFunction = Functor::ARVI<typename TInputImageR::PixelType,
        typename TInputImageB::PixelType,
        typename TInputImageNIR::PixelType,
        typename TOutputImage::PixelType> >
class ITK_EXPORT RAndBAndNIRVegetationIndexImageFilter :  public RAndBAndNIRIndexImageFilter<TInputImageR, TInputImageB,
      TInputImageNIR,
      TOutputImage, TFunction>
{
};

} // end namespace otb

#endif
