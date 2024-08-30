/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbVectorDataAdapter_h
#define otbVectorDataAdapter_h

#include "otbVectorDataToVectorDataFilter.h"

namespace otb
{

/** \class VectorDataAdapter
  * \brief Helper class to convert the vector data to generic type
  *
 *
 * \ingroup OTBVectorDataManipulation
  */

template <class TInputVectorData, class TOutputVectorData>
class ITK_EXPORT VectorDataAdapter : public otb::VectorDataToVectorDataFilter<TInputVectorData, TOutputVectorData>
{

public:
  /** Standard class typedefs. */
  typedef VectorDataAdapter Self;
  typedef otb::VectorDataToVectorDataFilter<TInputVectorData, TOutputVectorData> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(VectorDataAdapter, VectorDataToVectorDataFilter);

  typedef TInputVectorData  InputVectorDataType;
  typedef TOutputVectorData OutputVectorDataType;

  typedef typename OutputVectorDataType::DataNodeType OutputDataNodeType;
  typedef typename InputVectorDataType::DataNodeType  InputDataNodeType;

  typedef typename InputDataNodeType::PointType       InputPointType;
  typedef typename InputDataNodeType::LineType        InputLineType;
  typedef typename InputDataNodeType::PolygonType     InputPolygonType;
  typedef typename InputDataNodeType::PolygonListType InputPolygonListType;

  typedef typename InputLineType::Pointer        InputLinePointerType;
  typedef typename InputPolygonType::Pointer     InputPolygonPointerType;
  typedef typename InputPolygonListType::Pointer InputPolygonListPointerType;

  typedef typename OutputDataNodeType::PointType       OutputPointType;
  typedef typename OutputDataNodeType::LineType        OutputLineType;
  typedef typename OutputDataNodeType::PolygonType     OutputPolygonType;
  typedef typename OutputDataNodeType::PolygonListType OutputPolygonListType;

  typedef typename OutputLineType::Pointer        OutputLinePointerType;
  typedef typename OutputPolygonType::Pointer     OutputPolygonPointerType;
  typedef typename OutputPolygonListType::Pointer OutputPolygonListPointerType;


protected:
  VectorDataAdapter(){};
  ~VectorDataAdapter() override
  {
  }

  OutputPointType ProcessPoint(InputPointType point) const override;
  OutputLinePointerType ProcessLine(InputLinePointerType line) const override;
  OutputPolygonPointerType ProcessPolygon(InputPolygonPointerType polygon) const override;
  OutputPolygonListPointerType ProcessPolygonList(InputPolygonListPointerType polygonList) const override;

private:
  VectorDataAdapter(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorDataAdapter.hxx"
#endif

#endif
