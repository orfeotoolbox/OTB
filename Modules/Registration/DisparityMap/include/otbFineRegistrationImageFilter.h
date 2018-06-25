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

#ifndef otbFineRegistrationImageFilter_h
#define otbFineRegistrationImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkInterpolateImageFunction.h"
#include "itkContinuousIndex.h"

#include "itkTranslationTransform.h"
#include "itkImageToImageMetric.h"

namespace otb
{

/** \class FineRegistrationImageFilter
 * \brief Computes a displacement field between two images using a given metric.
 *
 * This filter tries to find at each location of the fixed image the corresponding best matching
 * patch of radius set by SetRadius() method in the moving image within a search windows whose radius
 * is defined by SetSearchRadius() method.
 *
 * To do so, it optimizes a metric set using the SetMetric() method, which accepts any itk metric
 * deriving from the itk::ImageToImageMetric. The MinimizeOn()/MinimizeOff() flag allows searching for
 * minimum or maximum depending on the metric (default is On).
 *
 * Once a coarse (pixel wise) offset has been found, this match is further refined using golden section search search
 * until convergence accuracy (given by the SetConvergenceAccuracy()) is reached, or when the max number of iteration () is reached.
 *
 * The filter proposes two outputs: GetOutput() return the image of the metric optimum at each location, and
 * the GetOutputDisplacementField() method returns the corresponding offset.
 *
 * If the UseSpacingOn() flag is used, the output displacement field takes the input image spacing into account.
 * otherwise, the displacement field is expressed in pixels (default ins On).
 *
 * This filter accepts fixed and moving images with different sizes and spacing. Metric and search windows radius
 * are expressed in terms of number of pixels in the fixed image.
 *
 * An initial offset can be used to reduce computation time in case of input and moving images with a significant
 * offset. This offset is taken into account in the output displacement field.
 *
 * It is possible to generate an output metric map and displacement field at a coarser resolution by setting
 * grid step to value higher than 1 (grid step is expressed in terms of number of fixed image pixels).
 * Default value is 1.
 *
 * The FineRegistrationImageFilter allows using the full range of itk::ImageToImageMetric provided by itk.
 *
 * \example DisparityMap/FineRegistrationImageFilterExample.cxx
 *
 * \sa      FastCorrelationImageFilter, DisparityMapEstimationMethod
 * \ingroup IntensityImageFilters, Streamed
 *
 * \ingroup OTBDisparityMap
 */
template <class TInputImage, class T0utputCorrelation, class TOutputDisplacementField>
class ITK_EXPORT FineRegistrationImageFilter : public itk::ImageToImageFilter<TInputImage, T0utputCorrelation>
{
public:
  /** Standard class typedefs. */
  typedef FineRegistrationImageFilter                             Self;
  typedef itk::ImageToImageFilter<TInputImage, T0utputCorrelation> Superclass;
  typedef itk::SmartPointer<Self>                                 Pointer;
  typedef itk::SmartPointer<const Self>                           ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(FineRegistrationImageFilter, ImageToImageFilter);

  /** Some convenient typedefs. */
  typedef typename T0utputCorrelation::RegionType                 OutputImageRegionType;
  typedef typename TOutputDisplacementField::PixelType             DisplacementValueType;
  typedef typename TInputImage::Pointer                           InputImagePointerType;
  typedef typename TInputImage::RegionType                        InputImageRegionType;
  typedef typename TInputImage::SizeType                          SizeType;
  typedef typename TInputImage::IndexType                         IndexType;
  typedef typename TInputImage::SpacingType                       SpacingType;
  typedef typename TInputImage::PointType                         PointType;
  typedef typename TInputImage::OffsetType                        OffsetType;
  typedef itk::InterpolateImageFunction<TInputImage, double>      InterpolatorType;
  typedef typename InterpolatorType::Pointer                      InterpolatorPointerType;
  typedef itk::ContinuousIndex<double, 2>                         ContinuousIndexType;
  typedef itk::ImageToImageMetric<TInputImage, TInputImage>       MetricType;
  typedef typename MetricType::Pointer                            MetricPointerType;
  typedef itk::TranslationTransform<double, 2>                     TranslationType;
  typedef typename TranslationType::Pointer                       TranslationPointerType;
  typedef typename itk::Transform<double, 2, 2>                     TransformType;
  typedef typename TransformType::Pointer                         TransformPointerType;

  /** Set/Get the Metric used to compare images */
  itkSetObjectMacro(Metric, MetricType);
  itkGetObjectMacro(Metric, MetricType);

  /** Set/Get the interpolator used to interpolate moving image at non-grid positions */
  itkSetObjectMacro(Interpolator, InterpolatorType);
  itkGetObjectMacro(Interpolator, InterpolatorType);

  /** Connect one of the operands for pixel-wise addition */
  void SetFixedInput( const TInputImage * image);

  /** Connect one of the operands for pixel-wise addition */
  void SetMovingInput( const TInputImage * image);

  /** Get the inputs */
  const TInputImage * GetFixedInput();
  const TInputImage * GetMovingInput();

  /** Get the output displacement field */
  TOutputDisplacementField * GetOutputDisplacementField();

  /** Set the radius of the area on which metric is evaluated */
  itkSetMacro(Radius, SizeType);
  itkGetMacro(Radius, SizeType);

  /** Set the searh radius */
  itkSetMacro(SearchRadius, SizeType);
  itkGetMacro(SearchRadius, SizeType);

  /** Set/Get convergence accuracy */
  itkSetMacro(ConvergenceAccuracy, double);
  itkGetMacro(ConvergenceAccuracy, double);
  
  /** Set/Get subpixel accuracy */
  itkSetMacro(SubPixelAccuracy, double);
  itkGetMacro(SubPixelAccuracy, double);
  
  /** Set/Get max number of iterations */
  itkSetMacro(MaxIter, int);
  itkGetMacro(MaxIter, int);

  /** True if metric should be minimized. False otherwise */
  itkSetMacro(Minimize, bool);
  itkBooleanMacro(Minimize);

  /** True if displacement field takes spacing into account. False otherwise */
  itkSetMacro(UseSpacing, bool);
  itkBooleanMacro(UseSpacing);

  /** Set default offset between the two images */
  itkSetMacro(InitialOffset, SpacingType);
  itkGetConstReferenceMacro(InitialOffset, SpacingType);

  /** Set the grid step */
  itkSetMacro(GridStep, OffsetType);
  itkGetConstReferenceMacro(GridStep, OffsetType);

  /** Set unsigned int radius */
  void SetRadius(unsigned int radius)
  {
    m_Radius.Fill(radius);
  }

 /** Set unsigned int radius */
  void SetSearchRadius(unsigned int radius)
  {
    m_SearchRadius.Fill(radius);
  }

  /** Set unsigned int grid step */
  void SetGridStep(unsigned int step)
  {
    m_GridStep.Fill(step);
  }

  /** Set/Get the transform for the initial offset */
  itkSetObjectMacro(Transform, TransformType);
  itkGetConstObjectMacro(Transform, TransformType);

protected:
  /** Constructor */
  FineRegistrationImageFilter();
  /** Destructor */
  ~FineRegistrationImageFilter() override {};

  /** Threaded generate data */
  void GenerateData() override;

  /** Generate the input requested regions  */
  void GenerateInputRequestedRegion(void) override;

  /** Generate output information */
  void GenerateOutputInformation(void) override;

private:
  FineRegistrationImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;
  
  inline double callMetric(double val1,double val2,double &oldRes,bool &flag);
  inline void updateOptParams(double potBestVal,double parx,double pary,                             //inputs
                              double &bestVal, typename TranslationType::ParametersType& optParams); //outputs
  inline void updatePoints(double& gn, double& in1, double& in2, double &in3,      //inputs
                           double& out1, double& out2, double& out3, double& out4); //outputs
  inline void updateMinimize(double& a, double& b);

  /** The radius for correlation */
  SizeType                      m_Radius;

  /** The search radius */
  SizeType                      m_SearchRadius;

  /** Minimize/maximize metric */
  bool                          m_Minimize;

  /** If true, displacement field uses spacing. Otherwise, uses pixel grid */
  bool                          m_UseSpacing;

  /** Search step */
  double                        m_ConvergenceAccuracy;
  double                        m_SubPixelAccuracy;
  
  /** Max number of iterations */
  int                           m_MaxIter;

  /** The interpolator */
  InterpolatorPointerType       m_Interpolator;

  /** The metric */
  MetricPointerType             m_Metric;

  /** The translation */
  TranslationPointerType        m_Translation;

  /** Default offset */
  SpacingType                   m_InitialOffset;

  /** Grid step */
  OffsetType                    m_GridStep;

  /** Transform for initial offset */
  TransformPointerType          m_Transform;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbFineRegistrationImageFilter.hxx"
#endif

#endif
