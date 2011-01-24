/*
 * FlatReshapeFilter.txx
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
#ifndef __VAHINEFLATRESHAPEFILTER_TXX
#define __VAHINEFLATRESHAPEFILTER_TXX
#include <itkProgressReporter.h>
#include "FlatReshapeFilter.h"

namespace otb {

template<class TBaseImage, class TMaskImage>
VahineFlatReshapeFilter<TBaseImage, TMaskImage>::VahineFlatReshapeFilter() :
  m_LowThreshold(1.0), m_HighThreshold(1.0),
  m_FlatReshape(true),
  m_NullValue(0.0){

}

template<class TBaseImage, class TMaskImage>
VahineFlatReshapeFilter<TBaseImage, TMaskImage>::~VahineFlatReshapeFilter(){
}

template<class TBaseImage, class TMaskImage>
void VahineFlatReshapeFilter<TBaseImage, TMaskImage>::CheckInputBaseAndMaskSize(){
  vahineDebug("CheckInputBaseAndMaskSize");
  m_Mask->UpdateOutputInformation();
  BaseImageRegion imageRegion = this->GetInput()->GetLargestPossibleRegion();
  m_MaskLargestPossibleRegion = m_Mask->GetLargestPossibleRegion();
  if(m_FlatReshape) {
    if(m_MaskLargestPossibleRegion != imageRegion) {
      itkExceptionMacro("The size of base image and mask image is different.");
      }
    } else {
    // unflat
    vahineDebug("unflat");
    unsigned int  nbPixelMask = GetNumberOfActivPixels();
    BaseImageSize imageSize = imageRegion.GetSize();
    unsigned int  nbPixelImage = imageSize[0]*imageSize[1];
    if(nbPixelMask != nbPixelImage) {
      itkExceptionMacro("The number of pixel of base image and the number of valid pixel in mask image is different.");
      }
    }
}

/*
template<class TBaseImage, class TMaskImage>
  void VahineFlatReshapeFilter<TBaseImage, TMaskImage>::GetMaskRegion(BaseImageRegion outputRegion, MaskImageIndex & maskImageStartIndex, MaskImageRegion & maskImageRegion){
  vahineDebug("GetMaskRegion");
  ConstMaskIteratorIndexType itMask(m_Mask, m_MaskLargestPossibleRegion);
  itMask.GoToBegin();
  //MaskImageRegion maskRegion;
  MaskImageIndex currentIndex, maskStartIndex;
  MaskImageSize maskSize;
  unsigned int count = 0;
  while(count<outputRegion.GetIndex()[1]+1){
    if(static_cast<double>(itMask.Get()) > m_Threshold){
      ++count;
      maskStartIndex = itMask.GetIndex();
    }
    ++itMask;
  }
  while(count<outputRegion.GetSize()[1]){
    if(static_cast<double>(itMask.Get()) > m_Threshold){
      ++count;
      currentIndex = itMask.GetIndex();
    }
    ++itMask;
  }
  maskImageStartIndex = maskStartIndex;
  maskStartIndex[0] = 0; // force first column
  maskSize[0] = m_MaskLargestPossibleRegion.GetSize()[0]; // force width of image
  maskSize[1] = currentIndex[1] - maskStartIndex[1] + 1;
  maskImageRegion.SetIndex( maskStartIndex );
  maskImageRegion.SetSize( maskSize );
}
*/

/**
 * Get number of activ pixel
 * We need to update mask image for accessing to mask value
 */
template<class TBaseImage, class TMaskImage>
unsigned int VahineFlatReshapeFilter<TBaseImage, TMaskImage>::GetNumberOfActivPixels(){
  vahineDebug("GetNumberOfActivPixels");
  m_Mask->Update();
  ConstMaskIteratorType itMask(m_Mask, m_Mask->GetLargestPossibleRegion() );
  itMask.GoToBegin();
  unsigned int count = 0;
  unsigned int nbpixel = 0;
  while(!itMask.IsAtEnd() ) {
    double maskValue = static_cast<double>(itMask.Get() );
    if( m_LowThreshold <= maskValue && maskValue <= m_HighThreshold ) {
      ++count;
      }
    ++itMask;
    ++nbpixel;
    }
  vahineDebug("count number of activ pixel = "<<count);
  vahineDebug("low threshold ="<<m_LowThreshold<<" high threshold ="<<m_HighThreshold);
  return count;
}

/**
 * Output image have one columns
 * We update ouput size image
 */
template<class TBaseImage, class TMaskImage>
void VahineFlatReshapeFilter<TBaseImage, TMaskImage>::GenerateOutputInformation(){
  vahineDebug("GenerateOutputInformation");
  // call the superclass' implementation of this method
  Superclass::GenerateOutputInformation();

  // get pointers to the input and output
  BaseImageConstPointer inputPtr = this->GetInput();
  BaseImagePointer      outputPtr = this->GetOutput();

  if ( !inputPtr || !outputPtr ) {
    return;
    }

  CheckInputBaseAndMaskSize();

  BaseImageIndex outputStartIndex;
  outputStartIndex.Fill(0);
  BaseImageSize outputSize;
  if( m_FlatReshape ) {
    // size along X
    outputSize[0] = 1;
    outputSize[1] = GetNumberOfActivPixels();
    } else {
    // unflat
    outputSize = m_Mask->GetLargestPossibleRegion().GetSize();
    }
  m_OutRegion.SetSize( outputSize );
  m_OutRegion.SetIndex( outputStartIndex );
  outputPtr->SetLargestPossibleRegion( m_OutRegion );
  outputPtr->SetNumberOfComponentsPerPixel(inputPtr->GetNumberOfComponentsPerPixel() );
}

template<class TBaseImage, class TMaskImage>
void VahineFlatReshapeFilter<TBaseImage, TMaskImage>::GenerateInputRequestedRegion(){
  vahineDebug("GenerateInputRequestedRegion");
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();

  // get pointers to the input and output
  BaseImagePointer inputPtr = const_cast<BaseImageType *>(this->GetInput() );
  BaseImagePointer outputPtr = this->GetOutput();

  if ( !inputPtr || !outputPtr ) {
    return;
    }

  // we need to compute the input requested region (size and start index)

  BaseImageRegion inputRequestedRegion = inputPtr->GetLargestPossibleRegion();
  /*
  const BaseImageRegion & outputRequestedRegion = outputPtr->GetRequestedRegion();
    BaseImageRegion inputRequestedRegion = inputPtr->GetRequestedRegion();
  vahineDebug("output request "<<outputRequestedRegion);
  BaseImageIndex dummy;
  GetMaskRegion(outputRequestedRegion, dummy, inputRequestedRegion);

    inputRequestedRegion.Crop( inputPtr->GetLargestPossibleRegion() );
    */
  inputPtr->SetRequestedRegion( inputRequestedRegion );
  vahineDebug("input request "<<inputRequestedRegion);
}

/**
 * Each activ paixel is an row with one pixel wise in new image
 * z-size (number of band) is unchanged
 */
template<class TBaseImage, class TMaskImage>
void VahineFlatReshapeFilter<TBaseImage, TMaskImage>::GenerateData(){
  CheckInputBaseAndMaskSize();
  MaskImageRegion maskRegion = m_Mask->GetLargestPossibleRegion();
  BaseImageRegion baseRegion = this->GetInput()->GetLargestPossibleRegion();
  vahineDebug("base region "<<baseRegion<<" mask region "<<maskRegion);
  vahineDebug("out region "<<m_OutRegion);
  ConstBaseIteratorType itBaseImage(this->GetInput(), baseRegion);
  ConstMaskIteratorType itMask(m_Mask, maskRegion);

  BaseImagePointer outputImage = BaseImageType::New();
  outputImage->SetRegions(m_OutRegion);
  unsigned int componentsPerPixel = this->GetInput()->GetNumberOfComponentsPerPixel();
  vahineDebug("components per pixel = "<<componentsPerPixel);
  outputImage->SetNumberOfComponentsPerPixel(componentsPerPixel);
  outputImage->Allocate();
  OutputIteratorType itOutput(outputImage, m_OutRegion);
  itOutput.GoToBegin();
  if( m_FlatReshape ) {
    for(itBaseImage.GoToBegin(), itMask.GoToBegin(); !itBaseImage.IsAtEnd(); ++itBaseImage, ++itMask) {
      double maskValue = static_cast<double>(itMask.Get() );
      if(m_LowThreshold <= maskValue && maskValue <= m_HighThreshold ) {
        itOutput.Set(itBaseImage.Get() );
        ++itOutput;
        }
      }
    } else {
    // unflat	operation
    itBaseImage.GoToBegin();
    for(itMask.GoToBegin(), itOutput.GoToBegin(); !itMask.IsAtEnd(); ++itMask, ++itOutput) {
      double maskValue = static_cast<double>(itMask.Get() );
      if(m_LowThreshold <= maskValue && maskValue <= m_HighThreshold ) {
        itOutput.Set(itBaseImage.Get() );
        ++itBaseImage;
        } else {
        PixelType nullPixel;
        nullPixel.Reserve(componentsPerPixel);
        nullPixel.Fill(m_NullValue);
        itOutput.Set(nullPixel);
        }
      }
    }
  this->GraftOutput(outputImage);
}

/**
 * Each activ pixel is an row with one pixel wise in new image
 * z-size (number of band) is unchanged
 */
/*
template<class TBaseImage, class TMaskImage>
  void VahineFlatReshapeFilter<TBaseImage, TMaskImage>::ThreadedGenerateData(const BaseImageRegion& outputRegionForThread, int threadId ){
  vahineDebug("ThreadedGenerateData");
  // Get the input and output pointers
  BaseImageConstPointer  inputPtr = this->GetInput();
  BaseImagePointer       outputPtr = this->GetOutput();

  // Define/declare an iterator that will walk the output region for this
  // thread.
  OutputIteratorType outIt(outputPtr, outputRegionForThread);

  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  MaskImageRegion maskImageRegion = MaskImageRegion();
  MaskImageIndex maskStartIndex = MaskImageIndex();
  GetMaskRegion(outputRegionForThread, maskStartIndex, maskImageRegion);
  ConstMaskIteratorIndexType maskIt(m_Mask, maskImageRegion);

  maskIt.SetIndex(maskStartIndex);

  // walk the output region, and sample the input image
  outIt.GoToBegin();
  while ( !outIt.IsAtEnd() ) {
      // copy the input pixel to the output
    vahineDebug("maskIt threadId "<<threadId<<" index "<<maskIt.GetIndex());
    while(static_cast<double>(m_Mask->GetPixel(maskIt.GetIndex()))<=m_Threshold){
      ++maskIt;
    }
      outIt.Set(inputPtr->GetPixel(maskIt.GetIndex()));
      ++outIt;
      ++maskIt;
      progress.CompletedPixel();
  }
}
*/
/**
 * Standard "PrintSelf" method
 */
template<class TBaseImage, class TMaskImage>
void VahineFlatReshapeFilter<TBaseImage, TMaskImage>::PrintSelf(std::ostream& os, itk::Indent indent) const {
  Superclass::PrintSelf( os, indent );
}

} // end namespace otb

#endif