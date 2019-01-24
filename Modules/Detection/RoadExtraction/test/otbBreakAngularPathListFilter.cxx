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
#include "otbBreakAngularPathListFilter.h"

#include "itkPolyLineParametricPath.h"
#include <fstream>
#include <vector>
#include "otbMath.h"
#include <cstdlib>

int otbBreakAngularPathListFilter(int itkNotUsed(argc), char * argv[])
{
  const char * outfname = argv[1];

  typedef std::vector<double>           PointsVectorType;
  typedef std::vector<PointsVectorType> PointsMatrixType;
  PointsMatrixType MatricePoints;
  PointsVectorType ListPoints;
  PointsVectorType ListMaxAngle;

  int cpt = 2;

  ListMaxAngle.clear();
  while (argv[cpt][0] != '|')
    {
    ListMaxAngle.push_back(static_cast<double>(::atof(argv[cpt])));
    ++cpt;
    }
  ++cpt;
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
  typedef itk::PolyLineParametricPath<Dimension>       PathType;
  typedef otb::BreakAngularPathListFilter<PathType>    BreakAngularPathListFilterType;
  typedef BreakAngularPathListFilterType::PathListType PathListType;
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

  // Instantiating object
  BreakAngularPathListFilterType::Pointer breakAngularFilter = BreakAngularPathListFilterType::New();
  breakAngularFilter->SetInput(InputPathList);

  std::ofstream file;
  file.open(outfname);

  for (PointsVectorType::iterator itAngle = ListMaxAngle.begin(); itAngle != ListMaxAngle.end(); ++itAngle)
    {

    breakAngularFilter->SetMaxAngle((*itAngle) * otb::CONST_PI / 180.);
    breakAngularFilter->Update();

    PathListType::Pointer OutputPathList = breakAngularFilter->GetOutput();

    typedef PathListType::ConstIterator   PathListIteratorType;
    typedef PathType::VertexListType      VertexListType;
    typedef VertexListType::ConstIterator VertexIteratorType;

    unsigned int         counter = 1;
    PathListIteratorType pathListIt = InputPathList->Begin();

    file << "--------------------------------------------------------------------------" << std::endl;
    file << "MAX ANGULAR :" << breakAngularFilter->GetMaxAngle() << "(" << (*itAngle) << " deg.)" << std::endl;
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
      file << std::endl;
      ++pathListIt;
      ++counter;
      }

    } //Enf for angle
  file.close();

  return EXIT_SUCCESS;
}
