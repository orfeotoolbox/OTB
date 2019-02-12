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

#include "otbLikelihoodPathListFilter.h"
#include "otbPolyLineParametricPathWithValue.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include <fstream>

int otbLikelihoodPathListFilter(int itkNotUsed(argc), char * argv[])
{

  std::cout << std::endl;

  const char * infname = argv[1];
  const char * outfname = argv[2];

  typedef std::vector<double>           PointsVectorType;
  typedef std::vector<PointsVectorType> PointsMatrixType;
  PointsMatrixType MatricePoints;
  PointsVectorType ListPoints;

  int cpt = 3;
  ListPoints.clear();

  while (argv[cpt] != nullptr)
    {
    if (argv[cpt][0] == '|')
      {
      if ((ListPoints.size() % 2) != 0)
        {
        itkGenericExceptionMacro(<< "Missing point in parameters !");
        }
      MatricePoints.push_back(ListPoints);
      ListPoints.clear();
      }
    else
      {
      ListPoints.push_back(static_cast<double>(::atof(argv[cpt])));
      }
    ++cpt;
    }
  MatricePoints.push_back(ListPoints);

  const unsigned int Dimension = 2;
  typedef double                                                  PixelType;
  typedef otb::Image<PixelType, Dimension>                        ImageType;
  typedef otb::ImageFileReader<ImageType>                         ReaderType;
  typedef otb::PolyLineParametricPathWithValue<double, Dimension> PathType;
  typedef otb::LikelihoodPathListFilter<PathType, ImageType>      LikelihoodPathListFilterType;
  typedef LikelihoodPathListFilterType::PathListType              PathListType;
  PathType::ContinuousIndexType cindex;

  PathListType::Pointer InputPathList = PathListType::New();

  //Generate PathList
  for (PointsMatrixType::iterator listpos = MatricePoints.begin(); listpos != MatricePoints.end(); ++listpos)
    {
    PathType::Pointer path = PathType::New();
    //Generate PathList
    std::cout << "List " << std::endl;
    for (PointsVectorType::iterator it = (*listpos).begin(); it != (*listpos).end(); ++it)
      {
      cindex[0] = *it;
      ++it;
      cindex[1] = *it;
      std::cout << "Point Index :" << cindex[0] << ";" << cindex[1] << std::endl;
      path->AddVertex(cindex);
      }
    InputPathList->PushBack(path);
    }

  //Instantiating object
  LikelihoodPathListFilterType::Pointer filter = LikelihoodPathListFilterType::New();
  ReaderType::Pointer                   reader = ReaderType::New();
  reader->SetFileName(infname);
  reader->Update();

  filter->SetInput(InputPathList);
  filter->SetInputImage(reader->GetOutput());
  filter->Update();

  std::cout << "Filter execution ended" << std::endl;

  PathListType::Pointer OutputPathList = filter->GetOutput();

  typedef PathListType::ConstIterator   PathListIteratorType;
  typedef PathType::VertexListType      VertexListType;
  typedef VertexListType::ConstIterator VertexIteratorType;

  std::ofstream file;
  file.open(outfname);
  unsigned int         counter = 1;
  PathListIteratorType pathListIt = InputPathList->Begin();

  file << "INPUT list of Path " << ": " << std::endl;
  while (pathListIt != InputPathList->End())
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
  counter = 1;
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
    file << " Value: " << pathListIt.Get()->GetValue();
    file << std::endl;
    ++pathListIt;
    ++counter;
    }
  file.close();

  return EXIT_SUCCESS;
}
