/*
 * MaskUpdateFilter.txx
 *
 * Vahine Framework
 * Copyright (C) 2008 to 2010 Ludovic Léau-Mercier and Laboratoire de Planétologie de Grenoble
 * See LICENCE and COPYING for details.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * CeCILLl License for more details and http://www.cecill.info
 *
 */

#ifndef __VAHINEMASKUPDATEFILTER_TXX
#define __VAHINEMASKUPDATEFILTER_TXX
#include "MaskUpdateFilter.h"

namespace otb {

template<class TMaskImage>
VahineMaskUpdateFilter<TMaskImage>::VahineMaskUpdateFilter() :
  m_LowThreshold(1.0), m_HighThreshold(1.0){
}

template<class TMaskImage>
VahineMaskUpdateFilter<TMaskImage>::~VahineMaskUpdateFilter(){
}

template<class TMaskImage>
void VahineMaskUpdateFilter<TMaskImage>::GenerateData(){
  const unsigned int inputMaskNumberOfbands = this->GetInput()->GetNumberOfComponentsPerPixel();

  m_RemedialMask->Update();
  MaskRegionType inputMaskRegion = this->GetInput()->GetLargestPossibleRegion();
  MaskRegionType remedialMaskRegion = m_RemedialMask->GetLargestPossibleRegion();

  ConstMaskIteratorType itRemedialMask(m_RemedialMask, remedialMaskRegion);
  ConstMaskIteratorType itInputMask(this->GetInput(), inputMaskRegion);

  MaskImagePointer maskOutput = MaskImageType::New();
  maskOutput->SetRegions(inputMaskRegion);
  maskOutput->SetNumberOfComponentsPerPixel(inputMaskNumberOfbands);
  if(m_InputBand>inputMaskNumberOfbands) {
    vahineDebug("Error on input band number");
    throw VahineException();
    }
  maskOutput->Allocate();

  MaskIteratorType itOutputMask(maskOutput, inputMaskRegion);
  itOutputMask.Begin();
  itRemedialMask.Begin();
  itInputMask.Begin();
  while(!itOutputMask.IsAtEnd() ) {
    MaskImagePixelType inputPixel = itInputMask.Get();
    MaskImagePixelType remedialValue = itRemedialMask.Get();
    //vahineDebug("in "<<inputPixel[m_InputBand]<<" rem
    // "<<remedialValue[m_RemedialBand]);
    if(m_LowThreshold <= inputPixel[m_InputBand] && inputPixel[m_InputBand] <= m_HighThreshold) {
      inputPixel[m_InputBand] = remedialValue[m_RemedialBand];
      ++itRemedialMask;
      }
    itOutputMask.Set(inputPixel);
    ++itInputMask;
    ++itOutputMask;
    }

  this->GraftOutput(maskOutput);
}

/**
 * Standard "PrintSelf" method
 */
template<class TMaskImage>
void VahineMaskUpdateFilter<TMaskImage>::PrintSelf(std::ostream& os, itk::Indent indent) const {
  Superclass::PrintSelf( os, indent );
}

} // end namespace otb

#endif