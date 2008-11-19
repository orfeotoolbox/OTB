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
     *  \brief 
     *
 
     *  \ingroup Functor
     */
  template <class TInput, class TOutput>
  class SimplifyPathFunctor
  {
    public:
      
      typedef typename TInput::ObjectType::VertexListType::ConstIterator VertexListConstIteratorType;
      typedef typename TInput::ObjectType::VertexListType::ConstPointer VertexListConstPointerType;
      typedef TOutput OutputPathPointerType;
      typedef typename OutputPathPointerType::ObjectType OutputPathType;
      
      void SetTolerance(double Tolerance )
      {
        m_Tolerance = Tolerance;
      }
      double GetTolerance(void)const
      {
        return (  m_Tolerance );
      }
      
      SimplifyPathFunctor() 
      {
        m_Tolerance = 1.0;
      };
      ~SimplifyPathFunctor() {};
      
      inline OutputPathPointerType operator()(const TInput & input)
      {

        OutputPathPointerType newPath = OutputPathType::New();
        newPath->Initialize();
	// Getting the verices list of the current input paths
        VertexListConstPointerType  vertexList = input->GetVertexList();
        VertexListConstIteratorType beginIt = vertexList->Begin();
        VertexListConstIteratorType beforeTheEndIt = --(vertexList->End());
       
	// Add the first vertex
        newPath->AddVertex(beginIt.Value());
      
        while ( beginIt != beforeTheEndIt)
        {    
          VertexListConstIteratorType endIt = beforeTheEndIt;
      // while the segment is not consistent, decrement endIt
          while (!this->TestPathConsistency(vertexList,beginIt, endIt))
          {
            --endIt;
          }
      // Add the final vertex
          newPath->AddVertex(endIt.Value());
          beginIt=endIt;
        }
        return newPath;
   
      }
      
      
      
    private:
      double m_Tolerance;
      
      bool TestPathConsistency(VertexListConstPointerType vertexList, 
                          VertexListConstIteratorType begin, 
                          VertexListConstIteratorType end)
      {   
        VertexListConstIteratorType segmentIt = begin;
        ++segmentIt;
    //Compute the distance of a point to a segment based on the cross product
        while (segmentIt != end)
        {
          double crossProduct = (end.Value()[0]-begin.Value()[0])*(segmentIt.Value()[1]-begin.Value()[1])
                - (end.Value()[1]-begin.Value()[1])*(segmentIt.Value()[0]-begin.Value()[0]);
          double lenghtSeg = (end.Value()[0]-begin.Value()[0])*(end.Value()[0]-begin.Value()[0])
                +(end.Value()[1]-begin.Value()[1])*(end.Value()[1]-begin.Value()[1]);
          if (lenghtSeg == 0) return false;
          double distsq = crossProduct*crossProduct/lenghtSeg;
          if (distsq > static_cast<double>(m_Tolerance) ) 
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
