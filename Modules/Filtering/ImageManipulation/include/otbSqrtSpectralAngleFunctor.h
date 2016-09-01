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
#ifndef otbSqrtSpectralAngleFunctor_h
#define otbSqrtSpectralAngleFunctor_h

#include "otbSpectralAngleFunctor.h"
#include "itkMacro.h"

namespace otb
{

namespace Functor
{

/** \class SqrtSpectralAngleFunctor
 *  \brief This functor computes the square root of a spectral angle according to a reference pixel.
 *
 * \ingroup OTBImageManipulation
 */
template <class TInputVectorPixel, class TOutputPixel>
class SqrtSpectralAngleFunctor : public SpectralAngleFunctor<TInputVectorPixel, TOutputPixel>
{
public:
  typedef SqrtSpectralAngleFunctor                              Self;
  typedef SpectralAngleFunctor<TInputVectorPixel, TOutputPixel> Superclass;

  SqrtSpectralAngleFunctor() {}
  ~SqrtSpectralAngleFunctor() ITK_OVERRIDE {}

protected:
  TOutputPixel Evaluate(const TInputVectorPixel& inPix) const ITK_OVERRIDE
  {
    return static_cast<TOutputPixel>(vcl_sqrt(Superclass::Evaluate(inPix)));
  }
};

} // end namespace Functor
}  // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#endif

#endif
