/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbMetaImageFunction_h
#define otbMetaImageFunction_h

#include "itkFunctionBase.h"
#include "itkPoint.h"
#include "itkVariableLengthVector.h"
#include "OTBImageBaseExport.h"

#include <vector>

namespace otb
{
/** \class MetaImageFunction
 *  \brief Concatenate results from multiple ImageFunction
 *
 *  The MetaImageFunction class allows calling multiple ImageFunction at the same location
 *  and to concatenate their result into a single VariableLengthVector.
 *
 *  In the case of ImageFunction which do not produce VariableLengthVector, one can wrap these
 *  function using the ImageFunctionAdaptor class, which will translate the image function output to
 *  a VariableLengthVector.
 *
 *  \ingroup ImageFunction
 *
 * \ingroup OTBImageBase
 */
template <class TOutputPrecision = double, class TCoordRep = double>
class OTBImageBase_EXPORT_TEMPLATE MetaImageFunction : public itk::FunctionBase<itk::Point<TCoordRep, 2>, itk::VariableLengthVector<TOutputPrecision>>
{
public:
  // Standard class typedefs
  typedef MetaImageFunction Self;
  typedef itk::FunctionBase<itk::Point<TCoordRep, 2>, itk::VariableLengthVector<TOutputPrecision>> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  // New macro
  itkNewMacro(Self);

  // RTTI typeinfo
  itkTypeMacro(MetaImageFunction, itk::FunctionBase);

  // Input and output typedef
  typedef TCoordRep CoordRepType;
  typedef itk::Point<CoordRepType, 2> PointType;
  typedef TOutputPrecision                     ValueType;
  typedef itk::VariableLengthVector<ValueType> OutputType;

  // Compatible functions typedefs
  typedef Superclass                       FunctionType;
  typedef typename FunctionType::Pointer   FunctionPointerType;
  typedef std::vector<FunctionPointerType> FunctionContainerType;

  /** Evaluate the function at the given location */
  OutputType Evaluate(const PointType& point) const override;

  /** Add a new function to the functions vector */
  void AddFunction(FunctionType* function);

  /** Add a new function the functions vector (compatibility via adapters) */
  //  template <typename T1, typename T2> void AddFunction(itk::ImageFunction<T1, T2, TCoordRep> * function);

  /** Clear functions vector */
  void ClearFunctions();

  /** Get the number of function registered */
  unsigned int GetNumberOfFunctions() const;

  /** Retrieve the nth function */
  FunctionType* GetNthFunction(unsigned int index);

  /** Remove the nth function */
  void RemoveNthFunction(unsigned int index);

protected:
  /** Constructor */
  MetaImageFunction();

  /** Destructor */
  ~MetaImageFunction() override;

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  MetaImageFunction(const Self&) = delete;
  void operator=(const Self&) = delete;

  FunctionContainerType m_FunctionContainer;
};


} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMetaImageFunction.hxx"
#endif

#endif
