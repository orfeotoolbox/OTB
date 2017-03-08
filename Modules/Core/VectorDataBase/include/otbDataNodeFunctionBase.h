/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbDataNodeFunctionBase_h
#define otbDataNodeFunctionBase_h

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
  * Subclasses must ITK_OVERRIDE Evaluate().
  *
  * This class is template over the input DataNode type and
  * the output (range) type.
  *
  *
  * \ingroup Functions
  * \sa DataNodeImageFunction
  * \sa DataNodeVectorDataFunction
 *
 * \ingroup OTBVectorDataBase
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
  OutputType Evaluate( const DataNodeType& node ) const ITK_OVERRIDE = 0;

protected:
  DataNodeFunctionBase(){};
  ~DataNodeFunctionBase() ITK_OVERRIDE{};

private:
  DataNodeFunctionBase(const Self& ); //purposely not implemented
  void operator=(const Self& ); //purposely not implemented
};

} // end namespace otb

#endif
