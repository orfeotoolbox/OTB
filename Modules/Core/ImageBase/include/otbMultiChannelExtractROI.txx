/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbMultiChannelExtractROI_txx
#define __otbMultiChannelExtractROI_txx

#include "otbMultiChannelExtractROI.h"

#include "itkImageRegionIterator.h"
#include "itkObjectFactory.h"
#include "itkExtractImageFilterRegionCopier.h"
#include "itkProgressReporter.h"

namespace otb
{

/**
 *
 */
template<class TInputPixelType, class TOutputPixelType>
MultiChannelExtractROI<TInputPixelType, TOutputPixelType>
::MultiChannelExtractROI() :
  ExtractROIBase<VectorImage<TInputPixelType, 2>, VectorImage<TOutputPixelType, 2> >(),
  m_FirstChannel(0),
  m_LastChannel(0),
  m_ChannelsKind(0)
{
  ClearChannels();
}

/**
 *
 */
template<class TInputPixelType, class TOutputPixelType>
void
MultiChannelExtractROI<TInputPixelType, TOutputPixelType>
::SetChannel(unsigned int channel)
{
  if (m_ChannelsKind == 1)
    {
    itkExceptionMacro(<< "m_Channels already set using channels interval.");
    }
  m_Channels.push_back(channel);
  if (m_ChannelsKind == 0)
    {
    m_ChannelsKind = 2;
    }
  this->Modified();
}

template<class TInputPixelType, class TOutputPixelType>
void
MultiChannelExtractROI<TInputPixelType, TOutputPixelType>
::SetFirstChannel(unsigned int id)
{
  if (m_ChannelsKind == 2)
    {
    itkExceptionMacro(<< "m_Channels already set using SetChannels method.");
    }
  m_FirstChannel = id;
  if (m_ChannelsKind == 0)
    {
    m_ChannelsKind = 1;
    }
  this->Modified();
}

template<class TInputPixelType, class TOutputPixelType>
void
MultiChannelExtractROI<TInputPixelType, TOutputPixelType>
::SetLastChannel(unsigned int id)
{
  if (m_ChannelsKind == 2)
    {
    itkExceptionMacro(<< "m_Channels already set using SetChannels method.");
    }
  m_LastChannel = id;
  if (m_ChannelsKind == 0)
    {
    m_ChannelsKind = 1;
    }
  this->Modified();
}

/**
 *
 */
template<class TInputPixelType, class TOutputPixelType>
void
MultiChannelExtractROI<TInputPixelType, TOutputPixelType>
::ClearChannels(void)
{
  m_FirstChannel = 0;
  m_LastChannel  = 0;
  m_Channels.clear();
  m_ChannelsKind = 0;
  m_ChannelsWorks.clear();
}

/**
 *
 */
template<class TInputPixelType, class TOutputPixelType>
void
MultiChannelExtractROI<TInputPixelType, TOutputPixelType>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

template<class TInputPixelType, class TOutputPixelType>
void
MultiChannelExtractROI<TInputPixelType, TOutputPixelType>
::ChannelsReInitialization()
{
  // The following conditions can be gathered but we'd loose in comprehension
  m_ChannelsWorks.clear();
  // First passage in the method:
  if (m_Channels.empty() == true)
    {
    // - User SetFirst/LastChannel()
    if (m_ChannelsKind == 1)
      {
      this->SetChannelsWorkWithLimits();
      }
    else
      {
      // - User called SetChannels()
      if (m_Channels.empty() == true && m_ChannelsKind == 2)
        {
        m_ChannelsWorks = m_Channels;
        }
      }
    }
  // Second passage in the method: Update already donne
  else
    {
    // - User SetFirst/LastChannel()
    if (m_ChannelsKind == 1)
      {
      m_Channels.clear();
      this->SetChannelsWorkWithLimits();
      }
    else
      {
      // - User called SetChannels()
      if (m_ChannelsKind == 2)
        {
        m_ChannelsWorks = m_Channels;
        }
      }
    }
}

template<class TInputPixelType, class TOutputPixelType>
void
MultiChannelExtractROI<TInputPixelType, TOutputPixelType>
::SetChannelsWorkWithLimits()
{
  if ((m_FirstChannel == 0) || (m_LastChannel == 0))
    {
    itkExceptionMacro(<< "otb::ExtractImageFilter::GenerateOutputInformation "
                      << "Channels must reside into [1...] "
                      << typeid(itk::ImageBase<InputImageDimension>*).name());
    }
  if (m_FirstChannel > m_LastChannel)
    {
    itkExceptionMacro(<< "otb::ExtractImageFilter::GenerateOutputInformation "
                      << "FirstChannel is greater than LastChannel"
                      << typeid(itk::ImageBase<InputImageDimension>*).name());
    }

  for (unsigned int channel = m_FirstChannel; channel <= m_LastChannel; channel++)
    {
    m_ChannelsWorks.push_back(channel);
    }

  m_Channels = m_ChannelsWorks;
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
MultiChannelExtractROI<TInputPixelType, TOutputPixelType>
::GenerateOutputInformation()
{
  // Call to the superclass implementation
  Superclass::GenerateOutputInformation();
  this->ChannelsReInitialization();

  typename Superclass::InputImageConstPointer inputPtr  = this->GetInput();
  typename Superclass::OutputImagePointer     outputPtr = this->GetOutput();

  unsigned int nbComponentsPerPixel = inputPtr->GetNumberOfComponentsPerPixel();
  if (m_ChannelsKind != 0)
    {
    // Test if the asked channels index exists in the input image
    ChannelsType m_BadChannels;
    m_BadChannels.clear();
    for (unsigned int i = 0; i < m_ChannelsWorks.size(); ++i)
      {
      if ((m_ChannelsWorks[i] < 1) || (m_ChannelsWorks[i] > nbComponentsPerPixel))
        {
        bool isInsideBadChannels = false;
        for (unsigned int j = 0; j < m_BadChannels.size(); ++j)
          {
          if (m_BadChannels[j] == m_ChannelsWorks[i]) isInsideBadChannels = true;

          }
        if (!isInsideBadChannels) m_BadChannels.push_back(m_ChannelsWorks[i]);
        }
      }
    if (m_BadChannels.empty() == false)
      {
      std::ostringstream oss;
      oss << "otb::ExtractImageFilter::GenerateOutputInformation : ";
      oss <<  "Channel(s) [ ";
      for (unsigned int i = 0; i < m_BadChannels.size(); ++i)
        {
        oss << m_BadChannels[i] << " ";
        }
      oss << "] not authorized.";
      oss << " Each channel index has to be in [1," << nbComponentsPerPixel << "].";
      itkExceptionMacro(<< oss.str().c_str());
      }
    nbComponentsPerPixel = m_ChannelsWorks.size();
    }

  // initialize the number of channels of the output image
  outputPtr->SetNumberOfComponentsPerPixel(nbComponentsPerPixel);
}

template<class TInputPixelType, class TOutputPixelType>
void
MultiChannelExtractROI<TInputPixelType, TOutputPixelType>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                       itk::ThreadIdType threadId)
{
  itkDebugMacro(<< "Actually executing");
  // Get the input and output pointers
  typename Superclass::InputImageConstPointer inputPtr = this->GetInput();
  typename Superclass::OutputImagePointer     outputPtr = this->GetOutput();

  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  // Define the portion of the input to walk for this thread
  InputImageRegionType inputRegionForThread;
  this->CallCopyOutputRegionToInputRegion(inputRegionForThread, outputRegionForThread);

  // Define the iterators.
  typedef itk::ImageRegionIterator<OutputImageType>     OutputIterator;
  typedef itk::ImageRegionConstIterator<InputImageType> InputIterator;

  OutputIterator outIt(outputPtr, outputRegionForThread);
  InputIterator inIt(inputPtr, inputRegionForThread);

  outIt.GoToBegin();
  inIt.GoToBegin();

  // if default behaviour
  if (m_ChannelsKind == 0)
    {
    // walk the output region, and sample the input image
    while (!outIt.IsAtEnd())
      {
      outIt.Set(inIt.Get());
      ++outIt;
      ++inIt;
      progress.CompletedPixel();
      }
    }
  // Specific behaviour
  else
    {
    // for each channel to process
    unsigned int channelIn(0);
    unsigned int channelOut(0);
    unsigned int nbChannels(0);

    InputImagePixelType pixelInput;
    while (!outIt.IsAtEnd())
      {
      OutputImagePixelType pixelOutput;
      pixelOutput.Reserve(outputPtr->GetVectorLength());
      pixelInput = inIt.Get();
      channelOut = 0;
      for (nbChannels = 0; nbChannels < m_ChannelsWorks.size(); ++nbChannels)
        {
        channelIn = m_ChannelsWorks[nbChannels] - 1;
        pixelOutput[channelOut] = static_cast<OutputValueType>(pixelInput[channelIn]);
        channelOut++;
        }
      outIt.Set(pixelOutput);
      ++outIt;
      ++inIt;
      progress.CompletedPixel();
      }
    }
}

} // end namespace otb

#endif
