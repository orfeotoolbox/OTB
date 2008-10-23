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
#ifndef __otbPointSetBasedResamplingFilter_h
#define __otbPointSetBasedResamplingFilter_h

#include "itkImageToImageFilter.h"
#include "itkTransform.h"
#include "itkInterpolateImageFunction.h"

namespace otb
{
/** \class PointSetBasedResamplingFilter
 *  \brief 
 *
 * \ingroup DisparityMap
 */

template <class TInputImage, class TPointSet, class TOutputImage>
class ITK_EXPORT PointSetBasedResamplingFilter
  : public itk::ImageToImageFilter<TInputImage,TOutputImage>
{
 public:
  /** Standard typedefs */
  typedef PointSetBasedResamplingFilter  Self;
  typedef itk::ImageToImageFilter<TInputImage,TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>           Pointer;
  typedef itk::SmartPointer<const Self>     ConstPointer;
  
  /** Type macro */
  itkNewMacro(Self);
  
  /** Creation through object factory macro */
  itkTypeMacro(PointSetBasedResamplingFilter,ImageToImageFilter);
  
  /** Template parameters typedefs */
  typedef TInputImage InputImageType;
  typedef TOutputImage OutputImageType;
  typedef TPointSet PointSetType;
  typedef typename InputImageType::ConstPointer InputImageConstPointerType;
  typedef typename OutputImageType::Pointer OutputImagePointerType;
  typedef typename PointSetType::Pointer PointSetPointerType;
  typedef typename OutputImageType::IndexType IndexType;
  typedef typename OutputImageType::SizeType SizeType;
  typedef typename OutputImageType::SpacingType SpacingType;
  typedef typename OutputImageType::PointType PointType;
  typedef typename OutputImageType::ValueType OutputPixelType;

  /** Transform typedefs */
  typedef itk::Transform<double,InputImageType::ImageDimension,OutputImageType::ImageDimension> TransformType;
  typedef typename TransformType::Pointer TransformPointerType;
  typedef typename TransformType::ParametersType ParametersType;

  /** Typedef for the generic interpolator. */
  typedef itk::InterpolateImageFunction<InputImageType,double> InterpolatorType;
  typedef typename InterpolatorType::Pointer InterpolatorPointerType;

  /** Using Decorator pattern for enabling the Transform to be passed in the data pipeline */
  typedef itk::DataObjectDecorator<TransformType> TransformOutputType;
  typedef typename TransformOutputType::Pointer TransformOutputPointerType;
  typedef typename TransformOutputType::ConstPointer TransformOutputConstPointerType;
 
  /**
   * Set the pointset containing the disparity.
   * \param pointset The pointset containing the disparity.
   */
  void SetPointSet(const TPointSet * pointset);
  /**
   * Get the pointset containing the disparity.
   * \return The pointset containing the disparity.
   */
   const TPointSet * GetPointSet(void);

   itkSetMacro(MetricThreshold,double);
   itkGetMacro(MetricThreshold,double);
   itkSetMacro(OutputSize,SizeType);
   itkGetConstReferenceMacro(OutputSize,SizeType);
   itkSetMacro(OutputSpacing,SpacingType);
   itkGetConstReferenceMacro(OutputSpacing,SpacingType);
   itkSetMacro(OutputOrigin,PointType);
   itkGetConstReferenceMacro(OutputOrigin,PointType);
   
   /** Set/Get the Transfrom. */
   itkSetObjectMacro(Transform,TransformType);
   itkGetObjectMacro(Transform,TransformType);
   
   /** Set/Get the Interpolator. */
   itkSetObjectMacro(Interpolator,InterpolatorType);
   itkGetObjectMacro(Interpolator,InterpolatorType);
   
protected:
  /** Constructor */
  PointSetBasedResamplingFilter();
  /** Destructor */
  virtual ~PointSetBasedResamplingFilter() {};
 /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;
  
  /** Main computation method */
  virtual void GenerateData(void);

 /** Generate output information */
  virtual void GenerateOutputInformation(void);
  /** 
   * \return The parameters of the transform associated with the nearest suitable point in pointset.
   */
  ParametersType GetNearestPointTransformParameters(IndexType &index);

private:
  PointSetBasedResamplingFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /**
   * The threshold of metric value.
   */
  double m_MetricThreshold;
  /** The output size */
  SizeType m_OutputSize;
  /** The output spacing. */
  SpacingType m_OutputSpacing;
  /** The output origin */
  PointType m_OutputOrigin;
  /** Default value */
  OutputPixelType m_DefaultValue;

 /**
   * The transform used for local registration.
   */
  TransformPointerType    m_Transform;
  /**
   * The interpolator used for local registration.
   */
  InterpolatorPointerType m_Interpolator;

};
}// End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPointSetBasedResamplingFilter.txx"
#endif

#endif
