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

#ifndef otbDataNodeVectorDataFunction_h
#define otbDataNodeVectorDataFunction_h

#include "otbDataNodeFunctionBase.h"
#include "otbVectorDataProperties.h"


namespace otb
{
/** \class DataNodeVectorDataFunction
  * \brief TODO
  *
  *
  *
  * \ingroup Functions
  * \sa
 *
 * \ingroup OTBVectorDataBase
 */

template <
class TOutput,
class TCoordRep = double,
class TPrecision = double
>
class ITK_EXPORT DataNodeVectorDataFunction :
    public DataNodeFunctionBase<DataNode<TCoordRep,
                                         2,
                                         TPrecision>,
                                TOutput>
{
public:
   /** Standard class typedefs. */
  typedef DataNodeVectorDataFunction               Self;
  typedef DataNodeFunctionBase<DataNode<TCoordRep,
                                        2,
                                        TPrecision>,
                               TOutput>            Superclass;
  typedef itk::SmartPointer<Self>                  Pointer;
  typedef itk::SmartPointer<const Self>            ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(DataNodeVetorDataFunction, DataNodeFunctionBase);

  /** Some typedefs. */
  typedef VectorData<TCoordRep, 2, TPrecision>    VectorDataType;
  typedef typename VectorDataType::ConstPointer   VectorDataConstPointerType;
  typedef TOutput                                 OutputType;

  /** CoordRepType typedef support. */
  typedef TCoordRep CoordRepType;

  /** PrecisionType typedef support. */
  typedef TPrecision PrecisionType;

  /** DataNode Type */
  typedef typename Superclass::DataNodeType       DataNodeType;

  /** Point Type */
  typedef typename DataNodeType::PointType        PointType;

  /** Set the input VetorData. */
  virtual void SetInputVectorData( const VectorDataType * ptr );

  /** Get the input VectorData. */
  const VectorDataType * GetInputVectorData() const
    { return m_VectorData.GetPointer(); }

  /** Evaluate the function at specified DataNode position.
   * Subclasses must provide this method. */
  TOutput Evaluate( const DataNodeType& node ) const override = 0;

protected:
  DataNodeVectorDataFunction();
  ~DataNodeVectorDataFunction() override {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /** Const pointer to the input VectorData. */
  VectorDataConstPointerType  m_VectorData;

private:
  DataNodeVectorDataFunction(const Self&) = delete;
  void operator=(const Self&) = delete;

};

}
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbDataNodeVectorDataFunction.hxx"
#endif

#endif
