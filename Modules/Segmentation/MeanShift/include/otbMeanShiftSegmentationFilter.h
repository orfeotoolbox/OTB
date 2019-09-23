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

#ifndef otbMeanShiftSegmentationFilter_h
#define otbMeanShiftSegmentationFilter_h

#include "itkMacro.h"
#include "otbMacro.h"
#include "otbMeanShiftSmoothingImageFilter.h"
#include "otbLabelImageRegionMergingFilter.h"
#include "otbLabelImageRegionPruningFilter.h"
#include "itkRelabelComponentImageFilter.h"
#include "itkConnectedComponentFunctorImageFilter.h"
#include <string>


namespace otb
{


namespace Functor
{

template <class TInput>
class ITK_EXPORT ConnectedLabelFunctor
{

public:
  typedef ConnectedLabelFunctor Self;

  std::string GetNameOfClass()
  {
    return "ConnectedLabelFunctor";
  }

  inline bool operator()(TInput& p1, TInput& p2)
  {
    // return static_cast<bool> (0);
    return static_cast<bool>(p1 == p2);
  }


  ConnectedLabelFunctor()
  {
  }

  ~ConnectedLabelFunctor()
  {
  }

private:
  ConnectedLabelFunctor(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // end of Functor namespace


/** \class MeanShiftSegmentationFilter
*
*  Performs segmentation of an image by chaining a mean shift filter and region
*  merging filter.
*  This filter relies on MeanShift Labeled image created from clustered filtered output of meanshift filter.
*  Mode search algorithm is not performed on tiles boundaries. Thus output depends on Thread numbers.
 *
 * \ingroup OTBMeanShift
*/


template <class TInputImage, class TOutputLabelImage, class TOutputClusteredImage = TInputImage, class TKernel = Meanshift::KernelUniform>
class ITK_EXPORT MeanShiftSegmentationFilter : public itk::ImageToImageFilter<TInputImage, TOutputLabelImage>
{
public:
  /** Standard Self typedef */
  typedef MeanShiftSegmentationFilter Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputLabelImage> Superclass;

  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Some convenient typedefs. */
  typedef TInputImage           InputSpectralImageType;
  typedef TOutputLabelImage     OutputLabelImageType;
  typedef TOutputClusteredImage OutputClusteredImageType;
  typedef TKernel               KernelType;
  typedef double                RealType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(MeanShiftSegmentationFilter, ImageToImageFilter);

  /** ImageDimension constants */
  itkStaticConstMacro(ImageDimension, unsigned int, TInputImage::ImageDimension);

  // Mean shift filter
  typedef OutputClusteredImageType MeanShiftFilteredImageType;
  typedef MeanShiftSmoothingImageFilter<InputSpectralImageType, MeanShiftFilteredImageType, KernelType> MeanShiftFilterType;
  typedef typename MeanShiftFilterType::Pointer MeanShiftFilterPointerType;
  // Region merging filter
  typedef typename MeanShiftFilterType::OutputLabelImageType InputLabelImageType;
  typedef typename MeanShiftFilterType::LabelType            InputLabelPixelType;
  typedef LabelImageRegionMergingFilter<InputLabelImageType, MeanShiftFilteredImageType, OutputLabelImageType, OutputClusteredImageType>
                                                    RegionMergingFilterType;
  typedef typename RegionMergingFilterType::Pointer RegionMergingFilterPointerType;
  typedef LabelImageRegionPruningFilter<OutputLabelImageType, OutputClusteredImageType, OutputLabelImageType, OutputClusteredImageType> RegionPruningFilterType;
  typedef typename RegionPruningFilterType::Pointer RegionPruningFilterPointerType;


  typedef Functor::ConnectedLabelFunctor<InputLabelPixelType> LabelFunctorType;

  typedef itk::ConnectedComponentFunctorImageFilter<InputLabelImageType, InputLabelImageType, LabelFunctorType> RelabelComponentFilterType;
  typedef typename RelabelComponentFilterType::Pointer RelabelComponentFilterPointerType;


  /** Sets the spatial bandwidth (or radius in the case of a uniform kernel)
    * of the neighborhood for each pixel
    */
  otbSetObjectMemberMacro(MeanShiftFilter, SpatialBandwidth, RealType);
  otbGetObjectMemberConstReferenceMacro(MeanShiftFilter, SpatialBandwidth, RealType);

  /** Sets the spectral bandwidth (or radius for a uniform kernel) for pixels
    * to be included in the same mode
    */
  otbSetObjectMemberMacro(MeanShiftFilter, RangeBandwidth, RealType);
  otbGetObjectMemberConstReferenceMacro(MeanShiftFilter, RangeBandwidth, RealType);

  /** Set the maximum number of iterations */
  otbSetObjectMemberMacro(MeanShiftFilter, MaxIterationNumber, unsigned int);
  otbGetObjectMemberConstReferenceMacro(MeanShiftFilter, MaxIterationNumber, unsigned int);

  /** Set the convergence threshold */
  otbSetObjectMemberMacro(MeanShiftFilter, Threshold, RealType);
  otbGetObjectMemberConstReferenceMacro(MeanShiftFilter, Threshold, RealType);

  /** Sets the minimum region size. (after merging step clustered regions, which size is under this
   *  threshold will be fused with adjacent region with smallest spectral distance).
   */
  otbSetObjectMemberMacro(RegionPruningFilter, MinRegionSize, RealType);
  otbGetObjectMemberMacro(RegionPruningFilter, MinRegionSize, RealType);


  /** Returns the const image of region labels */
  const OutputLabelImageType* GetLabelOutput() const;
  /** Returns the image of region labels */
  OutputLabelImageType* GetLabelOutput();
  /** Returns the const clustered output image, with one spectral value per region  */
  const OutputClusteredImageType* GetClusteredOutput() const;
  /** Returns the clustered output image, with one spectral value per region  */
  OutputClusteredImageType* GetClusteredOutput();

protected:
  MeanShiftSegmentationFilter();

  ~MeanShiftSegmentationFilter() override;

  //  virtual void GenerateOutputInformation(void);

  void GenerateData() override;

private:
  MeanShiftFilterPointerType        m_MeanShiftFilter;
  RegionMergingFilterPointerType    m_RegionMergingFilter;
  RegionPruningFilterPointerType    m_RegionPruningFilter;
  RelabelComponentFilterPointerType m_RelabelFilter;
};


} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMeanShiftSegmentationFilter.hxx"
#endif

#endif
