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

#ifndef otbThresholdVectorImageFilter_h
#define otbThresholdVectorImageFilter_h

#include "itkImageToImageFilter.h"

namespace otb
{

/** \class ThresholdVectorImageFilter
 * \brief Set image values to a user-specified value if they are below,
 * above, or between simple threshold values.
 *
 * ThresholdVectorImageFilter sets image values to a user-specified "outside"
 * value (by default, "black") if the image values are below, above, or
 * between simple threshold values.
 *
 * The pixels must support the operators >= and <=.
 *
 * \ingroup IntensityImageFilters Multithreaded
 *
 * \ingroup OTBImageManipulation
 */
  template <class TInputImage, class TOutputImage=TInputImage>
  class ITK_EXPORT ThresholdVectorImageFilter : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef ThresholdVectorImageFilter               Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage>  Superclass;
  typedef itk::SmartPointer<Self>                 Pointer;
  typedef itk::SmartPointer<const Self>           ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ThresholdVectorImageFilter, itk::ImageToImageFilter);


  /** Some additional typedefs.  */
  typedef TInputImage                                InputImageType;
  typedef typename InputImageType::ConstPointer InputImagePointer;
  typedef typename InputImageType::RegionType   InputImageRegionType;
  typedef typename InputImageType::PixelType    InputImagePixelType;
  typedef typename InputImageType::InternalPixelType    InputImageInternalPixelType;

  /** Some additional typedefs.  */
  typedef TOutputImage                             OutputImageType;
  typedef typename OutputImageType::Pointer     OutputImagePointer;
  typedef typename OutputImageType::RegionType  OutputImageRegionType;
  typedef typename OutputImageType::PixelType   OutputImagePixelType;
  typedef typename OutputImageType::InternalPixelType   OutputImageInternalPixelType;

  /** Set the "outside" pixel value. The default value
   * NumericTraits<PixelType>::Zero. */
  itkSetMacro(OutsideValue, OutputImageInternalPixelType);

  /** Get the "outside" pixel value. */
  itkGetConstMacro(OutsideValue, OutputImageInternalPixelType);

  /** The values greater than or equal to the value are set to OutsideValue. */
  void ThresholdAbove(const InputImageInternalPixelType &thresh);

  /** The values less than or equal to the value are set to OutsideValue. */
  void ThresholdBelow(const InputImageInternalPixelType &thresh);

  /** The values outside the range are set to OutsideValue. */
  void ThresholdOutside(const InputImageInternalPixelType &lower, const InputImageInternalPixelType &upper);

  /** Set/Get methods to set the lower threshold */
  itkSetMacro(Lower, InputImageInternalPixelType);
  itkGetConstMacro(Lower, InputImageInternalPixelType);

  /** Set/Get methods to set the upper threshold */
  itkSetMacro(Upper, InputImageInternalPixelType);
  itkGetConstMacro(Upper, InputImageInternalPixelType);


protected:
  ThresholdVectorImageFilter();
  ~ThresholdVectorImageFilter() override {};
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /** ThresholdVectorImageFilter can be implemented as a multithreaded filter.
   * Therefore, this implementation provides a ThreadedGenerateData() routine
   * which is called for each processing thread. The output image data is
   * allocated automatically by the superclass prior to calling
   * ThreadedGenerateData().  ThreadedGenerateData can only write to the
   * portion of the output image specified by the parameter
   * "outputRegionForThread"
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData()  */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            itk::ThreadIdType ThrethreadId ) override;

  void GenerateOutputInformation(void) override
  {
    Superclass::GenerateOutputInformation();

    this->GetOutput()->SetNumberOfComponentsPerPixel( this->GetInput()->GetNumberOfComponentsPerPixel() );
  }

private:
  ThresholdVectorImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  OutputImageInternalPixelType m_OutsideValue;
  InputImageInternalPixelType m_Lower;
  InputImageInternalPixelType m_Upper;
};


} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbThresholdVectorImageFilter.hxx"
#endif

#endif
