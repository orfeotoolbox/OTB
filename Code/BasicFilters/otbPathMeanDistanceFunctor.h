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
#ifndef __otbPathMeanDistanceFunctor_h
#define __otbPathMeanDistanceFunctor_h

#include "otbMath.h"

namespace otb
{

    /** \class PathMeanDistanceFunctor
     *  \brief Functor to select path according to the average distance between points
     *
     * This functor compute the average distance between each point of a path
     * and return true is the distance is above the threshold, false otherwise.
     *
     *  \ingroup Functor
     */
  template <class TInput1>
  class PathMeanDistanceFunctor
  {
    public:
      

      void SetThreshold(double threshold )
      {
        m_Threshold = threshold;
      }
      double GetThreshold(void)const
      {
        return (  m_Threshold );
      }
      
      PathMeanDistanceFunctor() 
      {
        m_Threshold = 0.2;
      };
      ~PathMeanDistanceFunctor() {};
      
      inline bool operator()(const TInput1 & input)
      {
        
        double meanDistance = 0.0;
        typedef typename TInput1::ObjectType::VertexListType::ConstIterator VertexListConstIteratorType;
        typedef typename TInput1::ObjectType::VertexType VertexType;
        VertexListConstIteratorType beginIt = input->GetVertexList()->Begin();
        VertexListConstIteratorType beforeTheEndIt = input->GetVertexList()->End();
        --beforeTheEndIt;
        while (beginIt != beforeTheEndIt)
        {
          VertexType v1 = beginIt.Value();
          ++beginIt;
          VertexType v2 = beginIt.Value();
          meanDistance+=vcl_sqrt(vcl_pow(v1[0]-v2[0],2)+vcl_pow(v1[1]-v2[1],2));
        }

        double nbVertices = static_cast<double>(input->GetVertexList()->Size());
        meanDistance = meanDistance / (nbVertices-1);
      
        if ( meanDistance > m_Threshold)
        {
          return true;
        }
        else
        {
          return false;
        }
      }
      
      
      
    private:
      double m_Threshold;
  };

}

#endif
