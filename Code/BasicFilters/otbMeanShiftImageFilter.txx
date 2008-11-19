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
#ifndef __otbMeanShiftImageFilter_txx
#define __otbMeanShiftImageFilter_txx

#include "otbMeanShiftImageFilter.h"

#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkImageRegionIterator.h"
#include "otbMacro.h"

#include "msImageProcessor.h"

namespace otb
{
  template <class TInputImage,class TOutputImage, class TBufferConverter>
  MeanShiftImageFilter<TInputImage,TOutputImage,TBufferConverter>
  ::MeanShiftImageFilter()
  {
    m_SpatialRadius      = 3;
    m_RangeRadius        = 10;
    m_MinimumRegionSize  = 10;
    m_Scale              = 100000.;
    
    this->SetNumberOfOutputs(2);
    this->SetNthOutput(1,OutputImageType::New());
  }

  template <class TInputImage,class TOutputImage, class TBufferConverter>
  const typename  MeanShiftImageFilter<TInputImage,TOutputImage,TBufferConverter>::OutputImageType *
  MeanShiftImageFilter<TInputImage,TOutputImage,TBufferConverter>
  ::GetClusteredOutput()const
  {
    if (this->GetNumberOfOutputs() < 2)
      {
	return 0;
      }
    return static_cast<const OutputImageType * >(this->itk::ProcessObject::GetOutput(1));
  }

  template <class TInputImage,class TOutputImage, class TBufferConverter>
  typename  MeanShiftImageFilter<TInputImage,TOutputImage,TBufferConverter>::OutputImageType *
  MeanShiftImageFilter<TInputImage,TOutputImage,TBufferConverter>
  ::GetClusteredOutput()
  {
    if (this->GetNumberOfOutputs() < 2)
      {
	return 0;
      }
    return static_cast< OutputImageType * >(this->itk::ProcessObject::GetOutput(1));
  }



  template <class TInputImage,class TOutputImage, class TBufferConverter>
  void
  MeanShiftImageFilter<TInputImage,TOutputImage,TBufferConverter>
  ::GenerateInputRequestedRegion()
  {
    // call the superclass' implementation of this method
    Superclass::GenerateInputRequestedRegion();
    
    // get pointers to the input and output
    typename Superclass::InputImagePointer inputPtr = 
      const_cast< TInputImage * >( this->GetInput() );
    typename Superclass::OutputImagePointer outputPtr = this->GetOutput();
    
    if ( !inputPtr || !outputPtr )
      {
	return;
      }
    
    // get a copy of the input requested region (should equal the output
    // requested region)
    typename TInputImage::RegionType inputRequestedRegion;
    inputRequestedRegion = inputPtr->GetRequestedRegion();
    
    // pad the input requested region by the operator radius
    inputRequestedRegion.PadByRadius( static_cast<unsigned int>(m_SpatialRadius) );
    
    // crop the input requested region at the input's largest possible region
    if ( inputRequestedRegion.Crop(inputPtr->GetLargestPossibleRegion()) )
      {
	inputPtr->SetRequestedRegion( inputRequestedRegion );
	return;
      }
    else
      {
	// Couldn't crop the region (requested region is outside the largest
	// possible region).  Throw an exception.

	// store what we tried to request (prior to trying to crop)
	inputPtr->SetRequestedRegion( inputRequestedRegion );
	
	// build an exception
	itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
	e.SetLocation(ITK_LOCATION);
	e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
	e.SetDataObject(inputPtr);
	throw e;
      }
  }

  template <class TInputImage,class TOutputImage, class TBufferConverter>
  void
  MeanShiftImageFilter<TInputImage,TOutputImage,TBufferConverter>
  ::ThreadedGenerateData(const RegionType& outputRegionForThread, int threadId )
  {    
    // Input and output pointers
    typename  InputImageType::ConstPointer inputPtr  = this->GetInput();
    typename OutputImageType::Pointer outputPtr = this->GetOutput();

    double invScale = 1/m_Scale;

    RegionType inputRequestedRegion = outputRegionForThread;
    RegionType outputRequestedRegion = outputRegionForThread;
    
    inputRequestedRegion.PadByRadius(m_SpatialRadius);
    inputRequestedRegion.Crop(inputPtr->GetRequestedRegion());
    
    // Iterators
    itk::ImageRegionConstIteratorWithIndex<InputImageType> inputIt(inputPtr,inputRequestedRegion);
    itk::ImageRegionIterator<OutputImageType> outputIt(outputPtr,outputRequestedRegion);
    

    //create image processing object
    msImageProcessor edisonProcessor;
    
    float * data = new float[inputRequestedRegion.GetNumberOfPixels()*inputPtr->GetNumberOfComponentsPerPixel()];

    unsigned int index = 0;

    for(inputIt.GoToBegin();!inputIt.IsAtEnd();++inputIt)
      {
	TBufferConverter::PixelToFloatArray(data,index,inputIt.Get(),m_Scale);
	index+=inputPtr->GetNumberOfComponentsPerPixel();
      }

    edisonProcessor.DefineLInput(data,inputRequestedRegion.GetSize()[1],inputRequestedRegion.GetSize()[0],inputPtr->GetNumberOfComponentsPerPixel());
   
    //define default kernel paramerters...
    kernelType	k[2] = {Uniform, Uniform};
    int		P[2] = {2, inputPtr->GetNumberOfComponentsPerPixel()};
    float   tempH[2] = {1.0 , 1.0};
    
    edisonProcessor.DefineKernel(k, tempH, P, 2);

    edisonProcessor.Filter(m_SpatialRadius,m_RangeRadius*m_Scale,MED_SPEEDUP);

    if(edisonProcessor.ErrorStatus)
      {
	itkExceptionMacro(<<"Error while running edison!");
      }


    typename OutputImageType::Pointer tmpOutput = OutputImageType::New();
    tmpOutput->SetRegions(inputRequestedRegion);
    tmpOutput->SetNumberOfComponentsPerPixel(outputPtr->GetNumberOfComponentsPerPixel());
    tmpOutput->Allocate();
    itk::ImageRegionIterator<OutputImageType> tmpIt(tmpOutput,inputRequestedRegion);
    itk::ImageRegionIterator<OutputImageType> tmp2It(tmpOutput,outputRequestedRegion);

    edisonProcessor.GetRawData(data);

    if(edisonProcessor.ErrorStatus)
      {
	itkExceptionMacro(<<"Error while running edison!");
      }


    index = 0;
    for(tmpIt.GoToBegin();!tmpIt.IsAtEnd();++tmpIt)
      {
	OutputPixelType pixel;

	TBufferConverter::FloatArrayToPixel(data,index,pixel,outputPtr->GetNumberOfComponentsPerPixel(),invScale);	
	tmpIt.Set(pixel);
	index+=outputPtr->GetNumberOfComponentsPerPixel();
      }

    tmp2It.GoToBegin();
    outputIt.GoToBegin();
    
    while(!tmp2It.IsAtEnd() && !outputIt.IsAtEnd())
      {
	outputIt.Set(tmp2It.Get());
	++tmp2It;
	++outputIt;
      }
    
    delete [] data;
  }

  template <class TInputImage,class TOutputImage, class TBufferConverter>
  void
  MeanShiftImageFilter<TInputImage,TOutputImage,TBufferConverter>
  ::AfterThreadedGenerateData()
  {
    double invScale = 1/m_Scale;

    typename OutputImageType::Pointer outputPtr = this->GetOutput();
    typename OutputImageType::Pointer clusteredOutputPtr = this->GetClusteredOutput();
    
    RegionType outputRequestedRegion = outputPtr->GetRequestedRegion();

    itk::ImageRegionIterator<OutputImageType> outputIt(outputPtr,outputRequestedRegion);
    itk::ImageRegionIterator<OutputImageType> clusteredOutputIt(clusteredOutputPtr,outputRequestedRegion);
    
    
    //create image processing object
    msImageProcessor edisonProcessor;
    
    float * data = new float[outputRequestedRegion.GetNumberOfPixels()*outputPtr->GetNumberOfComponentsPerPixel()];

    unsigned int index = 0;

    for(outputIt.GoToBegin();!outputIt.IsAtEnd();++outputIt)
      {
	TBufferConverter::PixelToFloatArray(data,index,outputIt.Get(),m_Scale);
	index+=outputPtr->GetNumberOfComponentsPerPixel();
      }

    edisonProcessor.DefineLInput(data,outputRequestedRegion.GetSize()[1],outputRequestedRegion.GetSize()[0],outputPtr->GetNumberOfComponentsPerPixel());

    // define default kernel paramerters...
    kernelType	k[2] = {Uniform, Uniform};
    int		P[2] = {2, outputPtr->GetNumberOfComponentsPerPixel()};
    float   tempH[2] = {1.0 , 1.0};
    
    edisonProcessor.DefineKernel(k, tempH, P, 2);

    edisonProcessor.FuseRegions(m_RangeRadius*m_Scale,m_MinimumRegionSize);

    if(edisonProcessor.ErrorStatus)
      {
	itkExceptionMacro(<<"Error while running edison!");
      }

    edisonProcessor.GetRawData(data);

    if(edisonProcessor.ErrorStatus)
      {
	itkExceptionMacro(<<"Error while running edison!");
      }

    index = 0;
    for(clusteredOutputIt.GoToBegin();!clusteredOutputIt.IsAtEnd();++clusteredOutputIt)
      {
	OutputPixelType pixel;
	TBufferConverter::FloatArrayToPixel(data,index,pixel,clusteredOutputPtr->GetNumberOfComponentsPerPixel(),invScale);
	clusteredOutputIt.Set(pixel);
	index+=clusteredOutputPtr->GetNumberOfComponentsPerPixel();
      }
    
    delete [] data;
  }



  template <class TInputImage,class TOutputImage, class TBufferConverter>
  void
  MeanShiftImageFilter<TInputImage,TOutputImage,TBufferConverter>
  ::PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os,indent);
    os<<indent<<"Spatial radius: "                <<m_SpatialRadius               <<std::endl;
    os<<indent<<"Range radius: "                  <<m_RangeRadius                 <<std::endl;
    os<<indent<<"Minimum region size: "           <<m_MinimumRegionSize           <<std::endl;
    os<<indent<<"Scale: "                         <<m_Scale                       <<std::endl;
  }

} // end namespace otb

#endif
