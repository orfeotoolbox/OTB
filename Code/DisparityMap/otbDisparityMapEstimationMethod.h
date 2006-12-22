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
#ifndef _otbDisparityMapEstimationMethod_h
#define _otbDisparityMapEstimationMethod_h

#include "itkProcessObject.h"
#include "itkImageToImageMetric.h"
#include "itkSingleValuedNonLinearOptimizer.h"
#include "itkDataObjectDecorator.h"
#include "itkPointSet.h"

namespace otb
{
  /** \class DisparityMapEstimationMethod
   *  \brief MAECENAS 
   *
   */
template <class TFixedImage, class TMovingImage>
class ITK_EXPORT DisparityMapEstimationMethod
  : public itk::ProcessObject
{
 public:
  /** Standard class typedefs. */
  typedef DisparityMapEstimationMethod  Self;
  typedef ProcessObject                 Superclass;
  typedef itk::SmartPointer<Self>            Pointer;
  typedef itk::SmartPointer<const Self>      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  /** Run-time type information (and related methods). */
  itkTypeMacro(DisparityMapEstimationMethod, ProcessObject);

  /**  Type of the Fixed image. */
  typedef TFixedImage  FixedImageType;
  typedef typename FixedImageType::Pointer FixedImagePointerType;
  typedef typename FixedImageType::PixelType FixedPixelType;

  /**  Type of the Moving image. */
  typedef TMovingImage MovingImageType;
  typedef typename MovingImageType::Pointer MovingImagePointerType;
  typedef typename MovingImageType::PixelType MovingPixelType;

  /**  Type of the metric. */
  typedef itk::ImageToImageMetric<FixedImageType,MovingImageType>  MetricType;
  typedef typename MetricType::Pointer MetricPointer;
  typedef typename MetricType::FixedImageRegionType FixedImageRegionType;

  /**  Type of the Transform . */
  typedef typename MetricType::TransformType TransformType;
  typedef typename TransformType::Pointer TransformPointer;

  /** Type for the output: Using Decorator pattern for enabling
   *  the Transform to be passed in the data pipeline */
  typedef itk::DataObjectDecorator<TransformType> TransformOutputType;
  typedef typename TransformOutputType::Pointer TransformOutputPointer;
  typedef typename TransformOutputType::ConstPointer TransformOutputConstPointer;
  
  /**  Type of the Interpolator. */
  typedef typename MetricType::InterpolatorType InterpolatorType;
  typedef typename InterpolatorType::Pointer InterpolatorPointer;

  /**  Type of the optimizer. */
  typedef itk::SingleValuedNonLinearOptimizer OptimizerType;

  /** Type of the Transformation parameters This is the same type used to
   *  represent the search space of the optimization algorithm */
  typedef typename MetricType::TransformParametersType ParametersType;

  /** Point set typedef */
  typedef itk::PointSet<ParametersType,FixedImageType::ImageDimension> PointSetType;
  typedef typename PointSetType::Pointer PointSetPointerType;

  /** Smart Pointer type to a DataObject. */
  typedef typename itk::DataObject::Pointer DataObjectPointer;

 
  void StartRegistration(void);

  /** Method that initiates the optimization process. */
  void StartOptimization(void);

  /** Set/Get the Fixed image. */
  itkSetObjectMacro(FixedImage,FixedImageType);
  itkGetObjectMacro(FixedImage,FixedImageType); 

  /** Set/Get the Moving image. */
  itkSetObjectMacro(MovingImage,MovingImageType);
  itkGetObjectMacro(MovingImage,MovingImageType);

  /** Set/Get the points set. */
  itkSetObjectMacro(PointSet,PointSetType);
  itkGetObjectMacro(PointSet,PointSetType);

  /** Set/Get the Optimizer. */
  itkSetObjectMacro( Optimizer,  OptimizerType );
  itkGetObjectMacro( Optimizer,  OptimizerType );

  /** Set/Get the Metric. */
  itkSetObjectMacro( Metric, MetricType );
  itkGetObjectMacro( Metric, MetricType );

  /** Set/Get the Transfrom. */
  itkSetObjectMacro( Transform, TransformType );
  itkGetObjectMacro( Transform, TransformType );

  /** Set/Get the Interpolator. */
  itkSetObjectMacro( Interpolator, InterpolatorType );
  itkGetObjectMacro( Interpolator, InterpolatorType );

  itkSetMacro(WinSize,unsigned int);
  itkGetMacro(WinSize,unsigned int);

  itkSetMacro(ExploSize,unsigned int);
  itkGetMacro(ExploSize,unsigned int);

/*   /\** Set/Get the initial transformation parameters. *\/ */
/*   virtual void SetInitialTransformParameters( const ParametersType & param ); */
/*   itkGetConstReferenceMacro( InitialTransformParameters, ParametersType ); */

protected:
  DisparityMapEstimationMethod();
  virtual ~DisparityMapEstimationMethod() {};
 /*  void PrintSelf(std::ostream& os, Indent indent) const; */

  /** Method invoked by the pipeline in order to trigger the computation of 
   * the registration. */
  void  GenerateData ();

/*   /\** Provides derived classes with the ability to set this private var *\/ */
/*   itkSetMacro( LastTransformParameters, ParametersType ); */


private:
  DisparityMapEstimationMethod(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  
  MetricPointer                    m_Metric;
  OptimizerType::Pointer           m_Optimizer;

  MovingImagePointerType          m_MovingImage;
  FixedImagePointerType           m_FixedImage;

  TransformPointer                 m_Transform;
  InterpolatorPointer              m_Interpolator;

  ParametersType                   m_InitialTransformParameters;
  ParametersType                   m_LastTransformParameters;
  
  bool                             m_FixedImageRegionDefined;
  FixedImageRegionType             m_FixedImageRegion;
    
  PointSetPointerType              m_PointSet;

  unsigned int                     m_ExploSize;
  unsigned int                     m_WinSize;
  };
}// end namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbDisparityMapEstimationMethod.txx"
#endif

#endif
