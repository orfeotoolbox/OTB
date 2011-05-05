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
template <class TImage, class TCoordRep, class TPrecision>
SpectralAngleDataNodeFeatureFunction<TImage, TCoordRep, TPrecision>
::SpectralAngleDataNodeFeatureFunction()
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

  ImageLineIteratorType lineIt(this->GetInputImage(), path);
  lineIt.GoToBegin();

  double accSpectralAngle=0.;
  double nbVisitedPixel=0.;

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
      accSpectralAngle += m_SpectralAngleFunctor(currPixel, this->GetRefPixel());
      nbVisitedPixel += 1;
      }
    ++lineIt;
    }

  OutputType output;

  if(nbVisitedPixel == 0)
    {
    //std::cout << "nbValidPixel: " << nbValidPixel << "nbVisitedPixel" << nbVisitedPixel << std::endl;
    //itkExceptionMacro(<< "The DataNode and the Support Image are disjointed");
    output.push_back(static_cast<PrecisionType>(0.));
    }
  else
    {
    output.push_back(static_cast<PrecisionType>(accSpectralAngle/nbVisitedPixel));
    }

  output.push_back(static_cast<PrecisionType>(accSpectralAngle));
  output.push_back(static_cast<PrecisionType>(nbVisitedPixel));

  return output;
}


} // end namespace otb

#endif
