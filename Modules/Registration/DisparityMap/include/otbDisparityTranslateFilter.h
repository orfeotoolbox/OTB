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

#ifndef otbDisparityTranslateFilter_h
#define otbDisparityTranslateFilter_h

#include "itkImageToImageFilter.h"
#include "otbVectorImage.h"
#include "otbImage.h"

namespace otb
{

/** \class DisparityTranslateFilter
 *  \brief Translate epipolar disparities into sensor disparities
 *
 *  This filter resamples a disparity map computed in epipolar geometry
 *  into a disparity map relative to sensor geometry. The output disparity
 *  map is aligned on the left sensor image. The disparity values are computed
 *  relative to the physical coordinates of the left and right sensor images.
 *  The vertical disparity mask input and the mask are optional.
 *
 *  \ingroup Streamed
 *  \ingroup Threaded
 *
 *
 * \ingroup OTBDisparityMap
 */
template <class TDisparityImage,  class TGridImage = otb::VectorImage<float,2> ,
  class TSensorImage = otb::Image<unsigned short,2> , class TMaskImage = otb::Image<unsigned char,2> >
class ITK_EXPORT DisparityTranslateFilter :
    public itk::ImageToImageFilter<TDisparityImage,TDisparityImage>
{
public:
  /** Standard class typedef */
  typedef DisparityTranslateFilter                          Self;
  typedef itk::ImageToImageFilter<TDisparityImage,
                                  TDisparityImage>          Superclass;
  typedef itk::SmartPointer<Self>                           Pointer;
  typedef itk::SmartPointer<const Self>                     ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(DisparityTranslateFilter, ImageToImageFilter);

  /** Useful typedefs */
  typedef TDisparityImage  DispMapType;
  typedef TGridImage       GridType;
  typedef TMaskImage       MaskType;
  typedef TSensorImage     SensorType;

  typedef typename DispMapType::PixelType    PixelType;
  typedef typename DispMapType::RegionType   RegionType;
  typedef typename DispMapType::SizeType     SizeType;
  typedef typename DispMapType::IndexType    IndexType;
  typedef typename DispMapType::SpacingType  SpacingType;
  typedef typename DispMapType::PointType    PointType;
  typedef typename DispMapType::IndexValueType IndexValueType;

  typedef typename GridType::RegionType      GridRegionType;

  /** Set/Get macro for NoDataValue */
  itkSetMacro(NoDataValue, PixelType);
  itkGetConstReferenceMacro(NoDataValue, PixelType);

  /** Set the horizontal disparity map */
  void SetHorizontalDisparityMapInput( const TDisparityImage * hmap );

  /** Set the vertical disparity map */
  void SetVerticalDisparityMapInput( const TDisparityImage * vmap );

  /** Set the inverse left epipolar grid  */
  void SetInverseEpipolarLeftGrid( const TGridImage * lgrid );

  /** Set the direct epipolar right grid */
  void SetDirectEpipolarRightGrid( const TGridImage * rgrid );

  /** Set the mask relative to the input disparity map */
  void SetDisparityMaskInput(const TMaskImage * mask);

  /** Set the left input sensor image
   *  (used as a support for the output disparity map)
   */
  void SetLeftSensorImageInput( const TSensorImage * left);

  /** Get the inputs */
  const TDisparityImage * GetHorizontalDisparityMapInput() const;
  const TDisparityImage * GetVerticalDisparityMapInput() const;
  const TGridImage * GetInverseEpipolarLeftGrid() const;
  const TGridImage * GetDirectEpipolarRightGrid() const;
  const TMaskImage * GetDisparityMaskInput() const;
  const TSensorImage * GetLeftSensorImageInput() const;

  /** Get output disparity maps */
  TDisparityImage * GetHorizontalDisparityMapOutput();
  TDisparityImage * GetVerticalDisparityMapOutput();

protected:
  /** Constructor */
  DisparityTranslateFilter();

  /** Destructor */
  ~DisparityTranslateFilter() override{};

  /** Generate output information */
  void GenerateOutputInformation() override;

  /** Generate input requested region */
  void GenerateInputRequestedRegion() override;

  /** Threaded generate data */
  void ThreadedGenerateData(const RegionType & outputRegionForThread, itk::ThreadIdType threadId) override;

  /** Override VerifyInputInformation() since this filter's inputs do
    * not need to occupy the same physical space.
    *
    * \sa ProcessObject::VerifyInputInformation
    */
 void VerifyInputInformation() override {}


private:
  DisparityTranslateFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  PixelType              m_NoDataValue;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbDisparityTranslateFilter.hxx"
#endif

#endif
