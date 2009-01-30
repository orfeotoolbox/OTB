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
#ifndef __otbNNearestPointsLinearInterpolateDeformationFieldGenerator_h
#define __otbNNearestPointsLinearInterpolateDeformationFieldGenerator_h

#include "otbPointSetToDeformationFieldGenerator.h"

namespace otb
{
/** \class NNearestPointsLinearInterpolateDeformationFieldGenerator
 *  \brief This class generate the deformation field by performing a linear interpolation of the deformation induced by the n nearest point.
 * \ingroup DisparityMap
 */
template <class TPointSet, class TDeformationField>
class ITK_EXPORT NNearestPointsLinearInterpolateDeformationFieldGenerator
      : public PointSetToDeformationFieldGenerator<TPointSet, TDeformationField>
{
public:
  /** Standard typedefs */
  typedef NNearestPointsLinearInterpolateDeformationFieldGenerator Self;
  typedef PointSetToDeformationFieldGenerator<TPointSet,TDeformationField> Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(NNearestPointsLinearInterpolateDeformationFieldGenerator,PointSetToDeformationFieldGenerator);

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

  itkSetMacro(NumberOfPoints,unsigned int);
  itkGetMacro(NumberOfPoints,unsigned int);

protected:
  /** Constructor */
  NNearestPointsLinearInterpolateDeformationFieldGenerator() {};
  /** Destructor */
  virtual ~NNearestPointsLinearInterpolateDeformationFieldGenerator() {};
  /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;
  /** Main computation method */
  virtual void GenerateData();

private:
  NNearestPointsLinearInterpolateDeformationFieldGenerator(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  unsigned int m_NumberOfPoints;
};
}// End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbNNearestPointsLinearInterpolateDeformationFieldGenerator.txx"
#endif

#endif
