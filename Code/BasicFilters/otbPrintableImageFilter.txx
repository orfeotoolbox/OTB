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
#ifndef __otbPrintableImageFilter_txx
#define __otbPrintableImageFilter_txx

#include "otbPrintableImageFilter.h"

#include "otbImageFileWriter.h"

namespace otb
{

template <class TInputImage, class TMaskImage>
PrintableImageFilter<TInputImage, TMaskImage>
::PrintableImageFilter()
{

  m_Rescaler = VectorRescalerType::New();
  m_Extractor = ChannelExtractorType::New();
  
  m_Rescaler->SetInput( m_Extractor->GetOutput() );
  
  m_MaskRescaler = RescalerFilterType::New();
  m_InvertIntensityImageFilter = InvertIntensityImageFilterType::New();
  m_Multiplier =  MultiplierType::New();
  m_MaskCaster = CasterFilterType::New();
  m_Adder = AdderFilterType::New();
  
  m_ChannelList = ChannelsType(3,2);
  m_ChannelList[1] = 3;
  m_ChannelList[2] = 4;
  m_ObjectColor.SetSize( 3 );
  m_ObjectColor.Fill(255);

  m_ForegroundMaskValue = 1;
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
const typename PrintableImageFilter<TInputImage,TMaskImage>::ChannelsType
PrintableImageFilter<TInputImage, TMaskImage>
::GetChannels() const
{
  return m_ChannelList;//m_Extractor->GetChannels();
}

template <class TInputImage, class TMaskImage>
void
PrintableImageFilter<TInputImage, TMaskImage>
::SetInputMask(const MaskImageType * mask)
{
  this->itk::ProcessObject::SetNthInput(1,const_cast<MaskImageType *>(mask));
  m_UseMask = true;
}


template <class TInputImage, class TMaskImage>
typename PrintableImageFilter<TInputImage,TMaskImage>::MaskImageType *
PrintableImageFilter<TInputImage, TMaskImage>
::GetInputMask()
{
  if (this->GetNumberOfInputs()<2)
  {
    return 0;
  }
  return static_cast<MaskImageType *>(this->itk::ProcessObject::GetInput(1));
}


template <class TInputImage, class TMaskImage>
void
PrintableImageFilter<TInputImage, TMaskImage>
::BeforeGenerateData()
{
  if(m_Extractor->GetNbChannels() == 0 && m_ChannelList.size()>3)
    {
      itkExceptionMacro(<<"Invalid channel list (must be three channels instead of "<<m_ChannelList.size());
    }
  if(m_Extractor->GetNbChannels() == 0 && m_Extractor->GetNbChannels()>3)
    {
      itkExceptionMacro(<<"Invalid channel list (must be three channels instead of "<<m_Extractor->GetNbChannels());
    }
  if( m_ObjectColor.GetSize() != 3 )
    {
      itkExceptionMacro(<<"Invalid m_ObjectColor pixel size");
    }
  
  if( m_UseMask == true )
    {
      MaskImagePointerType inputMaskPtr  = this->GetInputMask();
      typename InputImageType::ConstPointer inputPtr = this->GetInput();
 
      if( !inputMaskPtr )
	{
	  itkExceptionMacro(<<"No mask detected");
	}
      if( inputMaskPtr->GetLargestPossibleRegion().GetSize() !=  inputPtr->GetLargestPossibleRegion().GetSize())
	{
	  itkExceptionMacro(<<"Input size ("<<inputPtr->GetLargestPossibleRegion().GetSize()<<") and Mask size ("<<inputMaskPtr->GetLargestPossibleRegion().GetSize()<<") must be the same");
	}
    }
}

template <class TInputImage, class TMaskImage>
void
PrintableImageFilter<TInputImage, TMaskImage>
::GenerateData()
{
  this->BeforeGenerateData();

  // let this loop to be compliant with previous version of the class where m_ChannelList didn't exist...
  
  if(m_Extractor->GetNbChannels() == 0)
    {
      for(unsigned int i=0; i<m_ChannelList.size(); i++)
      m_Extractor->SetChannel( m_ChannelList[i]);
    }
  
  
  m_Extractor->SetInput(this->GetInput());
  m_Extractor->UpdateOutputInformation();

  typename TInputImage::PixelType minimum,maximum;
  minimum.SetSize(m_Extractor->GetNbChannels());
  maximum.SetSize(m_Extractor->GetNbChannels());
  minimum.Fill(0);
  maximum.Fill(255);

  m_Rescaler->SetOutputMinimum(minimum);
  m_Rescaler->SetOutputMaximum(maximum);
  m_Rescaler->SetClampThreshold(0.01);

  if( m_UseMask == false )
    {
      m_Rescaler->GraftOutput( this->GetOutput() );
      m_Rescaler->Update();
      this->GraftOutput( m_Rescaler->GetOutput() );
    }
  else
    {
      //m_InvertIntensityImageFilter->SetMaximum(m_ForegroundMaskValue);
      m_InvertIntensityImageFilter->SetInput(this->GetInputMask());
      if(m_ForegroundMaskValue>m_BackgroundMaskValue)
	{
	  m_InvertIntensityImageFilter->SetInsideValue(0);
	  m_InvertIntensityImageFilter->SetOutsideValue(1);
	}
      else
	{
	  m_InvertIntensityImageFilter->SetInsideValue(1);
	  m_InvertIntensityImageFilter->SetOutsideValue(0);
	}
      
      MaskPixelType thresh = static_cast<MaskPixelType>(static_cast<double>(m_ForegroundMaskValue+m_BackgroundMaskValue)/2);
      m_InvertIntensityImageFilter->SetLowerThreshold(thresh);
      m_InvertIntensityImageFilter->SetUpperThreshold(itk::NumericTraits<MaskPixelType>::max());

      typedef ImageFileWriter<MaskImageType> W44;
      typename W44::Pointer w4 = W44::New();
      w4->SetFileName("invert.tif");
      w4->SetInput(m_InvertIntensityImageFilter->GetOutput());
      w4->Update();
      
      
      m_Multiplier->SetInput1( m_Rescaler->GetOutput());
      m_Multiplier->SetInput2( m_InvertIntensityImageFilter->GetOutput());
      m_Multiplier->UpdateOutputInformation();
      
      typedef ImageFileWriter<OutputImageType> W00;
      W00::Pointer w00 = W00::New();
      w00->SetFileName("multiplier.tif");
      w00->SetInput(m_Multiplier->GetOutput());
      w00->Update();

      typename ImageListType::Pointer imList = ImageListType::New();
      for(unsigned int i=0; i<m_Extractor->GetNbChannels(); i++)
	imList->PushBack( this->GetInputMask());

      m_MaskCaster->SetInput(imList);

      typedef ImageFileWriter<OutputImageType> W1;
      W1::Pointer w1 = W1::New();
      w1->SetFileName("maskcaster.tif");
      w1->SetInput(m_MaskCaster->GetOutput());
      w1->Update();

      OutputPixelType mini,maxi;
      mini.SetSize(m_Extractor->GetNbChannels());
      mini.Fill( std::min(m_BackgroundMaskValue, m_ForegroundMaskValue));
      maxi = m_ObjectColor;
      //maxi.SetSize(m_Extractor->GetNbChannels());
      //maxi.Fill( 255 );
      
      m_MaskRescaler->SetOutputMinimum(mini);
      m_MaskRescaler->SetOutputMaximum(maxi);
      m_MaskRescaler->SetClampThreshold(1e-6);
      m_MaskRescaler->SetInput(m_MaskCaster->GetOutput());

      typedef ImageFileWriter<OutputImageType> W2;
      W2::Pointer w2 = W2::New();
      w2->SetFileName("maskrescaler.tif");
      w2->SetInput(m_MaskRescaler->GetOutput());
      w2->Update();

      m_Adder->SetInput1( m_Multiplier->GetOutput()  );
      m_Adder->SetInput2( m_MaskRescaler->GetOutput() );


      m_Adder->GraftOutput( this->GetOutput() );
 typedef ImageFileWriter<OutputImageType> W12;
      W12::Pointer w12 = W12::New();
      w12->SetFileName("adder.tif");
      w12->SetInput(m_Adder->GetOutput());
      w12->Update();
      m_Adder->Update();
std::cout<<"update3"<<std::endl;
      this->GraftOutput( m_Adder->GetOutput() );
    }
  
}

template <class TInputImage, class TMaskImage>
void
PrintableImageFilter<TInputImage, TMaskImage>
::PrintSelf( std::ostream& os, itk::Indent indent ) const
{
  Superclass::PrintSelf(os,indent);
}

} // end namespace otb

#endif
