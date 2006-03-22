#ifndef _otbMultiToMonoChannelExtractROI_txx
#define _otbMultiToMonoChannelExtractROI_txx

#include "otbMultiToMonoChannelExtractROI.h"

namespace otb
{

/**
 *
 */
template<class TInputPixelType, class TOutputPixelType>
MultiToMonoChannelExtractROI<TInputPixelType,TOutputPixelType>
::MultiToMonoChannelExtractROI() :    ExtractROIBase< itk::VectorImage<TInputPixelType,2> , itk::Image<TOutputPixelType,2> >(),
                                m_Channel(1)
{

}

/**
 *
 */
template<class TInputPixelType, class TOutputPixelType>
void 
MultiToMonoChannelExtractROI<TInputPixelType,TOutputPixelType>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}


/** 
 * ExtractImageFilter can produce an image which is a different resolution
 * than its input image.  As such, ExtractImageFilter needs to provide an
 * implementation for GenerateOutputInformation() in order to inform
 * the pipeline execution model.  The original documentation of this
 * method is below.
 *
 * \sa ProcessObject::GenerateOutputInformaton() 
 */
template<class TInputPixelType, class TOutputPixelType>
void 
MultiToMonoChannelExtractROI<TInputPixelType,TOutputPixelType>
::GenerateOutputInformation()
{
        typename Superclass::InputImageConstPointer  inputPtr = this->GetInput();
        // Analyse du canal traité
        if ( (m_Channel <= 0) || (m_Channel > inputPtr->GetVectorLength() ) )
        {
                        itkExceptionMacro(<< "otb::ExtractImMultiToMonoChannelExtractROIageFilter::GenerateOutputInformation "
                      << "le canal a traiter doivent etre dans l'intervalle [1;"<<inputPtr->GetVectorLength()<<"] "
                      << typeid(itk::ImageBase<InputImageDimension>*).name() );
        }

        // Appel à la methode de la classe de base
        Superclass::GenerateOutputInformation();
}


template<class TInputPixelType, class TOutputPixelType>
void 
MultiToMonoChannelExtractROI<TInputPixelType,TOutputPixelType>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                       int threadId)
{
  itkDebugMacro(<<"Actually executing");

  // Get the input and output pointers
  typename Superclass::InputImageConstPointer  inputPtr = this->GetInput();
  typename Superclass::OutputImagePointer outputPtr = this->GetOutput();

  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());
  
  // Define the portion of the input to walk for this thread
  InputImageRegionType inputRegionForThread;
  this->CallCopyOutputRegionToInputRegion(inputRegionForThread, outputRegionForThread);
  
  // Define the iterators.
  typedef itk::ImageRegionIterator<OutputImageType> OutputIterator;
  typedef itk::ImageRegionConstIterator<InputImageType> InputIterator;

  OutputIterator outIt(outputPtr, outputRegionForThread);
  InputIterator inIt(inputPtr, inputRegionForThread);

  // Parcours des canaux a traiter
  unsigned int channelIn(m_Channel-1);

  InputImagePixelType  pixelInput;
  while( !outIt.IsAtEnd() )
  {
                OutputImagePixelType pixelOutput;
                pixelInput = inIt.Get();
                pixelOutput = pixelInput[channelIn];
                outIt.Set( pixelOutput );
                ++outIt; 
                ++inIt; 
                progress.CompletedPixel();
  }

}


} // end namespace otb

#endif
