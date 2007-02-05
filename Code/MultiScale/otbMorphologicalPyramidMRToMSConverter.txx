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
  namespace MorphologicalPyramid
  {
    /**
     * Constructor
     */
    template <class TInputImage,class TOuputImage>
    MRToMSConverter<TInputImage, TOuputImage>
    ::MRToMSConverter()
    {
      this->SetNumberOfRequiredInputs(2);
      this->SetNumberOfRequiredOutputs(2);
      OutputImageListPointerType supFiltre = OutputImageListType::New();
      this->SetNthOutput(0,supFiltre.GetPointer());
      OutputImageListPointerType infFiltre = OutputImageListType::New();
      this->SetNthOutput(1,infFiltre.GetPointer());
      OutputImageListPointerType outputList = OutputImageListType::New();
      this->SetNthOutput(2,outputList.GetPointer());
      OutputImageListPointerType supDeci =   OutputImageListType::New();
      this->SetNthOutput(3,supDeci.GetPointer());
      OutputImageListPointerType infDeci =   OutputImageListType::New();
      this->SetNthOutput(4,infDeci.GetPointer());
    }
    /**
     * Get The inputList at full resolution.
     * \return The analysed image at each pyramid level
     * resampled at full resolution.
     */
    template <class TInputImage, class TOutputImage>
    typename MRToMSConverter<TInputImage,TOutputImage>
    ::OutputImageListType*
    MRToMSConverter<TInputImage,TOutputImage>
    ::GetOutput(void)
    {
      return dynamic_cast<OutputImageListType*>(this->itk::ProcessObject::GetOutput(2));
    }
    /**
     * Get The SupFiltre details at full resolution.
     * \return The brighter details extracted from the filtering operation.
     * resampled at full resolution.
     */
    template <class TInputImage, class TOutputImage>
    typename MRToMSConverter<TInputImage,TOutputImage>
    ::OutputImageListType*
    MRToMSConverter<TInputImage,TOutputImage>
    ::GetSupFiltreFullResolution(void)
    {
      return dynamic_cast<OutputImageListType*>(this->itk::ProcessObject::GetOutput(0));
    }
    /**
     * Get The InfFiltre details at full resolution.
     * \return The darker details extracted from the filtering operation.
     * resampled at full resolution.
     */
    template <class TInputImage, class TOutputImage>
    typename MRToMSConverter<TInputImage,TOutputImage>
    ::OutputImageListType*
    MRToMSConverter<TInputImage,TOutputImage>
    ::GetInfFiltreFullResolution(void)
    {
      return dynamic_cast<OutputImageListType*>(this->itk::ProcessObject::GetOutput(1));
    }
    /**
     * Get The SupDeci details at full resolution.
     * \return The brighter details extracted from the resampling operation.
     * resampled at full resolution.
     */
    template <class TInputImage, class TOutputImage>
    typename MRToMSConverter<TInputImage,TOutputImage>
    ::OutputImageListType*
    MRToMSConverter<TInputImage,TOutputImage>
    ::GetSupDeciFullResolution(void)
    {
      return dynamic_cast<OutputImageListType*>(this->itk::ProcessObject::GetOutput(3));
    }
    /**
     * Get The InfDeci details  at full resolution.
     * \return The brighter details extracted from the resampling operation.
     * resampled at full resolution.
     */
    template <class TInputImage, class TOutputImage>
    typename MRToMSConverter<TInputImage,TOutputImage>
    ::OutputImageListType*
    MRToMSConverter<TInputImage,TOutputImage>
    ::GetInfDeciFullResolution(void)
    {
      return dynamic_cast<OutputImageListType*>(this->itk::ProcessObject::GetOutput(4));
    }
    /**
     * Set The input list.
     * \param imageList The analysed image at each level of the pyramid.
     */
    template <class TInputImage, class TOutputImage>
    void 
    MRToMSConverter<TInputImage,TOutputImage>
    ::SetInput(InputImageListType * imageList)
    {
      this->SetNthInput(2,const_cast<InputImageListType *>(imageList));
    }
    /**
     * Set The SupFiltre details
     * \param imageList The brighter details extracted from the filtering operation.
     */
    template <class TInputImage, class TOutputImage>
    void 
    MRToMSConverter<TInputImage,TOutputImage>
    ::SetSupFiltre(InputImageListType * imageList)
    {
      this->SetNthInput(0,const_cast<InputImageListType *>(imageList));
    }
    /**
     * Set The InfFiltre details
     * \param imageList The darker details extracted from the filtering operation.
     */
    template <class TInputImage, class TOutputImage>
    void 
    MRToMSConverter<TInputImage,TOutputImage>
    ::SetInfFiltre(InputImageListType * imageList)
    {
      this->SetNthInput(1,const_cast<InputImageListType *>(imageList));
    }
    /**
     * Set The SupDeci details
     * \param imageList The brighter details extracted from the filtering operation.
     */
    template <class TInputImage, class TOutputImage>
    void 
    MRToMSConverter<TInputImage,TOutputImage>
    ::SetSupDeci(InputImageListType * imageList)
    {
      this->SetNthInput(3,const_cast<InputImageListType *>(imageList));
    }
    /**
     * Set The InfDeci details
     * \param imageList The darker details extracted from the filtering operation.
     */
    template <class TInputImage, class TOutputImage>
    void 
    MRToMSConverter<TInputImage,TOutputImage>
    ::SetInfDeci(InputImageListType * imageList)
    {
      this->SetNthInput(4,const_cast<InputImageListType *>(imageList));
    }
    /**
     * Get the input list.
     * \return The list of the analysed image at each pyramid level.
     */
    template <class TInputImage, class TOutputImage>
    typename MRToMSConverter<TInputImage,TOutputImage>
    ::InputImageListType*
    MRToMSConverter<TInputImage,TOutputImage>
    ::GetInput(void)
    {
      return dynamic_cast<InputImageListType *>(this->itk::ProcessObject::GetInput(2));
    }
    /**
     * Get The SupFiltre details.
     * \return The brighter details extracted from the filtering operation 
     */
    template <class TInputImage, class TOutputImage>
    typename MRToMSConverter<TInputImage,TOutputImage>
    ::InputImageListType*
    MRToMSConverter<TInputImage,TOutputImage>
    ::GetSupFiltre(void)
    {
      return dynamic_cast<InputImageListType *>(this->itk::ProcessObject::GetInput(0));
    }
    /**
     * Get The InfFiltre details.
     * \return The darker details extracted from the filtering operation
     */
    template <class TInputImage, class TOutputImage>
    typename MRToMSConverter<TInputImage,TOutputImage>
    ::InputImageListType*
    MRToMSConverter<TInputImage,TOutputImage>
    ::GetInfFiltre(void)
    {
      return dynamic_cast<InputImageListType *>(this->itk::ProcessObject::GetInput(1));
    }
    /**
     * Get The SupDeci details.
     * \return The brighter details extracted from the resampling operation 
     */
    template <class TInputImage, class TOutputImage>
    typename MRToMSConverter<TInputImage,TOutputImage>
    ::InputImageListType*
    MRToMSConverter<TInputImage,TOutputImage>
    ::GetSupDeci(void)
    {
      return dynamic_cast<InputImageListType *>(this->itk::ProcessObject::GetInput(3));
    }
    /**
     * Get The InfDeci details.
     * \return The darker details extracted from the resampling operation.
     */
    template <class TInputImage, class TOutputImage>
    typename MRToMSConverter<TInputImage,TOutputImage>
    ::InputImageListType*
    MRToMSConverter<TInputImage,TOutputImage>
    ::GetInfDeci(void)
    {
      return dynamic_cast<InputImageListType *>(this->itk::ProcessObject::GetInput(4));
    }
    /**
     * Main computation method
     */
    template <class TInputImage,class TOuputImage>
    void
    MRToMSConverter<TInputImage, TOuputImage>
    ::GenerateData()
    {
      // Input images lists pointers
      InputImageListPointerType  supFiltre = this->GetSupFiltre();
      InputImageListPointerType  infFiltre = this->GetInfFiltre();
      InputImageListPointerType  supDeci = this->GetSupDeci();
      InputImageListPointerType  infDeci = this->GetInfDeci();
      InputImageListPointerType  inputList = this->GetInput();
      
      // Output images lists pointers
      OutputImageListPointerType  supFiltreFullResolution = this->GetSupFiltreFullResolution();
      OutputImageListPointerType  infFiltreFullResolution = this->GetInfFiltreFullResolution();
      OutputImageListPointerType  supDeciFullResolution = this->GetSupDeciFullResolution();
      OutputImageListPointerType  infDeciFullResolution = this->GetInfDeciFullResolution();
      OutputImageListPointerType  outputList = this->GetOutput();

      // typedef of the resampling filter
      typedef otb::morphologicalPyramid::Resampler<InputImageType,OutputImageType> ResamplerType;

      // Definition of the resampler filters
      typename ResamplerType::Pointer resampler;

      // Full resolution size
      typename InputImageType::SizeType frsize = supFiltre->Front()->GetLargestPossibleRegion().GetSize();
      otbMsgDebugMacro(<<"MRToMSConverter: Full resolution size: "<<frsize);
      
      // SupFiltre resampling
      otbMsgDebugMacro(<<"MRToMSConverter: SupFiltre resampling.");
      InputImageListIteratorType it = supFiltre->Begin();
      // The first image does not need any resampling
      supFiltreFullResolution->PushBack(it.Get());
      ++it;
      for(;it!=supFiltre->End();++it)
	{
	  resampler = ResamplerType::New();
	  resampler->SetSize(frsize);
	  resampler->SetInput(it.Get());
	  resampler->Update();
	  supFiltreFullResolution->PushBack(resampler->GetOutput());
	}
      otbMsgDebugMacro(<<"MRToMSConverter: InfFiltre resampling.");
      // InfFiltre resampling
      it = infFiltre->Begin();
      // The first image does not need any resampling
      infFiltreFullResolution->PushBack(it.Get());
      ++it;
      for(;it!=infFiltre->End();++it)
	{
	  resampler = ResamplerType::New();
	  resampler->SetSize(frsize);
	  resampler->SetInput(it.Get());
	  resampler->Update();
	  infFiltreFullResolution->PushBack(resampler->GetOutput());
	}

      if(inputList)
	{
	  otbMsgDebugMacro(<<"MRToMSConverter: Optional inputList resampling.");
	  // Analyse image resampling
	  it = inputList->Begin();
	  // The first image does not need any resampling
	  outputList->PushBack(it.Get());
	  ++it;
	  for(;it!=inputList->End();++it)
	    {
	      resampler = ResamplerType::New();
	      resampler->SetSize(frsize);
	      resampler->SetInput(it.Get());
	      resampler->Update();
	      outputList->PushBack(resampler->GetOutput());
	    }
	}

      if(supDeci)
	{
	  otbMsgDebugMacro(<<"MRToMSConverter: Optional supDeci resampling.");
	  // SupDeci resampling
	  it = supDeci->Begin();
	  // The first image does not need any resampling
	  supDeciFullResolution->PushBack(it.Get());
	  ++it;
	  for(;it!=supDeci->End();++it)
	    {
	      resampler = ResamplerType::New();
	      resampler->SetSize(frsize);
	      resampler->SetInput(it.Get());
	      resampler->Update();
	      supDeciFullResolution->PushBack(resampler->GetOutput());
	    }
	}

      if(infDeci)
	{
	  otbMsgDebugMacro(<<"MRToMSConverter: Optional infDeci resampling.");
	  // InfDeci resampling
	  it = infDeci->Begin();
	  // The first image does not need any resampling
	  infDeciFullResolution->PushBack(it.Get());
	  ++it;
	  for(;it!=infDeci->End();++it)
	    {
	      resampler = ResamplerType::New();
	      resampler->SetSize(frsize);
	      resampler->SetInput(it.Get());
	      resampler->Update();
	      infDeciFullResolution->PushBack(resampler->GetOutput());
	    }
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
  } // End namespace MorphologicalPyramid
} // End namespace otb
#endif
