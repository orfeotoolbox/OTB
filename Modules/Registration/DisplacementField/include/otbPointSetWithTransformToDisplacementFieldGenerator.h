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
#ifndef __otbPointSetWithTransformToDisplacementFieldGenerator_h
#define __otbPointSetWithTransformToDisplacementFieldGenerator_h

#include "otbPointSetToDisplacementFieldGenerator.h"
#include "itkTransform.h"

namespace otb
{
/** \class PointSetWithTransformToDisplacementFieldGenerator
 *  \brief Base class for filters generating a displacement field from a point set enriched with local transform information.
 *
 *  The output of the filters deriving from this base class can be passed to the itk::WarpImageFilter for image resampling.
 *
 *  \sa DisparityMapEstimationMethod
 *  \sa itk::WarpImageFilter
 *
 *  \ingroup DisparityMap
 */
template <class TPointSet, class TDisplacementField>
class ITK_EXPORT PointSetWithTransformToDisplacementFieldGenerator
  : public PointSetToDisplacementFieldGenerator<TPointSet, TDisplacementField>
{
public:
  /** Standard typedefs */
  typedef PointSetWithTransformToDisplacementFieldGenerator                  Self;
  typedef PointSetToDisplacementFieldGenerator<TPointSet, TDisplacementField> Superclass;
  typedef itk::SmartPointer<Self>                                           Pointer;
  typedef itk::SmartPointer<const Self>                                     ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(PointSetWithTransformToDisplacementFieldGenerator, PointSetToDisplacementFieldGenerator);

  /** Template parameters typedefs */
  typedef TPointSet                      PointSetType;
  typedef typename PointSetType::Pointer PointSetPointerType;

  typedef TDisplacementField                                DisplacementFieldType;
  typedef typename DisplacementFieldType::Pointer           DisplacementFieldPointerType;
  typedef typename DisplacementFieldType::IndexType         IndexType;
  typedef typename DisplacementFieldType::SizeType          SizeType;
  typedef typename DisplacementFieldType::SpacingType       SpacingType;
  typedef typename DisplacementFieldType::PointType         PointType;
  typedef typename DisplacementFieldType::InternalPixelType ValueType;

  /** More typedefs */
  typedef typename Superclass::DistanceVectorType DistanceVectorType;
  typedef typename Superclass::IndexVectorType    IndexVectorType;

  /** Transform typedefs */
  typedef itk::Transform<double, 2, 2>           TransformType;
  typedef typename TransformType::Pointer        TransformPointerType;
  typedef typename TransformType::ParametersType ParametersType;

  /** Set/Get the Transfrom. */
  itkSetObjectMacro(Transform, TransformType);
  itkGetObjectMacro(Transform, TransformType);

protected:
  /** Constructor */
  PointSetWithTransformToDisplacementFieldGenerator();
  /** Destructor */
  virtual ~PointSetWithTransformToDisplacementFieldGenerator() {}
  /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  PointSetWithTransformToDisplacementFieldGenerator(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /**
   * The transform used for local registration.
   */
  TransformPointerType m_Transform;

};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPointSetWithTransformToDisplacementFieldGenerator.txx"
#endif

#endif
