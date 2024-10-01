/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbClosePathFunctor_h
#define otbClosePathFunctor_h

#include "otbMath.h"

namespace otb
{

/** \class ClosePathFunctor
*    \brief This filter close the input path, making the last point equal to the first one.
*
* This filter may be useful when a truly closed polygon is needed (to draw it for example)
*
* \sa UnaryFunctorObjectListFilter
*
 *  \ingroup Functor
 *
 * \ingroup OTBPath
 */
template <class TInput, class TOutput>
class ClosePathFunctor
{
public:
  typedef typename TInput::VertexListType::ConstIterator VertexListConstIteratorType;
  typedef typename TInput::VertexListType::ConstPointer  VertexListConstPointerType;
  typedef TOutput                                        OutputPathType;
  typedef typename OutputPathType::Pointer               OutputPathPointerType;

  ClosePathFunctor()
  {
  }
  virtual ~ClosePathFunctor()
  {
  }

  inline OutputPathPointerType operator()(const TInput* input)
  {
    OutputPathPointerType newPath = OutputPathType::New();
    newPath->Initialize();
    typename TInput::VertexType lastVertex;

    if (input->GetVertexList()->Size() > 0)
    {
      // Initialization of lastVertex to GetVertexList
      lastVertex = input->GetVertexList()->Begin().Value();

      for (VertexListConstIteratorType vertexIt = input->GetVertexList()->Begin(); vertexIt != input->GetVertexList()->End(); ++vertexIt)
      {
        newPath->AddVertex(vertexIt.Value());
        lastVertex = vertexIt.Value();
      }
      if (lastVertex != input->GetVertexList()->Begin().Value())
      {
        newPath->AddVertex(input->GetVertexList()->Begin().Value());
      }
    }

    newPath->SetMetaDataDictionary(input->GetMetaDataDictionary());
    return newPath;
  }
};
}

#endif
