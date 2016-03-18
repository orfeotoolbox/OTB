/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbPixelWiseBlockMatchingImageFilter_h
#define __otbPixelWiseBlockMatchingImageFilter_h


#include "itkImageToImageFilter.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkImageRegionIterator.h"
#include "otbImage.h"

namespace otb
{

namespace Functor
{
/** \class SSDBlockMatching
 *  \brief Functor to perform simple SSD block-matching
 *
 *  This functor is designed to work with the
 *  PixelWiseBlockMatchingImageFilter. It performs a simple
 *  SSD (Sum of Square Distances) block-matching. The functor is
 *  templated by the type of inputs images and output metric image,
 *  and is using two neighborhood iterators as inputs.
 *
 * \ingroup OTBDisparityMap
 */
template <class TInputImage, class TOutputMetricImage>
ITK_EXPORT class SSDBlockMatching
{
public:
  typedef itk::ConstNeighborhoodIterator<TInputImage> ConstNeighborhoodIteratorType;
  typedef typename TOutputMetricImage::ValueType      MetricValueType;

  // Implement the SSD operator
  inline MetricValueType operator()(ConstNeighborhoodIteratorType & a, ConstNeighborhoodIteratorType & b) const
  {
    MetricValueType ssd = 0;

    // For some reason, iterators do not work on neighborhoods
    for(unsigned int i = 0; i<a.Size(); ++i)
      {
      ssd += (a.GetPixel(i)-b.GetPixel(i))*(a.GetPixel(i)-b.GetPixel(i));
      }

    return ssd;
  }
};


/** \class SSDDivMeanBlockMatching
 *  \brief Functor to perform derived SSD block-matching (SSD divided by mean)
 *
 *  This functor is designed to work with the
 *  PixelWiseBlockMatchingImageFilter. It performs derived
 *  SSD (Sum of Square Distances) block-matching. The functor is
 *  templated by the type of inputs images and output metric image,
 *  and is using two neighborhood iterators as inputs.
 *
 * \ingroup OTBDisparityMap
 */
template <class TInputImage, class TOutputMetricImage>
ITK_EXPORT class SSDDivMeanBlockMatching
{
public:
  typedef itk::ConstNeighborhoodIterator<TInputImage> ConstNeighborhoodIteratorType;
  typedef typename TOutputMetricImage::ValueType      MetricValueType;

  // Implement the SSD DivMean operator
  inline MetricValueType operator()(ConstNeighborhoodIteratorType & a, ConstNeighborhoodIteratorType & b) const
  {
    MetricValueType ssd = 0;
    MetricValueType meana = 0;
    MetricValueType meanb = 0;

    for(unsigned int i = 0; i<a.Size(); ++i)
      {
      meana+=a.GetPixel(i);
      meanb+=b.GetPixel(i);
      }
    meana/=a.Size();
    meanb/=b.Size();

    // For some reason, iterators do not work on neighborhoods
    for(unsigned int i = 0; i<a.Size(); ++i)
      {
      ssd += (a.GetPixel(i)/meana-b.GetPixel(i)/meanb)*(a.GetPixel(i)/meana-b.GetPixel(i)/meanb);

      }

    return ssd;
  }
};


/** \class NCCBlockMatching
 *  \brief Functor to perform simple NCC block-matching
 *
 *  This functor is designed to work with the
 *  PixelWiseBlockMatchingImageFilter. It performs a simple
 *  NCC (Normalized Cross-Correlation) block-matching. The functor is
 *  templated by the type of inputs images and output metric image,
 *  and is using two neighborhood iterators as inputs.
 *
 * \ingroup OTBDisparityMap
 */
template <class TInputImage, class TOutputMetricImage>
ITK_EXPORT class NCCBlockMatching
{
public:
  typedef itk::ConstNeighborhoodIterator<TInputImage> ConstNeighborhoodIteratorType;
  typedef typename TOutputMetricImage::ValueType      MetricValueType;

  // Implement the NCC operator
  inline MetricValueType operator()(ConstNeighborhoodIteratorType & a, ConstNeighborhoodIteratorType & b) const
  {
    //MetricValueType meanA(0),meanB(0), sigmaA(0), sigmaB(0), cov(0), ncc(0);
    double  meanA=0.0;
    double  meanB=0.0;
    double  sigmaA=0.0;
    double  sigmaB=0.0;
    double  cov=0.0;
    double  ncc=0.0;
    double  valueA;
    double  valueB;
    double size=a.Size();
    // For some reason, iterators do not work on neighborhoods
    for(unsigned int i = 0; i<size; ++i)
      {
      meanA+=static_cast<double>(a.GetPixel(i));
      meanB+=static_cast<double>(b.GetPixel(i));
      }

    // Compute mean
    meanA/=size;
    meanB/=size;

    for(unsigned int i = 0; i<size; ++i)
      {
      valueA=static_cast<double>(a.GetPixel(i));
      valueB=static_cast<double>(b.GetPixel(i));
      cov+=(valueA-meanA)*(valueB-meanB);
      sigmaA+=(valueA-meanA)*(valueA-meanA);
      sigmaB+=(valueB-meanB)*(valueB-meanB);
      }

    cov/=size-1;
    sigmaA/=size-1;
    sigmaB/=size-1;
    sigmaA = vcl_sqrt(sigmaA);
    sigmaB = vcl_sqrt(sigmaB);

    if(sigmaA > 1e-20 && sigmaB > 1e-20)
      {
      ncc = vcl_abs(cov)/(sigmaA*sigmaB);
      }
    else
      {
      ncc = 0;
      }

    return static_cast<MetricValueType>(ncc);
  }
};

/** \class LPBlockMatching
 *  \brief Functor to perform block-matching based on the L^p pseudo-norm
 *
 *  This functor is designed to work with the
 *  PixelWiseBlockMatchingImageFilter. It performs a distance computation between
 *  two windows based on the L^p pseudo norm (p greater than 0). The functor is
 *  templated by the type of inputs images and output metric image,
 *  and is using two neighborhood iterators as inputs.
 *
 * \ingroup OTBDisparityMap
 */
template <class TInputImage, class TOutputMetricImage>
ITK_EXPORT class LPBlockMatching
{
public:
  typedef itk::ConstNeighborhoodIterator<TInputImage> ConstNeighborhoodIteratorType;
  typedef typename TOutputMetricImage::ValueType      MetricValueType;

  LPBlockMatching(): m_P(1)
    {
    }

  void SetP(double p)
    {
    if (p > 0.0)
      {
      m_P = p;
      }
    else
      {
      m_P = 1.0;
      }
    }

  // Implement the Lp metric
  inline MetricValueType operator()(ConstNeighborhoodIteratorType & a, ConstNeighborhoodIteratorType & b) const
  {
    MetricValueType score(0);

    // For some reason, iterators do not work on neighborhoods
    for(unsigned int i = 0; i<a.Size(); ++i)
      {
      score += vcl_pow( vcl_abs(static_cast<double>(a.GetPixel(i)-b.GetPixel(i))) , m_P);
      }

    return score;
  }

private:

  double m_P;
};

} // End Namespace Functor

/** \class PixelWiseBlockMatchingImageFilter
 *  \brief Perform 2D block matching between two images
 *
 *  This filter performs pixel-wise 2D block-matching
 *  between a pair of image. This is especially useful in the case of
 *  stereo pairs in epipolar geometry, where displacements
 *  corresponding to differences of elevation occur in the horizontal
 *  direction only (in that case, the exploration along the vertical
 *  direction can be disabled). Please note that only integer pixel
 *  displacement are explored. For finer results, consider up-sampling
 *  the input images or use the SubPixelDisparityImageFilter.
 *
 *  The block-matching metric itself is defined by a template functor
 *  on neighborhood iterators. A wide range of block-matching
 *  criterions can be implemented this way, but the default functor
 *  performs a simple SSD (Sum of Square Distances). The radius of the
 *  blocks can be set using the SetRadius() method. The filter will
 *  try to minimize the metric value by default. Setting the minimize
 *  flag to off using the MinimizeOff() method will make the filter
 *  try to maximize the metric.
 *
 *  Only a user defined area of disparities between the two images is
 *  explored, which can be set by using the SetMinimumHorizontalDisparity()
 *  , SetMinimumVerticalDisparity(), SetMaximumHorizontalDisparity()
 *  and SetMaximumVerticalDisparity() methods.
 *
 *  This filter has three outputs: the first is the metric image,
 *  which contains the metric optimum value corresponding to the
 *  estimated displacement. The second and last outputs are the
 *  horizontal and vertical disparity maps, which can be retrieved
 *  using the GetHorizontalDisparityOutput() and GetVerticalDisparityOutput()
 *  methods. They contain the horizontal and vertical local displacement
 *  between the two input images (displacement is given in pixels, from left
 *  image to right image).
 *
 *  Masks are not mandatory. A mask allows indicating pixels validity in
 *  either left or right image. Left and right masks can be used independently.
 *  If masks are used, only pixels whose mask values are strictly positive
 *  will be considered for disparity matching. The other will exhibit a null
 *  metric value and a disparity corresponding to the minimum allowed
 *  disparity.
 *
 *  The disparity exploration can also be reduced thanks to initial disparity
 *  maps. The user can provide initial disparity estimate (using the same image
 *  type and size as the output disparities), or global disparity values. Then
 *  an exploration radius indicates the disparity range to be explored around
 *  the initial estimate (global minimum and maximum values are still in use).
 *
 *  \sa FineRegistrationImageFilter
 *  \sa StereorectificationDisplacementFieldSource
 *  \sa SubPixelDisparityImageFilter
 *
 *  \ingroup Streamed
 *  \ingroup Threaded
 *
 *
 * \ingroup OTBDisparityMap
 */

template <class TInputImage, class TOutputMetricImage, class TOutputDisparityImage = TOutputMetricImage, class TMaskImage = otb::Image<unsigned char>,
          class TBlockMatchingFunctor = Functor::SSDBlockMatching<TInputImage,TOutputMetricImage> >
class ITK_EXPORT PixelWiseBlockMatchingImageFilter :
    public itk::ImageToImageFilter<TInputImage,TOutputDisparityImage>
{
public:
  /** Standard class typedef */
  typedef PixelWiseBlockMatchingImageFilter       Self;
  typedef itk::ImageToImageFilter<TInputImage,
                                  TOutputDisparityImage>    Superclass;
  typedef itk::SmartPointer<Self>                           Pointer;
  typedef itk::SmartPointer<const Self>                     ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(PixelWiseBlockMatchingImageFilter, ImageToImageFilter);

  /** Usefull typedefs */
  typedef TInputImage                                       InputImageType;
  typedef TOutputMetricImage                                OutputMetricImageType;
  typedef TOutputDisparityImage                             OutputDisparityImageType;
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

  /** Set left input */
  void SetLeftInput( const TInputImage * image);

  /** Set right input */
  void SetRightInput( const TInputImage * image);

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
  const TOutputDisparityImage * GetHorizontalDisparityOutput() const;
  TOutputDisparityImage * GetHorizontalDisparityOutput();

  /** Get the disparity output */
  const TOutputDisparityImage * GetVerticalDisparityOutput() const;
  TOutputDisparityImage * GetVerticalDisparityOutput();


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

  /** Set/Get the exploration radius in the disparity space */
  itkSetMacro(ExplorationRadius, SizeType);
  itkGetConstReferenceMacro(ExplorationRadius, SizeType);

  /** Set/Get the initial horizontal disparity */
  itkSetMacro(InitHorizontalDisparity,int);
  itkGetConstReferenceMacro(InitHorizontalDisparity,int);

  /** Set/Get the initial vertical disparity */
  itkSetMacro(InitVerticalDisparity,int);
  itkGetConstReferenceMacro(InitVerticalDisparity,int);

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

  /** Set initial horizontal disparity field (optional, override m_InitHorizontalDisparity) */
  void SetHorizontalDisparityInput( const TOutputDisparityImage * hfield);

  /** Set initial vertical disparity field (optional, override m_InitVerticalDisparity) */
  void SetVerticalDisparityInput( const TOutputDisparityImage * vfield);

  /** Get the initial disparity fields */
  const TOutputDisparityImage * GetHorizontalDisparityInput() const;
  const TOutputDisparityImage * GetVerticalDisparityInput() const;

  /** Set/Get macro for the subsampling step */
  itkSetMacro(Step, unsigned int);
  itkGetMacro(Step, unsigned int);

  /** Set/Get macro for the grid start index */
  itkSetMacro(GridIndex, IndexType);
  itkGetConstReferenceMacro(GridIndex, IndexType);

  /** Conversion function between full and subsampled grid region */
  static RegionType ConvertFullToSubsampledRegion(RegionType full, unsigned int step, IndexType index);

  /** Conversion function between subsampled and full grid region */
  static RegionType ConvertSubsampledToFullRegion(RegionType sub, unsigned int step, IndexType index);

protected:
  /** Constructor */
  PixelWiseBlockMatchingImageFilter();

  /** Destructor */
  virtual ~PixelWiseBlockMatchingImageFilter();

  /** Generate output information */
  virtual void GenerateOutputInformation();

  /** Generate input requrested region */
  virtual void GenerateInputRequestedRegion();

  /** Before threaded generate data */
  virtual void BeforeThreadedGenerateData();

  /** Threaded generate data */
  virtual void ThreadedGenerateData(const RegionType & outputRegionForThread, itk::ThreadIdType threadId);

private:
  PixelWiseBlockMatchingImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemeFnted

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

  /** The exploration radius for disparities (used if non null) */
  SizeType                      m_ExplorationRadius;

  /** Block-matching functor */
  BlockMatchingFunctorType      m_Functor;

  /** Initial horizontal disparity (0 by default, used if an exploration radius is set and if no input horizontal
    disparity map is given) */
  int                           m_InitHorizontalDisparity;

  /** Initial vertical disparity (0 by default, used if an exploration radius is set and if no input vertical
    disparity map is given) */
  int                           m_InitVerticalDisparity;

  /** Computation step : disparities are computed on locations of a subsampled grid */
  unsigned int                  m_Step;

  /** Starting index for the subsampled grid. The index is measured with respect to the input image grid
   *  Each coordinate shall lie in [0, m_Step-1]
   */
  IndexType                     m_GridIndex;

};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPixelWiseBlockMatchingImageFilter.txx"
#endif

#endif
