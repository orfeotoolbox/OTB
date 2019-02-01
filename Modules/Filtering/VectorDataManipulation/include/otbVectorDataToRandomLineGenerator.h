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

#ifndef otbVectorDataToRandomLineGenerator_h
#define otbVectorDataToRandomLineGenerator_h

#include "otbVectorDataSource.h"
#include "otbVectorData.h"
#include "itkPreOrderTreeIterator.h"
#include "itkMersenneTwisterRandomVariateGenerator.h"
#include <string>

namespace otb
{
/** \class VectorDataToRandomLineGenerator
 *  \brief Produces a VectorData Containing Random Lines
 *
 *  This generator produces a unique vector data containing
 *  randomly generated lines inside polygons contained in the input
 *  vectorData.
 *
 *  The user can set the range of point per line through
 *  SetMinLineSize() and SetMaxLineSize().
 *  The generated line number per polygon can be set through
 *  SetNumberOfOutputLine
 *
 *  \sa VectorDataSource
 *
 * \ingroup OTBVectorDataManipulation
 */
template <class TVectorData>
class ITK_EXPORT VectorDataToRandomLineGenerator :
    public VectorDataSource<TVectorData>
{
public:
  /** Standard class typedefs */
  typedef VectorDataToRandomLineGenerator              Self;
  typedef VectorDataSource<TVectorData>                Superclass;
  typedef itk::SmartPointer<Self>                      Pointer;
  typedef itk::SmartPointer<const Self>                ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(VectorDataToRandomLineGenerator, VectorDataSource);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  typedef TVectorData                                           VectorDataType;
  typedef typename VectorDataType::Pointer                      VectorDataPointerType;
  typedef typename VectorDataType::DataNodeType                 DataNodeType;
  typedef typename DataNodeType::PolygonType::RegionType        RegionType;
  typedef typename DataNodeType::PointType                      PointType;
  typedef typename DataNodeType::LineType                       LineType;
  typedef typename DataNodeType::LineType::VertexType           VertexType;
  typedef typename std::vector<PointType>                       PointVectorType;

  typedef itk::PreOrderTreeIterator<typename VectorDataType::DataTreeType> TreeIteratorType;

  typedef itk::Statistics::MersenneTwisterRandomVariateGenerator RandomGeneratorType;

  /** Connects the VectorData from which the extend will be extracted. */
  using Superclass::SetInput;
  void SetInput(const VectorDataType *);
  const VectorDataType * GetInput() const;

  itkGetConstMacro(NumberOfOutputLine, unsigned int);
  itkSetMacro(NumberOfOutputLine, unsigned int);

  itkGetConstMacro(MinLineSize, unsigned int);
  itkSetMacro(MinLineSize, unsigned int);

  itkGetConstMacro(MaxLineSize, unsigned int);
  itkSetMacro(MaxLineSize, unsigned int);

  /** Set the seed for random number generator */
  void SetSeed(unsigned int seed)
  {
    m_RandomGenerator->SetSeed(seed);
    m_RandomSizeGenerator->SetSeed(seed);
  }

protected:
  VectorDataToRandomLineGenerator();
  ~VectorDataToRandomLineGenerator() override {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /** Triggers the Computation of the sample list */
  void GenerateData(void) override;

  PointVectorType RandomPointsGenerator(DataNodeType * node);

private:
  VectorDataToRandomLineGenerator(const Self &) = delete;
  void operator =(const Self&) = delete;

  std::string GetNextID()
    {
      std::ostringstream oss;
      oss << m_CurrentID++;
      return oss.str();
    }

  RandomGeneratorType::Pointer m_RandomGenerator;
  RandomGeneratorType::Pointer m_RandomSizeGenerator;

  unsigned int                 m_NumberOfOutputLine;
  unsigned int                 m_MinLineSize;
  unsigned int                 m_MaxLineSize;
  unsigned int                 m_CurrentID;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorDataToRandomLineGenerator.hxx"
#endif

#endif
