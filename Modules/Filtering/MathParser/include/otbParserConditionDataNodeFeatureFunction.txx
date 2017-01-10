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
#ifndef otbParserConditionDataNodeFeatureFunction_txx
#define otbParserConditionDataNodeFeatureFunction_txx

#include "otbParserConditionDataNodeFeatureFunction.h"

namespace otb
{

/**
 * Constructor
 */
template <class TImage, class TCoordRep, class TPrecision>
ParserConditionDataNodeFeatureFunction<TImage, TCoordRep, TPrecision>
::ParserConditionDataNodeFeatureFunction()
{
  m_ParserConditionFunctor = ParserConditionFunctorType::New();
}

/**
 * Standard "PrintSelf" method
 */
template <class TImage, class TCoordRep, class TPrecision>
void
ParserConditionDataNodeFeatureFunction<TImage, TCoordRep, TPrecision>
::PrintSelf( std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf( os, indent );
  //os << indent << "ParserCondition : "    << m_ParserConditionFunctor << std::endl;
}

template <class TImage, class TCoordRep, class TPrecision>
typename ParserConditionDataNodeFeatureFunction<TImage, TCoordRep, TPrecision>
::OutputType
 ParserConditionDataNodeFeatureFunction<TImage, TCoordRep, TPrecision>
::Evaluate( const DataNodeType& node ) const
{
  // The parser evaluation can't be const since the parser state
  // changes at the first evaluation, but in fact we don't modify
  // anything here.
  // To fit with the rest of the framework, we try to keep the Evaluate
  // function const
  Self* _this = const_cast<Self*>(this);
  ParserConditionFunctorType& parser = *(_this->m_ParserConditionFunctor);
  // Ugly things over


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

  double nbValidPixel = 0.;
  double nbVisitedPixel = 0.;

  while(!lineIt.IsAtEnd())
    {
    if(this->IsInsideBuffer(lineIt.GetIndex()))
      {
      if( parser(this->GetInputImage()->GetPixel(lineIt.GetIndex())) )
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
