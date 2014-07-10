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
#include "itkMacro.h"

#include "otbParallelLinePathListFilter.h"
#include "itkPolyLineParametricPath.h"
#include <cstdlib>

int otbParallelLinePathListFilter(int itkNotUsed(argc), char ** itkNotUsed(argv))
{

  // We create some lines
  const unsigned int Dimension = 2;
  typedef itk::PolyLineParametricPath<Dimension> PathType;
  typedef otb::ObjectList<PathType>              PathListType;

  PathListType::Pointer lineList = PathListType::New();
  PathListType::Pointer parallelList = PathListType::New();

  typedef PathType::ContinuousIndexType ContinuousIndexType;
  ContinuousIndexType cindex;

  /*-----*/
  PathType::Pointer aLine = PathType::New();
  aLine->Initialize();
  cindex[0] = 1;
  cindex[1] = 1;
  aLine->AddVertex(cindex);

  cindex[0] = 1;
  cindex[1] = 100;
  aLine->AddVertex(cindex);

  lineList->PushBack(aLine);
  parallelList->PushBack(aLine);

  /*-----*/
  aLine = PathType::New();
  aLine->Initialize();
  cindex[0] = 10;
  cindex[1] = 1;
  aLine->AddVertex(cindex);

  cindex[0] = 10;
  cindex[1] = 100;
  aLine->AddVertex(cindex);
  parallelList->PushBack(aLine);
  lineList->PushBack(aLine);

  /*-----*/
  aLine = PathType::New();
  aLine->Initialize();
  cindex[0] = 174;
  cindex[1] = 99;
  aLine->AddVertex(cindex);

  cindex[0] = 281;
  cindex[1] = 1;
  aLine->AddVertex(cindex);

  lineList->PushBack(aLine);

  // Parallel lines are detected.

  typedef otb::ParallelLinePathListFilter<PathType> ParallelLinePathType;
  ParallelLinePathType::Pointer parallelLinePathListFilter = ParallelLinePathType::New();
  parallelLinePathListFilter->SetDistanceThreshold(10);
  parallelLinePathListFilter->SetAngularThreshold(10);
  parallelLinePathListFilter->SetCommonDistanceThreshold(10);
  parallelLinePathListFilter->SetInput(lineList);
  parallelLinePathListFilter->Update();

  PathListType::Pointer  pathList = parallelLinePathListFilter->GetOutput();
  PathListType::Iterator listIt = pathList->Begin();

  PathListType::Iterator parListIt = parallelList->Begin();

  // A path is a line segment in this case.
  while (listIt != pathList->End() && parListIt != parallelList->End())
    {
    PathType::VertexListType::ConstPointer vertexList = (listIt.Get())->GetVertexList();

    PathType::VertexListType::ConstPointer parVertexList = (parListIt.Get())->GetVertexList();

    PathType::VertexListType::ConstIterator pathIt = vertexList->Begin();
    PathType::VertexListType::ConstIterator parPathIt = parVertexList->Begin();
    // Loop over all the vertices in one path
    while (pathIt != vertexList->End() &&
           parPathIt != parVertexList->End())
      {

      if (pathIt.Value() != parPathIt.Value())
        {
        std::cout << pathIt.Index() << pathIt.Value() << std::endl;
        return EXIT_FAILURE;

        }
      ++pathIt;
      ++parPathIt;
      }
    ++listIt;
    ++parListIt;
    }

  return EXIT_SUCCESS;

}
