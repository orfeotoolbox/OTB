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
#ifndef __otbNNearestTransformsLinearInterpolateDeformationFieldGenerator_h
#define __otbNNearestTransformsLinearInterpolateDeformationFieldGenerator_h

#include "otbPointSetWithTransformToDeformationFieldGenerator.h"

namespace otb
{
/** \class NNearestTransformsLinearInterpolateDeformationFieldGenerator
 *  \brief This class generate the deformation field by performing a linear interpolation of the deformations from the transforms induced by the n nearest points.
 * \ingroup 
 * \ingroup
 */
template <class TPointSet, class TDeformationField>
class ITK_EXPORT NNearestTransformsLinearInterpolateDeformationFieldGenerator
  : public PointSetWithTransformToDeformationFieldGenerator<TPointSet, TDeformationField>
{
 public:
  /** Standard typedefs */
  typedef NNearestTransformsLinearInterpolateDeformationFieldGenerator Self;
  typedef PointSetWithTransformToDeformationFieldGenerator<TPointSet,TDeformationField> Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;
  
  /** Type macro */
  itkNewMacro(Self);
  
  /** Creation through object factory macro */
  itkTypeMacro(NNearestTransformsLinearInterpolateDeformationFieldGenerator,PointSetWithTransformsToDeformationFieldGenerator);
  
  /** Template parameters typedefs */
  typedef typename Superclass::PointSetType PointSetType;
  typedef typename Superclass::PointSetPointerType PointSetPointerType;
  typedef typename Superclass::DeformationFieldType DeformationFieldType;
  typedef typename Superclass::DeformationFieldPointerType DeformationFieldPointerType;
  typedef typename Superclass::IndexType IndexType;
  typedef typename DeformationFieldType::PixelType PixelType;
  typedef typename Superclass::ValueType ValueType;
  typedef typename Superclass::PointType PointType;
  typedef typename Superclass::IndexVectorType IndexVectorType;
  typedef typename Superclass::DistanceVectorType DistanceVectorType;
  typedef typename Superclass::TransformType TransformType;
  typedef typename TransformType::ParametersType ParametersType;


  itkSetMacro(NumberOfPoints,unsigned int);
  itkGetMacro(NumberOfPoints,unsigned int);

protected:
  /** Constructor */
  NNearestTransformsLinearInterpolateDeformationFieldGenerator() {};
  /** Destructor */
  virtual ~NNearestTransformsLinearInterpolateDeformationFieldGenerator() {};
 /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;
  /** Main computation method */
  virtual void GenerateData();

private:
  NNearestTransformsLinearInterpolateDeformationFieldGenerator(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  unsigned int m_NumberOfPoints;
};
}// End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbNNearestTransformsLinearInterpolateDeformationFieldGenerator.txx"
#endif

#endif
