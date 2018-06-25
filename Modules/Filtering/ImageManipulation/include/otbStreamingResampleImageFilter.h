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

#ifndef otbStreamingResampleImageFilter_h
#define otbStreamingResampleImageFilter_h

#include "itkImageToImageFilter.h"
#include "otbStreamingWarpImageFilter.h"
#include "itkTransformToDisplacementFieldSource.h"
#include "itkLinearInterpolateImageFunction.h"
#include "otbImage.h"
#include "itkVector.h"

#include "otbMacro.h"

namespace otb
{

/** \class StreamingResampleImageFilter
 *  \brief This class is a composite filter resampling an input image
 *         by setting a transform. The filter computes a displacement
 *         grid using the transform set and used it to warp the input.
 *
 * The otb::StreamingResampleImageFilter allows resampling a
 * otb::VectorImage using a transformation set with SetTransform()
 * method. First, a displacement grid, with a spacing m_DisplacementGridSpacing
 * and a size relative to this spacing, is built. Then, the image is
 * wraped using this displacement grid. The size (SetOutputSize()), the
 * spacing (SetOutputSpacing()), the start index (SetOutputIndex()) and
 * the  interpolator (SetInterpolator()) and the origin (SetOrigin())
 * can be set using the method between brackets.
 *
 *
 *
 * \ingroup Projection
 *
 *
 * \ingroup OTBImageManipulation
 **/

template <class TInputImage, class TOutputImage,
          class TInterpolatorPrecisionType = double>
class ITK_EXPORT StreamingResampleImageFilter :
    public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef StreamingResampleImageFilter                                Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage>    Superclass;
  typedef itk::SmartPointer<Self>                               Pointer;
  typedef itk::SmartPointer<const Self>                         ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(StreamingResampleImageFilter, itk::ImageToImageFilter);

  /** Typedef parameters*/
  typedef TInputImage                InputImageType;
  typedef TOutputImage               OutputImageType;

  /** Displacement field used to warp the image*/
  typedef itk::Vector<double,
                      TOutputImage::ImageDimension>              DisplacementType;
  typedef otb::Image<DisplacementType>                           DisplacementFieldType;

  /** filter warping input image using displacement field */
  typedef StreamingWarpImageFilter<InputImageType,
                                   OutputImageType,
                                   DisplacementFieldType>        WarpImageFilterType;

  /** Internal filters typedefs*/
  typedef itk::TransformToDisplacementFieldSource<DisplacementFieldType,
                                                 double>        DisplacementFieldGeneratorType;
  typedef typename DisplacementFieldGeneratorType::TransformType TransformType;
  typedef typename DisplacementFieldGeneratorType::SizeType      SizeType;
  typedef typename DisplacementFieldGeneratorType::SpacingType   SpacingType;
  typedef typename DisplacementFieldGeneratorType::OriginType    OriginType;
  typedef typename DisplacementFieldGeneratorType::IndexType     IndexType;
  typedef typename DisplacementFieldGeneratorType::RegionType    RegionType;

  /** Interpolator type */
  typedef itk::InterpolateImageFunction<InputImageType,
                                        TInterpolatorPrecisionType>       InterpolatorType;
  typedef typename InterpolatorType::Pointer                              InterpolatorPointerType;
  typedef itk::LinearInterpolateImageFunction<InputImageType,
                                              TInterpolatorPrecisionType> DefaultInterpolatorType;

  /** ImageBase typedef */
  typedef itk::ImageBase<OutputImageType::ImageDimension>     ImageBaseType;


  /** Accessors to internal filters parameters */
  void SetTransform(TransformType * transform)
  {
    m_DisplacementFilter->SetTransform(transform);
    this->Modified();
  }
  otbGetObjectMemberConstMacro(DisplacementFilter, Transform, const TransformType*);

  /** The Displacement field spacing & size */
  void SetDisplacementFieldSpacing( SpacingType spacing);

  const SpacingType & GetDisplacementFieldSpacing() const
  {
    return m_SignedOutputSpacing;
  };

  /** The resampled image parameters */
  // Output Origin
  void SetOutputOrigin(const OriginType & origin)
  {
    m_DisplacementFilter->SetOutputOrigin(origin);
    m_WarpFilter->SetOutputOrigin(origin);
    this->Modified();
  }
  otbGetObjectMemberConstReferenceMacro(WarpFilter, OutputOrigin, OriginType);

  // Output Start index
  otbSetObjectMemberMacro(WarpFilter, OutputStartIndex, IndexType);
  otbGetObjectMemberConstReferenceMacro(WarpFilter, OutputStartIndex, IndexType);

  // Output Size
  otbSetObjectMemberMacro(WarpFilter, OutputSize, SizeType);
  otbGetObjectMemberConstReferenceMacro(WarpFilter, OutputSize, SizeType);

  // Output Spacing
  otbSetObjectMemberMacro(WarpFilter, OutputSpacing, SpacingType);
  otbGetObjectMemberConstReferenceMacro(WarpFilter, OutputSpacing, SpacingType);

  /** Methods to Set/Get the interpolator */
  void SetInterpolator(InterpolatorType * interpolator)
  {
    m_WarpFilter->SetInterpolator(interpolator);
    this->Modified();
  }
  otbGetObjectMemberConstMacro(WarpFilter, Interpolator, const InterpolatorType *);

  /** Default Edgepadding value */
  otbSetObjectMemberMacro(WarpFilter,
                          EdgePaddingValue,
                          typename OutputImageType::PixelType);
  otbGetObjectMemberMacro(WarpFilter,
                                        EdgePaddingValue,
                                        typename OutputImageType::PixelType);

  /** Import output parameters from a given image */
  void SetOutputParametersFromImage(const ImageBaseType * image);

  /* Set number of threads for Deformation field generator*/
  void SetDisplacementFilterNumberOfThreads(unsigned int nbThread)
  {
    m_DisplacementFilter->SetNumberOfThreads(nbThread);
  }

  /** Override itk::ProcessObject method to let the internal filter do the propagation */
  void PropagateRequestedRegion(itk::DataObject *output) override;

protected:
  StreamingResampleImageFilter();

  /** Destructor */
  ~StreamingResampleImageFilter() override {};

  void GenerateData() override;

  void GenerateOutputInformation() override;

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  StreamingResampleImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

  //We need this to respect ConstRef macro and to be compliant with itk positive 
  //spacing
  SpacingType m_SignedOutputSpacing;

  typename DisplacementFieldGeneratorType::Pointer   m_DisplacementFilter;
  typename WarpImageFilterType::Pointer             m_WarpFilter;
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStreamingResampleImageFilter.hxx"
#endif

#endif
