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

#ifndef otbGridResampleImageFilter_h
#define otbGridResampleImageFilter_h


#include "itkImageToImageFilter.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkDefaultConvertPixelTraits.h"

#include "otbMacro.h"

namespace otb
{

/** \class GridResampleImageFilter
 *  \brief Resample input image on a new origin/spacing/size grid
 *
 *  This filter resample the input image on a new grid defined by
 *  OutputOrigin, OutputSpacing and OutputSize, using the provided
 *  interpolator.
 *
 *  This is equivalent to a itk::ResampleImageFilter using an
 *  itk::IdentityTransform, except that it is slightly more efficient
 *  and that in this simplified case it is possible to explicitly
 *  compute the input requested region. The GridResampleImageFilter
 *  therefore supports streaming, contrary to the
 *  itk::ResampleImageFilter.
 *
 *  When grid position is outside of the input image domain, the
 *  default EdgePaddingValue is used.
 *
 *  If CheckOutputBounds flag is set to true (default value), the
 *  interpolated value will be checked for output pixel type range
 *  prior to casting.
 *
 * \ingroup OTBImageManipulation
 * \ingroup Streamed
 * \ingroup Threaded
 **/

template <typename TInputImage, typename TOutputImage, typename TInterpolatorPrecision = double>
class ITK_EXPORT GridResampleImageFilter : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef GridResampleImageFilter Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(GridResampleImageFilter, itk::ImageToImageFilter);

  /** Number of dimensions. */
  itkStaticConstMacro(ImageDimension, unsigned int, TOutputImage::ImageDimension);
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);

  /** Typedef parameters*/
  typedef TInputImage                          InputImageType;
  typedef TOutputImage                         OutputImageType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;
  typedef typename TOutputImage::PixelType     OutputPixelType;


  typedef itk::DefaultConvertPixelTraits<OutputPixelType> OutputPixelConvertType;
  typedef typename OutputPixelConvertType::ComponentType  OutputPixelComponentType;

  /** Interpolator type */
  typedef itk::InterpolateImageFunction<InputImageType, TInterpolatorPrecision> InterpolatorType;
  typedef typename InterpolatorType::Pointer InterpolatorPointerType;
  typedef itk::LinearInterpolateImageFunction<InputImageType, TInterpolatorPrecision> DefaultInterpolatorType;
  typedef typename InterpolatorType::OutputType                  InterpolatorOutputType;
  typedef itk::DefaultConvertPixelTraits<InterpolatorOutputType> InterpolatorConvertType;
  typedef typename InterpolatorConvertType::ComponentType        InterpolatorComponentType;

  /** Input pixel continuous index typdef */
  typedef typename itk::ContinuousIndex<double, InputImageDimension> ContinuousInputIndexType;

  /** ImageBase typedef */
  typedef itk::ImageBase<OutputImageType::ImageDimension> ImageBaseType;
  typedef typename ImageBaseType::SpacingType             SpacingType;
  typedef typename ImageBaseType::SizeType                SizeType;
  typedef typename ImageBaseType::PointType               PointType;
  typedef typename ImageBaseType::IndexType               IndexType;

  itkSetMacro(OutputStartIndex, IndexType);
  itkGetConstReferenceMacro(OutputStartIndex, IndexType);

  itkSetMacro(OutputSize, SizeType);
  itkGetConstReferenceMacro(OutputSize, SizeType);

  itkSetMacro(OutputOrigin, PointType);
  itkGetConstReferenceMacro(OutputOrigin, PointType);

  itkSetMacro(OutputSpacing, SpacingType);
  itkGetConstReferenceMacro(OutputSpacing, SpacingType);

  itkSetMacro(EdgePaddingValue, OutputPixelType);
  itkGetConstReferenceMacro(EdgePaddingValue, OutputPixelType);

  itkSetMacro(CheckOutputBounds, bool);
  itkGetMacro(CheckOutputBounds, bool);
  itkBooleanMacro(CheckOutputBounds);

  itkSetMacro(InterpolationMargin, double);
  itkGetMacro(InterpolationMargin, double);

  itkSetObjectMacro(Interpolator, InterpolatorType);
  itkGetObjectMacro(Interpolator, InterpolatorType);

  /** Import output parameters from a given image */
  void SetOutputParametersFromImage(const ImageBaseType* image);

  /** Method Compute the Modified Time based on changed to the components. */
  itk::ModifiedTimeType GetMTime(void) const override;

protected:
  GridResampleImageFilter();

  /** Destructor */
  ~GridResampleImageFilter() override{};

  void GenerateOutputInformation() override;

  void GenerateInputRequestedRegion() override;

  void BeforeThreadedGenerateData() override;

  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId) override;

  void AfterThreadedGenerateData() override;

  inline void CastPixelWithBoundsChecking(const InterpolatorOutputType& value, const InterpolatorComponentType& minComponent,
                                          const InterpolatorComponentType& maxComponent, OutputPixelType& outputValue) const
  {
    // Method imported from itk::ResampleImageFilter
    const unsigned int nComponents = InterpolatorConvertType::GetNumberOfComponents(value);

    itk::NumericTraits<OutputPixelType>::SetLength(outputValue, nComponents);

    for (unsigned int n = 0; n < nComponents; n++)
    {
      InterpolatorComponentType component = InterpolatorConvertType::GetNthComponent(n, value);

      if (m_CheckOutputBounds && component < minComponent)
      {
        OutputPixelConvertType::SetNthComponent(n, outputValue, static_cast<OutputPixelComponentType>(minComponent));
      }
      else if (m_CheckOutputBounds && component > maxComponent)
      {
        OutputPixelConvertType::SetNthComponent(n, outputValue, static_cast<OutputPixelComponentType>(maxComponent));
      }
      else
      {
        OutputPixelConvertType::SetNthComponent(n, outputValue, static_cast<OutputPixelComponentType>(component));
      }
    }
  }


  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  GridResampleImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  IndexType   m_OutputStartIndex; // output image start index
  SizeType    m_OutputSize;       // output image size
  PointType   m_OutputOrigin;     // output image origin
  SpacingType m_OutputSpacing;    // output image spacing

  OutputPixelType m_EdgePaddingValue; // Default pixel value

  double m_InterpolationMargin;

  bool m_CheckOutputBounds; // Shall we check
                            // output bounds when
                            // casting?

  InterpolatorPointerType m_Interpolator; // Interpolator used
                                          // for resampling

  OutputImageRegionType m_ReachableOutputRegion; // Internal
                                                 // variable for
                                                 // speed-up. Computed
                                                 // in BeforeThreadedGenerateData
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGridResampleImageFilter.hxx"
#endif

#endif
