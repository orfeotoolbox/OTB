#ifndef _otbVectorToRGBImageFilter_txx
#define _otbVectorToRGBImageFilter_txx

#include "otbVectorToRGBImageFilter.h"

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkProgressReporter.h"
#include "itkPixelTraits.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputPixel, class TOutputPixel, unsigned int VImageDimension>
VectorToRGBImageFilter< TInputPixel, TOutputPixel, VImageDimension >
::VectorToRGBImageFilter() :    m_RedChannel(1),
                                m_GreenChannel(2),
                                m_BlueChannel(3)
{
}


/**
 * ThreadedGenerateData Performs the pixel-wise addition
 */
template <class TInputPixel, class TOutputPixel, unsigned int VImageDimension>
void
VectorToRGBImageFilter< TInputPixel, TOutputPixel, VImageDimension >
::ThreadedGenerateData( const OutputImageRegionType &outputRegionForThread,
                        int threadId)
{

  typename OutputImageType::Pointer outputPtr = this->GetOutput();

  typename InputImageType::Pointer inputPtr = (InputImageType*)(this->GetInput(0));
  
  itk::ImageRegionIterator<OutputImageType> outputIt(outputPtr, outputRegionForThread);

  itk::ProgressReporter progress(this, 
                            threadId, 
                            outputRegionForThread.GetNumberOfPixels());
  outputIt.GoToBegin();
  typename OutputImageType::PixelType arrayPixel ;

  itk::ImageRegionConstIterator< InputImageType > inputIt(inputPtr, outputRegionForThread);
  inputIt.GoToBegin();
  
  while( !outputIt.IsAtEnd() ) 
    {
    typename OutputImageType::PixelType pix;

    pix.SetRed(inputIt.Get().GetElement(m_RedChannel-1));
    pix.SetGreen(inputIt.Get().GetElement(m_GreenChannel-1));
    pix.SetBlue(inputIt.Get().GetElement(m_BlueChannel-1));
    outputIt.Set(pix);
    ++inputIt;
    ++outputIt;
    progress.CompletedPixel();
    }
}

} // end namespace otb

#endif
