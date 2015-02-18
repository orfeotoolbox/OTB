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
#ifndef __otbBreakAngularPathListFilter_txx
#define __otbBreakAngularPathListFilter_txx

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

        alpha1 = vcl_atan2((pixel1[1] - pixel2[1]), (pixel1[0] - pixel2[0]));
        alpha2 = vcl_atan2((pixel2[1] - pixel3[1]), (pixel2[0] - pixel3[0]));
        alpha1 = (alpha1 >= 0) ? alpha1 : (alpha1 + CONST_2PI);
        alpha2 = (alpha2 >= 0) ? alpha2 : (alpha2 + CONST_2PI);
        if (vcl_abs(alpha1 - alpha2) > static_cast<double>(maxAngle))
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
