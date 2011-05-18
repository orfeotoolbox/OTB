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
#ifndef __otbSpectralAngleDataNodeFeatureFunction_txx
#define __otbSpectralAngleDataNodeFeatureFunction_txx

#include "otbSpectralAngleDataNodeFeatureFunction.h"

//#include "itkImageIteratorWithIndex.h"

namespace otb
{

/**
 * Constructor
 */
template <class TImage, class TCoordRep, class TPrecision>
SpectralAngleDataNodeFeatureFunction<TImage, TCoordRep, TPrecision>
::SpectralAngleDataNodeFeatureFunction() : m_StartNeighborhoodRadius(2), m_StopNeighborhoodRadius(3)
{
  //Example for QuickBird images (on a specific image)
  m_RefPixel.SetSize(4);
  m_RefPixel.SetElement(0, 252.284);
  m_RefPixel.SetElement(1, 357.3);
  m_RefPixel.SetElement(2, 232.644);
  m_RefPixel.SetElement(3, 261.558);
}

/**
 * Standard "PrintSelf" method
 */
template <class TImage, class TCoordRep, class TPrecision>
void
SpectralAngleDataNodeFeatureFunction<TImage, TCoordRep, TPrecision>
::PrintSelf(
  std::ostream& os,
  itk::Indent indent) const
{
  Superclass::PrintSelf( os, indent );
  os << indent << "Reference Pixel: " << m_RefPixel << std::endl;
}


template <class TImage, class TCoordRep, class TPrecision>
typename SpectralAngleDataNodeFeatureFunction<TImage, TCoordRep, TPrecision>
::OutputType
 SpectralAngleDataNodeFeatureFunction<TImage, TCoordRep, TPrecision>
::Evaluate( const DataNodeType& node ) const
{

  // TODO faire avce un ikk
  const typename ImageLineIteratorType::PathType* path;

  switch (node.GetNodeType())
        {
        case FEATURE_POINT:
          {
          itkExceptionMacro(<< "This DataNode type is not handle yet");
          break;
          }
        case FEATURE_LINE:
          {
          path = node.GetLine();
          break;
          }
        case FEATURE_POLYGON:
          {
          path = node.GetPolygonExteriorRing();
          break;
          }
          default:
          {
          itkExceptionMacro(<< "This DataNode type is not handle yet");
          break;
          }
        }

  std::vector< std::pair<IndexType, IndexType> > splitedLineIdNeigh;
  std::vector< std::pair<IndexType, IndexType> > splitedLineIdCentral;
  // Split line and polygon into segment (ie. line with two vertex
  typename LineType::VertexListConstIteratorType it1 = path->GetVertexList()->Begin();
  typename LineType::VertexListConstIteratorType it2 = path->GetVertexList()->Begin();
  typename LineType::VertexListConstIteratorType itStop = path->GetVertexList()->End();
   
  ++it2;
  if( it2 == itStop )
    {
      itkExceptionMacro(<< "Invalid DataNode, must at least contain two points");
    }
  
 

  //unsigned int count = 0;
  while ( it1 != itStop && it2 != itStop)
    {
      IndexType id1, id2;
      id1[0] = static_cast<int>(it1.Value()[0]);
      id1[1] = static_cast<int>(it1.Value()[1]);
      id2[0] = static_cast<int>(it2.Value()[0]);
      id2[1] = static_cast<int>(it2.Value()[1]);
      splitedLineIdCentral.push_back(IndexPairType( id1, id2 ));
      

      for(unsigned int j=m_StartNeighborhoodRadius; j<m_StopNeighborhoodRadius; j++)
        {
          IndexType shift11, shift12;
          shift11[0] = id1[0]-j;
          shift11[1] = id1[1]-j;
          shift12[0] = id1[0]+j;
          shift12[1] = id1[1]+j;
          IndexType shift21, shift22;
          shift21[0] = id2[0]-j;
          shift21[1] = id2[1]-j;
          shift22[0] = id2[0]+j;
          shift22[1] = id2[1]+j;
          
          splitedLineIdNeigh.push_back(IndexPairType( shift11, shift21 ));
          splitedLineIdNeigh.push_back(IndexPairType( shift12, shift22 ));
        }
      ++it1;
      ++it2;
    }

  if( node.GetNodeType() == FEATURE_POLYGON )
    {
      it2--;
      IndexType id1, id2;
      id1[0] = static_cast<int>(path->GetVertexList()->Begin().Value()[0]);
      id1[1] = static_cast<int>(path->GetVertexList()->Begin().Value()[1]);
      id2[0] = static_cast<int>(it2.Value()[0]);
      id2[1] = static_cast<int>(it2.Value()[1]);

       splitedLineIdCentral.push_back(IndexPairType( id1, id2 ));

      for(unsigned int j=m_StartNeighborhoodRadius; j<m_StopNeighborhoodRadius; j++)
        {
          IndexType shift11, shift12;
          shift11[0] = id1[0]-j;
          shift11[1] = id1[1]-j;
          shift12[0] = id1[0]+j;
          shift12[1] = id1[1]+j;
          IndexType shift21, shift22;
          shift21[0] = id2[0]-j;
          shift21[1] = id2[1]-j;
          shift22[0] = id2[0]+j;
          shift22[1] = id2[1]+j;
          
          splitedLineIdNeigh.push_back(IndexPairType( shift11, shift21 ));
          splitedLineIdNeigh.push_back(IndexPairType( shift12, shift22 ));
        }
    }


  double centralAccSpectralAngle=0.;
  double centralNbVisitedPixel=0.;

   for(unsigned int i=0; i<splitedLineIdCentral.size(); i++)
     {
       LineIteratorType lineIt( this->GetInputImage(), splitedLineIdCentral[i].first, splitedLineIdCentral[i].second);
       lineIt.GoToBegin();
       //itk::ImageIteratorWithIndex<TImage> lol(this->GetInputImage());
       while(!lineIt.IsAtEnd())
         {
           if(this->IsInsideBuffer(lineIt.GetIndex()))
             {
               /*
                 std::cout << "m_SpectralAngleFunctor : "
                 << m_SpectralAngleFunctor(lineIt.Get(), this->GetRefPixel())
                 << "  -  currentPixel : "
                 << lineIt.Get()
                 << "  -  RefPixel : "
                 << this->GetRefPixel()
                 << std::endl;
               */
               PixelType currPixel;
               currPixel.SetSize(std::min(this->GetRefPixel().Size(), lineIt.Get().Size()));
               for (unsigned int i=0; i<std::min(this->GetRefPixel().Size(), lineIt.Get().Size()); i++)
                 {
                   currPixel[i] = (lineIt.Get())[i];
                 }
               centralAccSpectralAngle += m_SpectralAngleFunctor(currPixel, this->GetRefPixel());
               centralNbVisitedPixel += 1;
             }
           ++lineIt;
         }
     }

   double neighAccSpectralAngle=0.;
   double neighNbVisitedPixel=0.;

   for(unsigned int i=0; i<splitedLineIdNeigh.size(); i++)
     {
       LineIteratorType lineIt( this->GetInputImage(), splitedLineIdCentral[i].first, splitedLineIdCentral[i].second);
       lineIt.GoToBegin();
       
       while(!lineIt.IsAtEnd())
         {
           if(this->IsInsideBuffer(lineIt.GetIndex()))
             {
               /*
                 std::cout << "m_SpectralAngleFunctor : "
                 << m_SpectralAngleFunctor(lineIt.Get(), this->GetRefPixel())
                 << "  -  currentPixel : "
                 << lineIt.Get()
                 << "  -  RefPixel : "
                 << this->GetRefPixel()
                 << std::endl;
               */
               PixelType currPixel;
               currPixel.SetSize(std::min(this->GetRefPixel().Size(), lineIt.Get().Size()));
               for (unsigned int i=0; i<std::min(this->GetRefPixel().Size(), lineIt.Get().Size()); i++)
                 {
                   currPixel[i] = (lineIt.Get())[i];
                 }
               neighAccSpectralAngle += m_SpectralAngleFunctor(currPixel, this->GetRefPixel());
               neighNbVisitedPixel += 1;
             }
           ++lineIt;
         }
     }

  OutputType output;

  double meanCurr = 0.;
  if( centralNbVisitedPixel != 0.)
    {
      meanCurr /= static_cast<double>(centralAccSpectralAngle);
    }

  double meanNeigh = 0.;
  if( neighNbVisitedPixel != 0.)
    {
      meanNeigh /= static_cast<double>(neighAccSpectralAngle);
    }

  if(meanNeigh == 0.)
    {
      output.push_back(static_cast<PrecisionType>(0.));
    }
  else
    {
      output.push_back(static_cast<PrecisionType>(meanCurr/meanNeigh));
    }

  output.push_back(static_cast<PrecisionType>(centralAccSpectralAngle));
  output.push_back(static_cast<PrecisionType>(centralNbVisitedPixel));

  output.push_back(static_cast<PrecisionType>(neighAccSpectralAngle));
  output.push_back(static_cast<PrecisionType>(neighNbVisitedPixel));
  
  return output;
}


} // end namespace otb

#endif
