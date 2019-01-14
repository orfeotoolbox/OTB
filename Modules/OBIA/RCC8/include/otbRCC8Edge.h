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

#ifndef otbRCC8Edge_h
#define otbRCC8Edge_h

#include "itkDataObject.h"
#include "itkObjectFactory.h"
#include "otbRCC8Value.h"

namespace otb
{
/** \class RCC8Edge
 *  \brief Base class to represent an edge in a RCC8 Graph.
 *
 * \sa RCC8Graph, RCC8VertexBase
 *
 * \ingroup OTBRCC8
 */
class ITK_EXPORT RCC8Edge : public itk::DataObject
{
public:
  /** Standard class typedefs */
  typedef RCC8Edge                      Self;
  typedef itk::DataObject               Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  /** Run-time type information (and related methods). */
  itkTypeMacro(RCC8Edge, DataObject);
  /** RCC8 values typedef */
  typedef RCC8Value RCC8ValueType;
  /** RCC8 relation value accessors */
  itkGetMacro(Value, RCC8ValueType);
  itkSetMacro(Value, RCC8ValueType);

protected:
  /** Constructor */
  RCC8Edge();
  /** Desctructor */
  ~RCC8Edge() override {}
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  /** The RCC8 value */
  RCC8ValueType m_Value;
};
} // end namespace otb
#endif
