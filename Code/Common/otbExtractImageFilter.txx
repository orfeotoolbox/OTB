#ifndef _otbExtractImageFilter_txx
#define _otbExtractImageFilter_txx

#include "itkExtractImageFilter.h"
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
template <class TInputImage, class TOutputImage>
ExtractImageFilter<TInputImage,TOutputImage>
::ExtractImageFilter() :        itk::ExtractImageFilter<TInputImage,TOutputImage>(),
                                m_StartX(0),
                                m_StartY(0),
                                m_SizeX(0),
                                m_SizeY(0),
                                m_FirstChannel(0),
                                m_LastChannel(0),
                                m_ChannelsWorksBool(false)
{
        ClearChannels();
}


/**
 *
 */
template <class TInputImage, class TOutputImage>
void
ExtractImageFilter<TInputImage,TOutputImage>
::SetChannel(unsigned int channel)
{
        m_Channels.push_back( channel );
}
/**
 *
 */
template <class TInputImage, class TOutputImage>
void
ExtractImageFilter<TInputImage,TOutputImage>
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
template <class TInputImage, class TOutputImage>
void 
ExtractImageFilter<TInputImage,TOutputImage>
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
template <class TInputImage, class TOutputImage>
void 
ExtractImageFilter<TInputImage,TOutputImage>
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
                
                for( int channel = m_FirstChannel ; channel <= m_LastChannel ; channel ++ )
                {
                        m_ChannelsWorks.push_back(channel);
                }
        }
        
        // Si aucun canal n'a ete precise, alors tous les canaux sont traites
        if( m_ChannelsWorks.empty() == true )
        {
                //Controle que les dimensions des images Input/Output coincides
/*                if( InputImageDimension != OutputImageDimension )
                {
                        itkExceptionMacro(<< "otb::ExtractImageFilter::GenerateOutputInformation "
                        << "les dimensions des images Input ("<<InputImageDimension<<") et Output ("<<OutputImageDimension<<") ne coincident pas.");
                }*/
                m_ChannelsWorksBool = false;
        }
        else
        {
/*                OutputImagePixelType pixelOutput;
                
                if( m_ChannelsWorks.size() != OutputImageDimension )
                {
                        itkExceptionMacro(<< "otb::ExtractImageFilter::GenerateOutputInformation "
                        << "le PixelType de l'image Output ("<<OutputImageDimension<<") ne coincident pas "
                        << "avec le nombre de canaux a traiter ("<<m_ChannelsWorks.size()<<").");
                
                }*/
                m_ChannelsWorksBool = true;
        }
        
        
        
        // Determine la zone a extraire
        // Si SizeX(Y) est nulle, alors SizeX(Y) est egale à la SizeX(Y) de l'image
        typename Superclass::InputImageConstPointer  inputPtr  = this->GetInput();
        // Recupere Region de l'image d'entree
        const InputImageRegionType& inputRegion = inputPtr->GetRequestedRegion();
        if ( m_SizeX == 0 )
        {
                m_SizeX = inputRegion.GetSize()[1] - m_StartX;
        }
        if ( m_SizeY == 0 )
        {       
                m_SizeY = inputRegion.GetSize()[0] - m_StartY;
        }
  
        InputImageIndexType start;
        start[0] = m_StartX;
        start[1] = m_StartY;
        InputImageSizeType size;
        size[0] = m_SizeX;
        size[1] = m_SizeY;
        InputImageRegionType desiredRegion;
        desiredRegion.SetSize(  size  );
        desiredRegion.SetIndex( start );
        // Appel à la methode de base d'initialisation de la region
        this->SetExtractionRegion( desiredRegion );

        // Appel à la methode de la classe de base
        Superclass::GenerateOutputInformation();

}


template <class TInputImage, class TOutputImage>
void 
ExtractImageFilter<TInputImage,TOutputImage>
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
  typedef itk::ImageRegionIterator<TOutputImage> OutputIterator;
  typedef itk::ImageRegionConstIterator<TInputImage> InputIterator;

  OutputIterator outIt(outputPtr, outputRegionForThread);
  InputIterator inIt(inputPtr, inputRegionForThread);

  // Si traitement classique
  if ( m_ChannelsWorksBool == false )
  {
        // walk the output region, and sample the input image
        while( !outIt.IsAtEnd() )
        {
                // copy the input pixel to the output
                outIt.Set( inIt.Get());
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
        OutputImagePixelType pixelOutput;
        InputImagePixelType  pixelInput;
        while( !outIt.IsAtEnd() )
        {
                pixelInput = inIt.Get();
                for ( nbChannels = 0 ; nbChannels < m_ChannelsWorks.size() ; nbChannels++)
                {
                        channelIn = m_ChannelsWorks[nbChannels] - 1;
                        double toto = (double)( pixelInput[channelIn]);
//                        pixelOutput[channelOut] = pixelInput[channelIn];
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
