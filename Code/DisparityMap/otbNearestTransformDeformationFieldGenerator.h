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
#ifndef _otbNearestTransformDeformationFieldGenerator_h
#define _otbNearestTransformDeformationFieldGenerator_h

#include "otbPointSetWithTransformToDeformationFieldGenerator.h"

namespace otb
{
/** \class NearestTransformDeformationFieldGenerator
 *  \brief This class implements deformation field generation by taking into account the local transform of the
 *  nearest point in pointset.
 * 
 * \ingroup 
 * \ingroup
 */
template <class TPointSet, class TDeformationField>
class ITK_EXPORT NearestTransformDeformationFieldGenerator
  : public PointSetWithTransformToDeformationFieldGenerator<TPointSet, TDeformationField>
{
 public:
  /** Standard typedefs */
  typedef NearestTransformDeformationFieldGenerator Self;
  typedef PointSetWithTransformToDeformationFieldGenerator<TPointSet,TDeformationField> Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;
  
  /** Type macro */
  itkNewMacro(Self);
  
  /** Creation through object factory macro */
  itkTypeMacro(NearestTransformDeformationFieldGenerator,PointSetWithTransformToDeformationFieldGenerator);
  
  /** Template parameters typedefs */
  typedef typename Superclass::PointSetType PointSetType;
  typedef typename Superclass::PointSetPointerType PointSetPointerType;
  typedef typename Superclass::DeformationFieldType DeformationFieldType;
  typedef typename Superclass::DeformationFieldPointerType DeformationFieldPointerType;
  typedef typename Superclass::IndexType IndexType;
  typedef typename Superclass::PointType PointType;
  typedef typename DeformationFieldType::PixelType PixelType;
  typedef typename Superclass::ValueType ValueType;
  typedef typename Superclass::IndexVectorType IndexVectorType;
  typedef typename Superclass::TransformType TransformType;
  typedef typename TransformType::ParametersType ParametersType;

protected:
  /** Constructor */
  NearestTransformDeformationFieldGenerator() {};
  /** Destructor */
  virtual ~NearestTransformDeformationFieldGenerator() {};
 /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;
  /** Main computation method */
  virtual void GenerateData();

private:
  NearestTransformDeformationFieldGenerator(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};
}// End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbNearestTransformDeformationFieldGenerator.txx"
#endif

#endif
