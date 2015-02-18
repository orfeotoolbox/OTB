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

namespace otb
{

/**
 * Constructor
 */
template<class TImage, class TCoordRep, class TPrecision>
SpectralAngleDataNodeFeatureFunction<TImage, TCoordRep, TPrecision>::SpectralAngleDataNodeFeatureFunction() :
  m_Radius(2)
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
template<class TImage, class TCoordRep, class TPrecision>
void SpectralAngleDataNodeFeatureFunction<TImage, TCoordRep, TPrecision>::PrintSelf(std::ostream& os,
                                                                                    itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Reference Pixel: " << m_RefPixel << std::endl;
}

template<class TImage, class TCoordRep, class TPrecision>
typename SpectralAngleDataNodeFeatureFunction<TImage, TCoordRep, TPrecision>::OutputType SpectralAngleDataNodeFeatureFunction<
    TImage, TCoordRep, TPrecision>::Evaluate(const DataNodeType& node) const
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

  std::vector<std::pair<IndexType, IndexType> > splitedLineIdNeigh;
  std::vector<std::pair<IndexType, IndexType> > splitedLineIdCentral;

  // Split line and polygon into segment (ie. line with two vertex
  VertexListConstIteratorType it1 = path->GetVertexList()->Begin();
  VertexListConstIteratorType it2 = path->GetVertexList()->Begin();
  VertexListConstIteratorType itStop = path->GetVertexList()->End();

  ++it2;
  if (it2 == itStop)
    {
    itkExceptionMacro(<< "Invalid DataNode, must at least contain two points");
    }

  while (it1 != itStop && it2 != itStop)
    {
    IndexType id1, id2;
    id1[0] = static_cast<int> (it1.Value()[0]);
    id1[1] = static_cast<int> (it1.Value()[1]);
    id2[0] = static_cast<int> (it2.Value()[0]);
    id2[1] = static_cast<int> (it2.Value()[1]);

    // Compute the direction of the current line
    itk::Vector<double, 2> direction;
    direction[0] = it2.Value()[0] - it1.Value()[0];
    direction[1] = it2.Value()[1] - it1.Value()[1];
    direction.Normalize();

    // Compute the orthogonal direction of the current line
    itk::Vector<double, 2> orthogonalDirection;
    orthogonalDirection[0] = direction[1];
    orthogonalDirection[1] = -direction[0];

    splitedLineIdCentral.push_back(IndexPairType(id1, id2));

    for (unsigned int j = 1; j <= m_Radius; ++j)
      {
      IndexType shift11, shift12;
      shift11[0] = id1[0] - j * orthogonalDirection[0];
      shift11[1] = id1[1] - j * orthogonalDirection[1];
      shift12[0] = id1[0] + j * orthogonalDirection[0];
      shift12[1] = id1[1] + j * orthogonalDirection[1];
      IndexType shift21, shift22;
      shift21[0] = id2[0] - j * orthogonalDirection[0];
      shift21[1] = id2[1] - j * orthogonalDirection[1];
      shift22[0] = id2[0] + j * orthogonalDirection[0];
      shift22[1] = id2[1] + j * orthogonalDirection[1];

      splitedLineIdCentral.push_back(IndexPairType(shift11, shift21));
      splitedLineIdCentral.push_back(IndexPairType(shift12, shift22));
      }

    ++it1;
    ++it2;
    }

  // in FEATURE_POLYGON case, first point appears twice (fisrt vertex and last vertew, thus we create a line of 1 point...)
  if (node.GetNodeType() == FEATURE_POLYGON)
    {
    splitedLineIdCentral.pop_back();
    }

  double centralAccSpectralAngle = 0.;
  //double centralAccSpectralAngleSecondOrder = 0.;
  double centralNbVisitedPixel = 0.;

  for (unsigned int i = 0; i < splitedLineIdCentral.size(); ++i)
    {
    LineIteratorType lineIt(this->GetInputImage(), splitedLineIdCentral[i].first, splitedLineIdCentral[i].second);
    lineIt.GoToBegin();

    while (!lineIt.IsAtEnd())
      {
      if (this->IsInsideBuffer(lineIt.GetIndex()))
        {
        PixelType currPixel = this->GetInputImage()->GetPixel( lineIt.GetIndex() );
        double angle = m_SpectralAngleFunctor(currPixel, this->GetRefPixel());
        centralAccSpectralAngle += angle;
        //centralAccSpectralAngleSecondOrder += angle * angle;
        centralNbVisitedPixel += 1;
        }
      ++lineIt;
      }
    }

  OutputType output;

  double meanCentral = 0.;
  //double stddevCentral = 0.;
  if (centralNbVisitedPixel != 0.)
    {
    meanCentral = static_cast<double> (centralAccSpectralAngle) / centralNbVisitedPixel;
    //stddevCentral = vcl_sqrt( centralAccSpectralAngleSecondOrder/centralNbVisitedPixel - meanCentral*meanCentral );
    }

  if (meanCentral == 0.)
    {
    output.push_back(static_cast<PrecisionType> (0.));
    }
  else
    {
    // Compute the descriptor here
    // meanCentral is in [0, pi]
    // We need a descriptor in [0 1]

    double descriptor = meanCentral / otb::CONST_PI;
    output.push_back(static_cast<PrecisionType>( descriptor ));
    }

  output.push_back(static_cast<PrecisionType> (centralAccSpectralAngle));
  output.push_back(static_cast<PrecisionType> (centralNbVisitedPixel));

  return output;
}

} // end namespace otb

#endif
