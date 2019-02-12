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

#include "itkMacro.h"

#include "otbSimplifyPathListFilter.h"
#include "itkPolyLineParametricPath.h"
#include <fstream>
#include <cstdlib>

/**
 * This filter is intended to test the multithreading capabilities
 * of the otb::UnaryFunctorObjectListFilter
 */

int otbSimplifyManyPathListFilter(int itkNotUsed(argc), char * argv[])
{
  const char * outfname = argv[1];
  const double tolerance = atof(argv[2]);

//   typedef std::vector<double> PointsVectorType;
//   typedef std::vector< PointsVectorType > PointsMatrixType;
//   PointsMatrixType MatricePoints;
//   PointsVectorType ListPoints;

  const unsigned int Dimension = 2;
  typedef itk::PolyLineParametricPath<Dimension>    PathType;
  typedef otb::SimplifyPathListFilter<PathType>     SimplifyPathListFilterType;
  typedef SimplifyPathListFilterType::InputListType PathListType;
  PathType::ContinuousIndexType cindex;

  PathListType::Pointer InputPathList = PathListType::New();

  //Generate PathList
  unsigned long int numberOfPaths = 10000;
  srand(123456);

  for (unsigned long int i = 0; i < numberOfPaths; ++i)
    {
    PathType::Pointer path = PathType::New();
    //Generate PathList
    unsigned int numberOfPoints = static_cast<int>((rand() / (RAND_MAX + 1.0)) * 90) + 10;
//     std::cout << "List :" << numberOfPoints << " points" << std::endl;
    cindex[0] = 0;
    cindex[1] = 0;
    for (unsigned int j = 0; j < numberOfPoints; ++j)
      {
      cindex[0] += (rand() / (RAND_MAX + 1.0)) * 100 - 50;
      cindex[1] += (rand() / (RAND_MAX + 1.0)) * 100 - 50;
//       std::cout << "Point Index :"<<cindex[0]<<", "<<cindex[1]<<std::endl;
      path->AddVertex(cindex);
      }
    InputPathList->PushBack(path);
    }
  // Instantiating object
  SimplifyPathListFilterType::Pointer simplifyFilter = SimplifyPathListFilterType::New();

  simplifyFilter->SetInput(InputPathList);
  simplifyFilter->GetFunctor().SetTolerance(tolerance);
  simplifyFilter->Update();

  PathListType::Pointer OutputPathList = simplifyFilter->GetOutput();

  typedef PathListType::ConstIterator   PathListIteratorType;
  typedef PathType::VertexListType      VertexListType;
  typedef VertexListType::ConstIterator VertexIteratorType;

  std::ofstream file;
  file.open(outfname);
  unsigned int         counter = 1;
  PathListIteratorType pathListIt = InputPathList->Begin();

  file << "TOLERANCE: " << simplifyFilter->GetFunctor().GetTolerance() << "(" << tolerance << ")" << std::endl;

  pathListIt = OutputPathList->Begin();
  file << "OUTPUT list of Path " << ": " << std::endl;
  while (pathListIt != OutputPathList->End())
    {
    file << "Path " << counter << ": ";
    for (VertexIteratorType vIt = pathListIt.Get()->GetVertexList()->Begin();
         vIt != pathListIt.Get()->GetVertexList()->End();
         ++vIt)
      {
      if (vIt != pathListIt.Get()->GetVertexList()->Begin())
        {
        file << ", ";
        }
      file << vIt.Value();
      }
    file << std::endl;
    ++pathListIt;
    ++counter;
    }
  file.close();

  return EXIT_SUCCESS;
}
