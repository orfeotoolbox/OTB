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
#ifndef __otbMorphologicalPyramidMRToMSConverter_txx
#define __otbMorphologicalPyramidMRToMSConverter_txx

#include "otbMorphologicalPyramidMRToMSConverter.h"
#include "otbMorphologicalPyramidResampler.h"
#include "otbMacro.h"

namespace otb
{
  namespace morphologicalPyramid
  {
    /**
     * Constructor
     */
    template <class TInputImage,class TOuputImage>
    MRToMSConverter<TInputImage, TOuputImage>
    ::MRToMSConverter()
    {
      m_InfFiltreFullResolution = OutputImageListType::New();
      m_SupFiltreFullResolution = OutputImageListType::New();
      m_NumberOfIterations = 0;
    }
    /**
     * Main computation method
     */
    template <class TInputImage,class TOuputImage>
    void
    MRToMSConverter<TInputImage, TOuputImage>
    ::GenerateData()
    {
      // typedef of the resampling filter
      typedef otb::morphologicalPyramid::Resampler<InputImageType,OutputImageType> ResamplerType;

      // Definition of the resampler filters
      typename ResamplerType::Pointer resampler;

      // Full resolution size
      typename InputImageType::SizeType frsize = m_SupFiltre->Front()->GetLargestPossibleRegion().GetSize();
      otbMsgDebugMacro(<<"Full resolution size: "<<frsize);

      /// Full resolution details reconstruction  
      for(int i=0; i<m_NumberOfIterations; i++)
	{
	  resampler = ResamplerType::New();
	  resampler->SetSize(frsize);
	  resampler->SetInput(m_SupFiltre->GetNthElement(i));
	  resampler->Update();
	  m_SupFiltreFullResolution->PushBack(resampler->GetOutput());
	  otbMsgDebugMacro(<<"SupFiltre Details reconstruction level "<<i<<" OK");
	  resampler = ResamplerType::New();
	  resampler->SetSize(frsize);
	  resampler->SetInput(m_InfFiltre->GetNthElement(i));
	  resampler->Update();
	  m_InfFiltreFullResolution->PushBack(resampler->GetOutput());
	  otbMsgDebugMacro(<<"InfFiltre Details reconstruction level "<<i<<" OK");
	  resampler = ResamplerType::New();
	  resampler->SetSize(frsize);
	  resampler->SetInput(this->GetInput()->GetNthElement(i));
	  resampler->Update();
	  this->GetOutput()->PushBack(resampler->GetOutput());
	  otbMsgDebugMacro(<<"Analyse Details reconstruction level "<<i<<" OK");	   
	} 
    }
    /**
     * PrintSelf method
     */
    template <class TInputImage,class TOuputImage>
    void
    MRToMSConverter<TInputImage, TOuputImage>
    ::PrintSelf(std::ostream& os, itk::Indent indent) const
    {
      Superclass::PrintSelf(os,indent);
    }
  } // End namespace morphologicalPyramid
} // End namespace otb
#endif
