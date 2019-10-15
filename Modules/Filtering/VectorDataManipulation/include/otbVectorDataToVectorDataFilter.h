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

#ifndef otbVectorDataToVectorDataFilter_h
#define otbVectorDataToVectorDataFilter_h

#include "otbVectorDataSource.h"

namespace otb
{

/** \class VectorDataToVectorDataFilter
   * \brief Base class for filters that take an VectorData as input and produce an VectorData as output.
   *
   * VectorDataToVectorDataFilter is the base class for all process objects that output
   * VectorData data and require VectorData data as input. Specifically, this class
   * defines the SetInput() method for defining the input to a filter.
   *
   *
   *
   * \ingroup VectorDataFilter
 *
 * \ingroup OTBVectorDataManipulation
 */
template <class TInputVectorData, class TOutputVectorData>
class ITK_EXPORT VectorDataToVectorDataFilter : public VectorDataSource<TOutputVectorData>
{
public:
  /** Standard class typedefs. */
  typedef VectorDataToVectorDataFilter        Self;
  typedef VectorDataSource<TOutputVectorData> Superclass;
  typedef itk::SmartPointer<Self>             Pointer;
  typedef itk::SmartPointer<const Self>       ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(VectorDataToVectorDataFilter, VectorDataSource);

  /** Some typedefs. */
  typedef TInputVectorData                        InputVectorDataType;
  typedef TOutputVectorData                       OutputVectorDataType;
  typedef typename TInputVectorData::ConstPointer InputVectorDataPointer;
  typedef typename TOutputVectorData::Pointer     OutputVectorDataPointer;

  typedef typename InputVectorDataType::DataNodeType                InputDataNodeType;
  typedef typename OutputVectorDataType::DataNodeType               OutputDataNodeType;
  typedef typename InputVectorDataType::DataTreeType::TreeNodeType  InputInternalTreeNodeType;
  typedef typename OutputVectorDataType::DataTreeType::TreeNodeType OutputInternalTreeNodeType;

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

  typedef itk::DataObject::Pointer DataObjectPointer;

  using Superclass::SetInput;
  virtual void SetInput(const InputVectorDataType* input);
  const InputVectorDataType* GetInput(void);

protected:
  /** Constructor */
  VectorDataToVectorDataFilter();
  /** Destructor */
  ~VectorDataToVectorDataFilter() override
  {
  }

  virtual OutputPointType ProcessPoint(InputPointType itkNotUsed(point)) const
  {
    itkExceptionMacro(<< "Subclass should reimplement this method");
  }
  virtual OutputLinePointerType ProcessLine(InputLinePointerType itkNotUsed(line)) const
  {
    itkExceptionMacro(<< "Subclass should reimplement this method");
  }
  virtual OutputPolygonPointerType ProcessPolygon(InputPolygonPointerType itkNotUsed(polygon)) const
  {
    itkExceptionMacro(<< "Subclass should reimplement this method");
  }
  virtual OutputPolygonListPointerType ProcessPolygonList(InputPolygonListPointerType itkNotUsed(polygonList)) const
  {
    itkExceptionMacro(<< "Subclass should reimplement this method");
  }

  void GenerateOutputInformation(void) override;
  void GenerateData(void) override;

  /** Go through the vector data tree and process the nodes */
  virtual void ProcessNode(InputInternalTreeNodeType* source, OutputInternalTreeNodeType* destination) const;

  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  VectorDataToVectorDataFilter(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorDataToVectorDataFilter.hxx"
#endif

#endif
