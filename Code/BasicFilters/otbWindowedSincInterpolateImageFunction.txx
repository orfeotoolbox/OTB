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
#include "otbWindowedSincInterpolateImageFunction.h"

namespace otb
{
/** Constructor */
template<class TInputImage, class TWindowFunction, class TBoundaryCondition, class TCoordRep>
WindowedSincInterpolateImageFunction<TInputImage, TWindowFunction, TBoundaryCondition, TCoordRep>
::WindowedSincInterpolateImageFunction()
{
}
  
/** Destructor */
template<class TInputImage, class TWindowFunction, class TBoundaryCondition, class TCoordRep>
WindowedSincInterpolateImageFunction<TInputImage, TWindowFunction, TBoundaryCondition, TCoordRep>
::~WindowedSincInterpolateImageFunction()
{
}

/*
// Overload method to add the construction of resampledprfil
template<class TInputImage, class TWindowFunction, class TBoundaryCondition, class TCoordRep>
void
WindowedSincInterpolateImageFunction<TInputImage, TWindowFunction, TBoundaryCondition, TCoordRep>
::SetRadius(unsigned int rad)
{
  Superclass::SetRadius(rad);
  this->GetFunction().SetRadius(rad);
  this->Modified();
}
*/

// Overload method to add the construction of resampledprfil
template<class TInputImage, class TWindowFunction, class TBoundaryCondition, class TCoordRep>
void
WindowedSincInterpolateImageFunction<TInputImage, TWindowFunction, TBoundaryCondition, TCoordRep>
::SetSigma(unsigned int sigma)
{
  m_Sigma = sigma;
  //Initialisation du Radius avec le sigma
  this->ResetOffsetTable();
  this->SetRadius( sigma );
  this->Modified();
}

template<class TInputImage, class TWindowFunction, class TBoundaryCondition, class TCoordRep>
void
WindowedSincInterpolateImageFunction<TInputImage, TWindowFunction, TBoundaryCondition, TCoordRep>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{ 
  Superclass::PrintSelf( os, indent );
}

}//namespace otb
