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

#ifndef otbLabeledSampleLocalizationGenerator_h
#define otbLabeledSampleLocalizationGenerator_h

#include "otbVectorDataSource.h"
#include "otbVectorData.h"
#include "itkPreOrderTreeIterator.h"
#include "itkMersenneTwisterRandomVariateGenerator.h"
#include "itkEuclideanDistanceMetric.h"
#include <string>


namespace otb
{
/** \class LabeledSampleLocalizationGenerator
 *  \brief Produces a VectorData from potentialy multiple VectorData
 *
 *  This generator produces a unique vector data containing labeled positions
 *  extracted from inputs.
 *
 *  Input points are transmitted to the output. In addition, 'no class'
 *  points are randomly picked inside input polygons making sure
 *  they are at least at a given distance (InhibitionRadius) of every
 *  known points.
 *
 *  Classes are specified by the VectorData with a metadata field identified by
 *  a specific key. This key can be provided by the SetClassKey() method
 *  (using "Class" as a default key).
 *  The field is retrieved by GetFieldAsInt(), thus must be int-compatible
 *
 *
 * \ingroup OTBObjectDetection
 */
template <class TVectorData>
class ITK_EXPORT LabeledSampleLocalizationGenerator :
    public VectorDataSource<TVectorData>
{
public:
  /** Standard class typedefs */
  typedef LabeledSampleLocalizationGenerator           Self;
  typedef VectorDataSource<TVectorData>                Superclass;
  typedef itk::SmartPointer<Self>                      Pointer;
  typedef itk::SmartPointer<const Self>                ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(LabeledSampleLocalizationGenerator, VectorDataSource);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  typedef TVectorData                                           VectorDataType;
  typedef typename VectorDataType::Pointer                      VectorDataPointerType;
  typedef typename VectorDataType::DataNodeType                 DataNodeType;
  typedef typename DataNodeType::PolygonType::RegionType        RegionType;
  typedef typename DataNodeType::PointType                      PointType;
  typedef typename DataNodeType::PolygonType::VertexType        VertexType;
  typedef typename std::vector<PointType>                       PointVectorType;

  typedef itk::PreOrderTreeIterator<typename VectorDataType::DataTreeType> TreeIteratorType;

  typedef itk::Statistics::MersenneTwisterRandomVariateGenerator RandomGeneratorType;
  typedef itk::Statistics::EuclideanDistanceMetric<PointType>    EuclideanDistanceMetricType;

  /** Connects the VectorDatas from which the localizations are going to be extracted. */
  using Superclass::PushBackInput;
  void PushBackInput(const VectorDataType *);
  const VectorDataType * GetInput(unsigned int idx) const;

  /** Field name containing the class identifier */
  itkGetConstMacro(ClassKey, std::string);
  itkSetMacro(ClassKey, std::string);

  /** Identifier for the negative samples class */
  itkGetConstMacro(NoClassIdentifier, int);
  itkSetMacro(NoClassIdentifier, int);

  /** The density of auto-generated negative samples inside the polygons */
  itkGetConstMacro(RandomLocalizationDensity, double);
  itkSetMacro(RandomLocalizationDensity, double);

  /** The minimum distance between a generated negative sample and positive samples */
  itkGetConstMacro(InhibitionRadius, double);
  itkSetMacro(InhibitionRadius, double);

  /** The maximum iteration number during negative sample positions */
  itkGetConstMacro(NbMaxIteration, unsigned long int);
  itkSetMacro(NbMaxIteration, unsigned long int);

  /** Set the number of samples position to generate inside the inhibition radius window around a positive point */
  itkGetConstMacro(NumberOfPositiveSamplesPerPoint, unsigned int);
  itkSetMacro(NumberOfPositiveSamplesPerPoint, unsigned int);

  /** Set the seed for random number generator */
  void SetSeed(unsigned int seed)
  {
    m_RandomGenerator->SetSeed(seed);
  }

protected:
  LabeledSampleLocalizationGenerator();
  ~LabeledSampleLocalizationGenerator() override {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /** Triggers the Computation of the sample list */
  void GenerateData(void) override;

  PointVectorType RandomPointsGenerator(DataNodeType * node);

  PointVectorType PointDensification(DataNodeType * node);

private:
  LabeledSampleLocalizationGenerator(const Self &) = delete;
  void operator =(const Self&) = delete;

  std::string GetNextID()
  {
    std::ostringstream oss;
    oss << m_CurrentID++;
    return oss.str();
  }

  RandomGeneratorType::Pointer m_RandomGenerator;

  std::string               m_ClassKey;
  int                       m_NoClassIdentifier;
  double                    m_RandomLocalizationDensity;
  double                    m_InhibitionRadius;
  unsigned long int         m_NbMaxIteration;
  unsigned int              m_NumberOfPositiveSamplesPerPoint;
  unsigned int              m_CurrentID;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLabeledSampleLocalizationGenerator.hxx"
#endif

#endif
