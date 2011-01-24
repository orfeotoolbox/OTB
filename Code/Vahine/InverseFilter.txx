/*
 * InverseFilter.txx
 *
 * Vahine Framework
 * Copyright (C) 2008, 2009 Ludovic Léau-Mercier and Laboratoire de Planétologie de Grenoble
 * See LICENCE and COPYING for details.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * CeCILLl License for more details and http://www.cecill.info
 *
 */
#ifndef __VAHINEINVERSEFILTER_TXX
#define __VAHINEINVERSEFILTER_TXX
#include "InverseFilter.h"

namespace otb {
template<class TInputImage, class TOutputImage>
VahineInverseFilter<TInputImage,TOutputImage>::VahineInverseFilter(){
  m_GrsirAxe = VnlVectorType(0);
  m_Interpolation = 0;
}

template<class TInputImage, class TOutputImage>
void VahineInverseFilter<TInputImage, TOutputImage>::GenerateData(){
  InputImagePointer pInputImage = const_cast<InputImageType *>(this->GetInput() );

  pInputImage->UpdateOutputInformation();
  OutputImagePointer pOutputImage = OutputImageType::New();

  unsigned int size = m_GrsirAxe.size();
  vahineDebug("size of grsir axe = "<<size);
  vahineDebug("input image largest possible region ="<<pInputImage->GetLargestPossibleRegion() );
  pOutputImage->SetRegions(pInputImage->GetLargestPossibleRegion() );
  pOutputImage->Allocate();

  ConstVectorIteratorType itConstInputImage(pInputImage, pInputImage->GetLargestPossibleRegion() );
  OutIterator             itOutputImage(pOutputImage,pOutputImage->GetLargestPossibleRegion() );

  unsigned int nbInversedPixel = 0;
  for(itConstInputImage.GoToBegin(), itOutputImage.GoToBegin();
      !itConstInputImage.IsAtEnd();
      ++itConstInputImage, ++itOutputImage) {
    PixelType pixel = itConstInputImage.Get(); // input pixel
    RealType  value = 0;                       // output value
    try{
      VnlVectorType p(pixel.GetSize() );
      p.copy_in(pixel.GetDataPointer() );
      value = static_cast<RealType>(dot_product(p,m_GrsirAxe) );
      if(value != 0) {
        value = m_Interpolation->get(value);
        ++nbInversedPixel;
        }
      }catch(...) {
      vahineDebug("Error in generate data");
      throw OVERFLOW; // for debugging
      }
    itOutputImage.Set(value);
    }
  vahineDebug("Number of pixel inversed = "<<nbInversedPixel<<" pointer output information"<<pOutputImage);
  this->GraftOutput(pOutputImage);
}

}

#endif