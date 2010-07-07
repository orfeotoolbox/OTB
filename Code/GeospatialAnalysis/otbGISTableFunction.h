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
#ifndef __otbGISTableFunction_h
#define __otbGISTableFunction_h

#include "itkFunctionBase.h"
#include "otbGISTable.h"

namespace otb
{

/** \class GISTableFunction
 * \brief Evaluates a const transaction on a GISTable
 *
 * GISTableFunction is a baseclass for all objects that evaluate
 * a const transaction on a GISTable.
 *
 * The input table is set via method SetInputTable().
 *
 *
 * \sa GISTable
 * \sa GISConnection
 *
 * \ingroup GISTableFunctions
 */
template <
    class TInputTable,
    class TOutput
    >
class ITK_EXPORT GISTableFunction :
  public FunctionBase<TInputTable,
      TOutput>
{
public:
  /** Dimension underlying input table. */
  itkStaticConstMacro(TableDimension, unsigned int,
                      TInputTable::TSpatialDimension);

  /** Standard class typedefs. */
  typedef GISTableFunction Self;
  typedef FunctionBase<
      TInputTable,
      TOutput>                                                   Superclass;
  typedef SmartPointer<Self>       Pointer;
  typedef SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(GISTableFunction, FunctionBase);

  /** InputTableType typedef support. */
  typedef TInputTable InputTableType;

  /** Connection typedef support */
  typedef typename InputTableType::ConnectionType ConnectionType;

  /** InputTablePointer typedef support */
  typedef typename InputTableType::ConstPointer InputTableConstPointer;

  /** OutputType typedef support. */
  typedef TOutput OutputType;

  /** Set the input table. */
  virtual void SetInputTable(const InputTableType* ptr);

  /** Get the input Table. */
  const InputTableType * GetInputTable() const
  { return m_Table.GetPointer(); }

  /** Evaluate the function.
   * Subclasses must provide this method. */
  virtual TOutput Evaluate() const = 0;

protected:
  GISTableFunction();
  virtual ~GISTableFunction() {}
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** Const pointer to the input table. */
  InputTableConstPointer m_Table;

private:
  GISTableFunction(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#if OTB_MANUAL_INSTANTIATION
# include "itkGISTableFunction.txx"
#endif

#endif
