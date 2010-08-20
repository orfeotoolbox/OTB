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
#ifndef __otbFineCorrelationImageFilter_h
#define __otbFineCorrelationImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkImageRegionIterator.h"
#include "itkInterpolateImageFunction.h"
#include "itkContinuousIndex.h"


namespace otb
{

/** \class FineCorrelationImageFilter
 * \brief Computes a deformation field between two images
 *
 * This class compute a correlation field and the associated
 * deformation field between two images.
 *
 * The m_Radius parameter defines the size of the window to compute
 * the normalized correlation.
 *
 * The m_SearchRadius parameter defines the size of the area where to
 * search for a correlation peak in the moving image.
 *
 * Once the correlation peak has been found, it can be further refined
 * by trying to fit a quadric surface in the premises of the
 * maxima, thus obtaining sub-pixel precision. This option is
 * activated by setting m_RefineLocation to true.
 *
 * TOutputCorrelation is supposed to be a scalar image with floating
 * point precision.
 *
 * TOutputDeformationField is supposed to be an image whose pixel is a
 * fixed size vector of size 2, with floating point precision.
 *
 * Images spacing is ignored during computation and in the output
 * deformation field.
 *
 * All computation are done in double precision.
 * 
 * \ingroup IntensityImageFilters   Multithreaded, Streamed
 *
 */
template <class TInputImage, class T0utputCorrelation, class TOutputDeformationField>
class ITK_EXPORT FineCorrelationImageFilter : public itk::ImageToImageFilter<TInputImage,T0utputCorrelation>
{
public:
  /** Standard class typedefs. */
  typedef FineCorrelationImageFilter                              Self;
  typedef itk::ImageToImageFilter<TInputImage,T0utputCorrelation> Superclass;
  typedef itk::SmartPointer<Self>                                 Pointer;
  typedef itk::SmartPointer<const Self>                           ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(FineCorrelationImageFilter, ImageToImageFilter);

  /** Some convenient typedefs. */
  typedef typename T0utputCorrelation::RegionType                 OutputImageRegionType;
  typedef typename TOutputDeformationField::PixelType             DeformationValueType;
  typedef typename TInputImage::Pointer                           InputImagePointerType;
  typedef typename TInputImage::RegionType                        InputImageRegionType;
  typedef typename TInputImage::SizeType                          SizeType;
  typedef typename TInputImage::IndexType                         IndexType;
  typedef typename itk::ImageRegionIterator<TInputImage>          InputImageRegionIteratorType;
  typedef typename itk::InterpolateImageFunction
  <TInputImage, double>                                           InterpolatorType;
  typedef typename InterpolatorType::Pointer                      InterpolatorPointerType;
  typedef typename itk::ContinuousIndex<double, 2>                ContinuousIndexType;

  /** Typedef for refinements mode */
  typedef enum {COARSE = 0, LSQR_QUADFIT = 1, SUBPIXEL = 2}       RefinementModeType;

  /** Connect one of the operands for pixel-wise addition */
  void SetFixedInput( const TInputImage * image);

  /** Connect one of the operands for pixel-wise addition */
  void SetMovingInput( const TInputImage * image);

  /** Get the inputs */
  const TInputImage * GetFixedInput();
  const TInputImage * GetMovingInput();

  /** Get the output deformation field */
  TOutputDeformationField * GetOutputDeformationField();
 
  /** Typedef of the input iterator */
  typedef itk::ConstNeighborhoodIterator<TInputImage>             NeighborhoodIteratorType;  
  typedef typename NeighborhoodIteratorType::RadiusType           RadiusType;
  typedef typename NeighborhoodIteratorType::NeighborhoodType     NeighborhoodType;
  typedef typename NeighborhoodIteratorType::OffsetType           OffsetType;

  itkSetMacro(Radius, RadiusType);
  itkGetMacro(Radius, RadiusType);
  itkSetMacro(SearchRadius,RadiusType);
  itkGetMacro(SearchRadius,RadiusType);
  
  /** Set the subpixel precision */
  itkSetMacro(SubPixelPrecision,unsigned int);
  itkGetMacro(SubPixelPrecision,unsigned int);

  /** Set/Get Refinement mode */
  itkSetEnumMacro(RefinementMode,RefinementModeType);
  itkGetEnumMacro(RefinementMode,RefinementModeType);

  void SetRefinementModeToCoarse()
  {
    m_RefinementMode = COARSE;
  }

  void SetRefinementModeToLSQRQuadFit()
  {
    m_RefinementMode = LSQR_QUADFIT;
  }

  void SetRefinementModeToSubPixel()
  {
    m_RefinementMode = SUBPIXEL;
  }

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

  /** Set the refinement mode */
  
protected:
  /** Constructor */
  FineCorrelationImageFilter();
  /** Destructor */
  virtual ~FineCorrelationImageFilter() {};

  /** Threaded generate data */
  virtual void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,int threadId );

  /** Generate the input requested regions  */
  virtual void GenerateInputRequestedRegion(void);

  /** Things to do before threaded computation */
  virtual void BeforeThreadedGenerateData();

private:
  FineCorrelationImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

 /** Compute correlation from offset */
  inline double Correlation(const NeighborhoodType & fixed, const NeighborhoodType & moving, const OffsetType & offset, unsigned int scale=1) const;

  /** Refine the location of the correlation maximum */
  inline double RefineLocation(const NeighborhoodType & correlMap,const OffsetType & maxPos, DeformationValueType & value) const;

  /** Compute subpixel neighborhood */
  const NeighborhoodType ComputeSubPixelNeighborhood(const IndexType & location, unsigned int scale) const;

  typedef std::pair<double,OffsetType> PairType;
  typedef std::vector<PairType>        PairVectorType;

  /** Compare pairs by correlation values */
  static bool CompPairs(const PairType &p1, const PairType &p2)
  {
    return p1.first>p2.first;
  }

  /** The radius for correlation */
  RadiusType m_Radius;

  /** The search radius */
  RadiusType m_SearchRadius;

  /** The interpolator */
  InterpolatorPointerType m_Interpolator;

  /** Choose the refinement type */
  RefinementModeType      m_RefinementMode;

  /** Precision used in subpixel mode */
  unsigned int            m_SubPixelPrecision;

  double                  m_Epsilon;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbFineCorrelationImageFilter.txx"
#endif

#endif
