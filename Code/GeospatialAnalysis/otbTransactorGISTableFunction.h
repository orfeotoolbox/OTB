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
#ifndef __otbTransactorGISTableFunction_h
#define __otbTransactorGISTableFunction_h

#include "otbGISTableFunction.h"

namespace otb
{


/** \class TransactorGISTableFunction
 * \brief Evaluates a const transaction on a GISTable using a transactor
 *
 * TransactorGISTableFunction is a baseclass for all objects that evaluate
 * a const transaction on a GISTable using an external transactor.
 *
 *
 *
 *
 * \ingroup TransactorGISTableFunctions
 */
template <
class TInputTable,
  class TOutput, class TTransactor
>
class ITK_EXPORT TransactorGISTableFunction :
    public GISTableFunction< TInputTable,
                       TOutput >
{
public:
  /** Dimension underlying input table. */
  itkStaticConstMacro(TableDimension, unsigned int,
                      TInputTable::SpatialDimension);

  /** Standard class typedefs. */
  typedef TransactorGISTableFunction                                  Self;
  typedef GISTableFunction< TInputTable, TOutput >              Superclass;
  typedef SmartPointer<Self>                                    Pointer;
  typedef SmartPointer<const Self>                              ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(TransactorGISTableFunction, GISTableFunction);

  /** InputTableType typedef support. */
  typedef TInputTable InputTableType;
  typedef TTransactor TransactorType;

  /** Connection typedef support */
  typedef typename InputTableType::ConnectionType ConnectionType;

  /** InputTablePointer typedef support */
  typedef typename InputTableType::ConstPointer InputTableConstPointer;

  /** OutputType typedef support. */
  typedef TOutput OutputType;

  /** Evaluate the function using the transactor. */
  virtual TOutput Evaluate( ){
    m_Table->GetConnection()->PerformTransaction( TransactorType() );
    };


protected:
  TransactorGISTableFunction();
  virtual ~TransactorGISTableFunction() {}
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** Const pointer to the input Table. */
  InputTableConstPointer  m_Table;

private:
  TransactorGISTableFunction(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace otb


#endif
