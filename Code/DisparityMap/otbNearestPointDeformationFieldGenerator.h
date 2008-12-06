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
#ifndef __otbNearestPointDeformationFieldGenerator_h
#define __otbNearestPointDeformationFieldGenerator_h

#include "otbPointSetToDeformationFieldGenerator.h"

namespace otb
{
/** \class NearestPointDeformationFieldGenerator
 *  \brief This class implements the simplest way to deformation field generation using a point set.
 *  In each pixel location, the deformation is considered to be the deformation of the nearest seed point in the point set.
 *
 * \ingroup DisparityMap
 */
template <class TPointSet, class TDeformationField>
class ITK_EXPORT NearestPointDeformationFieldGenerator
  : public PointSetToDeformationFieldGenerator<TPointSet, TDeformationField>
{
 public:
  /** Standard typedefs */
  typedef NearestPointDeformationFieldGenerator Self;
  typedef PointSetToDeformationFieldGenerator<TPointSet,TDeformationField> Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(NearestPointDeformationFieldGenerator,PointSetToDeformationFieldGenerator);

  /** Template parameters typedefs */
  typedef typename Superclass::PointSetType PointSetType;
  typedef typename Superclass::PointSetPointerType PointSetPointerType;
  typedef typename Superclass::DeformationFieldType DeformationFieldType;
  typedef typename Superclass::DeformationFieldPointerType DeformationFieldPointerType;
  typedef typename Superclass::IndexType IndexType;
  typedef typename DeformationFieldType::PixelType PixelType;
  typedef typename Superclass::ValueType ValueType;
  typedef typename Superclass::IndexVectorType IndexVectorType;

protected:
  /** Constructor */
  NearestPointDeformationFieldGenerator() {};
  /** Destructor */
  virtual ~NearestPointDeformationFieldGenerator() {};
 /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;
  /** Main computation method */
  virtual void GenerateData();

private:
  NearestPointDeformationFieldGenerator(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};
}// End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbNearestPointDeformationFieldGenerator.txx"
#endif

#endif
