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

#ifndef otbSubPixelDisparityImageFilter_h
#define otbSubPixelDisparityImageFilter_h

#include "otbPixelWiseBlockMatchingImageFilter.h"

#include "itkTranslationTransform.h"
#include "itkResampleImageFilter.h"

namespace otb
{

/** \class SubPixelDisparityImageFilter
 *  \brief Perform sub-pixel disparity estimation from input integer disparities and the image pair that was used
 *
 *  This filter is intended to be placed after a PixelWiseBlockMatchingImageFilter. Its role is to produce an estimate
 *  of the disparities (both horizontal and vertical) with sub-pixel precision. The integer input disparities are used
 *  as starting points for the disparity refinement. The refinement is done within a 3x3 neighborhood around this
 *  position (it is neighborhood in the 2D disparity space). If no input disparity is given, the shift between
 *  input images is assumed to be null along both directions.
 *
 *  Left and right masks can be used to skip the sub-pixel disparity refinement for couples of pixels carrying a
 *  non-positive mask value.
 *
 *  The exploration limits for horizontal and vertical disparities can be set like in PixelWiseBlockMatchingFilter.
 *  If the 3x3 neighborhood around the initial estimate is not inside the exploration area, refinement is skipped for
 *  the current pixel.
 *
 *  Before trying to refine the integer disparity, the filter checks that the initial position is an extrema for the
 *  metric used. Depending on the 3x3 neighborhood, the refinement can be done along the horizontal axis only, along
 *  the vertical axis only, and also in 2D. Three refinement methods are proposed : parabolic, triangular and dichotomy.
 *  The parabolic method tries to fit a parabola to the metric scores on the 3x3 neighborhood. The triangular
 *  method tries to fit local scores to a circular cone. The dichotomy method tries to find the local extrema by
 *  a dichotomic search (non-integer disparity positions are tested after a resampling of the right image).
 *
 *  \sa PixelWiseBlockMatchingImageFilter
 *  \sa FineRegistrationImageFilter
 *  \sa StereorectificationDisplacementFieldSource
 *
 *  \ingroup Streamed
 *  \ingroup Threaded
 *
 *
 * \ingroup OTBDisparityMap
 */
template <class TInputImage, class TOutputMetricImage, class TDisparityImage = TOutputMetricImage,
          class TMaskImage = otb::Image<unsigned char>,
          class TBlockMatchingFunctor = Functor::SSDBlockMatching<TInputImage,TOutputMetricImage> >
class ITK_EXPORT SubPixelDisparityImageFilter :
    public itk::ImageToImageFilter<TInputImage,TDisparityImage>
{
public:
  /** Standard class typedef */
  typedef SubPixelDisparityImageFilter       Self;
  typedef itk::ImageToImageFilter<TInputImage,
                                  TDisparityImage>    Superclass;
  typedef itk::SmartPointer<Self>                           Pointer;
  typedef itk::SmartPointer<const Self>                     ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SubPixelDisparityImageFilter, ImageToImageFilter);

  /** Useful typedefs */
  typedef TInputImage                                       InputImageType;
  typedef TOutputMetricImage                                OutputMetricImageType;
  typedef TDisparityImage                                   OutputDisparityImageType;
  typedef TMaskImage                                        InputMaskImageType;
  typedef TBlockMatchingFunctor                             BlockMatchingFunctorType;

  typedef typename InputImageType::SizeType                 SizeType;
  typedef typename InputImageType::IndexType                IndexType;
  typedef typename InputImageType::RegionType               RegionType;
  typedef typename InputImageType::SpacingType              SpacingType;
  typedef typename InputImageType::PointType                PointType;

  typedef typename TOutputMetricImage::ValueType            MetricValueType;

  typedef typename OutputDisparityImageType::PixelType      DisparityPixelType;

  typedef itk::ConstNeighborhoodIterator<TInputImage>       ConstNeighborhoodIteratorType;

  typedef itk::ResampleImageFilter<TInputImage, TInputImage, double>  ResamplerFilterType;
  typedef itk::TranslationTransform<double,2> TransformationType;
  typedef otb::PixelWiseBlockMatchingImageFilter
          < InputImageType,
            OutputMetricImageType,
            OutputDisparityImageType,
            InputMaskImageType,
            BlockMatchingFunctorType> BlockMatchingFilterType;

  itkStaticConstMacro(PARABOLIC,int,0);
  itkStaticConstMacro(TRIANGULAR,int,1);
  itkStaticConstMacro(DICHOTOMY,int,2);

  /** Set left input */
  void SetLeftInput( const TInputImage * image);

  /** Set right input */
  void SetRightInput( const TInputImage * image);

  /** Set initial horizontal disparity field */
  void SetHorizontalDisparityInput( const TDisparityImage * hfield);

  /** Set initial vertical disparity field */
  void SetVerticalDisparityInput( const TDisparityImage * vfield);

  /** Set the input metric image */
  void SetMetricInput(const TOutputMetricImage * image);

  /** Get the initial disparity fields */
  const TDisparityImage * GetHorizontalDisparityInput() const;
  const TDisparityImage * GetVerticalDisparityInput() const;

  /** Set mask input (optional) */
  void SetLeftMaskInput(const TMaskImage * image);

  /** Set right mask input (optional) */
  void SetRightMaskInput(const TMaskImage * image);

  /** Get the inputs */
  const TInputImage * GetLeftInput() const;
  const TInputImage * GetRightInput() const;
  const TMaskImage  * GetLeftMaskInput() const;
  const TMaskImage  * GetRightMaskInput() const;

  /** Get the metric output */
  const TOutputMetricImage * GetMetricOutput() const;
  TOutputMetricImage * GetMetricOutput();

  /** Get the disparity output */
  const TDisparityImage * GetHorizontalDisparityOutput() const;
  TDisparityImage * GetHorizontalDisparityOutput();

  /** Get the disparity output */
  const TDisparityImage * GetVerticalDisparityOutput() const;
  TDisparityImage * GetVerticalDisparityOutput();

  /** Set unsigned int radius */
  void SetRadius(unsigned int radius)
  {
    m_Radius.Fill(radius);
  }

  /** Set/Get the radius of the area on which metric is evaluated */
  itkSetMacro(Radius, SizeType);
  itkGetConstReferenceMacro(Radius, SizeType);

  /*** Set/Get the minimum disparity to explore */
  itkSetMacro(MinimumHorizontalDisparity,int);
  itkGetConstReferenceMacro(MinimumHorizontalDisparity,int);

  /*** Set/Get the maximum disparity to explore */
  itkSetMacro(MaximumHorizontalDisparity,int);
  itkGetConstReferenceMacro(MaximumHorizontalDisparity,int);

  /*** Set/Get the minimum disparity to explore */
  itkSetMacro(MinimumVerticalDisparity,int);
  itkGetConstReferenceMacro(MinimumVerticalDisparity,int);

  /*** Set/Get the maximum disparity to explore */
  itkSetMacro(MaximumVerticalDisparity,int);
  itkGetConstReferenceMacro(MaximumVerticalDisparity,int);

  itkSetMacro(Minimize, bool);
  itkGetConstReferenceMacro(Minimize,bool);
  itkBooleanMacro(Minimize);

  /** Get the functor for parameters setting */
  BlockMatchingFunctorType &  GetFunctor()
  {
    return m_Functor;
  }

  /** Get the functor (const version) */
  const BlockMatchingFunctorType &  GetFunctor() const
  {
    return m_Functor;
  }

  /** Set/Get the refinement method (PARABOLIC, TRIANGULAR or DICHOTOMY) */
  itkSetMacro(RefineMethod,int);
  itkGetMacro(RefineMethod,int);

  void SetInputsFromBlockMatchingFilter(const BlockMatchingFilterType * filter);

protected:
  /** Constructor */
  SubPixelDisparityImageFilter();

  /** Destructor */
  ~SubPixelDisparityImageFilter() override;

  /** \brief Verify that the input images are compatible
   *
   * This method needs to be re-implemented from ImageToImageFilter since
   * the initial images and disparity maps may not have the same size
   */
  void VerifyInputInformation() override;
  
  /** Generate output information */
  void GenerateOutputInformation() override;

  /** Generate input requested region */
  void GenerateInputRequestedRegion() override;

  /** Before threaded generate data */
  void BeforeThreadedGenerateData() override;

  /** Threaded generate data */
  void ThreadedGenerateData(const RegionType & outputRegionForThread, itk::ThreadIdType threadId) override;

  /** After threaded generate data */
  void AfterThreadedGenerateData() override;

private:
  SubPixelDisparityImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** parabolic refinement method */
  void ParabolicRefinement(const RegionType& outputRegionForThread, itk::ThreadIdType threadId);

  /** triangular refinement method */
  void TriangularRefinement(const RegionType& outputRegionForThread, itk::ThreadIdType threadId);

  /** dichotomy refinement method */
  void DichotomyRefinement(const RegionType& outputRegionForThread, itk::ThreadIdType threadId);

  /** The radius of the blocks */
  SizeType                      m_Radius;

  /** The min disparity to explore */
  int                           m_MinimumHorizontalDisparity;

  /** The max disparity to explore */
  int                           m_MaximumHorizontalDisparity;

   /** The min disparity to explore */
  int                           m_MinimumVerticalDisparity;

  /** The max disparity to explore */
  int                           m_MaximumVerticalDisparity;

  /** Should we minimize or maximize ? */
  bool                          m_Minimize;

  /** Block-matching functor */
  BlockMatchingFunctorType      m_Functor;

  /** Refinement method (PARABOLIC, TRIANGULAR or DICHOTOMY)*/
  int                           m_RefineMethod;

  /** Stores the number of pixels whose refined position has a worse metric than the initial position */
  std::vector<double>           m_WrongExtrema;

  /** Stores the computed grid step of the disparity map (internal purpose only) */
  unsigned int                  m_Step;

  /** Stores the computed grid start index of the disparity map (internal purpose only) */
  IndexType                     m_GridIndex;

};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSubPixelDisparityImageFilter.hxx"
#endif

#endif

