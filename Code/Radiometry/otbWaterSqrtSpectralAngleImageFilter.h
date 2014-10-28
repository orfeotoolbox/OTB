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
#ifndef __otbWaterSqrtSpectralAngleImageFilter_h
#define __otbWaterSqrtSpectralAngleImageFilter_h

#include "otbWaterIndicesFunctor.h"
#include "itkUnaryFunctorImageFilter.h"

namespace otb
{

/** \class WaterSqrtSpectralAngleImageFilter
 *  \brief Compute a radiometric water indice
 *
 *  This filter calculates a pixel wise water indice by calculating
 *  the spectral angle distance with a reference radiometry.
 *  This class is templated over the input multispectral image type,
 *  and the output image type.
 *  By default, the filter use the channel 0 as blue, the channel 1 as
 *  green, the channel 2 as red, and the channel 3 as near
 *  infrared. It can be changed with the SetBlue, SetGreen, SetRed and
 *  SetIR methods.
 *
 *  In the output image, value clse to zero denotes a highest
 *  probability of water.
 *
 *  \sa SpectralAngleDistanceImageFilter
 */

template <class TInputVectorImage, class TOutputImage,
    class TFunction = Functor::WaterSqrtSpectralAngleFunctor <
        typename TInputVectorImage::PixelType,
        typename TOutputImage::PixelType> >
class ITK_EXPORT WaterSqrtSpectralAngleImageFilter :
  public itk::UnaryFunctorImageFilter<TInputVectorImage, TOutputImage, TFunction>
{
public:
  /** Standard class typedefs. */
  typedef WaterSqrtSpectralAngleImageFilter                                        Self;
  typedef itk::UnaryFunctorImageFilter<TInputVectorImage, TOutputImage, TFunction> Superclass;
  typedef itk::SmartPointer<Self>                                                  Pointer;
  typedef itk::SmartPointer<const Self>                                            ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  /** Run-time type information (and related methods). */
  itkTypeMacro(WaterSqrtSpectralAngleImageFilter, itk::UnaryFunctorImageFilter);

  /** Some convenient typedef. */
  typedef TInputVectorImage                        InputVectorImageType;
  typedef typename InputVectorImageType::PixelType InputVectorPixelType;
  typedef TOutputImage                             OutputImageType;

protected:
  WaterSqrtSpectralAngleImageFilter() {}
  virtual ~WaterSqrtSpectralAngleImageFilter() {}

private:
  WaterSqrtSpectralAngleImageFilter(Self &); // purposely not implemented
  void operator =(const Self&);    // purposely not implemented

};
}  // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#endif

#endif
