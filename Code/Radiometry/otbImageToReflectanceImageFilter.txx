
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
#include "otbImageToReflectanceImageFilter.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"

namespace otb
{
  
  template <class TInputImage, class TOutputImage>
  ImageToReflectanceImageFilter<TInputImage, TOutputImage>
  ::ImageToReflectanceImageFilter()
  {
    m_Alpha.Fill(0);
    m_Beta.Fill(0);
    m_ZenithalSolarRadius = 1.;
    m_FluxNormalizationCoefficient = 1.;
    m_SolarIllumination.Fill(1.);
    m_IsSetFluxNormalizationCoefficient = false;
    m_Day = 1;
    m_Mounth = 1;
  }
  
  
  template <class TInputImage, class TOutputImage>
  ImageToReflectanceImageFilter<TInputImage, TOutputImage>
  ::~ImageToReflectanceImageFilter()
  {
  }
  
  /**
   * ThreadedGenerateData Performs the pixel-wise addition
   */
  template <class TInputImage, class TOutputImage>
  void
  ImageToReflectanceImageFilter<TInputImage,TOutputImage>
  ::ThreadedGenerateData( const OutputImageRegionType &outputRegionForThread, int threadId )
  {
    typename InputImageType::Pointer  inputPtr  = const_cast<InputImageType *>(this->GetInput());
    typename OutputImageType::Pointer outputPtr = static_cast<OutputImageType *>(this->GetOutput());
  
    // Define the portion of the input to walk for this thread, using
    // the CallCopyOutputRegionToInputRegion method allows for the input
    // and output images to be different dimensions
    
    InputImageRegionType inputRegionForThread;
    this->CallCopyOutputRegionToInputRegion(inputRegionForThread, outputRegionForThread);
    
    // Define the iterators
    itk::ImageRegionConstIterator<InputImageType>  inputIt(inputPtr, inputRegionForThread);
    itk::ImageRegionIterator<OutputImageType>      outputIt(outputPtr, outputRegionForThread);
    
    itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());
    
    inputIt.GoToBegin();
    outputIt.GoToBegin();
    
    double coefTemp = 0.;  

    if (!m_IsSetFluxNormalizationCoefficient)
      {
	if (m_Day*m_Mounth != 0 && m_Day<32 && m_Mounth<12)
	  {
	    otb_6s_real dsol = 0.;
	    otb_6s_integer day = static_cast<otb_6s_integer>(m_Day);
	    otb_6s_integer mounth = static_cast<otb_6s_integer>(m_Mounth);
	    int cr(0);
            cr = otb_6s_varsol_(&day, &mounth, &dsol);
	    coefTemp = vcl_cos(m_ZenithalSolarRadius)*static_cast<double>(dsol);
	  }
	else
	  {
	    itkExceptionMacro( << "Day has to be included between 1 and 31, Mounth beetween 1 and 12.");
	  }
      }
    else
      {
	coefTemp = vcl_cos(m_ZenithalSolarRadius)*m_FluxNormalizationCoefficient*m_FluxNormalizationCoefficient;
      }

    this->GetFunctor().SetIlluminationCorrectionCoefficient(1. / coefTemp);
    
    while( !inputIt.IsAtEnd() ) 
      {
	InputPixelType inPixel = inputIt.Get();
	OutputPixelType outPixel = outputIt.Get();
	
	for (unsigned int j=0; j<inPixel.GetSize(); j++)
	  {
	    this->GetFunctor().SetAlpha(static_cast<double>(m_Alpha[j]));
	    this->GetFunctor().SetBeta(static_cast<double>(m_Beta[j]));
	    this->GetFunctor().SetSolarIllumination(static_cast<double>(m_SolarIllumination[j]));
	    outPixel[j] = this->GetFunctor()( inPixel[j] );
	  }

	outputIt.Set(outPixel);
	++inputIt;
	++outputIt;
	progress.CompletedPixel();  // potential exception thrown here    
      }
    
  }
} // end namespace otb
