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

#ifndef otbStreamingWarpImageFilter_h
#define otbStreamingWarpImageFilter_h

#include "itkWarpImageFilter.h"
#include "otbStreamingTraits.h"

namespace otb
{

/** \class StreamingWarpImageFilter
 * \brief This class acts like the itk::WarpImageFilter, but it does not request the largest possible region of the image to warp.
 *
 * Instead, the user should assess the maximum displacement in the displacement field and set it via the SetMaximumDisplacement() method.
 *
 * The filter will then compute an appropriate security margin according to the image spacing, the maximum displacement and the interpolator
 * radius in otb::StreamingTraits.
 *
 * This security margin is used to stream the input image, making this filter an entirely streamable one.
 *
 * If the maximum displacement is wrong, this filter is likely to request data outside of the input image buffered region. In this case, pixels
 * outside the region will be set to Zero according to itk::NumericTraits.
 *
 * \sa itk::WarpImageFilter
 *
 * \ingroup Streamed
 * \ingroup Threaded
 *
 * \ingroup OTBTransform
 */

template <class TInputImage, class TOutputImage, class TDisplacementField>
class ITK_EXPORT StreamingWarpImageFilter
  :  public itk::WarpImageFilter<TInputImage, TOutputImage, TDisplacementField>
{
public:
  /** Standard class typedefs. */
  typedef StreamingWarpImageFilter                                            Self;
  typedef itk::WarpImageFilter<TInputImage, TOutputImage, TDisplacementField> Superclass;
  typedef itk::SmartPointer<Self>                                             Pointer;
  typedef itk::SmartPointer<const Self>                                       ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(StreamingWarpImageFilter, itk::WarpImageFilter);

  /** template parameters typedef */
  typedef TInputImage                               InputImageType;
  typedef typename  InputImageType::Pointer         InputImagePointerType;
  typedef TOutputImage                              OutputImageType;
  typedef typename OutputImageType::PointType       PointType;
  typedef typename OutputImageType::IndexType       IndexType;
  typedef typename OutputImageType::PixelType       PixelType;
  typedef typename OutputImageType::SpacingType     SpacingType;
  typedef typename OutputImageType::Pointer         OutputImagePointerType;
  typedef typename OutputImageType::RegionType      OutputImageRegionType;
  typedef TDisplacementField                         DisplacementFieldType;
  typedef typename DisplacementFieldType::PixelType  DisplacementValueType;
  typedef typename DisplacementFieldType::Pointer    DisplacementFieldPointerType;
  typedef typename DisplacementFieldType::RegionType DisplacementFieldRegionType;

  /** Accessors */
  itkSetMacro(MaximumDisplacement, DisplacementValueType);
  itkGetConstReferenceMacro(MaximumDisplacement, DisplacementValueType);

  const SpacingType & GetOutputSpacing() const override
  {
    return m_OutputSignedSpacing;
  };

  void SetOutputSpacing( const SpacingType OutputSpacing ) override ;
  void SetOutputSpacing( const double *values ) override ;

protected:
  /** Constructor */
  StreamingWarpImageFilter();
  /** Destructor */
  ~StreamingWarpImageFilter() override {}
  /** PrintSelf */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;
  /**
   * This filters requires only a part of the input and of the displacement field to
   * produce its output. As such, we need to overload the GenerateInputRequestedRegion() method.
   */
  void GenerateInputRequestedRegion() override;

  void GenerateOutputInformation() override;

  /**
   * Re-implement the method ThreadedGenerateData to mask area outside the deformation grid
   */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            itk::ThreadIdType threadId ) override;

private:
  StreamingWarpImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

  //Because of itk positive spacing we need this member to be compliant with otb
  //signed spacing
  SpacingType m_OutputSignedSpacing;

  // Assessment of the maximum displacement for streaming
  DisplacementValueType m_MaximumDisplacement;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStreamingWarpImageFilter.hxx"
#endif

#endif
