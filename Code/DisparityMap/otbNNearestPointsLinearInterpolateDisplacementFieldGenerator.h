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
#ifndef __otbNNearestPointsLinearInterpolateDisplacementFieldGenerator_h
#define __otbNNearestPointsLinearInterpolateDisplacementFieldGenerator_h

#include "otbPointSetToDisplacementFieldGenerator.h"

namespace otb
{
/** \class NNearestPointsLinearInterpolateDisplacementFieldGenerator
 *  \brief This class generate the displacement field by performing a linear interpolation of the displacement induced by the n nearest point.
 * \ingroup DisparityMap
 */
template <class TPointSet, class TDisplacementField>
class ITK_EXPORT NNearestPointsLinearInterpolateDisplacementFieldGenerator
  : public PointSetToDisplacementFieldGenerator<TPointSet, TDisplacementField>
{
public:
  /** Standard typedefs */
  typedef NNearestPointsLinearInterpolateDisplacementFieldGenerator          Self;
  typedef PointSetToDisplacementFieldGenerator<TPointSet, TDisplacementField> Superclass;
  typedef itk::SmartPointer<Self>                                           Pointer;
  typedef itk::SmartPointer<const Self>                                     ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(NNearestPointsLinearInterpolateDisplacementFieldGenerator, PointSetToDisplacementFieldGenerator);

  /** Template parameters typedefs */
  typedef typename Superclass::PointSetType                PointSetType;
  typedef typename Superclass::PointSetPointerType         PointSetPointerType;
  typedef typename Superclass::DisplacementFieldType        DisplacementFieldType;
  typedef typename Superclass::DisplacementFieldPointerType DisplacementFieldPointerType;
  typedef typename Superclass::IndexType                   IndexType;
  typedef typename DisplacementFieldType::PixelType         PixelType;
  typedef typename Superclass::ValueType                   ValueType;
  typedef typename Superclass::PointType                   PointType;
  typedef typename Superclass::IndexVectorType             IndexVectorType;
  typedef typename Superclass::DistanceVectorType          DistanceVectorType;

  itkSetMacro(NumberOfPoints, unsigned int);
  itkGetMacro(NumberOfPoints, unsigned int);

protected:
  /** Constructor */
  NNearestPointsLinearInterpolateDisplacementFieldGenerator() {};
  /** Destructor */
  virtual ~NNearestPointsLinearInterpolateDisplacementFieldGenerator() {}
  /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;
  /** Main computation method */
  virtual void GenerateData();

private:
  NNearestPointsLinearInterpolateDisplacementFieldGenerator(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  unsigned int m_NumberOfPoints;
};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbNNearestPointsLinearInterpolateDisplacementFieldGenerator.txx"
#endif

#endif
