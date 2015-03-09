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
#ifndef __otbClosePathFunctor_h
#define __otbClosePathFunctor_h

#include "otbMath.h"

namespace otb
{

/** \class ClosePathFunctor
*    \brief This filter close the input path, making the last point equal to the first one.
*
* This filter may be useful when a truely closed polygon is needed (to draw it for example)
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
  {}
  virtual ~ClosePathFunctor() {}

  inline OutputPathPointerType operator ()(const TInput * input)
  {
    OutputPathPointerType newPath = OutputPathType::New();
    newPath->Initialize();
    typename TInput::VertexType lastVertex;

    if(input->GetVertexList()->Size()>0)
      {
      for (VertexListConstIteratorType vertexIt = input->GetVertexList()->Begin();
          vertexIt != input->GetVertexList()->End();
          ++vertexIt)
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
