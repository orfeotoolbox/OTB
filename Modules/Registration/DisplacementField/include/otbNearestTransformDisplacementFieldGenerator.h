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
#ifndef __otbNearestTransformDisplacementFieldGenerator_h
#define __otbNearestTransformDisplacementFieldGenerator_h

#include "otbPointSetWithTransformToDisplacementFieldGenerator.h"

namespace otb
{
/** \class NearestTransformDisplacementFieldGenerator
 *  \brief This class implements displacement field generation by taking into account the local transform of the
 *  nearest point in pointset.
 *
 * \ingroup DisparityMap
 */
template <class TPointSet, class TDisplacementField>
class ITK_EXPORT NearestTransformDisplacementFieldGenerator
  : public PointSetWithTransformToDisplacementFieldGenerator<TPointSet, TDisplacementField>
{
public:
  /** Standard typedefs */
  typedef NearestTransformDisplacementFieldGenerator                                      Self;
  typedef PointSetWithTransformToDisplacementFieldGenerator<TPointSet, TDisplacementField> Superclass;
  typedef itk::SmartPointer<Self>                                                        Pointer;
  typedef itk::SmartPointer<const Self>                                                  ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(NearestTransformDisplacementFieldGenerator, PointSetWithTransformToDisplacementFieldGenerator);

  /** Template parameters typedefs */
  typedef typename Superclass::PointSetType                PointSetType;
  typedef typename Superclass::PointSetPointerType         PointSetPointerType;
  typedef typename Superclass::DisplacementFieldType        DisplacementFieldType;
  typedef typename Superclass::DisplacementFieldPointerType DisplacementFieldPointerType;
  typedef typename Superclass::IndexType                   IndexType;
  typedef typename Superclass::PointType                   PointType;
  typedef typename DisplacementFieldType::PixelType         PixelType;
  typedef typename Superclass::ValueType                   ValueType;
  typedef typename Superclass::IndexVectorType             IndexVectorType;
  typedef typename Superclass::TransformType               TransformType;
  typedef typename TransformType::ParametersType           ParametersType;

protected:
  /** Constructor */
  NearestTransformDisplacementFieldGenerator() {};
  /** Destructor */
  virtual ~NearestTransformDisplacementFieldGenerator() {}
  /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;
  /** Main computation method */
  virtual void GenerateData();

private:
  NearestTransformDisplacementFieldGenerator(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbNearestTransformDisplacementFieldGenerator.txx"
#endif

#endif
