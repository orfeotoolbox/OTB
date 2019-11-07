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

#ifndef otbKuanImageFilter_h
#define otbKuanImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkImage.h"
#include "itkNumericTraits.h"

namespace otb
{

/** \class KuanImageFilter
 * \brief Anti-speckle image filter
 *
 * This class implements Kuan filter for despeckleing of SAR
 * images.
 *
 * (http://www.isprs.org/proceedings/XXXV/congress/comm2/papers/110.pdf)
 *
 * \ingroup OTBImageNoise
 */

template <class TInputImage, class TOutputImage>
class ITK_EXPORT KuanImageFilter : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /**   Extract input and output image dimension */
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  typedef TInputImage  InputImageType;
  typedef TOutputImage OutputImageType;

  /** standard class typedefs */
  typedef KuanImageFilter Self;
  typedef itk::ImageToImageFilter<InputImageType, OutputImageType> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Object factory management */
  itkNewMacro(Self);

  /** typemacro */
  itkTypeMacro(KuanImageFilter, ImageToImageFilter);

  typedef typename InputImageType::PixelType                    InputPixelType;
  typedef typename OutputImageType::PixelType                   OutputPixelType;
  typedef typename itk::NumericTraits<InputPixelType>::RealType InputRealType;
  typedef typename InputImageType::RegionType                   InputImageRegionType;
  typedef typename OutputImageType::RegionType                  OutputImageRegionType;
  typedef typename InputImageType::SizeType                     SizeType;

  /** Set the radius of the neighborhood used in this filter */
  itkSetMacro(Radius, SizeType);

  /** Get the radius of the neighborhood used in this filter  */
  itkGetConstReferenceMacro(Radius, SizeType);

  /** Set the number of look used for computation */
  itkSetMacro(NbLooks, double);
  /** Getthe number of look used for computation */
  itkGetConstReferenceMacro(NbLooks, double);

  /** KuanImageFilter needs a larger input requested region than
   * the output requested region.  As such, KuanImageFilter needs
   * to provide an implementation for GenerateInputRequestedRegion()
   * in order to inform the pipeline execution model.
   *
   * \sa ImageToImageFilter::GenerateInputRequestedRegion() */
  void GenerateInputRequestedRegion() throw(itk::InvalidRequestedRegionError) override;

protected:
  KuanImageFilter();
  ~KuanImageFilter() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /** KuanImageFilter can be multithreaded.
   */

  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId) override;

private:
  KuanImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** Radius of the filter */
  SizeType m_Radius;
  /** Number of look of the filter */
  double m_NbLooks;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbKuanImageFilter.hxx"
#endif

#endif
