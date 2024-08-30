/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbLineSegmentDetector_h
#define otbLineSegmentDetector_h

#include "otbImage.h"
#include "otbVectorDataSource.h"
#include "otbVectorData.h"

#include "itkUnaryFunctorImageFilter.h"
#include "itkGradientRecursiveGaussianImageFilter.h"
#include "itkGradientImageFilter.h"

namespace otb
{

namespace Functor
{
/** \class MagnitudeFunctor
 *  \brief This functor computes the magnitude of a covariant vector.
 *
 * \ingroup OTBEdge
 */
template <class TInputPixel, class TOutputPixel>
class MagnitudeFunctor
{
public:
  inline TOutputPixel operator()(const TInputPixel& input)
  {
    return static_cast<TOutputPixel>(2 * std::sqrt(input[0] * input[0] + input[1] * input[1]));
  }
};

/** \class OrientationFunctor
 *  \brief This functor computes the orientation of a covariant vector<br>
 *   Orientation values lies between -Pi and Pi.
 *
 * \ingroup OTBEdge
 */
template <class TInputPixel, class TOutputPixel>
class OrientationFunctor
{
public:
  inline TOutputPixel operator()(const TInputPixel& input)
  {
    return static_cast<TOutputPixel>(std::atan2(input[0], -input[1]));
  }
};
} // end namespace Functor

/** \class LineSegmentDetector
 *  \brief this class implement a fast line detector with false detection control using
 *         the a contrario method
 *
 *  The implementation is based on the online version of the lsd
 *  documentation and code lsd 1.5. The OTB version use a 3x3 filter
 *  to compute the gradient(vs. 2x2 in the original implementation)
 *  and implements the density control, the NOTINIT status and the
 *  incremental rectangle optimisation.
 *
 *  \sa StreamingLineSegmentDetector (streamed version)
 *  \ingroup FeatureExtraction
 *
 *  See Publication :
 *  " LSD: A line segment detector ", R. Grompone, J.Jackubowicz, J-M.Morel, G.Randall
 *  And Website :
 *  http://www.ipol.im/pub/algo/gjmr_line_segment_detector/
 *
 *
 * \ingroup OTBEdge
 */

template <class TInputImage, class TPrecision = double>
class ITK_EXPORT LineSegmentDetector : public VectorDataSource<otb::VectorData<TPrecision>>
{
public:
  /** typedef for the classes standards. */
  typedef LineSegmentDetector                      Self;
  typedef VectorDataSource<VectorData<TPrecision>> Superclass;
  typedef itk::SmartPointer<Self>                  Pointer;
  typedef itk::SmartPointer<const Self>            ConstPointer;

  /** Method for management of the object factory. */
  itkNewMacro(Self);

  /** Return the name of the class. */
  itkTypeMacro(LineSegmentDetector, VectorDataSource);

  /** Definition of the input image */
  typedef TInputImage                          InputImageType;
  typedef typename InputImageType::PixelType   InputPixelType;
  typedef typename InputImageType::IndexType   InputIndexType;
  typedef typename InputImageType::SizeType    SizeType;
  typedef typename InputImageType::RegionType  RegionType;
  typedef typename InputImageType::SpacingType SpacingType;
  typedef typename InputImageType::PointType   OriginType;

  /** Definition of the output vector data. */
  typedef VectorData<TPrecision>                VectorDataType;
  typedef typename VectorDataType::DataNodeType DataNodeType;
  typedef typename VectorDataType::LineType     LineType;
  typedef typename VectorDataType::PointType    PointType;
  typedef typename LineType::VertexType         VertexType;

  /** Definition of temporary image ised to store LABELS*/
  typedef Image<TPrecision, 2> OutputImageType;
  typedef typename OutputImageType::PixelType OutputPixelType;
  typedef typename OutputImageType::IndexType OutputIndexType;
  typedef typename OutputImageType::SizeType  OutputSizeType;

  /** Histogram to store the coordinate of ordered pixels*/
  typedef std::vector<OutputIndexType>               IndexVectorType;
  typedef typename IndexVectorType::iterator         IndexVectorIteratorType;
  typedef std::vector<IndexVectorType>               CoordinateHistogramType;
  typedef typename CoordinateHistogramType::iterator CoordinateHistogramIteratorType;

  /** typedef structure to store REGION*/
  typedef std::vector<IndexVectorType>           VectorOfIndexVectorType;
  typedef std::vector<float>                     DirectionVectorType;
  typedef typename DirectionVectorType::iterator DirectionVectorIteratorType;

  /** */
  typedef itk::GradientRecursiveGaussianImageFilter<OutputImageType> GradientFilterType;
  typedef typename GradientFilterType::Pointer                       GradientFilterPointerType;
  typedef typename GradientFilterType::OutputImageType               GradientOutputImageType;

  typedef itk::UnaryFunctorImageFilter<GradientOutputImageType, OutputImageType,
                                       Functor::MagnitudeFunctor<typename GradientOutputImageType::PixelType, TPrecision>>
                                                                   MagnitudeFilterType;
  typedef typename MagnitudeFilterType::Pointer                    MagnitudeFilterPointerType;
  typedef typename MagnitudeFilterType::OutputImageType::PixelType MagnitudePixelType;
  typedef typename MagnitudeFilterType::OutputImageType            MagnitudeImageType;
  typedef typename MagnitudeImageType::Pointer                     MagnitudeImagePointerType;

  typedef itk::UnaryFunctorImageFilter<GradientOutputImageType, OutputImageType,
                                       Functor::OrientationFunctor<typename GradientOutputImageType::PixelType, TPrecision>>
                                                          OrientationFilterType;
  typedef typename OrientationFilterType::Pointer         OrientationFilterPointerType;
  typedef typename OrientationFilterType::OutputImageType OutputImageDirType;
  typedef typename OutputImageDirType::RegionType         OutputImageDirRegionType;

  /** Create an image to store the label USED(255), NOTINI(127) or NOTUSED (0)*/
  typedef otb::Image<unsigned char, 2> LabelImageType;
  typedef typename LabelImageType::Pointer LabelImagePointerType;

  /** Vector to store the rectangle characterization  center, width, orientation , ( begin , end ) of the central line*/
  typedef std::vector<double>                  RectangleType;
  typedef typename RectangleType::iterator     RectangleIteratorType;
  typedef std::vector<RectangleType>           RectangleListType;
  typedef typename RectangleListType::iterator RectangleListTypeIterator;

  /** Set/Get the input image of this process object.  */
  using Superclass::SetInput;
  virtual void SetInput(const InputImageType* input);
  virtual const InputImageType* GetInput(void);

  /** Custom Get methods to access intermediate data*/
  LabelImagePointerType GetMap()
  {
    return m_UsedPointImage;
  }
  MagnitudeImagePointerType GetGradMod()
  {
    return m_MagnitudeFilter->GetOutput();
  }
  typename OutputImageDirType::Pointer GetGradOri()
  {
    return m_OrientationFilter->GetOutput();
  }

protected:
  LineSegmentDetector();
  ~LineSegmentDetector() override
  {
  }

  void GenerateInputRequestedRegion() override;

  /** Generate Data method*/
  void GenerateData() override;

  /** Sort the image and store the coordinates in a histogram
   *  this method is used to determine the seeds where to begin the search segments
   *  Points with large gradient modulus are more able to belong to a segment
   */
  virtual CoordinateHistogramType SortImageByModulusValue(MagnitudeImagePointerType modulusImage);

  /** */
  virtual void LineSegmentDetection(CoordinateHistogramType& CoordinateHistogram);

  /** Return true if the pixel status is NOTUSED*/
  virtual bool IsNotUsed(InputIndexType& index) const;

  /** Return true if the pixel status is USED*/
  virtual bool IsUsed(InputIndexType& index) const;

  /** Return true if the pixel status is NOTINI*/
  virtual bool IsNotIni(InputIndexType& index) const;

  /** Set Pixel flag to USED*/
  virtual void SetPixelToUsed(InputIndexType index);

  /** Set Pixel flag to NOTINI*/
  virtual void SetPixelToNotIni(InputIndexType index);

  /** Set Pixels flag to NOTINI*/
  virtual void SetRegionToNotIni(IndexVectorType region);

  /** search for a segment which begins from a seed "index "*/
  virtual bool GrowRegion(InputIndexType index, IndexVectorType& region, double& regionAngle);

  /** Define if two are aligned */
  virtual bool IsAligned(double Angle, double regionAngle, double prec) const;

  /** For each region of the region List it builds a rectangle */
  virtual int ComputeRectangles();

  /** */
  virtual RectangleType Region2Rectangle(IndexVectorType region, double regionAngle);

  /** */
  virtual double ComputeRegionOrientation(IndexVectorType region, double x, double y, double angleRegion) const;

  /** */
  virtual double angle_diff(double a, double b) const;

  /**  Compute the Number Of False Alarm for a rectangle*/
  virtual double ComputeRectNFA(const RectangleType& rec) const;

  /** */
  virtual double ImproveRectangle(RectangleType& rectangle) const;

  /** NFA For a rectangle*/
  virtual double NFA(int n, int k, double p, double logNT) const;

  /** Create a copy of a rectangle*/
  virtual void CopyRectangle(RectangleType& rDst, RectangleType& rSrc) const;

  /** Printself method*/
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  LineSegmentDetector(const Self&) = delete;
  void operator=(const Self&) = delete;

  VectorOfIndexVectorType m_RegionList;
  DirectionVectorType     m_DirectionVector;
  LabelImagePointerType   m_UsedPointImage;
  RectangleListType       m_RectangleList;

  double       m_Threshold;
  double       m_Prec;
  double       m_DirectionsAllowed;
  unsigned int m_MinimumRegionSize;

  /** Gradient filter */
  GradientFilterPointerType m_GradientFilter;

  /** Magnitude filter */
  MagnitudeFilterPointerType m_MagnitudeFilter;

  /** Orientation filter */
  OrientationFilterPointerType m_OrientationFilter;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLineSegmentDetector.hxx"
#endif

#endif
