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
#ifndef __otbImageLayerGenerator_txx
#define __otbImageLayerGenerator_txx

#include "otbImageLayerGenerator.h"

namespace otb
{

template < class TImageLayer >  
ImageLayerGenerator<TImageLayer>
::ImageLayerGenerator()
{

}

template < class TImageLayer >  
ImageLayerGenerator<TImageLayer>
::~ImageLayerGenerator()
{

}

template < class TImageLayer >  
void 
ImageLayerGenerator<TImageLayer>
::GenerateLayer(){}

template < class TImageLayer >  
unsigned int 
ImageLayerGenerator<TImageLayer>
::GetOptimalSubSamplingRate(){}

template < class TImageLayer >  
void 
ImageLayerGenerator<TImageLayer>
::PrintSelf(std::ostream& os, itk::Indent indent) const{}

template < class TImageLayer >  
void 
ImageLayerGenerator<TImageLayer>
::GenerateLayerInformation(){}

template < class TImageLayer >  
void 
ImageLayerGenerator<TImageLayer>
::GenerateQuicklook(){}

template < class TImageLayer >  
void 
ImageLayerGenerator<TImageLayer>
::GenerateHistograms(){}

} // end namespace otb

#endif


