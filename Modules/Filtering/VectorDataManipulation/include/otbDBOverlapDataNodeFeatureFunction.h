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

#ifndef otbDBOverlapDataNodeFeatureFunction_h
#define otbDBOverlapDataNodeFeatureFunction_h

#include "otbDataNodeVectorDataFunction.h"

#include "itkPreOrderTreeIterator.h"

namespace otb
{
/** \class DBOverlapDataNodeFeatureFunction
  * \brief
  *
  *
  * \ingroup Functions
  * \sa DataNodeVectorDataFunction
 *
 * \ingroup OTBVectorDataManipulation
  */

template <
class TCoordRep = double,
class TPrecision = double
>
class ITK_EXPORT DBOverlapDataNodeFeatureFunction :
    public DataNodeVectorDataFunction<std::vector<TPrecision>, TCoordRep, TPrecision>
{
public:
  /** Standard class typedefs. */
  typedef DBOverlapDataNodeFeatureFunction                    Self;
  typedef DataNodeVectorDataFunction<std::vector<TPrecision>, TCoordRep, TPrecision>
                                                              Superclass;
  typedef itk::SmartPointer<Self>                             Pointer;
  typedef itk::SmartPointer<const Self>                       ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(DBOverlapDataNodeFeatureFunction, DataNodeVectorDataFunction);

  /** Some typedefs. */
  typedef typename Superclass::DataNodeType           DataNodeType;
  typedef typename DataNodeType::PointType            PointType;
  typedef typename DataNodeType::LineType             LineType;
  typedef typename LineType::VertexType               VertexType;
  typedef typename DataNodeType::PolygonType          PolygonType;

  typedef typename Superclass::VectorDataType         VectorDataType;
  typedef typename VectorDataType::DataTreeType       DataTreeType;
  typedef itk::PreOrderTreeIterator<typename VectorDataType::DataTreeType>
                                                      TreeIteratorType;

  typedef TCoordRep                                   CoordRepType;

  typedef TPrecision                                  PrecisionType;

  typedef std::vector<PrecisionType>                  OutputType;

  OutputType Evaluate( const DataNodeType& node ) const override;

  /** Set/Get methods */
  itkGetConstMacro(DistanceThreshold, PrecisionType);
  itkSetMacro(DistanceThreshold, PrecisionType);

  /** Method to compute the distance of a point to a segment */
  double ComputeEuclideanDistanceMetricToSegment(VertexType q1, VertexType q2, VertexType p) const;

protected:
  DBOverlapDataNodeFeatureFunction();
  ~DBOverlapDataNodeFeatureFunction() override {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  DBOverlapDataNodeFeatureFunction(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** Road / Building distance Threshold */
  PrecisionType               m_DistanceThreshold;
};

}
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbDBOverlapDataNodeFeatureFunction.hxx"
#endif

#endif
