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
#ifndef __otbGISTableToGISTableFilter_h
#define __otbGISTableToGISTableFilter_h

#include "otbGISTableSource.h"

namespace otb
{

/** \class GISTableToGISTableFilter
   * \brief Base class for filters that take an GISTable as input and produce an GISTable as output.
   *
   * GISTableToGISTableFilter is the base class for all process objects that output
   * GISTable data and require GISTable data as input. Specifically, this class
   * defines the SetInput() method for defining the input to a filter.
   *
   *
   *
   * \ingroup GISTableFilter
 *
 * \ingroup OTBVectorDataManipulation
 */
template <class TInputGISTable, class TOutputGISTable>
class ITK_EXPORT GISTableToGISTableFilter : public otb::GISTableSource<TOutputGISTable>
{
public:
  /** Standard class typedefs. */
  typedef GISTableToGISTableFilter             Self;
  typedef otb::GISTableSource<TOutputGISTable> Superclass;
  typedef itk::SmartPointer<Self>              Pointer;
  typedef itk::SmartPointer<const Self>        ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(GISTableToGISTableFilter, GISTableSource);

  /** Some typedefs. */
  typedef TInputGISTable                        InputGISTableType;
  typedef TOutputGISTable                       OutputGISTableType;
  typedef typename TInputGISTable::ConstPointer InputGISTablePointer;
  typedef typename TOutputGISTable::Pointer     OutputGISTablePointer;

  typedef itk::DataObject::Pointer DataObjectPointer;

  virtual void SetInput(const InputGISTableType *input);
  const InputGISTableType * GetInput(void);

protected:
  /** Constructor */
  GISTableToGISTableFilter();
  /** Destructor */
  virtual ~GISTableToGISTableFilter() {}
  /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  GISTableToGISTableFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGISTableToGISTableFilter.txx"
#endif

#endif
