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
#ifndef otbBSplinesInterpolateDisplacementFieldGenerator_h
#define otbBSplinesInterpolateDisplacementFieldGenerator_h

#include "otbPointSetToDisplacementFieldGenerator.h"

namespace otb
{
/** \class BSplinesInterpolateDisplacementFieldGenerator
 *  \brief This filters encapsulate the itk::DisplacementFieldSource to produce a BSpline interpolation of the point in point set whose metric values are
 * sufficient.
 * \sa itk::DisplacementFieldSource
 * \ingroup DisparityMap
 *
 * \ingroup OTBDisplacementField
 */
template <class TPointSet, class TDisplacementField>
class ITK_EXPORT BSplinesInterpolateDisplacementFieldGenerator
  : public PointSetToDisplacementFieldGenerator<TPointSet, TDisplacementField>
{
public:
  /** Standard typedefs */
  typedef BSplinesInterpolateDisplacementFieldGenerator                      Self;
  typedef PointSetToDisplacementFieldGenerator<TPointSet, TDisplacementField> Superclass;
  typedef itk::SmartPointer<Self>                                           Pointer;
  typedef itk::SmartPointer<const Self>                                     ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(BSplinesInterpolateDisplacementFieldGenerator, PointSetToDisplacementFieldGenerator);

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

protected:
  /** Constructor */
  BSplinesInterpolateDisplacementFieldGenerator() {};
  /** Destructor */
  ~BSplinesInterpolateDisplacementFieldGenerator() ITK_OVERRIDE {}
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;
  /** Main computation method */
  void GenerateData() ITK_OVERRIDE;

private:
  BSplinesInterpolateDisplacementFieldGenerator(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbBSplinesInterpolateDisplacementFieldGenerator.txx"
#endif

#endif
