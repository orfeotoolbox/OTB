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
#ifndef __otbDataNodeFunctionBase_h
#define __otbDataNodeFunctionBase_h

#include "itkFunctionBase.h"

namespace otb
{
/** \class DataNodeFunctionBase
  * \brief Base class for DataNode functions.
  *
  * DataNodeFunctionBase is the base class for DataNode function objects. Specifically,
  * the abstract method Evaluate() maps a DataNode from the input space to a element
  * in the output space.
  *
  * Subclasses must override Evaluate().
  *
  * This class is template over the input DataNode type and
  * the output (range) type.
  *
  *
  * \ingroup Functions
  * \sa DataNodeImageFunction
  * \sa DataNodeVectorDataFunction
 */

template <class TDataNode, class TOutput>
class ITK_EXPORT DataNodeFunctionBase :
    public itk::FunctionBase<TDataNode, TOutput>
{
public:
  /** Standard class typedefs. */
  typedef DataNodeFunctionBase                  Self;
  typedef itk::FunctionBase<TDataNode, TOutput> Superclass;
  typedef itk::SmartPointer<Self>               Pointer;
  typedef itk::SmartPointer<const Self>         ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(otb::DataNodeFunctionBase, itk::FunctionBase);

  /** Input type */
  typedef TDataNode  DataNodeType;

  /** Output type */
  typedef TOutput    OutputType;

  /** Evaluate at the specified input position */
  virtual OutputType Evaluate( const DataNodeType& node ) const = 0;

protected:
  DataNodeFunctionBase(){};
  ~DataNodeFunctionBase(){};

private:
  DataNodeFunctionBase(const Self& ); //purposely not implemented
  void operator=(const Self& ); //purposely not implemented
};

} // end namespace otb

#endif
