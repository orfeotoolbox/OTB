/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbLabelMapToVectorDataFilter_h
#define otbLabelMapToVectorDataFilter_h

#include "otbVectorDataSource.h"
#include "itkLabelMap.h"
#include "otbLabelObjectFieldsFunctor.h"
#include "otbLabelObjectToPolygonFunctor.h"
#include "otbCorrectPolygonFunctor.h"
#include "otbDataNode.h"

#include <string>
#include <sstream>

namespace otb
{

/** \class LabelMapToVectorDataFilter
   * \brief This class vectorizes a LabelObject to a VectorData.
   *
   * LabelMapToVectorDataFilter converts a LabelMap to a
   * VectorData where all the pixels get the attribute value of the label object they belong.
   * It uses the class otbLabelObjectToPolygonFunctor which follows a finite states machine described in
   *
   * "An algorithm for the rapid computation of boundaries of run-length
   * encoded regions", Francis K. H. Queck, in Pattern Recognition 33
   * (2000), p 1637-1649.
   *
   * Note : the output vector data contains index coordinates (not physical coordinates).
   * For instance, a polygon around the pixel at index (0,0) would have the following
   * points : [-0.5,-0.5] , [0.5,-0.5] , [0.5,0.5] , [-0.5,0.5]
   *
   * \sa VectorDataSource
 * \ingroup GeoSpatialAnalysis
 *
 * \ingroup OTBConversion
 */
template <class TLabelMap, class TVectorData, class TFieldsFunctor = Functor::LabelObjectFieldsFunctor<typename TLabelMap::LabelObjectType>>
class ITK_EXPORT LabelMapToVectorDataFilter : public VectorDataSource<TVectorData>
{
public:
  /** Standard class typedefs. */
  typedef LabelMapToVectorDataFilter    Self;
  typedef VectorDataSource<TVectorData> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Some convenient typedefs. */
  typedef TLabelMap      InputLabelMapType;
  typedef TVectorData    OutputVectorDataType;
  typedef TFieldsFunctor FieldsFunctorType;

  typedef typename OutputVectorDataType::Pointer      OutputVectorDataPointer;
  typedef typename OutputVectorDataType::ConstPointer OutputVectorDataConstPointer;

  typedef typename InputLabelMapType::LabelObjectType LabelObjectType;

  typedef typename InputLabelMapType::ConstIterator   ConstIteratorType;
  typedef typename OutputVectorDataType::DataNodeType DataNodeType;
  typedef typename DataNodeType::Pointer              DataNodePointerType;
  typedef typename DataNodeType::PolygonType          PolygonType;
  typedef typename PolygonType::Pointer               PolygonPointerType;

  /** Some typedefs specific to functors*/
  typedef otb::Functor::LabelObjectToPolygonFunctor<LabelObjectType, PolygonType> FunctorType;
  typedef otb::CorrectPolygonFunctor<PolygonType> CorrectFunctorType;

  /** Standard New method. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(LabelMapToVectorDataFilter, VectorDataSource);

  /** Set/Get the LabelMap input of this process object.  */
  using Superclass::SetInput;
  virtual void SetInput(const InputLabelMapType* input);
  virtual void SetInput(unsigned int idx, const InputLabelMapType* input);
  const InputLabelMapType* GetInput(void);
  const InputLabelMapType* GetInput(unsigned int idx);

  /* Set the functor used to provide additional OGR fields */
  void SetFieldsFunctor(const FieldsFunctorType& functor)
  {
    m_FieldsFunctor = functor;
    this->Modified();
  }

  /* Get the functor used to provide additional OGR fields */
  FieldsFunctorType& GetFieldsFunctor()
  {
    return m_FieldsFunctor;
  }

protected:
  LabelMapToVectorDataFilter();
  ~LabelMapToVectorDataFilter() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;
  void GenerateData() override;

private:
  LabelMapToVectorDataFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  FieldsFunctorType m_FieldsFunctor;
}; // end of class

} // end namespace itk

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLabelMapToVectorDataFilter.hxx"
#endif

#endif
