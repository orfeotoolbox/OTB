/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbKeyPointSetsMatchingFilter_txx
#define __otbKeyPointSetsMatchingFilter_txx

#include "otbKeyPointSetsMatchingFilter.h"

namespace otb
{

template <class TPointSet, class TDistance>
KeyPointSetsMatchingFilter<TPointSet, TDistance>
::KeyPointSetsMatchingFilter()
{
  this->SetNumberOfRequiredInputs(2);
  m_UseBackMatching   = false;
  m_DistanceThreshold = 0.6;
  // Object used to measure distance
  m_DistanceCalculator = DistanceType::New();
}

template <class TPointSet, class TDistance>
const typename KeyPointSetsMatchingFilter<TPointSet, TDistance>
::PointSetType *
KeyPointSetsMatchingFilter<TPointSet, TDistance>
::GetInput1()
{
  return static_cast<const PointSetType *>(this->itk::ProcessObject::GetInput(0));
}

template <class TPointSet, class TDistance>
void
KeyPointSetsMatchingFilter<TPointSet, TDistance>
::SetInput1(const PointSetType * pointset)
{
  this->itk::ProcessObject::SetNthInput(0, const_cast<PointSetType *>(pointset));
}

template <class TPointSet, class TDistance>
const typename KeyPointSetsMatchingFilter<TPointSet, TDistance>
::PointSetType *
KeyPointSetsMatchingFilter<TPointSet, TDistance>
::GetInput2()
{
  return static_cast<const PointSetType *>(this->itk::ProcessObject::GetInput(1));
}

template <class TPointSet, class TDistance>
void
KeyPointSetsMatchingFilter<TPointSet, TDistance>
::SetInput2(const PointSetType * pointset)
{
  this->itk::ProcessObject::SetNthInput(1, const_cast<PointSetType *>(pointset));
}

template <class TPointSet, class TDistance>
void
KeyPointSetsMatchingFilter<TPointSet, TDistance>
::GenerateData()
{
//   std::cout<<"GenerateData()"<<std::endl;

  // Get the input pointers
  const PointSetType * ps1 =  this->GetInput1();
  const PointSetType * ps2 =  this->GetInput2();

  // Check if one of the pointsets is empty
  if (ps1->GetNumberOfPoints() == 0 || ps2->GetNumberOfPoints() == 0)
    {
    itkExceptionMacro(<< "Empty input pointset !");
    }

  // Get the output pointer
  LandmarkListPointerType landmarks = this->GetOutput();

  // Define iterators on points and point data.
  PointsIteratorType    pIt  = ps1->GetPoints()->Begin();
  PointDataIteratorType pdIt = ps1->GetPointData()->Begin();

  // iterate on pointset 1
  while (pdIt != ps1->GetPointData()->End()
         && pIt != ps1->GetPoints()->End())
    {
    // Get point and point data at current location
    bool          matchFound = false;
    unsigned int  currentIndex = pIt.Index();
    PointDataType data = pdIt.Value();
    PointType     point = pIt.Value();

    // These variables will hold the matched point and point data
    PointDataType dataMatch;
    PointType     pointMatch;

    // call to the matching routine
    NeighborSearchResultType searchResult1 = NearestNeighbor(data, ps2);

    // Check if the neighbor distance is lower than the threshold
    if (searchResult1.second < m_DistanceThreshold)
      {
      // Get the matched point and point data
      dataMatch = ps2->GetPointData()->GetElement(searchResult1.first);
      pointMatch = ps2->GetPoints()->GetElement(searchResult1.first);

      // If the back matching option is on
      if (m_UseBackMatching)
        {
        // Peform the back search
        NeighborSearchResultType searchResult2 = NearestNeighbor(dataMatch, ps1);

        // Test if back search finds the same match
        if (currentIndex == searchResult2.first)
          {
          matchFound = true;
          }
        }
      else // else back matching
        {
        matchFound = true;
        }
      }

    // If we found a match, add the proper landmark
    if (matchFound)
      {
      LandmarkPointerType landmark = LandmarkType::New();
      landmark->SetPoint1(point);
      landmark->SetPointData1(data);
      landmark->SetPoint2(pointMatch);
      landmark->SetPointData2(dataMatch);
      landmark->SetLandmarkData(searchResult1.second);

      // Add the new landmark to the landmark list
      landmarks->PushBack(landmark);
      }
    ++pdIt;
    ++pIt;
    }
}

template <class TPointSet, class TDistance>
typename KeyPointSetsMatchingFilter<TPointSet, TDistance>::NeighborSearchResultType
KeyPointSetsMatchingFilter<TPointSet, TDistance>
::NearestNeighbor(const PointDataType& data1, const PointSetType * pointset)
{
//   std::cout<<"Call to NearestNeighbor()"<<std::endl;
// Declare the result
  NeighborSearchResultType result;

  // Define iterators on points and point data.
  PointDataIteratorType pdIt = pointset->GetPointData()->Begin();

  // local variables
  unsigned int nearestIndex = 0;
  double       d1 = m_DistanceCalculator->Evaluate(data1, pdIt.Value());
  ++pdIt;
  double d2 = m_DistanceCalculator->Evaluate(data1, pdIt.Value());
  ++pdIt;

  if (d1 > d2)
    {
    nearestIndex = 1;
    }
  // Initialize distances
  double nearestDistance = std::min(d1, d2);
  double secondNearestDistance = std::max(d1, d2);
  double distanceValue;

  // iterate on the pointset
  while (pdIt != pointset->GetPointData()->End())
    {
    // Evaluate the distance
    distanceValue = m_DistanceCalculator->Evaluate(data1, pdIt.Value());

//       std::cout<<nearestIndex<<" "<<nearestDistance<<" "<<secondNearestDistance<<std::endl;

    // Check if this point is the nearest neighbor
    if (distanceValue < nearestDistance)
      {
      secondNearestDistance = nearestDistance;
      nearestDistance = distanceValue;
      nearestIndex = pdIt.Index();

      }
    // Else check if it is the second nearest neighbor
    else if (distanceValue < secondNearestDistance)
      {
      secondNearestDistance = distanceValue;
      }
    ++pdIt;
    }

  // Fill results
  result.first = nearestIndex;
  if (secondNearestDistance == 0)
    {
    result.second = 1;
    }
  else
    {
    result.second = nearestDistance / secondNearestDistance;
    }

  // return the result
  return result;

}

template <class TPointSet, class TDistance>
void
KeyPointSetsMatchingFilter<TPointSet, TDistance>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
