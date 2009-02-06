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
#ifndef __otbRAndGAndNIRVegetationIndexImageFilter_txx
#define __otbRAndGAndNIRVegetationIndexImageFilter_txx

#include "otbRAndGAndNIRVegetationIndexImageFilter.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputImageR, class TInputImageG, class TInputImageNIR,
class TOutputImage, class TFunction  >
RAndGAndNIRVegetationIndexImageFilter<TInputImageR,TInputImageG,TInputImageNIR,TOutputImage,TFunction>
::RAndGAndNIRVegetationIndexImageFilter()
{
}


/**
 * Connect one of the operands for pixel-wise addition
 */
template <class TInputImageR, class TInputImageG, class TInputImageNIR,
class TOutputImage, class TFunction  >
void
RAndGAndNIRVegetationIndexImageFilter<TInputImageR,TInputImageG,TInputImageNIR,TOutputImage,TFunction>
::SetInputR( const TInputImageR * image )
{
  // Process object is not const-correct so the const casting is required.
  SetInput1( image );
}


/**
 * Connect one of the operands for pixel-wise addition
 */
template <class TInputImageR, class TInputImageG, class TInputImageNIR,
class TOutputImage, class TFunction  >
void
RAndGAndNIRVegetationIndexImageFilter<TInputImageR,TInputImageG,TInputImageNIR,TOutputImage,TFunction>
::SetInputG( const TInputImageG * image )
{
  SetInput2( image );
}

/**
 * Connect one of the operands for pixel-wise addition
 */
template <class TInputImageR, class TInputImageG, class TInputImageNIR,
class TOutputImage, class TFunction  >
void
RAndGAndNIRVegetationIndexImageFilter<TInputImageR,TInputImageG,TInputImageNIR,TOutputImage,TFunction>
::SetInputNIR( const TInputImageNIR * image )
{
  SetInput3( image );
}

template <class TInputImageR, class TInputImageG, class TInputImageNIR, class TOutputImage, class TFunction  >
void
RAndGAndNIRVegetationIndexImageFilter<TInputImageR, TInputImageG, TInputImageNIR, TOutputImage, TFunction>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

}

#endif
