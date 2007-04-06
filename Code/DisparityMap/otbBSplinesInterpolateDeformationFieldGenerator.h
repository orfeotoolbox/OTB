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
#ifndef _otbBSplinesInterpolateDeformationFieldGenerator_h
#define _otbBSplinesInterpolateDeformationFieldGenerator_h

#include "otbPointSetToDeformationFieldGenerator.h"

namespace otb
{
/** \class BSplinesInterpolateDeformationFieldGenerator
 *  \brief This filters encapsulate the itk::DeformationFieldSource to produce a BSpline interpolation of the point in point set whose metric values are 
 * sufficient.
 * \sa itk::DeformationFieldSource
 * \ingroup 
 * \ingroup
 */
template <class TPointSet, class TDeformationField>
class ITK_EXPORT BSplinesInterpolateDeformationFieldGenerator
  : public PointSetToDeformationFieldGenerator<TPointSet, TDeformationField>
{
 public:
  /** Standard typedefs */
  typedef BSplinesInterpolateDeformationFieldGenerator Self;
  typedef PointSetToDeformationFieldGenerator<TPointSet,TDeformationField> Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;
  
  /** Type macro */
  itkNewMacro(Self);
  
  /** Creation through object factory macro */
  itkTypeMacro(BSplinesInterpolateDeformationFieldGenerator,PointSetToDeformationFieldGenerator);
  
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

protected:
  /** Constructor */
  BSplinesInterpolateDeformationFieldGenerator() {};
  /** Destructor */
  virtual ~BSplinesInterpolateDeformationFieldGenerator() {};
 /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;
  /** Main computation method */
  virtual void GenerateData();

private:
  BSplinesInterpolateDeformationFieldGenerator(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};
}// End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbBSplinesInterpolateDeformationFieldGenerator.txx"
#endif

#endif
