/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbFrostImageFilter_h
#define otbFrostImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkImage.h"
#include "itkNumericTraits.h"

namespace otb
{

/** \class FrostImageFilter
 * \brief Anti-speckle image filter
 *
 * Uses a negative exponential convolution kernel.
 * The kernel is defined as follows:
 * exp(-A*D), where 
 * D is the distance from the current pixel to the center pixel
 * A = k*Ci*Ci  with Ci = VAR[I]/ (E[I]*E[I])
 * The final result is normalized by the sum of the kernel coefficients.
 *
 * (http://www.isprs.org/proceedings/XXXV/congress/comm2/papers/110.pdf)
 * 
 * \ingroup OTBImageNoise
 */

template <class TInputImage, class TOutputImage>
class ITK_EXPORT FrostImageFilter :  public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Extract input and output images sizes. */
  itkStaticConstMacro(InputImageDimension,
                      unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension,
                      unsigned int,
                      TOutputImage::ImageDimension);

  /** typedef to simplify variables definition and declaration. */
  typedef TInputImage InputImageType;
  /** typedef to simplify variables definition and declaration. */
  typedef TOutputImage OutputImageType;

  /** typedef for standard classes. */
  typedef FrostImageFilter                                         Self;
  typedef itk::ImageToImageFilter<InputImageType, OutputImageType> Superclass;
  typedef itk::SmartPointer<Self>                                  Pointer;
  typedef itk::SmartPointer<const Self>                            ConstPointer;

  /** "object factory" management method. */
  itkNewMacro(Self);

  /** Return the class name. */
  itkTypeMacro(FrostImageFilter, ImageToImageFilter);

  /** Supported images definition. */
  typedef typename InputImageType::PixelType  InputPixelType;
  typedef typename OutputImageType::PixelType OutputPixelType;
  /** "typedef" to define a real. */
  typedef typename itk::NumericTraits<InputPixelType>::RealType InputRealType;

  typedef typename InputImageType::RegionType  InputImageRegionType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;

  /** "typedef" to define an image size. */
  typedef typename InputImageType::SizeType SizeType;

  /** Set the radius used to define the neighborhood for the filter calculation. */
  itkSetMacro(Radius, SizeType);

  /** Get the radius used to define the neighborhood for the filter calculation. */
  itkGetConstReferenceMacro(Radius, SizeType);

  /** Set the damping factor. */
  itkSetMacro(Deramp, double);
  /** Get the damping factor. */
  itkGetConstReferenceMacro(Deramp, double);

  /** To be allowed to use the pipeline method FrostImageFilter needs
    * an input processing area larger than the output one.
    * \sa ImageToImageFilter::GenerateInputRequestedRegion() */
  void GenerateInputRequestedRegion()
    throw(itk::InvalidRequestedRegionError) override;

protected:
  FrostImageFilter();
  ~FrostImageFilter() override {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /** FrostImageFilter can be implemented for a multithreaded filter treatment.
   * Thus, this implementation give the ThreadedGenerateData() method.
   * that is called for each process thread. Image datas are automatically allocated
   * through the parent class calling the ThreadedGenerateData() method.
   * ThreadedGenerateData() can only write the area of the image specified by the parameter "outputRegionForThread"
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData() */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            itk::ThreadIdType threadId) override;

private:
  FrostImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

  /** Radius declaration */
  SizeType m_Radius;
  /** Decrease factor declaration */
  double m_Deramp;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbFrostImageFilter.hxx"
#endif

#endif
