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
#ifndef _otbPointSetWithTransformToDeformationFieldGenerator_h
#define _otbPointSetWithTransformToDeformationFieldGenerator_h

#include "otbPointSetToDeformationFieldGenerator.h"
#include "itkTransform.h"

namespace otb
{
/** \class PointSetWithTransformToDeformationFieldGenerator
 *  \brief Base class for filters generating a deformation field from a point set enriched with local transform information.
 *  The output of the filters deriving from this base class can be passed to the itk::WarpImageFilter for image resampling.
 *  \sa DisparityMapEstimationMethod
 *   \sa itk::WarpImageFilter
 *  \ingroup 
 */
template <class TPointSet, class TDeformationField>
class ITK_EXPORT PointSetWithTransformToDeformationFieldGenerator
  : public PointSetToDeformationFieldGenerator<TPointSet,TDeformationField>
{
 public:
  /** Standard typedefs */
  typedef PointSetWithTransformToDeformationFieldGenerator Self;
  typedef PointSetToDeformationFieldGenerator<TPointSet,TDeformationField> Superclass;
  typedef itk::SmartPointer<Self>             Pointer;
  typedef itk::SmartPointer<const Self>       ConstPointer;
  
  /** Type macro */
  itkNewMacro(Self);
  
  /** Creation through object factory macro */
  itkTypeMacro(PointSetWithTransformToDeformationFieldGenerator,PointSetToDeformationFieldGenerator);
  
  /** Template parameters typedefs */
  typedef TPointSet PointSetType;
  typedef typename PointSetType::Pointer PointSetPointerType;

  typedef TDeformationField DeformationFieldType;
  typedef typename DeformationFieldType::Pointer DeformationFieldPointerType;
  typedef typename DeformationFieldType::IndexType IndexType;
  typedef typename DeformationFieldType::SizeType SizeType;
  typedef typename DeformationFieldType::SpacingType SpacingType;
  typedef typename DeformationFieldType::PointType PointType;
  typedef typename DeformationFieldType::ValueType ValueType;

  /** More typedefs */
  typedef typename Superclass::DistanceVectorType DistanceVectorType;
  typedef typename Superclass::IndexVectorType IndexVectorType;

  /** Transform typedefs */
  typedef itk::Transform<double,2,2> TransformType;
  typedef typename TransformType::Pointer TransformPointerType;
  typedef typename TransformType::ParametersType ParametersType;

  /** Set/Get the Transfrom. */
  itkSetObjectMacro(Transform,TransformType);
  itkGetObjectMacro(Transform,TransformType);
  
protected:
  /** Constructor */
  PointSetWithTransformToDeformationFieldGenerator();
  /** Destructor */
  virtual ~PointSetWithTransformToDeformationFieldGenerator() {};
 /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  PointSetWithTransformToDeformationFieldGenerator(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /**
   * The transform used for local registration.
   */
  TransformPointerType m_Transform;
  
};
}// End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPointSetWithTransformToDeformationFieldGenerator.txx"
#endif

#endif
