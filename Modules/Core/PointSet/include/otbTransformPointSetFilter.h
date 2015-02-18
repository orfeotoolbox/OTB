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
#ifndef __otbTransformPointSetFilter_h
#define __otbTransformPointSetFilter_h

#include "otbPointSetToPointSetFilter.h"

namespace otb
{

/** \class TransformPointSetFilter
 * \brief TransformPointSetFilter applies a transform to all the
 * points of a PointSet.
 *
 * The additional content of the PointSet is left untouched.
 *
 * PointSets that have added information like normal vector on the points, will
 * have to take care of transforming this data by other means.
 *
 * \ingroup PointSetFilters
 */
template <class TInputPointSet, class TOutputPointSet, class TTransform>
class ITK_EXPORT TransformPointSetFilter :
  public PointSetToPointSetFilter<TInputPointSet, TOutputPointSet>
{
public:
  /** Standard class typedefs. */
  typedef TransformPointSetFilter                                   Self;
  typedef PointSetToPointSetFilter<TInputPointSet, TOutputPointSet> Superclass;
  typedef itk::SmartPointer<Self>                                   Pointer;
  typedef itk::SmartPointer<const Self>                             ConstPointer;

  typedef TInputPointSet                       InputPointSetType;
  typedef TOutputPointSet                      OutputPointSetType;
  typedef typename InputPointSetType::Pointer  InputPointSetPointer;
  typedef typename OutputPointSetType::Pointer OutputPointSetPointer;

  /** Type for representing coordinates. */
  typedef typename TInputPointSet::CoordRepType CoordRepType;

  /** Type of the transform. */
  typedef TTransform TransformType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(TransformPointSetFilter, PointSetToPointSetFilter);

  /** Set transform. */
  itkSetObjectMacro(Transform, TransformType);

  /** Get transform. */
  itkGetObjectMacro(Transform, TransformType);

protected:
  TransformPointSetFilter();
  virtual ~TransformPointSetFilter() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Generate Requested Data */
  virtual void GenerateData(void);

  /** Transform to apply to all the PointSet points. */
  typename TransformType::Pointer m_Transform;

private:
  TransformPointSetFilter(const TransformPointSetFilter &); //purposely not implemented
  void operator =(const TransformPointSetFilter&); //purposely not implemented

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbTransformPointSetFilter.txx"
#endif

#endif
