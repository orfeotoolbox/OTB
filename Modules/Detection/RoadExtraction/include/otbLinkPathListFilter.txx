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
#ifndef otbLinkPathListFilter_txx
#define otbLinkPathListFilter_txx

#include "otbLinkPathListFilter.h"
#include "otbMath.h"

namespace otb
{
/**
 * Constructor
 */
template <class TPath>
LinkPathListFilter<TPath>
::LinkPathListFilter()
{
  m_DistanceThreshold = 10.;
  m_AngularThreshold = 3.14;
  m_ModuloPI = false;
}

template <class TPath>
void
LinkPathListFilter<TPath>
::GenerateData()
{
  // IO
  const PathListType * inputPtr  = this->GetInput();
  PathListType *       outputPtr = this->GetOutput();

  // Input iterator
  IteratorType inputIt = inputPtr->Begin();

  std::vector<bool> eraseFlagVector;

  // First we copy all input path to output
  while (inputIt != inputPtr->End())
    {
    outputPtr->PushBack(inputIt.Get());
    eraseFlagVector.push_back(false);
    ++inputIt;
    }

  IteratorType outputIt1 = outputPtr->Begin();
  unsigned int index1 = 0;

  while (outputIt1 != outputPtr->End())
    {
    if (!eraseFlagVector[index1])
      {
      IteratorType outputIt2 = outputIt1;
      ++outputIt2;
      // indicate if a path was found and wath is its index
      unsigned int pathToLinkIndex = 0;
      double       distance = -1.0;
      unsigned int index2 = index1 + 1;
      bool         found = false;
      bool         revert1 = false;
      bool         revert2 = false;
      if (!eraseFlagVector[index1])
        {
        while (outputIt2 != outputPtr->End())
          {
          if (!eraseFlagVector[index2])
            {
            //otbMsgDevMacro(<<"Examining links between path "<<index1<<" and path "<<index2);
            // Examining end of path1 with beginning of path2
            VertexIteratorType vSourceIt = outputIt1.Get()->GetVertexList()->End();
            VertexIteratorType vTargetIt = outputIt2.Get()->GetVertexList()->Begin();
            --vSourceIt;
            VertexType v2 = vSourceIt.Value();
            --vSourceIt;
            VertexType v1 = vSourceIt.Value();
            VertexType v3 = vTargetIt.Value();
            ++vTargetIt;
            VertexType v4 = vTargetIt.Value();
            double     tmpDistance = vcl_sqrt(vcl_pow(v2[0] - v3[0], 2) + vcl_pow(v2[1] - v3[1], 2));
            if ((tmpDistance < static_cast<double>(m_DistanceThreshold)) && ((!found) || (tmpDistance < distance)))
              {
              if (VerifyAngularCondition(v1, v2, v3, v4))
                {
                // we found a better candidate
                //otbMsgDevMacro(<<"Link between end of path "<<index1<<" and beginning of path "<<index2<<" is currently the best choice");
                distance = tmpDistance;
                pathToLinkIndex = index2;
                revert1 = false;
                revert2 = false;
                if (!found)
                  {
                  found = true;
                  }
                }
              }
            // Examining end of path1 with end of path2
            vTargetIt = outputIt2.Get()->GetVertexList()->End();
            --vTargetIt;
            v3 = vTargetIt.Value();
            --vTargetIt;
            v4 = vTargetIt.Value();
            tmpDistance = vcl_sqrt(vcl_pow(v2[0] - v3[0], 2) + vcl_pow(v2[1] - v3[1], 2));

            if ((tmpDistance < static_cast<double>(m_DistanceThreshold)) && ((!found) || (tmpDistance < distance)))
              {
              if (VerifyAngularCondition(v1, v2, v3, v4))
                {
                // we found a better candidate
                //otbMsgDevMacro(<<"Link between end of path "<<index1<<" and end of path "<<index2<<" is currently the best choice");
                distance = tmpDistance;
                pathToLinkIndex = index2;
                revert1 = false;
                revert2 = true;
                if (!found)
                  {
                  found = true;
                  }
                }
              }
            // Examining beginning of path1 with end of path2
            vSourceIt = outputIt1.Get()->GetVertexList()->Begin();
            v2 = vSourceIt.Value();
            ++vSourceIt;
            v1 = vSourceIt.Value();
            tmpDistance = vcl_sqrt(vcl_pow(v2[0] - v3[0], 2) + vcl_pow(v2[1] - v3[1], 2));

            if ((tmpDistance < static_cast<double>(m_DistanceThreshold)) && ((!found) || (tmpDistance < distance)))
              {
              if (VerifyAngularCondition(v1, v2, v3, v4))
                {
                // we found a better candidate
                //otbMsgDevMacro(<<"Link between beginning of path "<<index1<<" and end of path "<<index2<<" is currently the best choice");
                distance = tmpDistance;
                pathToLinkIndex = index2;
                revert1 = true;
                revert2 = true;
                if (!found)
                  {
                  found = true;
                  }
                }
              }
            // Examining beginning of path1 with beginning of path2
            vTargetIt = outputIt2.Get()->GetVertexList()->Begin();
            v3 = vTargetIt.Value();
            ++vTargetIt;
            v4 = vTargetIt.Value();
            tmpDistance = vcl_sqrt(vcl_pow(v2[0] - v3[0], 2) + vcl_pow(v2[1] - v3[1], 2));
            if ((tmpDistance < static_cast<double>(m_DistanceThreshold)) && ((!found) || (tmpDistance < distance)))
              {
              if (VerifyAngularCondition(v1, v2, v3, v4))
                {
                // we found a better candidate
                //otbMsgDevMacro(<<"Link between beginning of path "<<index1<<" and beginning of path "<<index2<<" is currently the best choice");
                distance = tmpDistance;
                pathToLinkIndex = index2;
                revert1 = true;
                revert2 = false;
                if (!found)
                  {
                  found = true;
                  }
                }
              }
            }
          ++index2;
          ++outputIt2;
          }
        if (found)
          {
          //otbMsgDevMacro(<<"Search ended, merging path "<<index1<<" and path "<<pathToLinkIndex);
          // an appropriate path was found
          outputIt2 = outputPtr->Begin() + pathToLinkIndex;
          PathPointerType newPath = this->LinkPath(outputIt1.Get(), revert1, outputIt2.Get(), revert2);
          outputPtr->PushBack(newPath);
          // add a non erased flag for the new path
          eraseFlagVector.push_back(false);
          // mark the old path as erased
          eraseFlagVector[index1] = true;
          eraseFlagVector[pathToLinkIndex] = true;
          }
        }
      }
    ++index1;
    // This replaces ++outputIt1 because the iterators is somehow invalidated by a pushback.
    outputIt1 = outputPtr->Begin() + index1;
    }

  // search ended, now removing the erased path
  typename std::vector<bool>::reverse_iterator it = eraseFlagVector.rbegin();
  index1 = eraseFlagVector.size() - 1;
  while (it != eraseFlagVector.rend())
    {
    if (eraseFlagVector[index1])
      {
      outputPtr->Erase(index1);
      }
    --index1;
    ++it;
    }
}
/**
 * Verify the angular condition to link a path.
 * \param v1 First vertex (before first path ending),
 * \param v2 second vertex (first path ending),
 * \param v3 third vertex (second path beginning),
 * \param v4 fourth vertex (second path after beginning),
 * \return True or false wether the condition is verified or not.
 **/
template <class TPath>
bool
LinkPathListFilter<TPath>
::VerifyAngularCondition(VertexType v1, VertexType v2, VertexType v3, VertexType v4)
{
  double alpha1 = vcl_atan2((v2[1] - v1[1]), (v2[0] - v1[0]));
  double alpha2 = vcl_atan2((v4[1] - v3[1]), (v4[0] - v3[0]));
  double alpha3 = vcl_atan2((v3[1] - v2[1]), (v3[0] - v2[0]));

  if (m_ModuloPI)
    {
    alpha1 = (alpha1 >= 0) ? alpha1 : (alpha1 + CONST_PI);
    alpha2 = (alpha2 >= 0) ? alpha2 : (alpha2 + CONST_PI);
    alpha3 = (alpha3 >= 0) ? alpha3 : (alpha3 + CONST_PI);
    }
  else
    {
    alpha1 = (alpha1 >= 0) ? alpha1 : (alpha1 + CONST_2PI);
    alpha2 = (alpha2 >= 0) ? alpha2 : (alpha2 + CONST_2PI);
    alpha3 = (alpha3 >= 0) ? alpha3 : (alpha3 + CONST_2PI);
    }

  bool resp = (vcl_abs(alpha1 - alpha2) < static_cast<double>(m_AngularThreshold))
              && (vcl_abs(alpha1 - alpha3) < static_cast<double>(m_AngularThreshold))
              && (vcl_abs(alpha2 - alpha3) < static_cast<double>(m_AngularThreshold));
  return resp;
}
/**
 * Actually link the end of path p1 to the beginning of path p2.
 * \param p1 First path to link,
 * \param revert1 wether p1 need to be reverted or not,
 * \param p2 second path to link,
 * \param revert2 wether p2 need to be reverted or not,
 * \return the linked path.
 */
template <class TPath>
typename LinkPathListFilter<TPath>
::PathPointerType
LinkPathListFilter<TPath>
::LinkPath(PathPointerType p1, bool revert1, PathPointerType p2, bool revert2)
{
  PathPointerType resp = PathType::New();

  VertexIteratorType it;

  if (revert1)
    {
    for (it = (p1->GetVertexList()->End()); it != p1->GetVertexList()->Begin(); )
      {
      --it;
      resp->AddVertex((it).Value());
      }
    }
  else
    {
    for (it = p1->GetVertexList()->Begin(); it != p1->GetVertexList()->End(); ++it)
      {
      resp->AddVertex((it).Value());
      }
    }

  if (revert2)
    {
    for (it = (p2->GetVertexList()->End()); it != p2->GetVertexList()->Begin(); )
      {
      --it;
      resp->AddVertex((it).Value());
      }
    }
  else
    {
    for (it = p2->GetVertexList()->Begin(); it != p2->GetVertexList()->End(); ++it)
      {
      resp->AddVertex(it.Value());
      }
    }
  return resp;
}
/**
 * PrintSelf Method
 */
template <class TPath>
void
LinkPathListFilter<TPath>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // End namespace otb
#endif
