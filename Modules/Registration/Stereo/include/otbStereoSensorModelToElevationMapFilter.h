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

#ifndef otbStereoSensorModelToElevationMapFilter_h
#define otbStereoSensorModelToElevationMapFilter_h

#include "itkImageToImageFilter.h"
#include "itkInterpolateImageFunction.h"
#include "otbGenericRSTransform.h"
#include "itkNormalizedCorrelationImageToImageMetric.h"

namespace otb
{
/**
 * \class StereoSensorModelToElevationFilter
 * \brief Using sensor models to perform correlation along epipolars
 *
 * This filter uses a pair of stereo images along-track to build an elevation
 * map.
 *
 * Images are not resampled to epipolar geometry. Instead, for each
 * patch of the master image, corresponding patches are interpolated
 * in the slave image according to a range of candidate
 * elevations. Colocalisation is achieved through the
 * GenericRSTransform for each pixel of the patch, which means the
 * interpolated path follows the warping between the two images (it is
 * not necesseraly aligned with slave image boundary). The elevation
 * achieving the higher correlation is kept in the final elevation
 * map.
 *
 * Setting-up the elevation exploration range is simple : the
 * LowerElevation and HigherElevation parameters allows setting the
 * exploration range offsets with respect to the local elevation, and
 * the ElevationStep parameter allows setting the step between
 * candidate elevation. The local elevation can either be constant
 * over the whole scene, or interpolated in a Digital Elevation
 * Model. The latter allows accounting for highly varying elevation in
 * large scene, and allows reducing the elevation exploration
 * range. Please note that one can also use a geoid file along with
 * the DEM.
 *
 * Correlation parameters are as follows :
 * - The radius allows tuning patches size (default is 3),
 * - The CorrelationThreshold allows setting a threshold below which
 *   correlation is considered to fail. If the correlation maxima is
 *   below this threshold, the estimated elevation is
 *   discarded (default is 0.7).
 * - The VarianceThreshold allows discarding master patches for which
 *   variance is too small to yield reliable correlation. If the
 *   variance of the current master patch lies below this threshold, no
 *   elevation exploration is performed at this location (default is 4).
 *
 *   This filter supports multi-threading and streaming. It can
 *   process images of any size and does not requires input images
 *   size to match. If images extents do not match, elevation will
 *   only be estimated over their overlapping extent. The elevation
 *   map is produced in the geometry and grid of the master input
 *   image.
 *
 *   \ingroup Streamed
 *   \ingroup Threaded
 *
 * \ingroup OTBStereo
 */
template <class TInputImage, class TOutputHeight>
class ITK_EXPORT StereoSensorModelToElevationFilter
  : public itk::ImageToImageFilter<TInputImage, TOutputHeight>
{
public:
  /** Standard class typedefs */
  typedef StereoSensorModelToElevationFilter                 Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputHeight> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(StereoSensorModelToElevationFilter, ImageToImageFilter);

  /** Convenient typedefs */
  typedef TInputImage                                        InputImageType;
  typedef typename InputImageType::PixelType                 InputPixelType;
  typedef TOutputHeight                                      OutputImageType;
  typedef typename OutputImageType::RegionType               OutputRegionType;
  typedef typename OutputImageType::PixelType                HeightType;
  typedef typename OutputImageType::PointType                OutputPointType;

  typedef itk::InterpolateImageFunction<TInputImage, double> InterpolatorType;
  typedef typename InterpolatorType::Pointer                 InterpolatorPointerType;
  typedef GenericRSTransform<>                               GenericRSTransformType;
  typedef GenericRSTransform<double, 3, 3>                   GenericRSTransform3DType;
  typedef itk::NormalizedCorrelationImageToImageMetric
    <TInputImage, TInputImage>                               CorrelationMetricType;
  typedef typename InputImageType::SizeType                  RadiusType;

  /** Set the master input image */
  void SetMasterInput(const TInputImage * image);

  /** Set the slave input iamge */
  void SetSlaveInput(const TInputImage * image);

  /** Get the master input image */
  const TInputImage * GetMasterInput() const;

  /** Get the slave input image */
  const TInputImage * GetSlaveInput() const;

  /** Get the output correlation map */
  TOutputHeight * GetCorrelationOutput();

  /** Set the lower offset for the local elevation
   * exploration. Default is -20 meters.
   */
  itkSetMacro(LowerElevation, double);

  /** Set the lower offset for the local elevation
   * exploration. Default is 20 meters.
   */
  itkSetMacro(HigherElevation, double);

  /** Set the step of the elevation exploration. Default is 1 meters. */
  itkSetMacro(ElevationStep, double);

  /** Set the threshold of the correlation to keep estimated
   *  height. Default is 0.7.
   */
  itkSetMacro(CorrelationThreshold, double);

  /** Get the threshold of the correlation to keep estimated
   *  height. Default is 0.7.
   */
  itkGetMacro(CorrelationThreshold, double);

  /** Set the threshold on the variance of the master patch to start
   *  elevation estimation.  Default is 4.
   */
  itkSetMacro(VarianceThreshold, double);

  /** Set the threshold on the variance of the master patch to start
   *  elevation estimation.  Default is 4.
   */
  itkGetMacro(VarianceThreshold, double);

  /** Set the SubtractInitialElevation flag. If set to true, the
   *  elevation map will only contains the difference between the
   *  estimated elevation and the initial one. If set to false,
   *  elevation is absolute. Default is false. */
  itkSetMacro(SubtractInitialElevation, bool);

  /** Get the SubtractInitialElevation flag. If set to true, the
   *  elevation map will only contains the difference between the
   *  estimated elevation and the initial one. If set to false,
   *  elevation is absolute. Default is false. */
  itkGetMacro(SubtractInitialElevation, bool);

  /** Toggle the SubtractInitialElevation flag. If set to true, the
   *  elevation map will only contains the difference between the
   *  estimated elevation and the initial one. If set to false,
   *  elevation is absolute. Default is false. */
  itkBooleanMacro(SubtractInitialElevation);

  /** Set the radius of the patches. Default is 3.  */
  void SetRadius(unsigned int radius)
  {
    m_Radius.Fill(radius);
    this->Modified();
  }

protected:
  /** Constructor */
  StereoSensorModelToElevationFilter();

  /** Destructor */
  ~StereoSensorModelToElevationFilter() override;

  /** Threaded generate data */
  void ThreadedGenerateData(const OutputRegionType& outputRegionForThread,
                                    itk::ThreadIdType threadId) override;

  /** Generate the input requested regions  */
  void GenerateInputRequestedRegion(void) override;

  /** Things to do before the threaded generate-data */
  void BeforeThreadedGenerateData() override;

  /** Override VerifyInputInformation() since this filter's inputs do
      * not need to occupy the same physical space.
      *
      * \sa ProcessObject::VerifyInputInformation
      */
  void VerifyInputInformation() override {}


private:
  StereoSensorModelToElevationFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  inline double Correlation(const std::vector<double>& master,
                            const std::vector<double>& slave) const;

  /** Interpolator for slave image */
  InterpolatorPointerType m_Interpolator;

  /** Radius of the patches */
  RadiusType m_Radius;

  /** Lower offset for the local elevation exploration. */
  double  m_LowerElevation;

  /** Higher offset for the local elevation exploration. */
  double  m_HigherElevation;

  /** Step of the elevation exploration. */
  double  m_ElevationStep;

  /** Threshold of the correlation to keep estimated height. */
  double m_CorrelationThreshold;

  /** Threshold on the variance of the master patch to start elevation
   *  estimation. */
  double m_VarianceThreshold;

  /** If set to true, the elevation map will only contains the
   *  difference between the estimated elevation and the initial
   *  one. If set to false, elevation is absolute. Default is false. */
  bool m_SubtractInitialElevation;
  
  /** Master to slave transform */
  GenericRSTransform3DType::Pointer m_MasterToSlave;
  
};
} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStereoSensorModelToElevationMapFilter.hxx"
#endif

#endif
