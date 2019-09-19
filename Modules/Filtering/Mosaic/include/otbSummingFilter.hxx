#ifndef __SummingFilter_hxx
#define __SummingFilter_hxx

#include "otbSummingFilter.h"
#include "itkProgressReporter.h"

namespace otb {

/**
 * Generates output information:
 */
template <class TInputImage, class TOutputImage>
void SummingFilter<TInputImage, TOutputImage>
::GenerateOutputInformation(void)
{
  itkDebugMacro( << "Generate output informations" );
  Superclass::GenerateOutputInformation();

}

/**
 * Processing.
 */
template <class TInputImage, class TOutputImage>
void SummingFilter<TInputImage, TOutputImage>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                       itk::ThreadIdType threadId )
{

  // Debug info
  itkDebugMacro(<<"Actually executing thread " << threadId
                << " in region " << outputRegionForThread);

  // Support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId,
                                 outputRegionForThread.GetNumberOfPixels() );

  // Iterate through the thread region
  OutputIteratorType outputIt(this->GetOutput(), outputRegionForThread);

  // Prepare input iterators
  unsigned int nbInputImages = this->GetNumberOfInputs();
  unsigned int nbInputBands;

  InputIteratorType inputIt[nbInputImages];
  for (unsigned int i = 0 ; i < nbInputImages ; i++)
    {
    InputImageType * currentImage = const_cast<InputImageType *>(this->GetInput(i) );
    inputIt[i] = InputIteratorType(currentImage, outputRegionForThread);
    nbInputBands = currentImage->GetNumberOfComponentsPerPixel();
    }

  OutputImagePixelType pix;
  pix.SetSize(nbInputBands);

  // Sum images
  for ( outputIt.GoToBegin(); !outputIt.IsAtEnd(); ++outputIt )
    {
    pix.Fill(0.0);
    for (unsigned int i = 0 ; i < nbInputImages ; i++)
      {

      for (unsigned int band = 0 ; band < nbInputBands ; band++)
        {
        pix[band] += inputIt[i].Get()[band];

        }
      ++inputIt[i];
      }
    outputIt.Set(pix);

    // Update process
    progress.CompletedPixel();
    }

}

}

#endif
