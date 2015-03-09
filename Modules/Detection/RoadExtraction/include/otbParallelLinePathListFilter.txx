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
#ifndef __otbParallelLinePathListFilter_txx
#define __otbParallelLinePathListFilter_txx

#include "otbParallelLinePathListFilter.h"
#include "otbMath.h"

namespace otb
{
/**
 * Constructor
 */
template <class TPath>
ParallelLinePathListFilter<TPath>
::ParallelLinePathListFilter()
{
  m_DistanceThreshold = 100.;
  m_CommonDistanceThreshold = 10.;
  m_AngularThreshold = 3.14;
}

template <class TPath>
void
ParallelLinePathListFilter<TPath>
::GenerateData()
{
  // IO
  const PathListType * inputPtr  = this->GetInput();
  PathListType *       outputPtr = this->GetOutput();

  // Input iterator
  IteratorType inputIt = inputPtr->Begin();

  std::vector<bool>         eraseFlagVector1;
  std::vector<unsigned int> parallelLineIndex;

  // First we copy all input path to output
  while (inputIt != inputPtr->End())
    {
    // Write the next input path always to the
    // last element of the list. This last element is
    // newly created.
    outputPtr->PushBack(inputIt.Get());

    // If a new element is copied to the output vector,
    // "false" is written to the corresponding index of
    // the vector eraseFlagVector.
    eraseFlagVector1.push_back(false);

    ++inputIt;
    }

  unsigned int VectorSize = static_cast<unsigned int>(eraseFlagVector1.size());
  //std::cout<<" Number of Lines: "<< VectorSize <<std::endl;

  IteratorType outputIt1 = outputPtr->Begin();
  unsigned int index1 = 0;
  unsigned int firstLineCounter = 0, parallelLineCounter = 0;
  unsigned int commonDistCounter = 0, maxDistCounter = 0;

// Iterate through all lines of the pathlist
  while (firstLineCounter < VectorSize)
    {
    if (!eraseFlagVector1[index1])
      {
      IteratorType outputIt2 = outputIt1;
      ++outputIt2;
      unsigned int index2 = index1 + 1;

      // Check if any of the following lines are parallel
      while (outputIt2 != outputPtr->End())
        {

        if (!eraseFlagVector1[index2])
          { // Read the first and the last vertex of each line pair that is checked
          VertexIteratorType vSourceIt = outputIt1.Get()->GetVertexList()->Begin();
          VertexType         v1 = vSourceIt.Value();
          vSourceIt = outputIt1.Get()->GetVertexList()->End();
          --vSourceIt;
          VertexType v2 = vSourceIt.Value();

          VertexIteratorType vTargetIt = outputIt2.Get()->GetVertexList()->Begin();
          VertexType         v3 = vTargetIt.Value();
          vTargetIt = outputIt2.Get()->GetVertexList()->End();
          --vTargetIt;
          VertexType v4 = vTargetIt.Value();

          // Check for parallel lines
          if (VerifyAngularCondition(v1, v2, v3, v4))
            {
            ++parallelLineCounter;
            if (VerifyMaxDistanceCondition(v1, v2, v3, v4))
              {
              ++maxDistCounter;
              if (VerifyCommonDistanceCondition(v1, v2, v3, v4))
                {
                ++commonDistCounter;
                // Write index of first parallel path
                parallelLineIndex.push_back(index1);

                // Write index of second parallel path
                parallelLineIndex.push_back(index2);
                }
              }
            }
          }

        ++index2;
        ++outputIt2;
        }
      }

    // mark the old path as erased
    eraseFlagVector1[index1] = true;
    ++firstLineCounter;
    ++index1;
    ++outputIt1;
    } // end of for loop

  otbMsgDevMacro( << "Number of line pairs that pass the angular condition:          " << parallelLineCounter );
  otbMsgDevMacro( << "Number of line pairs that pass the maximum distance condition: " << maxDistCounter );
  otbMsgDevMacro( << "Number of line pairs that pass the common distance condition:  " << commonDistCounter  );

  // Write all parallel lines and set the non-erase flagg.
  typename std::vector<unsigned int>::iterator lineIt1 = parallelLineIndex.begin();
  unsigned int                                 sortLineIndex = 0;
  while (lineIt1 != parallelLineIndex.end())
    {
    IteratorType outputIt3 = outputPtr->Begin() + parallelLineIndex[sortLineIndex];

    PathPointerType newPath1 = this->WriteParallelPath(outputIt3.Get());
    outputPtr->PushBack(newPath1);
    // add a non-erase flag for the new path
    eraseFlagVector1.push_back(false);
    ++sortLineIndex;
    otbMsgDevMacro( << "Number of lines written in the path list: " << sortLineIndex );
    ++lineIt1;
    }

  // Search ended, now removing the erased path from the first line pointer
  typename std::vector<bool>::reverse_iterator it1 = eraseFlagVector1.rbegin();
  index1 = eraseFlagVector1.size() - 1;
  while (it1 != eraseFlagVector1.rend())
    {
    if (eraseFlagVector1[index1])
      {
      outputPtr->Erase(index1);
      }

    --index1;
    ++it1;
    }

}
/**
 * Verify the angular condition to find parallel lines.
 * This function verifies if two lines are parallel by
 * computing the angle in relation to the y-axis.
 * First line segment: v1 is the first vertex, v2 the second one
 * (not necessarily the path ending).
 * Second line segment: v3 is the first vertex, v4 the second one
 * (not necessarily the path ending).
 * Return true if the condition is verified.
 *
 * This is the first criteria to be fullfilled.
 **/
template <class TPath>
bool
ParallelLinePathListFilter<TPath>
::VerifyAngularCondition(VertexType v1, VertexType v2, VertexType v3, VertexType v4)
{
  double alpha1 = vcl_atan2((v2[1] - v1[1]), (v2[0] - v1[0]));
  double alpha2 = vcl_atan2((v4[1] - v3[1]), (v4[0] - v3[0]));

  alpha1 = (alpha1 >= 0) ? alpha1 : (alpha1 + CONST_PI);
  alpha2 = (alpha2 >= 0) ? alpha2 : (alpha2 + CONST_PI);

  // Return true if the angle between the two lines is smaller than
  // the specified threshold.
  bool angle = (vcl_abs(alpha1 - alpha2) < static_cast<double>(m_AngularThreshold));

  return angle;
}

/**
 * Verify the maximum distance condition to find parallel lines.
 * The orthogonal distance between two parallel lines is calculated.
 * First line segment: v1 is the first vertex, v2 the second one
 * (not necessarily the path ending).
 * Second line segment: v3 is the first vertex, v4 the second one
 * (not necessarily the path ending).
 * Return true if the condition is verified.
 *
 * This is the second criteria to be fullfilled.
 **/
template <class TPath>
bool
ParallelLinePathListFilter<TPath>
::VerifyMaxDistanceCondition(VertexType v1, VertexType v2, VertexType v3, VertexType itkNotUsed(v4))
{
  // Compute the direction vector of the first line
  VertexType vectorDir12;

  vectorDir12[0] = v2[0] - v1[0];
  vectorDir12[1] = v2[1] - v1[1];

  // Compute the orthogonal distance between the two parallel lines
  // with equation d = |(v3 - v1)X(v2 - v1)|/|v2 - v1|
  double distance = 0., denominator = 0., nominator = 0.;
  denominator = vcl_abs((v3[0] - v1[0]) * vectorDir12[1] - (v3[1] - v1[1]) * vectorDir12[0]);
  nominator = sqrt(pow(vectorDir12[0], 2) + pow(vectorDir12[1], 2));
  distance = denominator / nominator;
  //std::cout<< "Distance between two parallel lines: " << distance <<std::endl;

  // Check if the orthogonal distance between the lines
  // is beneath the chosen threshold.
  bool dist = (distance <= static_cast<double>(m_DistanceThreshold));

  return dist;
}

/**
 * Verify the common distance condition to find parallel lines.
 * The overlapping part of the parallel lines is computed. In case
 * no overlapping part exists or the value is below the specified
 * threshold, false is returned.
 * First line segment: v1 is the first vertex, v2 the second one
 * (not necessarily the path ending).
 * Second line segment: v3 is the first vertex, v4 the second one
 * (not necessarily the path ending).
 * Return true if the condition is verified.
 *
 * This is the third criteria to be fullfilled.
 **/
template <class TPath>
bool
ParallelLinePathListFilter<TPath>
::VerifyCommonDistanceCondition(VertexType v1, VertexType v2, VertexType v3, VertexType v4)
{
// Compute the length of each line
  double length12 = 0.;
  // double length34 = 0.;
  length12 = sqrt(pow((v2[0] - v1[0]), 2) + pow((v2[1] - v1[1]), 2));
  //length34 = sqrt(pow((v4[0] - v3[0]), 2) + pow((v4[1] - v3[1]), 2));

  // Set v1[0] to zero and align the y-axis of
  // the new coordinate system with line one (v1 and v2).
  // Compute the coordinates of the first and of the
  // second line in the new coordinate system.
  VertexType tempv1, tempv2, tempv3, tempv4;

  //Initialization
  tempv1.Fill(0.);
  tempv2.Fill(0.);
  tempv3.Fill(0.);
  tempv4.Fill(0.);

  if (v1[1] == v2[1])
    {
    if (v1[0] < v2[0])
      {
      tempv1[0] = 0., tempv1[1] = 0.;
      tempv2[0] = 0., tempv2[1] = length12;
      tempv3[0] = v3[0] - v1[0], tempv3[1] = v3[1] - v1[1];
      tempv4[0] = v4[0] - v1[0], tempv4[1] = v4[1] - v1[1];
      }
    else
      {
      tempv2[0] = 0., tempv2[1] = 0.;
      tempv1[0] = 0., tempv1[1] = length12;
      tempv3[0] = v3[0] - v2[0], tempv3[1] = v3[1] - v2[1];
      tempv4[0] = v4[0] - v2[0], tempv4[1] = v4[1] - v2[1];
      }
    }
  // Check the direction of the line (vector).
  // The origin of the new coordinate system is
  // set to the smaller of the two vertices.
  // Then, a rotation and translation of the vertices
  // of the second line (v3 and v4) to the new coordinate
  // system is performed.

  VertexType temptransv3, temptransv4;

  if (v1[1] < v2[1])
    {
    if (v1[0] == v2[0])
      {
      tempv1[0] = 0., tempv1[1] = 0.;
      tempv2[0] = 0., tempv2[1] = length12;
      tempv3[0] = v3[0] - v1[0], tempv3[1] = v3[1] - v1[1];
      tempv4[0] = v4[0] - v1[0], tempv4[1] = v4[1] - v1[1];
      }

    else
      {
      // Coordinates of the first line in the new coordinate system
      tempv1[0] = 0.;
      tempv1[1] = 0.;
      tempv2[0] = 0.;
      tempv2[1] = length12;

      // Rotate the system clockwise
      double sinealpha;
      if (v2[0] > v1[0])
        {
        sinealpha = (v2[0] - v1[0]) / length12;
        }
      else
        {
        sinealpha = (v1[0] - v2[0]) / length12;
        }
      double alpha1 = vcl_asin(sinealpha);

      // Translation
      temptransv3[0] = v3[0] - v1[0];
      temptransv3[1] = v3[1] - v1[1];
      temptransv4[0] = v4[0] - v1[0];
      temptransv4[1] = v4[1] - v1[1];

      // Rotation
      tempv3[0] = temptransv3[0] * cos(alpha1) + temptransv3[1] * sin(alpha1);
      tempv3[1] = temptransv3[1] * cos(alpha1) - temptransv3[0] * sin(alpha1);
      tempv4[0] = temptransv4[0] * cos(alpha1) + temptransv4[1] * sin(alpha1);
      tempv4[1] = temptransv4[1] * cos(alpha1) - temptransv4[0] * sin(alpha1);

      otbMsgDevMacro(<< "tempv1[0], tempv1[1], tempv2[0], tempv2[1]: " );
      otbMsgDevMacro(<< tempv1[0] << ", " << tempv1[1] << ", " << tempv2[0] << ", " << tempv2[1] );
      otbMsgDevMacro(<< "Alpha: " << alpha1 );
      otbMsgDevMacro(<< "tempv3[0], tempv3[1], tempv4[0], tempv4[1]: "
                       << tempv3[0] << ", " << tempv3[1] << ", "
                       << tempv4[0] << ", " << tempv4[1] );
      otbMsgDevMacro(<< "Calculated length of the second line: "
                        << sqrt(pow((tempv4[0] - tempv3[0]), 2) + pow((tempv4[1] - tempv3[1]), 2)) );
      otbMsgDevMacro(<< "Original length of line 1:       " << length12 );
      }
    }

  if (v2[1] < v1[1])
    {

    if (v1[0] == v2[0])
      {
      tempv2[0] = 0., tempv2[1] = 0.;
      tempv1[0] = 0., tempv1[1] = length12;
      tempv3[0] = v3[0] - v2[0], tempv3[1] = v3[1] - v2[1];
      tempv4[0] = v4[0] - v2[0], tempv4[1] = v4[1] - v2[1];
      }
    else
      {
      tempv1[0] = 0.;
      tempv1[1] = 0.;
      tempv2[0] = 0.;
      tempv2[1] = length12;

      // Rotate the system clockwise
      double sinealpha;
      if (v2[0] > v1[0])
        {
        sinealpha = (v2[0] - v1[0]) / length12;
        }
      else
        {
        sinealpha = (v1[0] - v2[0]) / length12;
        }

      double alpha1 = vcl_asin(sinealpha);

      // Translation
      temptransv3[0] = v3[0] - v2[0];
      temptransv3[1] = v3[1] - v2[1];
      temptransv4[0] = v4[0] - v2[0];
      temptransv4[1] = v4[1] - v2[1];

      // Rotation
      tempv3[0] = temptransv3[0] * cos(alpha1) + temptransv3[1] * sin(alpha1);
      tempv3[1] = temptransv3[1] * cos(alpha1) - temptransv3[0] * sin(alpha1);
      tempv4[0] = temptransv4[0] * cos(alpha1) + temptransv4[1] * sin(alpha1);
      tempv4[1] = temptransv4[1] * cos(alpha1) - temptransv4[0] * sin(alpha1);

      otbMsgDevMacro( << "tempv1[0], tempv1[1], tempv2[0], tempv2[1]: " );
      otbMsgDevMacro( << tempv1[0] << ", " << tempv1[1] << ", " << tempv2[0] << ", " << tempv2[1] );
      otbMsgDevMacro( << "Alpha: " << alpha1 );
      otbMsgDevMacro( << "tempv3[0], tempv3[1], tempv4[0], tempv4[1]: "
                       << tempv3[0] << ", " << tempv3[1] << ", " << tempv4[0] << ", " << tempv4[1] );
      otbMsgDevMacro( << "Calculated length of the second line: "
                       << sqrt(pow((tempv4[0] - tempv3[0]), 2) + pow((tempv4[1] - tempv3[1]), 2)) );
      otbMsgDevMacro( << "Original length of line 1:       " << length12 );
      }
    }

  // Compute the length of the overlapping part
  // of the two lines. This is done by simply
  // comparing the y-values of the lines (remember
  // that both lines are parallel and also parallel
  // to the new y-axis).
  double commonDist = 0.;

  if (tempv3[1] >= tempv4[1])
    {
    if (tempv3[1] >= 0 && tempv3[1] <= tempv2[1])
      {
      if (tempv4[1] >= 0)
        {
        commonDist = vcl_abs(tempv4[1] - tempv3[1]);
        }

      else if (tempv4[1] < 0)
        {
        commonDist = tempv3[1];
        }
      }
    else if (tempv3[1] >= 0 && tempv3[1] >= tempv2[1])
      {
      if (tempv4[1] >= 0)
        {
        commonDist = tempv2[1] - tempv4[1];
        }

      else if (tempv4[1] < 0)
        {
        commonDist = tempv2[1];
        }

      }
    else if (tempv4[1] >= tempv2[1])
      { // No overlapping parts exist. The (negative) distance
        // between the two closest endpoints is calculated.
      commonDist = -vcl_abs(tempv4[1] - tempv2[1]);
      }
    else if (tempv3[1] < 0)
      { // No overlapping parts exist. The (negative) distance
        // between the two closest endpoints is calculated.
      commonDist = tempv3[1];
      }
    }

  else
    {
    if (tempv4[1] >= 0 && tempv4[1] <= tempv2[1])
      {
      if (tempv3[1] >= 0)
        {
        commonDist = vcl_abs(tempv3[1] - tempv4[1]);
        }

      else if (tempv3[1] < 0)
        {
        commonDist = tempv4[1];
        }
      }
    else if (tempv4[1] >= 0 && tempv4[1] >= tempv2[1])
      {
      if (tempv3[1] >= 0)
        {
        commonDist = tempv2[1] - tempv3[1];
        }

      else if (tempv3[1] < 0)
        {
        commonDist = tempv2[1];
        }

      }
    else if (tempv3[1] >= tempv2[1])
      { // No overlapping parts exist. The (negative) distance
        // between the two closest endpoints is calculated.
      commonDist = -vcl_abs(tempv3[1] - tempv2[1]);
      }
    else if (tempv4[1] < 0)
      { // No overlapping parts exist. The (negative) distance
        // between the two closest endpoints is calculated.
      commonDist = tempv4[1];
      }
    }
  // The common parallel parts of the two lines have to be greater than
  // the provided threshold.
  otbMsgDevMacro( << "Calculated common distance of the parallel lines: " << commonDist );
  bool common = (commonDist >= static_cast<double>(m_CommonDistanceThreshold));

  return common;
}

/**
 * Write parallel line in a new path list.
 * p1: First parallel path (line segment)
 */
template <class TPath>
typename ParallelLinePathListFilter<TPath>
::PathPointerType
ParallelLinePathListFilter<TPath>
::WriteParallelPath(PathPointerType p1)
{
  PathPointerType resp = PathType::New();

  VertexIteratorType it;

  for (it = p1->GetVertexList()->Begin(); it != p1->GetVertexList()->End(); ++it)
    {
    resp->AddVertex((it).Value());
    }

  return resp;
}

/**
 * PrintSelf Method
 */
template <class TPath>
void
ParallelLinePathListFilter<TPath>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // End namespace otb
#endif
