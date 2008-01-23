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
#ifndef _otbWindowedSincInterpolateImageFunctionBase_txx
#define _otbWindowedSincInterpolateImageFunctionBase_txx


#include "otbWindowedSincInterpolateImageFunctionBase.h"

namespace otb
{
/** Constructor */
template<class TInputImage, class TWindowFunction, class TBoundaryCondition, class TCoordRep>
WindowedSincInterpolateImageFunctionBase<TInputImage, TWindowFunction, TBoundaryCondition, TCoordRep>
::WindowedSincInterpolateImageFunctionBase()
{
}
  
/** Destructor */
template<class TInputImage, class TWindowFunction, class TBoundaryCondition, class TCoordRep>
WindowedSincInterpolateImageFunctionBase<TInputImage, TWindowFunction, TBoundaryCondition, TCoordRep>
::~WindowedSincInterpolateImageFunctionBase()
{
}

// Overload method to add the construction of resampledprofil
template<class TInputImage, class TWindowFunction, class TBoundaryCondition, class TCoordRep>
void
WindowedSincInterpolateImageFunctionBase<TInputImage, TWindowFunction, TBoundaryCondition, TCoordRep>
::SetRadius(unsigned int rad)
{
  this->ResetOffsetTable();
  this->Superclass::SetRadius(rad);
  this->Modified();
}

template<class TInputImage, class TWindowFunction, class TBoundaryCondition, class TCoordRep>
void
WindowedSincInterpolateImageFunctionBase<TInputImage, TWindowFunction, TBoundaryCondition, TCoordRep>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{ 
  Superclass::PrintSelf( os, indent );
}

}//namespace otb
#endif
