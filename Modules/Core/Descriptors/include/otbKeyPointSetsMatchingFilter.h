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

#ifndef otbKeyPointSetsMatchingFilter_h
#define otbKeyPointSetsMatchingFilter_h

#include "otbObjectListSource.h"
#include "otbLandmark.h"
#include "itkEuclideanDistanceMetric.h"

namespace otb
{
/** \class KeyPointSetsMatchingFilter
 *  \brief This class matches two point sets according to their associated data.
 *
 *   The matching criteria is that the ratio between the distance to the first nearest neighbor and the
 *   second nearest neighbor is lower than the distance threshold. The distance used can be set via the TDistance
 *   template parameters. It has to implement the Evaluate() method (see EuclideanDistanceMetric for more details).
 *
 *   By default, the algorithm tries to match points from pointset 1 to points from pointset 2. If back matching is activated,
 *   it will also try to match points from pointset 2 to points from pointset 2, and discard matches that do not appear both in
 *   forward and backward matching.
 *
 *   Matches are stored in a landmark object containing both matched points and point data. The landmark data will hold the distance value
 *   between the data.
 *
 *   \sa Landmark
 *   \sa PointSet
 *   \sa EuclideanDistanceMetric
 *
 * \ingroup OTBDescriptors
 */
template <class TPointSet, class TDistance = itk::Statistics::EuclideanDistanceMetric<typename TPointSet::PixelType>>
class ITK_EXPORT KeyPointSetsMatchingFilter
    : public ObjectListSource<ObjectList<Landmark<typename TPointSet::PointType, typename TPointSet::PixelType, double>>>
{
public:
  /// standard class typedefs
  typedef KeyPointSetsMatchingFilter Self;
  typedef ObjectListSource<ObjectList<Landmark<typename TPointSet::PointType, typename TPointSet::PixelType, double>>> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /// template typedefs
  typedef TPointSet                                      PointSetType;
  typedef typename PointSetType::Pointer                 PointSetPointerType;
  typedef typename PointSetType::PointType               PointType;
  typedef typename PointSetType::PixelType               PointDataType;
  typedef typename PointSetType::PointsContainer         PointsContainerType;
  typedef typename PointsContainerType::ConstIterator    PointsIteratorType;
  typedef typename PointSetType::PointDataContainer      PointDataContainerType;
  typedef typename PointDataContainerType::ConstIterator PointDataIteratorType;
  typedef TDistance                                      DistanceType;
  typedef typename DistanceType::Pointer                 DistancePointerType;
  typedef Landmark<typename TPointSet::PointType, typename TPointSet::PixelType, double> LandmarkType;
  typedef typename LandmarkType::Pointer     LandmarkPointerType;
  typedef ObjectList<LandmarkType>           LandmarkListType;
  typedef typename LandmarkListType::Pointer LandmarkListPointerType;
  typedef std::pair<unsigned int, double> NeighborSearchResultType;

  /// standard macros
  itkNewMacro(Self);
  itkTypeMacro(KeyPointSetsMatchingFilter, ObjectListSource);

  /// Accessors
  itkBooleanMacro(UseBackMatching);
  itkSetMacro(UseBackMatching, bool);
  itkGetMacro(UseBackMatching, bool);
  itkSetMacro(DistanceThreshold, double);
  itkGetMacro(DistanceThreshold, double);

  /// Set the first pointset
  void SetInput1(const PointSetType* pointset);
  /// Get the first pointset
  const PointSetType* GetInput1();
  /// Set the second pointset
  void SetInput2(const PointSetType* pointset);
  /// Get the second pointset
  const PointSetType* GetInput2();

protected:
  /// Constructor
  KeyPointSetsMatchingFilter();
  /// Destructor
  ~KeyPointSetsMatchingFilter() override
  {
  }
  /// PrintSelf method
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /// Generate Data
  void GenerateData() override;

  /**
   * Find the nearest neighbor of data1 in pointset.
   * \return a pair of (index, distance).
   */
  NeighborSearchResultType NearestNeighbor(const PointDataType& data1, const PointSetType* pointset);

private:
  KeyPointSetsMatchingFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  // Find back matches from 2 to 1 to validate them
  bool m_UseBackMatching;

  // Distance threshold to decide matching
  double m_DistanceThreshold;

  // Distance calculator
  DistancePointerType m_DistanceCalculator;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbKeyPointSetsMatchingFilter.hxx"
#endif
#endif
