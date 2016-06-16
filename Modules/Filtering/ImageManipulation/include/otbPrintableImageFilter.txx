/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef otbPrintableImageFilter_txx
#define otbPrintableImageFilter_txx

#include "otbPrintableImageFilter.h"

namespace otb
{

template <class TInputImage, class TMaskImage>
PrintableImageFilter<TInputImage, TMaskImage>
::PrintableImageFilter()
{

  m_Rescaler = VectorRescalerType::New();
  m_Extractor = ChannelExtractorType::New();

  m_Rescaler->SetInput(m_Extractor->GetOutput());
  m_MaskFilter = FunctorFilterType::New();

  m_ChannelList = ChannelsType(3, 2);
  m_ChannelList[1] = 3;
  m_ChannelList[2] = 4;
  m_ObjectColor.SetSize(3);
  m_ObjectColor.Fill(255);
  m_UseMask = false;
  m_BackgroundMaskValue = 0;
}

template <class TInputImage, class TMaskImage>
void
PrintableImageFilter<TInputImage, TMaskImage>
::SetChannel(unsigned int channel)
{
  m_ChannelList.push_back(channel);
  m_Extractor->SetChannel(channel);
  this->Modified();
}

template <class TInputImage, class TMaskImage>
const typename PrintableImageFilter<TInputImage, TMaskImage>::ChannelsType
PrintableImageFilter<TInputImage, TMaskImage>
::GetChannels() const
{
  return m_ChannelList;
}

template <class TInputImage, class TMaskImage>
void
PrintableImageFilter<TInputImage, TMaskImage>
::SetInputMask(const MaskImageType * mask)
{
  this->itk::ProcessObject::SetNthInput(1, const_cast<MaskImageType *>(mask));
  m_UseMask = true;
}

template <class TInputImage, class TMaskImage>
typename PrintableImageFilter<TInputImage, TMaskImage>::MaskImageType *
PrintableImageFilter<TInputImage, TMaskImage>
::GetInputMask()
{
  if (this->GetNumberOfInputs() < 2)
    {
    return ITK_NULLPTR;
    }
  return static_cast<MaskImageType *>(this->itk::ProcessObject::GetInput(1));
}

template <class TInputImage, class TMaskImage>
void
PrintableImageFilter<TInputImage, TMaskImage>
::GenerateOutputInformation()
{
  // Call to the superclass implementation
  Superclass::GenerateOutputInformation();

  typename Superclass::OutputImagePointer outputPtr = this->GetOutput();
  outputPtr->SetNumberOfComponentsPerPixel(3);
}

template <class TInputImage, class TMaskImage>
void
PrintableImageFilter<TInputImage, TMaskImage>
::BeforeGenerateData()
{
  if (m_Extractor->GetNbChannels() == 0 && m_ChannelList.size() > 3)
    {
    itkExceptionMacro(<< "Invalid channel list (must be three channels instead of " << m_ChannelList.size());
    }
  if (m_Extractor->GetNbChannels() == 0 && m_Extractor->GetNbChannels() > 3)
    {
    itkExceptionMacro(<< "Invalid channel list (must be three channels instead of " << m_Extractor->GetNbChannels());
    }
  if (m_ObjectColor.GetSize() != 3)
    {
    itkExceptionMacro(<< "Invalid m_ObjectColor pixel size");
    }

  if (m_UseMask == true)
    {
    MaskImagePointerType                  inputMaskPtr  = this->GetInputMask();
    typename InputImageType::ConstPointer inputPtr = this->GetInput();

    if (!inputMaskPtr)
      {
      itkExceptionMacro(<< "No mask detected");
      }
    if (inputMaskPtr->GetLargestPossibleRegion().GetSize() !=  inputPtr->GetLargestPossibleRegion().GetSize())
      {
      itkExceptionMacro(
        << "Input size (" << inputPtr->GetLargestPossibleRegion().GetSize() << ") and Mask size (" <<
        inputMaskPtr->GetLargestPossibleRegion().GetSize() << ") must be the same");
      }
    }
}

/*
 * If no mask used, just rescale the input between 0 and 255.
 * Else, sur M%askFunctor.
 */
template <class TInputImage, class TMaskImage>
void
PrintableImageFilter<TInputImage, TMaskImage>
::GenerateData()
{
  this->BeforeGenerateData();

  // let this loop to be compliant with previous version of the class where m_ChannelList didn't exist...
  if (m_Extractor->GetNbChannels() == 0)
    {
    for (unsigned int i = 0; i < m_ChannelList.size(); ++i)
      m_Extractor->SetChannel(m_ChannelList[i]);
    }

  m_Extractor->SetInput(this->GetInput());
  m_Extractor->UpdateOutputInformation();

  typename TInputImage::PixelType minimum, maximum;
  minimum.SetSize(m_Extractor->GetNbChannels());
  maximum.SetSize(m_Extractor->GetNbChannels());
  minimum.Fill(0);
  maximum.Fill(255);

  m_Rescaler->SetOutputMinimum(minimum);
  m_Rescaler->SetOutputMaximum(maximum);
  m_Rescaler->SetClampThreshold(0.01);

  if (m_UseMask == false)
    {
    m_Rescaler->GraftOutput(this->GetOutput());
    m_Rescaler->Update();
    this->GraftOutput(m_Rescaler->GetOutput());
    }
  else
    {
    m_MaskFilter->SetInput1(m_Rescaler->GetOutput());
    m_MaskFilter->SetInput2(this->GetInputMask());
    m_MaskFilter->GraftOutput(this->GetOutput());
    m_MaskFilter->Update();
    this->GraftOutput(m_MaskFilter->GetOutput());
    }

}

template <class TInputImage, class TMaskImage>
void
PrintableImageFilter<TInputImage, TMaskImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
