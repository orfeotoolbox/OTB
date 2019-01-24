/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbRadiometricMomentsImageFilter_h
#define otbRadiometricMomentsImageFilter_h

#include "itkImageToImageFilter.h"
#include "otbRadiometricMomentsFunctor.h"
#include "itkConstNeighborhoodIterator.h"

namespace otb
{

/**
 * \class RadiometricMomentsImageFilter
 * \brief Calculate the radiometric moments.
 *
 * Calculate the radiometric moments over a specified neighborhood
 *
 * This class is templated over the input image and the output image.
 *
 * \ingroup ImageFilters
 *
 * \ingroup OTBMoments
 */


template <class TInputImage, class TOutputImage>
class ITK_EXPORT RadiometricMomentsImageFilter :
  public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
    typedef RadiometricMomentsImageFilter                      Self;
    typedef itk::ImageToImageFilter<TInputImage,TOutputImage>  Superclass;
    typedef itk::SmartPointer<Self>                            Pointer;
    typedef itk::SmartPointer<const Self>                      ConstPointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(RadiometricMomentsImageFilter, ImageToImageFilter);

    /** Some convenient typedefs. */
    typedef typename Superclass::InputImageType           InputImageType;
    typedef typename InputImageType::ConstPointer         InputImagePointer;
    typedef typename InputImageType::RegionType           InputImageRegionType;
    typedef typename InputImageType::PixelType            InputImagePixelType;
    typedef typename InputImageType::SizeType             InputImageSizeType;
    typedef typename Superclass::OutputImageType          OutputImageType;
    typedef typename OutputImageType::Pointer             OutputImagePointer;
    typedef typename OutputImageType::RegionType          OutputImageRegionType;
    typedef typename OutputImageType::PixelType           OutputImagePixelType;
    typedef typename OutputImageType::InternalPixelType   ScalarType;

    typedef Functor::RadiometricMomentsFunctor< itk::ConstNeighborhoodIterator<InputImageType>, ScalarType>
                                                          FunctorType;

    typedef itk::ProcessObject ProcessObjectType;

    /**Set/Get the radius of neighborhood.*/
    itkSetMacro(Radius, InputImageSizeType);
    itkGetMacro(Radius, InputImageSizeType);

    /** Set unsigned int radius */
    void SetRadius(unsigned int radius)
    {
      m_Radius.Fill(radius);
    }

    typedef itk::ConstNeighborhoodIterator<TInputImage>   NeighborhoodIteratorType;
    typedef typename NeighborhoodIteratorType::RadiusType RadiusType;
    typedef unsigned char                                 RadiusSizeType;

protected:
  RadiometricMomentsImageFilter();
  ~RadiometricMomentsImageFilter() override {}
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId) override;
  void GenerateInputRequestedRegion(void) override;
  void GenerateOutputInformation(void) override;

private:
  RadiometricMomentsImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

  InputImageSizeType m_Radius;
  FunctorType m_Functor;
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRadiometricMomentsImageFilter.hxx"
#endif

#endif
