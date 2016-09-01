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
#ifndef otbNDVIDataNodeFeatureFunction_txx
#define otbNDVIDataNodeFeatureFunction_txx

#include "otbNDVIDataNodeFeatureFunction.h"

namespace otb
{

/**
 * Constructor
 */
template <class TImage, class TCoordRep, class TPrecision>
NDVIDataNodeFeatureFunction<TImage, TCoordRep, TPrecision>
::NDVIDataNodeFeatureFunction()
{
  m_NDVIThreshold = 0.3;    //Advised for QuickBird images
  this->SetREDChannelIndex(3); //for QuickBird images
  this->SetNIRChannelIndex(4); //for QuickBird images
}

/**
 * Standard "PrintSelf" method
 */
template <class TImage, class TCoordRep, class TPrecision>
void
NDVIDataNodeFeatureFunction<TImage, TCoordRep, TPrecision>
::PrintSelf(
  std::ostream& os,
  itk::Indent indent) const
{
  Superclass::PrintSelf( os, indent );
  os << indent << "NDVI Threshold: "    << m_NDVIThreshold << std::endl;
  os << indent << "RED Channel Index: " << this->GetREDChannelIndex() << std::endl;
  os << indent << "NIR Channel Index: " << this->GetNIRChannelIndex() << std::endl;
}


template <class TImage, class TCoordRep, class TPrecision>
typename NDVIDataNodeFeatureFunction<TImage, TCoordRep, TPrecision>
::OutputType
 NDVIDataNodeFeatureFunction<TImage, TCoordRep, TPrecision>
::Evaluate( const DataNodeType& node ) const
{
  if(!((this->GetREDChannelIndex() <= this->GetInputImage()->GetNumberOfComponentsPerPixel())
      && (this->GetNIRChannelIndex() <= this->GetInputImage()->GetNumberOfComponentsPerPixel())
      && (this->GetREDChannelIndex() > 0)
      && (this->GetNIRChannelIndex() > 0)))
    {
    itkExceptionMacro(<< "Invalid RED or NIR Channel Index");
    }

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

  double nbValidPixel=0.;
  double nbVisitedPixel=0.;

  while(!lineIt.IsAtEnd())
    {
    if(this->IsInsideBuffer(lineIt.GetIndex()))
      {
      PixelType pixel = this->GetInputImage()->GetPixel(lineIt.GetIndex());
      if(m_NDVIFunctor(pixel [this->GetREDChannelIndex() - 1],
                       pixel [this->GetNIRChannelIndex() - 1]) >= this->GetNDVIThreshold())
        {
        nbValidPixel += 1;
        }
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
    output.push_back(static_cast<PrecisionType>(nbValidPixel/nbVisitedPixel));
    }

  output.push_back(static_cast<PrecisionType>(nbValidPixel));
  output.push_back(static_cast<PrecisionType>(nbVisitedPixel));

  return output;
}


} // end namespace otb

#endif
