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
#ifndef __otbSimplifyPathFunctor_h
#define __otbSimplifyPathFunctor_h

#include "otbMath.h"

namespace otb
{

/** \class SimplifyPathFunctor
*    \brief This filter performs a simplification of the input path.
*
*  It reduces the number of vertices in each path, according to a tolerance criterion. It aims at
*  removing aligned vertices while keeping sharp angular points.
*
*  In order to ensure the unicity of its output, each path is considered first from begin to end, then
*  from begin to the first vertex before the end. At each step, the consistency of the path is checked :
*  the equation of the line passing by the first and last vertices is computed. Then, for each
*  vertices between them, the euclidean distance to this line is computed. If for one vertex, this distance
*  is upper than the tolerance threshold, the path is considered to be inconsistent and no vertices can be removed.
*
*  If the path is considered consistent (which will occure at least with a 2 vertices path),
* only the beginning and ending
*  vertices are kept and a new search iteration begin at its end.
*
*  This filter is part of the road extraction framework.
*
* \sa UnaryFunctorObjectListFilter
*
 *  \ingroup Functor
 */
template <class TInput, class TOutput>
class SimplifyPathFunctor
{
public:

  typedef typename TInput::VertexListType::ConstIterator VertexListConstIteratorType;
  typedef typename TInput::VertexListType::ConstPointer  VertexListConstPointerType;
  typedef TOutput                                        OutputPathType;
  typedef typename OutputPathType::Pointer               OutputPathPointerType;

  void SetTolerance(double Tolerance)
  {
    m_Tolerance = Tolerance;
  }
  double GetTolerance(void) const
  {
    return (m_Tolerance);
  }

  SimplifyPathFunctor()
  {
    m_Tolerance = 1.0;
  }
  ~SimplifyPathFunctor() {}

  inline OutputPathPointerType operator ()(const TInput * input)
  {

    OutputPathPointerType newPath = OutputPathType::New();
    newPath->Initialize();
    // Getting the verices list of the current input paths
    VertexListConstPointerType  vertexList = input->GetVertexList();
    if(vertexList->Size()>0)
      {
      VertexListConstIteratorType beginIt = vertexList->Begin();
      VertexListConstIteratorType beforeTheEndIt = --(vertexList->End());

      // Add the first vertex
      newPath->AddVertex(beginIt.Value());

      while (beginIt != beforeTheEndIt)
        {
        VertexListConstIteratorType endIt = beforeTheEndIt;
        // while the segment is not consistent, decrement endIt
        while (!this->TestPathConsistency(beginIt, endIt))
          {
          --endIt;
          }
        // Add the final vertex
        newPath->AddVertex(endIt.Value());
        beginIt = endIt;
        }
      }

    newPath->SetMetaDataDictionary(input->GetMetaDataDictionary());
    return newPath;

  }

private:
  double m_Tolerance;

  bool TestPathConsistency(VertexListConstIteratorType begin,
                           VertexListConstIteratorType end) const
  {
    VertexListConstIteratorType segmentIt = begin;
    ++segmentIt;
    //Compute the distance of a point to a segment based on the cross product
    while (segmentIt != end)
      {
      double crossProduct = (end.Value()[0] - begin.Value()[0]) * (segmentIt.Value()[1] - begin.Value()[1])
                            - (end.Value()[1] - begin.Value()[1]) * (segmentIt.Value()[0] - begin.Value()[0]);
      double lengthSeg = (end.Value()[0] - begin.Value()[0]) * (end.Value()[0] - begin.Value()[0])
                         + (end.Value()[1] - begin.Value()[1]) * (end.Value()[1] - begin.Value()[1]);
      if (lengthSeg == 0) return false;
      double distsq = crossProduct * crossProduct / lengthSeg;
      if (distsq > static_cast<double>(m_Tolerance))
        {
        return false;
        }
      ++segmentIt;
      }
    return true;
  }

};

}

#endif
