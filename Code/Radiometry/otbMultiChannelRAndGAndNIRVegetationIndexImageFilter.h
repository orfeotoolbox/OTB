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
#ifndef __otbMultiChannelRAndGAndNIRVegetationIndexImageFilter_h
#define __otbMultiChannelRAndGAndNIRVegetationIndexImageFilter_h

#include "otbMultiChannelRAndGAndNIRIndexImageFilter.h"

namespace otb
{

/** \class MultiChannelRAndGAndNIRVegetationIndexImageFilter
   * \brief DEPRECATED : use otbMultiChannelRAndGAndNIRIndexImageFilter, Implements mutli channel R and G and NIR pixel-wise generic vegetation index operation on one vector image.
   *
   * This class is parameterized over the type of the input image and
   * the type of the output image.  It is also parameterized by the
   * operation to be applied, using a Functor style.
   *
   * \deprecated use otbMultiChannelRAndGAndNIRIndexImageFilter
   * \sa UnaryFunctorImageFilter
   * \ingroup Radiometry
 */
template <class TInputImage, class TOutputImage,
      class TFunction = Functor::AVI< typename TInputImage::InternalPixelType,
                       typename TInputImage::InternalPixelType,
                       typename TInputImage::InternalPixelType,
                       typename TOutputImage::PixelType>  >
class ITK_EXPORT MultiChannelRAndGAndNIRVegetationIndexImageFilter
  : public MultiChannelRAndGAndNIRIndexImageFilter<TInputImage,TOutputImage,TFunction>
{
};

} // end namespace otb

#endif
