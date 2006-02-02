#ifndef _otbVectorToScalarImageFilter_txx
#define _otbVectorToScalarImageFilter_txx

#include "otbVectorToScalarImageFilter.h"

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
VectorToScalarImageFilter< TInputPixel, TOutputPixel, VImageDimension >
::VectorToScalarImageFilter() :    m_Channel(1)
{
}


/**
 * ThreadedGenerateData Performs the pixel-wise addition
 */
template <class TInputPixel, class TOutputPixel, unsigned int VImageDimension>
void
VectorToScalarImageFilter< TInputPixel, TOutputPixel, VImageDimension >
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
//    typename OutputImageType::PixelType pix;

//    pix.Set();
    outputIt.Set(inputIt.Get().GetElement(m_Channel-1));
    ++inputIt;
    ++outputIt;
    progress.CompletedPixel();
    }
}

} // end namespace otb

#endif
