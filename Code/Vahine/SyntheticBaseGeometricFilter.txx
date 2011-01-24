/*
 * SyntheticBaseGeometricFilter.txx
 *
 * Vahine Framework
 * Copyright (C) 2008 to 2009 Ludovic Léau-Mercier and Laboratoire de Planétologie de Grenoble
 * See LICENCE and COPYING for details.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * CeCILLl License for more details and http://www.cecill.info
 *
 */
#ifndef __VAHINESYNTHETICBASEGEOMETRICFILTER_TXX
#define __VAHINESYNTHETICBASEGEOMETRICFILTER_TXX
#include "SyntheticBaseGeometricFilter.h"

namespace otb {

template<class TImage>
VahineSyntheticBaseGeometricFilter<TImage>::VahineSyntheticBaseGeometricFilter() :
  m_Debug(false){
}

template<class TImage>
VahineSyntheticBaseGeometricFilter<TImage>::~VahineSyntheticBaseGeometricFilter(){
}

template<class TImage>
void VahineSyntheticBaseGeometricFilter<TImage>::GenerateData(){
  // size along X
  unsigned int cols = this->GetInput()->GetLargestPossibleRegion().GetSize(0);
  // size along Y
  unsigned int rows = this->GetInput()->GetLargestPossibleRegion().GetSize(1);

  if(m_ColumnIndex>=cols) {
    itkExceptionMacro("Invalid columns index");
    }
  const unsigned int InputNumberOfbands = this->GetInput()->GetNumberOfComponentsPerPixel();

  // create output image
  OutputImagePointer image = OutputImageType::New();
  OutputIndexType    start;
  start[0] = 0;
  start[1] = 1;
  OutputSizeType size;
  size[0] = 1;   // one columns
  size[1] = rows;
  OutputRegionType region;
  region.SetSize(size);
  region.SetIndex(start);
  image->SetRegions(region);
  image->SetNumberOfComponentsPerPixel(InputNumberOfbands);
  image->Allocate();

  LinearIteratorType inputIt(this->GetInput(), this->GetInput()->GetLargestPossibleRegion() );
  IteratorType       outputIt(image, image->GetLargestPossibleRegion() );
  inputIt.SetDirection(1);
  unsigned i = 0;
  while(i<m_ColumnIndex) {
    inputIt.NextLine();
    ++i;
    }
  for(inputIt.GoToBeginOfLine(), outputIt.GoToBegin(); !inputIt.IsAtEndOfLine(); ++inputIt, ++outputIt) {
    outputIt.Set(inputIt.Get() );
    }
  this->GraftOutput(image);
}

/**
 * Standard "PrintSelf" method
 */
template<class TImage>
void VahineSyntheticBaseGeometricFilter<TImage>::PrintSelf(std::ostream& os, itk::Indent indent) const {
  Superclass::PrintSelf( os, indent );
}

} // end namespace otb

#endif