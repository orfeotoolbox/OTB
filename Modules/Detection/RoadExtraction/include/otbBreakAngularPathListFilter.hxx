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

#ifndef otbBreakAngularPathListFilter_hxx
#define otbBreakAngularPathListFilter_hxx

#include "otbBreakAngularPathListFilter.h"
#include "otbMath.h"

namespace otb
{
/**
 * Constructor
 */
template <class TPath>
BreakAngularPathListFilter<TPath>
::BreakAngularPathListFilter()
{
}

template <class TPath>
void
BreakAngularPathListFilter<TPath>
::BreakAngularPath(const MaxAngleType maxAngle, const PathPointerType inputPath, PathListPointerType outputPathList)
{
  typename PathType::VertexListType::ConstPointer  vertexList = inputPath->GetVertexList();
  typename PathType::VertexListType::ConstIterator pathIt = vertexList->Begin();

  typename PathType::VertexType pixel1, pixel2, pixel3;

  // Initialization
  PathPointerType newPath = PathType::New();
  newPath->Initialize();

  double alpha1(0.), alpha2(0.);

  while (pathIt != vertexList->End())
    {
    // Add Pixel 1
    newPath->AddVertex(pathIt.Value());
    pixel1 = pathIt.Value();
    ++pathIt;
    if (pathIt != vertexList->End())
      {
      pixel2 = pathIt.Value();
      ++pathIt;
      if (pathIt != vertexList->End())
        {
        pixel3 = pathIt.Value();

        alpha1 = std::atan2((pixel1[1] - pixel2[1]), (pixel1[0] - pixel2[0]));
        alpha2 = std::atan2((pixel2[1] - pixel3[1]), (pixel2[0] - pixel3[0]));
        alpha1 = (alpha1 >= 0) ? alpha1 : (alpha1 + CONST_2PI);
        alpha2 = (alpha2 >= 0) ? alpha2 : (alpha2 + CONST_2PI);
        if (std::abs(alpha1 - alpha2) > static_cast<double>(maxAngle))
          {
          // Add Pixel 2
          newPath->AddVertex(pixel2);
          //Create new PathType in the out path list
          outputPathList->PushBack(newPath);
          // Reinit
          newPath = PathType::New();

          }
        --pathIt; // Return previous pixel
        }
      else
        {
        // Add last Pixel (Pixel 2)
        newPath->AddVertex(pixel2);
        }
      }
    }
  //Create new PathType in the out list
  outputPathList->PushBack(newPath);
}

template <class TPath>
void
BreakAngularPathListFilter<TPath>
::GenerateData()
{
  const PathListType * inputPathList  = this->GetInput();
  PathListType *       outputPathList = this->GetOutput();

  typename PathListType::ConstIterator listIt = inputPathList->Begin();
  outputPathList->Clear();

  PathListPointerType newTempPathList = PathListType::New();
  while (listIt != inputPathList->End())
    {
    (void) BreakAngularPath(m_MaxAngle, listIt.Get(), outputPathList);
    ++listIt;
    }
}

/**
 * PrintSelf Method
 */
template <class TPath>
void
BreakAngularPathListFilter<TPath>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Angular max value : " << m_MaxAngle << std::endl;
}

} // End namespace otb
#endif
