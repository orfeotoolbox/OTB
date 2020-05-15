/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbClampVectorImageFilter_h
#define otbClampVectorImageFilter_h

#include "itkImageToImageFilter.h"

namespace otb
{

/** \class ClampVectorImageFilter
 * \brief Set image values to a user-specified value if they are below,
 * above, or between simple threshold values.
 *
 * ClampVectorImageFilter  clamp image values to be between an upper
 * and lower value. Values lower than m_Lower values are set to lower,
 * and values greater than upper threshold are set to upper threshold
 * value.
 *
 * By default lower and upper thresholds are set to the maximum and
 * minimum bounds of the image pixel type.
 *
 * The pixels must support the operators >= and <=.
 *
 * \ingroup IntensityImageFilters Multithreaded
 *
 * \ingroup OTBImageManipulation
 */
template <class TInputImage, class TOutputImage = TInputImage>
class ITK_EXPORT ClampVectorImageFilter : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef ClampVectorImageFilter Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ClampVectorImageFilter, itk::ImageToImageFilter);


  /** Some additional typedefs.  */
  typedef TInputImage                                InputImageType;
  typedef typename InputImageType::ConstPointer      InputImagePointer;
  typedef typename InputImageType::RegionType        InputImageRegionType;
  typedef typename InputImageType::PixelType         InputImagePixelType;
  typedef typename InputImageType::InternalPixelType InputImageInternalPixelType;

  /** Some additional typedefs.  */
  typedef TOutputImage                                OutputImageType;
  typedef typename OutputImageType::Pointer           OutputImagePointer;
  typedef typename OutputImageType::RegionType        OutputImageRegionType;
  typedef typename OutputImageType::PixelType         OutputImagePixelType;
  typedef typename OutputImageType::InternalPixelType OutputImageInternalPixelType;


  /** The values greater than or equal to the value are set to OutsideValue. */
  void ClampAbove(const OutputImageInternalPixelType& thresh);

  /** The values less than or equal to the value are set to OutsideValue. */
  void ClampBelow(const OutputImageInternalPixelType& thresh);

  /** The values outside the range are set to OutsideValue. */
  void ClampOutside(const OutputImageInternalPixelType& lower, const OutputImageInternalPixelType& upper);

  /** Set/Get methods to set the lower threshold */
  void SetLower(OutputImageInternalPixelType val)
  {
    m_Lower  = val;
    m_DLower = static_cast<double>(val);
    this->Modified();
  }
  itkGetConstMacro(Lower, OutputImageInternalPixelType);

  /** Set/Get methods to set the upper threshold */
  void SetUpper(OutputImageInternalPixelType val)
  {
    m_Upper  = val;
    m_DUpper = static_cast<double>(val);
    this->Modified();
  }
  itkGetConstMacro(Upper, OutputImageInternalPixelType);


protected:
  ClampVectorImageFilter();
  ~ClampVectorImageFilter() override{};
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /** ClampVectorImageFilter can be implemented as a multithreaded filter.
   * Therefore, this implementation provides a ThreadedGenerateData() routine
   * which is called for each processing thread. The output image data is
   * allocated automatically by the superclass prior to calling
   * ThreadedGenerateData().  ThreadedGenerateData can only write to the
   * portion of the output image specified by the parameter
   * "outputRegionForThread"
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData()  */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId) override;

  void GenerateOutputInformation(void) override
  {
    Superclass::GenerateOutputInformation();

    this->GetOutput()->SetNumberOfComponentsPerPixel(this->GetInput()->GetNumberOfComponentsPerPixel());
  }

private:
  ClampVectorImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  double m_DLower;
  double m_DUpper;

  OutputImageInternalPixelType m_Lower;
  OutputImageInternalPixelType m_Upper;
};


} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbClampVectorImageFilter.hxx"
#endif

#endif
