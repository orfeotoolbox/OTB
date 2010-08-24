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
#ifndef __otbFineRegistrationImageFilter_h
#define __otbFineRegistrationImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkInterpolateImageFunction.h"
#include "itkContinuousIndex.h"

#include "itkTranslationTransform.h"
#include "itkImageToImageMetric.txx"

namespace otb
{

/** \class FineRegistrationImageFilter
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
class ITK_EXPORT FineRegistrationImageFilter : public itk::ImageToImageFilter<TInputImage,T0utputCorrelation>
{
public:
  /** Standard class typedefs. */
  typedef FineRegistrationImageFilter                              Self;
  typedef itk::ImageToImageFilter<TInputImage,T0utputCorrelation> Superclass;
  typedef itk::SmartPointer<Self>                                 Pointer;
  typedef itk::SmartPointer<const Self>                           ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(FineRegistrationImageFilter, ImageToImageFilter);

  /** Some convenient typedefs. */
  typedef typename T0utputCorrelation::RegionType                 OutputImageRegionType;
  typedef typename TOutputDeformationField::PixelType             DeformationValueType;
  typedef typename TInputImage::Pointer                           InputImagePointerType;
  typedef typename TInputImage::RegionType                        InputImageRegionType;
  typedef typename TInputImage::SizeType                          SizeType;
  typedef typename TInputImage::IndexType                         IndexType;
  typedef typename TInputImage::SpacingType                       SpacingType;
  typedef typename itk::InterpolateImageFunction
  <TInputImage, double>                                           InterpolatorType;
  typedef typename InterpolatorType::Pointer                      InterpolatorPointerType;
  typedef typename itk::ContinuousIndex<double, 2>                ContinuousIndexType;
  typedef typename itk::ImageToImageMetric<TInputImage,
                                           TInputImage>           MetricType;
  typedef typename MetricType::Pointer                            MetricPointerType;
  typedef typename itk::TranslationTransform<double,2>            TranslationType;
  typedef typename TranslationType::Pointer                       TranslationPointerType;

  /** Set/Get the Metric used to compare images */
  itkSetObjectMacro(Metric,MetricType);
  itkGetObjectMacro(Metric,MetricType);

  /** Set/Get the interpolator used to inpterolate moving image at non-grid positions */
  itkSetObjectMacro(Interpolator,InterpolatorType);
  itkGetObjectMacro(Interpolator,InterpolatorType);

  /** Connect one of the operands for pixel-wise addition */
  void SetFixedInput( const TInputImage * image);

  /** Connect one of the operands for pixel-wise addition */
  void SetMovingInput( const TInputImage * image);

  /** Get the inputs */
  const TInputImage * GetFixedInput();
  const TInputImage * GetMovingInput();

  /** Get the output deformation field */
  TOutputDeformationField * GetOutputDeformationField();
 
  /** Set the radius of the area on which metric is evaluated */
  itkSetMacro(Radius, SizeType);
  itkGetMacro(Radius, SizeType);

  /** Set the searh radius */
  itkSetMacro(SearchRadius,SizeType);
  itkGetMacro(SearchRadius,SizeType);
  
  /** Set/Get subpixel accuracy */
  itkSetMacro(SubPixelAccuracy,double);
  itkGetMacro(SubPixelAccuracy,double);

  /** True if metric should be minimized. False otherwise */
  itkSetMacro(Minimize,bool);
  itkBooleanMacro(Minimize);

  /** True if deformation field takes spacing into account. False otherwise */
  itkSetMacro(UseSpacing,bool);
  itkBooleanMacro(UseSpacing);

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

protected:
  /** Constructor */
  FineRegistrationImageFilter();
  /** Destructor */
  virtual ~FineRegistrationImageFilter() {};

  /** Threaded generate data */
  virtual void GenerateData();

  /** Generate the input requested regions  */
  virtual void GenerateInputRequestedRegion(void);

private:
  FineRegistrationImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** The radius for correlation */
  SizeType                      m_Radius;

  /** The search radius */
  SizeType                      m_SearchRadius;

  /** Minimize/maximize metric */
  bool                          m_Minimize;

  /** If true, deformation field uses spacing. Otherwise, uses pixel grid */
  bool                          m_UseSpacing;

  /** Search step */
  double                        m_SubPixelAccuracy;

  /** The interpolator */
  InterpolatorPointerType       m_Interpolator;

  /** The metric */
  MetricPointerType             m_Metric;

  /** The translation */
  TranslationPointerType        m_Translation;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbFineRegistrationImageFilter.txx"
#endif

#endif
