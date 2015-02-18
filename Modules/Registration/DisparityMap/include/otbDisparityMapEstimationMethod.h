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
#ifndef __otbDisparityMapEstimationMethod_h
#define __otbDisparityMapEstimationMethod_h

#include "itkImageToImageMetric.h"
#include "itkSingleValuedNonLinearOptimizer.h"
#include "itkDataObjectDecorator.h"
#include "itkPointSet.h"
#include "otbPointSetSource.h"

namespace otb
{
/** \class DisparityMapEstimationMethod
 *  \brief This class performs an estimation of the local displacement
 *  modelled by a given transform between the fixed and the moving image,
 *  at each point of the given point set.
 *
 *  It uses the ITK registration framework locally for each point and thus
 *  provides the flexibility of this framework. The parameters of each transform
 *  are stored in the ouptut point set associated data. Optimizer, metric,
 *  interpolator and transform fixed parameters have to be set by the user.
 *
 *  This filters returns the pointset enriched with a set of value as PointData, in order of apparition :
 *  - The final metric value,
 *  - The column displacement value,
 *  - The row displacement value,
 *  - The final estimated parameters of the transform.
 *
 *  This class is derived from the MAECENAS code provided by Jordi Inglada,
 *  from CNES.
 *
 *  \sa FineRegistrationImageFilter, FastCorrelationImageFilter
 */
template <class TFixedImage, class TMovingImage, class TPointSet>
class ITK_EXPORT DisparityMapEstimationMethod
  : public PointSetSource<TPointSet>
{
public:
  /** Standard class typedefs. */
  typedef DisparityMapEstimationMethod  Self;
  typedef PointSetSource<TPointSet>     Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(DisparityMapEstimationMethod, ProcessObject);

  /** Typedef for the Fixed image. */
  typedef TFixedImage                        FixedImageType;
  typedef typename FixedImageType::Pointer   FixedImagePointerType;
  typedef typename FixedImageType::PixelType FixedPixelType;
  typedef typename FixedImageType::SizeType  SizeType;

  /** Typedef for the Moving image. */
  typedef TMovingImage                        MovingImageType;
  typedef typename MovingImageType::Pointer   MovingImagePointerType;
  typedef typename MovingImageType::PixelType MovingPixelType;

  /** Typedef for the input and ouptut point set */
  typedef TPointSet                      PointSetType;
  typedef typename PointSetType::Pointer PointSetPointerType;

  /**  Typedef for the generic metric. */
  typedef itk::ImageToImageMetric<FixedImageType, MovingImageType> MetricType;
  typedef typename MetricType::Pointer                             MetricPointerType;
  typedef typename MetricType::FixedImageRegionType                FixedImageRegionType;

  /** Typedef for the generic transform . */
  typedef typename MetricType::TransformType TransformType;
  typedef typename TransformType::Pointer    TransformPointerType;

  /** Using Decorator pattern for enabling the Transform to be passed in the data pipeline */
  typedef itk::DataObjectDecorator<TransformType>    TransformOutputType;
  typedef typename TransformOutputType::Pointer      TransformOutputPointerType;
  typedef typename TransformOutputType::ConstPointer TransformOutputConstPointerType;

  /** Typedef for the generic interpolator. */
  typedef typename MetricType::InterpolatorType InterpolatorType;
  typedef typename InterpolatorType::Pointer    InterpolatorPointerType;

  /** Typedef for  the generic optimizer. */
  typedef itk::SingleValuedNonLinearOptimizer OptimizerType;
  typedef typename OptimizerType::Pointer     OptimizerPointerType;

  /** Type of the Transformation parameters This is the same type used to
   *  represent the search space of the optimization algorithm */
  typedef typename MetricType::TransformParametersType ParametersType;

  /** Smart Pointer type to a DataObject. */
  typedef typename itk::DataObject::Pointer DataObjectPointer;

  /** Set/Get the Optimizer. */
  itkSetObjectMacro(Optimizer, OptimizerType);
  itkGetObjectMacro(Optimizer, OptimizerType);

  /** Set/Get the Metric. */
  itkSetObjectMacro(Metric, MetricType);
  itkGetObjectMacro(Metric, MetricType);

  /** Set/Get the Transfrom. */
  itkSetObjectMacro(Transform, TransformType);
  itkGetObjectMacro(Transform, TransformType);

  /** Set/Get the Interpolator. */
  itkSetObjectMacro(Interpolator, InterpolatorType);
  itkGetObjectMacro(Interpolator, InterpolatorType);

  /** Set/Get the window radius */
  itkSetMacro(WinSize, SizeType);
  itkGetMacro(WinSize, SizeType);

  /** Set/Get the exploration area radius */
  itkSetMacro(ExploSize, SizeType);
  itkGetMacro(ExploSize, SizeType);

  /** Set/Get the initial transformation parameters. */
  itkSetMacro(InitialTransformParameters, ParametersType);
  itkGetConstReferenceMacro(InitialTransformParameters, ParametersType);

  /**
   * Set the source pointset.
   * \param pointset The source pointset.
   */
  void SetPointSet(const TPointSet * pointset);
  /**
   * Get the source pointset.
   * \return The source pointset.
   */
  const TPointSet * GetPointSet(void);

  /**
   * Set the fixed image.
   * \param image The fixed image.
   **/
  void SetFixedImage(const TFixedImage * image);

  /**
   * Get the fixed image.
   * \return The fixed image.
   **/
  const TFixedImage * GetFixedImage(void);

  /**
   * Set the moving image.
   * \param image The mobing image.
   **/
  void SetMovingImage(const TMovingImage * image);

  /**
   * Get the fixed image.
   * \return The fixed image.
   **/
  const TMovingImage * GetMovingImage(void);

protected:
  /**
   * Constructor.
   */
  DisparityMapEstimationMethod();
  /**
   * Destructor.
   */
  virtual ~DisparityMapEstimationMethod();
  /**
   * Standard PrintSelf method.
   */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;
  /**
   * Main computation method.
   */
  void  GenerateData();

private:
  DisparityMapEstimationMethod(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
  /**
   * The metric used for local registration.
   */
  MetricPointerType m_Metric;
  /**
   * The optimizer used for local registration.
   */
  OptimizerPointerType m_Optimizer;
  /**
   * The transform used for local registration.
   */
  TransformPointerType m_Transform;
  /**
   * The interpolator used for local registration.
   */
  InterpolatorPointerType m_Interpolator;
  /**
   * The initial transform parameters.
   */
  ParametersType m_InitialTransformParameters;
  /**
   * The size of the exploration area
   */
  SizeType m_ExploSize;
  /**
   * The size of the window
   */
  SizeType m_WinSize;
};
} // end namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbDisparityMapEstimationMethod.txx"
#endif

#endif
