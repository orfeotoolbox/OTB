#ifndef _otbMultiChannelExtractROI_txx
#define _otbMultiChannelExtractROI_txx

#include "otbMultiChannelExtractROI.h"

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkObjectFactory.h"
#include "itkExtractImageFilterRegionCopier.h"
#include "itkProgressReporter.h"


namespace otb
{

/**
 *
 */
template<class TInputPixelType, class TOutputPixelType>
MultiChannelExtractROI<TInputPixelType,TOutputPixelType>
::MultiChannelExtractROI() :    ExtractROIBase< itk::VectorImage<TInputPixelType,2> , itk::VectorImage<TOutputPixelType,2> >(),
                                m_FirstChannel(0),
                                m_LastChannel(0),
                                m_ChannelsWorksBool(false)
{
        ClearChannels();
}


/**
 *
 */
template<class TInputPixelType, class TOutputPixelType>
void
MultiChannelExtractROI<TInputPixelType,TOutputPixelType>
::SetChannel(unsigned int channel)
{
        m_Channels.push_back( channel );
}
/**
 *
 */
template<class TInputPixelType, class TOutputPixelType>
void
MultiChannelExtractROI<TInputPixelType,TOutputPixelType>
::ClearChannels(void)
{
        m_FirstChannel = 0;
        m_LastChannel  = 0;
        m_Channels.clear();
        m_ChannelsWorksBool = false;
}



/**
 *
 */
template<class TInputPixelType, class TOutputPixelType>
void 
MultiChannelExtractROI<TInputPixelType,TOutputPixelType>
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
MultiChannelExtractROI<TInputPixelType,TOutputPixelType>
::GenerateOutputInformation()
{
        // Analyse des canuax a traiter
        ChannelsType Channels;
        if( m_Channels.empty() == false )
        {
                m_ChannelsWorks = m_Channels;
        }
        // Si l'utilisateur a defini l'une de ces bornes, on les prend en compte
        if ( (m_LastChannel != 0) && (m_FirstChannel != 0) )
        {
                if ((m_FirstChannel == 0) || (m_LastChannel == 0))
                {
                        itkExceptionMacro(<< "otb::ExtractImageFilter::GenerateOutputInformation "
                      << "les canaux a traiter doivent etre dans l'intervalle [1...] "
                      << typeid(itk::ImageBase<InputImageDimension>*).name() );
                }
                if ( m_FirstChannel > m_LastChannel )
                {
                        itkExceptionMacro(<< "otb::ExtractImageFilter::GenerateOutputInformation "
                      << "le canal specifie par la methode 'SetFirstChannel' est plus grand que le canal specifie par la methode 'SetLastChannel'"
                      << typeid(itk::ImageBase<InputImageDimension>*).name() );
                }
                
                for( int channel = m_FirstChannel ; channel <= m_LastChannel ; channel ++ )
                {
                        m_ChannelsWorks.push_back(channel);
                }
        }
        
        // Si aucun canal n'a ete precise, alors tous les canaux sont traites
        if( m_ChannelsWorks.empty() == true )
        {
                m_ChannelsWorksBool = false;
        }
        else
        {
                m_ChannelsWorksBool = true;
        }

        typename Superclass::InputImageConstPointer  inputPtr = this->GetInput();
        typename Superclass::OutputImagePointer outputPtr = this->GetOutput();

        // initialise le nombre de canaux de l'image output (lié aux "channel" sélectionnés)
        if( m_ChannelsWorksBool == false )
        {
                outputPtr->SetVectorLength(inputPtr->GetVectorLength() );
        }
        else
        {
                outputPtr->SetVectorLength( m_ChannelsWorks.size() );
        }

	outputPtr->SetNumberOfComponentsPerPixel( outputPtr->GetVectorLength() );

        // Appel à la methode de la classe de base
        Superclass::GenerateOutputInformation();

}


template<class TInputPixelType, class TOutputPixelType>
void 
MultiChannelExtractROI<TInputPixelType,TOutputPixelType>
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

int cpt(0);
  // Si traitement classique
  if ( m_ChannelsWorksBool == false )
  {
        // walk the output region, and sample the input image
        while( !outIt.IsAtEnd() )
        {
        cpt++;
//std::cout << cpt << std::endl;
        
                outIt.Set( inIt.Get() );
                ++outIt; 
                ++inIt; 
                progress.CompletedPixel();
        }
  }
  // Traitement particulier des canaux 
  else
  {
        // Parcours des canaux a traiter
        unsigned int channelIn(0);
        unsigned int channelOut(0);
        unsigned int nbChannels(0);

        InputImagePixelType  pixelInput;
        while( !outIt.IsAtEnd() )
        {
                OutputImagePixelType pixelOutput;
                pixelOutput.Reserve( outputPtr->GetVectorLength() );
                pixelInput = inIt.Get();
                channelOut = 0;
                for ( nbChannels = 0 ; nbChannels < m_ChannelsWorks.size() ; nbChannels++)
                {
                        channelIn = m_ChannelsWorks[nbChannels] - 1;
                        pixelOutput[channelOut] = pixelInput[channelIn];
                        channelOut++;
                }
                outIt.Set( pixelOutput );
                ++outIt; 
                ++inIt; 
                progress.CompletedPixel();
        }
  }

}


} // end namespace otb

#endif
