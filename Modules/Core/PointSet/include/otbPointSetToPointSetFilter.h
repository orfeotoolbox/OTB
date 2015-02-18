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
#ifndef __otbPointSetToPointSetFilter_h
#define __otbPointSetToPointSetFilter_h

#include "otbPointSetSource.h"
#include "otbStreamingTraits.h"

namespace otb
{

/** \class PointSetToPointSetFilter
 * \brief Base class to process PointSet data
 *
 * PointSetToPointSetFilter is the base class for all process objects that output
 * PointSet data and require a PointSet as input. Specifically, this class
 * defines the SetInput() method for defining the input to a filter.
 *
 * \ingroup PointSetFilters
 */
template <class TInputPointSet, class TOutputPointSet>
class ITK_EXPORT PointSetToPointSetFilter : public PointSetSource<TOutputPointSet>
{
public:
  /** Standard class typedefs. */
  typedef PointSetToPointSetFilter        Self;
  typedef PointSetSource<TOutputPointSet> Superclass;
  typedef itk::SmartPointer<Self>         Pointer;
  typedef itk::SmartPointer<const Self>   ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(PointSetToPointSetFilter, PointSetSource);

  /** Some Image related typedefs. */
  typedef   TInputPointSet                           InputPointSetType;
  typedef   typename InputPointSetType::Pointer      InputPointSetPointer;
  typedef   typename InputPointSetType::ConstPointer InputPointSetConstPointer;

  /** Some PointSet related typedefs. */
  typedef   typename Superclass::OutputPointSetType    OutputPointSetType;
  typedef   typename Superclass::OutputPointSetPointer OutputPointSetPointer;
  typedef   typename Superclass::PointsContainerType   PointsContainerType;
  typedef   itk::ProcessObject                         ProcessObjectType;

  /** Set the input image of this process object.  */
  void SetInput(unsigned int idx, const InputPointSetType *input);
  void SetInput(const InputPointSetType *input);

  /** Get the input image of this process object.  */
  InputPointSetType * GetInput(unsigned int idx);
  InputPointSetType * GetInput();

protected:
  PointSetToPointSetFilter();
  virtual ~PointSetToPointSetFilter() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  PointSetToPointSetFilter(const PointSetToPointSetFilter &); //purposely not implemented
  void operator =(const PointSetToPointSetFilter&); //purposely not implemented

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPointSetToPointSetFilter.txx"
#endif

#endif
