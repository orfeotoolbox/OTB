/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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
#include <fstream>
#include <cstdlib>

#include "otbPolygon.h"

int otbPolygon(int itkNotUsed(argc), char* argv[])
{
  typedef otb::Polygon<>                   PolygonType;
  typedef PolygonType::ContinuousIndexType ContinuousIndexType;
  typedef PolygonType::VertexListType      VertexListType;
  typedef VertexListType::ConstIterator    IteratorType;

  // Instantiating object
  PolygonType::Pointer polygon1 = PolygonType::New();
  PolygonType::Pointer polygon2 = PolygonType::New();

  const char* outfile = argv[1];

  // Reading vertices from command line
  int  cpt   = 2;
  bool first = true;
  while (argv[cpt] != nullptr && argv[cpt + 1] != nullptr)
  {
    if (argv[cpt][0] == 'n')
    {
      first = false;
      ++cpt;
    }
    else
    {
      ContinuousIndexType newVertex;
      newVertex[0] = atof(argv[cpt]);
      newVertex[1] = atof(argv[cpt + 1]);
      if (first)
        polygon1->AddVertex(newVertex);
      else
        polygon2->AddVertex(newVertex);
      ++cpt;
      ++cpt;
    }
  }

  IteratorType        begin1 = polygon1->GetVertexList()->Begin();
  IteratorType        end1   = polygon1->GetVertexList()->End();
  IteratorType        begin2 = polygon2->GetVertexList()->Begin();
  IteratorType        end2   = polygon2->GetVertexList()->End();
  IteratorType        it;
  ContinuousIndexType current, firstVertex;

  std::ofstream file;
  file.open(outfile);

  for (it = begin1; it != end1; ++it)
  {
    file << "polygon1->IsInside(" << it.Value() << ") = " << polygon1->IsInside(it.Value()) << std::endl;
    file << "polygon1->IsOnEdge(" << it.Value() << ") = " << polygon1->IsOnEdge(it.Value()) << std::endl;
    file << "polygon2->IsInside(" << it.Value() << ") = " << polygon2->IsInside(it.Value()) << std::endl;
    file << "polygon2->IsOnEdge(" << it.Value() << ") = " << polygon2->IsOnEdge(it.Value()) << std::endl;
  }
  file << std::endl << std::endl;
  for (it = begin2; it != end2; ++it)
  {
    file << "polygon1->IsInside(" << it.Value() << ") = " << polygon1->IsInside(it.Value()) << std::endl;
    file << "polygon1->IsOnEdge(" << it.Value() << ") = " << polygon1->IsOnEdge(it.Value()) << std::endl;
    file << "polygon2->IsInside(" << it.Value() << ") = " << polygon2->IsInside(it.Value()) << std::endl;
    file << "polygon2->IsOnEdge(" << it.Value() << ") = " << polygon2->IsOnEdge(it.Value()) << std::endl;
  }
  file << std::endl << std::endl;

  current     = begin1.Value();
  firstVertex = current;
  ++begin1;
  for (it = begin1; it != end1; ++it)
  {
    file << "polygon1->NbCrossing(" << current << ", " << it.Value() << ") = " << polygon1->NbCrossing(current, it.Value()) << std::endl;
    file << "polygon1->NbTouching(" << current << ", " << it.Value() << ") = " << polygon1->NbTouching(current, it.Value()) << std::endl;
    file << "polygon2->NbCrossing(" << current << ", " << it.Value() << ") = " << polygon2->NbCrossing(current, it.Value()) << std::endl;
    file << "polygon2->NbTouching(" << current << ", " << it.Value() << ") = " << polygon2->NbTouching(current, it.Value()) << std::endl;
    current = it.Value();
  }
  file << "polygon1->NbCrossing(" << current << ", " << firstVertex << ") = " << polygon1->NbCrossing(current, firstVertex) << std::endl;
  file << "polygon1->NbTouching(" << current << ", " << firstVertex << ") = " << polygon1->NbTouching(current, firstVertex) << std::endl;
  file << "polygon2->NbCrossing(" << current << ", " << firstVertex << ") = " << polygon2->NbCrossing(current, firstVertex) << std::endl;
  file << "polygon2->NbTouching(" << current << ", " << firstVertex << ") = " << polygon2->NbTouching(current, firstVertex) << std::endl;

  file << std::endl << std::endl;

  current     = begin2.Value();
  firstVertex = current;
  ++begin2;
  for (it = begin2; it != end2; ++it)
  {
    file << "polygon1->NbCrossing(" << current << ", " << it.Value() << ") = " << polygon1->NbCrossing(current, it.Value()) << std::endl;
    file << "polygon1->NbTouching(" << current << ", " << it.Value() << ") = " << polygon1->NbTouching(current, it.Value()) << std::endl;
    file << "polygon2->NbCrossing(" << current << ", " << it.Value() << ") = " << polygon2->NbCrossing(current, it.Value()) << std::endl;
    file << "polygon2->NbTouching(" << current << ", " << it.Value() << ") = " << polygon2->NbTouching(current, it.Value()) << std::endl;
    current = it.Value();
  }
  file << "polygon1->NbCrossing(" << current << ", " << firstVertex << ") = " << polygon1->NbCrossing(current, firstVertex) << std::endl;
  file << "polygon1->NbTouching(" << current << ", " << firstVertex << ") = " << polygon1->NbTouching(current, firstVertex) << std::endl;
  file << "polygon2->NbCrossing(" << current << ", " << firstVertex << ") = " << polygon2->NbCrossing(current, firstVertex) << std::endl;
  file << "polygon2->NbTouching(" << current << ", " << firstVertex << ") = " << polygon2->NbTouching(current, firstVertex) << std::endl;

  file << std::endl << std::endl;
  file << "Bounding Boxs computation : " << std::endl;
  PolygonType::RegionType r1 = polygon1->GetBoundingRegion();
  file << "Bounding Box 1" << std::endl;
  file << "Index : " << r1.GetIndex() << " , Size : " << r1.GetSize() << std::endl;
  PolygonType::RegionType r2 = polygon2->GetBoundingRegion();
  file << "Bounding Box 2" << std::endl;
  file << "Index : " << r2.GetIndex() << " , Size : " << r2.GetSize() << std::endl;

  file << std::endl << std::endl;
  file << "Surface computation : " << std::endl;
  file << "Surface 1 :" << (double)polygon1->GetArea() << std::endl;
  file << "Surface 2 :" << polygon2->GetArea() << std::endl;

  file << std::endl << std::endl;
  file << "Length computation : " << std::endl;
  file << "Length 1 :" << (double)polygon1->GetLength() << std::endl;
  file << "Length 2 :" << polygon2->GetLength() << std::endl;

  file.close();

  return EXIT_SUCCESS;
}
