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
#ifndef _otbSimplifyPathListFilter_txx
#define _otbSimplifyPathListFilter_txx

#include "otbSimplifyPathListFilter.h"
#include "otbMacro.h"
#include "vnl/vnl_math.h"

namespace otb
{
  /**
   * Constructor
   */
  template <class TPath>
  SimplifyPathListFilter<TPath>
  ::SimplifyPathListFilter()
  {
    m_Tolerance = 1.0;
  }
  
  /**
   * Test if the segment composed of the points in vertextlist between the begin and the end iterators is valid.
   * \param vertexList The list of vertices composing the segment,
   * \param begin The Beginning of the segment,
   * \param end The end of the segment, 
   * \return True if the segment is valid.
   */
  
  template <class TPath>
  bool 
  SimplifyPathListFilter<TPath>
  ::TestPathConsistency(VertexListConstPointerType vertexList, VertexListConstIteratorType begin, VertexListConstIteratorType end)
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
  
    /**
   * Simplify one path of the list
   * \param path pointer to path to be simplified,
   * \return pointer to simplified path.
   */
  template <class TPath>
      typename SimplifyPathListFilter<TPath>::PathPointerType
      SimplifyPathListFilter<TPath>
  ::SimplifyPath(PathPointerType path)
      {
        PathPointerType newPath = PathType::New();
        newPath->Initialize();
	// Getting the verices list of the current input paths
        VertexListConstPointerType  vertexList = path->GetVertexList();
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
  
  
  template <class TPath>
  void
  SimplifyPathListFilter<TPath>
  ::GenerateData(void)
  {
    // input/output wiring
    PathListConstPointerType inputPtr = this->GetInput();
    PathListPointerType outputPtr = this->GetOutput();
    
    // Reset the output list in case of multiple updates
    outputPtr->Clear();

    // path iterator typedef
    typedef typename PathListType::ConstIterator PathIteratorType;

    // iterator definition 
    PathIteratorType inputIt = inputPtr->Begin();
    while(inputIt != inputPtr->End())
    {

      PathPointerType newPath = SimplifyPath(inputIt.Get());
      // otbMsgDevMacro(<< "Path  had " << vertexList->Size() << " now has " << newPath->GetVertexList()->Size());
      outputPtr->PushBack(newPath);
      ++inputIt;
    }
  }
  
  
  /**
   * PrintSelf Method
   */
  template <class TPath>
  void
  SimplifyPathListFilter<TPath>
  ::PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
  }
} // End namespace otb
#endif
