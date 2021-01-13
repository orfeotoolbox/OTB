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

#ifndef otbMultiDisparityMapTo3DFilter_h
#define otbMultiDisparityMapTo3DFilter_h

#include "itkImageToImageFilter.h"
#include "otbGenericRSTransform.h"
#include "otbLineOfSightOptimizer.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkImageRegionConstIterator.h"
#include "otbVectorImage.h"
#include "otbImage.h"

namespace otb
{

/** \class MultiDisparityMapTo3DFilter
 *  \brief Project a group of disparity map into 3D points
 *
 *  This filter uses a group of N horizontal disparity map (and vertical if any) to project 3D points.
 *  The output image contains the 3D points coordinates for each location of input disparity. The
 *  3D positions are compute by a least square optimisation between the N lines of sight.
 *  The 3D coordinates (sorted by band) are : longitude , latitude (in degree, wrt WGS84) and altitude (in meters)
 *  BEWARE : this filter is not fully compatible with the filters using the epipolar geometry. The N disparity
 *  maps shall come from the matching of a single 'reference' sensor image versus N 'moving' sensor images. In
 *  addition, the disparities shall be computed in physical space (not in index space)
 *  N disparity masks can be provided for each disparity map.
 *
 *  \sa FineRegistrationImageFilter
 *  \sa LineOfSightOptimizer
 *  \sa SubPixelDisparityImageFilter
 *  \sa PixelWiseBlockMatchingImageFilter
 *
 *  \ingroup Streamed
 *  \ingroup Threaded
 *
 *
 * \ingroup OTBDisparityMap
 */
template <class TDisparityImage, class TOutputImage = otb::VectorImage<float, 2>, class TMaskImage = otb::Image<unsigned char>,
          class TResidueImage = otb::Image<float>>
class ITK_EXPORT MultiDisparityMapTo3DFilter : public itk::ImageToImageFilter<TDisparityImage, TOutputImage>
{
public:
  /** Standard class typedef */
  typedef MultiDisparityMapTo3DFilter Self;
  typedef itk::ImageToImageFilter<TDisparityImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(MultiDisparityMapTo3DFilter, ImageToImageFilter);

  /** Useful typedefs */
  typedef TDisparityImage DisparityMapType;
  typedef TOutputImage    OutputImageType;
  typedef TMaskImage      MaskImageType;
  typedef TResidueImage   ResidueImageType;

  typedef typename OutputImageType::RegionType RegionType;
  typedef typename OutputImageType::PixelType  DEMPixelType;

  // 3D RS transform
  // TODO: Allow tuning precision (i.e. double or float)
  typedef double PrecisionType;
  typedef otb::GenericRSTransform<PrecisionType, 3, 3> RSTransformType;

  // 3D points
  typedef typename RSTransformType::InputPointType TDPointType;

  typedef otb::LineOfSightOptimizer<PrecisionType>  OptimizerType;
  typedef typename OptimizerType::PointSetType      PointSetType;
  typedef typename PointSetType::PointsContainer    PointsContainer;
  typedef typename PointSetType::PointDataContainer LabelContainer;

  typedef std::map<unsigned int, itk::ImageRegionConstIterator<DisparityMapType>> DispMapIteratorList;

  typedef std::map<unsigned int, itk::ImageRegionConstIterator<MaskImageType>> MaskIteratorList;

  /** Set the number of moving images (referred earlier as N) */
  void SetNumberOfMovingImages(unsigned int nb);

  /** Get the number of moving images (referred earlier as N) */
  unsigned int GetNumberOfMovingImages();

  /** Set horizontal disparity map input corresponding to the moving image 'index' */
  void SetHorizontalDisparityMapInput(unsigned int index, const TDisparityImage* hmap);

  /** Set vertical disparity map input corresponding to the moving image 'index'*/
  void SetVerticalDisparityMapInput(unsigned int index, const TDisparityImage* vmap);

  /** Set mask associated to disparity maps corresponding to the moving image 'index'
   * (optional, pixels with a null mask value are ignored)
   */
  void SetDisparityMaskInput(unsigned int index, const TMaskImage* mask);

  /** Get the inputs */
  const TDisparityImage* GetHorizontalDisparityMapInput(unsigned int index) const;
  const TDisparityImage* GetVerticalDisparityMapInput(unsigned int index) const;
  const TMaskImage* GetDisparityMaskInput(unsigned int index) const;

  /** Get residue output*/
  const TResidueImage* GetResidueOutput() const;
  TResidueImage*       GetResidueOutput();

  /** Set ImageMetadata of the reference image */
  void SetReferenceImageMetadata(const ImageMetadata* imd)
  {
    this->m_ReferenceImageMetadata = imd;
    this->Modified();
  }

  /** Get ImageMetadata of the reference image */
  const ImageMetadata* GetReferenceImageMetadata() const
  {
    return this->m_ReferenceImageMetadata;
  }

  /** Set ImageMetadata of the moving image 'index' */
  void SetMovingImageMetadata(unsigned int index, const ImageMetadata* imd);

  /** Get ImageMetadata of the moving image 'index' */
  const ImageMetadata* GetMovingImageMetadata(unsigned int index) const;

protected:
  /** Constructor */
  MultiDisparityMapTo3DFilter();

  /** Destructor */
  ~MultiDisparityMapTo3DFilter() override;

  /** Generate output information */
  void GenerateOutputInformation() override;

  /** Generate input requested region */
  void GenerateInputRequestedRegion() override;

  /** Before threaded generate data */
  void BeforeThreadedGenerateData() override;

  /** Threaded generate data */
  void ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType threadId) override;

private:
  MultiDisparityMapTo3DFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** Reference sensor image transform */
  RSTransformType::Pointer m_ReferenceToGroundTransform;

  /** Moving sensor image transforms */
  std::vector<RSTransformType::Pointer> m_MovingToGroundTransform;

  /** ImageMetadata of reference sensor image */
  const ImageMetadata*   m_ReferenceImageMetadata = nullptr;
  
  /** ImageMetadata of moving sensor images */
  std::vector<const ImageMetadata*> m_MovingImageMetadatas;


};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMultiDisparityMapTo3DFilter.hxx"
#endif

#endif
