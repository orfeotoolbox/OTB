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
#ifndef _otbOverlapSaveConvolutionImageFilter_txx
#define _otbOverlapSaveConvolutionImageFilter_txx

#include "otbOverlapSaveConvolutionImageFilter.h"

#include "itkConstNeighborhoodIterator.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkZeroFluxNeumannBoundaryCondition.h"
#include "itkOffset.h"
#include "itkProgressReporter.h"

//debug
#include "itkImageRegionIterator.h"
#include "otbMath.h"

namespace otb
{

template <class TInputImage, class TOutputImage> 
OverlapSaveConvolutionImageFilter<TInputImage, TOutputImage> 
::OverlapSaveConvolutionImageFilter()
  {
    m_Radius.Fill(1);
    m_Filter.SetSize(3*3);
    m_Filter.Fill(1);
    m_NormalizeFilter = false;
  }

template <class TInputImage, class TOutputImage>
void 
OverlapSaveConvolutionImageFilter<TInputImage, TOutputImage> 
::GenerateInputRequestedRegion() throw (itk::InvalidRequestedRegionError)
  {
    // call the superclass' implementation of this method
    Superclass::GenerateInputRequestedRegion();
  
    // get pointers to the input and output
    typename Superclass::InputImagePointer inputPtr = const_cast< TInputImage * >( this->GetInput() );
    typename Superclass::OutputImagePointer outputPtr = this->GetOutput();
  
    if ( !inputPtr || !outputPtr )
      {
	return;
      }

    // get a copy of the input requested region (should equal the output
    // requested region)
    typename TInputImage::RegionType inputRequestedRegion;
    inputRequestedRegion = inputPtr->GetRequestedRegion();

    typename TInputImage::RegionType::IndexType index=inputRequestedRegion.GetIndex();
    typename TInputImage::RegionType::SizeType size=inputRequestedRegion.GetSize();

    // pad the input requested region by the operator radius
    for (unsigned int i = 0; i < 2; i++) //for dimension 2
      {
	size[i] += 2*m_Radius[i]+1; //increase size of input images'region by size of filter
	index[i] -= static_cast<long>(m_Radius[i]); //translate origin
      }
    inputRequestedRegion.SetSize(size);
    inputRequestedRegion.SetIndex(index);
	 
    // crop the input requested region at the input's largest possible region
    if(inputRequestedRegion.Crop(inputPtr->GetLargestPossibleRegion()))
      {
	//std::cout<<"InputRequestedRegion: "<<inputRequestedRegion<<std::endl;
	inputPtr->SetRequestedRegion(inputRequestedRegion);
	return;
      }
    else
      {
	// Couldn't crop the region (requested region is outside the largest
	// possible region).  Throw an exception.

	// store what we tried to request (prior to trying to crop)
	inputPtr->SetRequestedRegion(inputRequestedRegion);

	// build an exception
	itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
	e.SetLocation(ITK_LOCATION);
	e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
	e.SetDataObject(inputPtr);
	throw e;
      }
  }

template< class TInputImage, class TOutputImage> 
void 
OverlapSaveConvolutionImageFilter< TInputImage, TOutputImage>
::ProcessRegion(const OutputImageRegionType& region, const OutputImageRegionType& outputRegionForThread)
{
     itk::ImageRegionIteratorWithIndex<OutputImageType> it;
  
    // Input/Output pointers 
    typename OutputImageType::Pointer output = this->GetOutput();
    typename InputImageType::ConstPointer input = this->GetInput();

    //******************************
    //    int m=0,n=0;
    //******************************	
    typename InputImageType::SizeType sizeOfFilter;
    sizeOfFilter[0]=2*m_Radius[0]+1;
    sizeOfFilter[1]=2*m_Radius[1]+1;
  
    //InputRealType sum;
    InputRealType norm;
    
    OutputImageRegionType outputRegion = region;
    outputRegion.Crop(outputRegionForThread);

    
    OutputImageRegionType inputRegionForThread = outputRegionForThread;

    typename TInputImage::RegionType::IndexType index=inputRegionForThread.GetIndex();
    typename TInputImage::RegionType::SizeType size=inputRegionForThread.GetSize();

    // pad the input requested region by the operator radius
    for (unsigned int i = 0; i < 2; i++) //for dimension 2
      {
	size[i] += 2*m_Radius[i]+1; //increase size of input images'region by size of filter
	index[i] -= static_cast<long>(m_Radius[i]); //translate origin
      }
    inputRegionForThread.SetSize(size);
    inputRegionForThread.SetIndex(index);
    

    typename InputImageType::RegionType pieceRegion;
    typename InputImageType::SizeType   pieceSize = region.GetSize();
    typename InputImageType::IndexType  pieceIndex = region.GetIndex();

    pieceIndex[0]-=m_Radius[0];
    pieceIndex[1]-=m_Radius[1];
    pieceSize[0]+=sizeOfFilter[0];
    pieceSize[1]+=sizeOfFilter[1];

    pieceRegion.SetIndex(pieceIndex);
    pieceRegion.SetSize(pieceSize);
	
    typename InputImageType::RegionType inputRegion = pieceRegion;
    inputRegion.Crop(inputRegionForThread);
    inputRegion.Crop(input->GetLargestPossibleRegion());
    typename InputImageType::IndexType inputIndex = inputRegion.GetIndex();
    typename InputImageType::SizeType inputSize = inputRegion.GetSize();


    unsigned int pieceNbOfPixel = pieceRegion.GetNumberOfPixels();
    unsigned int sizeFFT=(pieceSize[0]/2+1)*pieceSize[1];
		
    //Iterator of input image

    itk::ZeroFluxNeumannBoundaryCondition<InputImageType> nbc;
    itk::ConstNeighborhoodIterator<InputImageType> bit(m_Radius,input,inputRegion);
    bit.OverrideBoundaryCondition(&nbc);
    bit.GoToBegin();
		
    //Iterator of output image
    it = itk::ImageRegionIteratorWithIndex<OutputImageType>(output,outputRegion);

    typedef typename itk::fftw::Proxy<double> FFTWProxyType;
    //variables for loops
    int i,j,k,l;
	
    //memory allocation
    InputPixelType* resampledFilterPiece;
    resampledFilterPiece=static_cast<FFTWProxyType::PixelType*>(fftw_malloc(pieceNbOfPixel*sizeof(InputPixelType)));

    FFTWProxyType::ComplexType* filterPieceFFT;
    filterPieceFFT=static_cast<FFTWProxyType::ComplexType*>(fftw_malloc(sizeFFT*sizeof(FFTWProxyType::ComplexType)));

    InputPixelType *inputPiece;
    inputPiece=static_cast<FFTWProxyType::PixelType*>(fftw_malloc(pieceNbOfPixel*sizeof(InputPixelType)));

    FFTWProxyType::ComplexType* inputPieceFFT;
    inputPieceFFT=static_cast<FFTWProxyType::ComplexType*>(fftw_malloc(sizeFFT*sizeof(FFTWProxyType::ComplexType)));

    //resampling filter (fill array with filter's values + zero padding)
    for(k=0;k<pieceNbOfPixel;k++)
      {
	resampledFilterPiece[k]=0.0;//Fill filter with zero
      }		
		
    k=0;
    for(j=0;j<sizeOfFilter[1];j++)
      {
	for(i=0;i<sizeOfFilter[0];i++)
	  {	
	    resampledFilterPiece[i+j*pieceSize[0]]=m_Filter.GetElement(k);//Copy values
	    k++;
	  }
      }
		
    // left zero padding
    unsigned int leftskip = static_cast<unsigned int>(std::max(0L,inputIndex[0]-pieceIndex[0]));
    unsigned int topskip =  pieceSize[0]*static_cast<unsigned int>(std::max(0L,inputIndex[1]-pieceIndex[1]));
			
   //  for( l = 0;l<pieceSize[1]*pieceSize[0];++l)
//       {
//  	inputPiece[l]=itk::NumericTraits<InputPixelType>::Zero;
//       }


   
    //Compute FFTs of one piece (part of input image) and of filter
		
    //Generating plans
    FFTWProxyType::PlanType inputPlan=FFTWProxyType::Plan_dft_r2c_2d(pieceSize[0],pieceSize[1],inputPiece,inputPieceFFT,FFTW_ESTIMATE);

    for(l = 0;l<inputSize[1];++l)
      {
	for(k = 0;k<inputSize[0];++k)
	  {
	    inputPiece[topskip+pieceSize[0]*l+k+leftskip]=bit.GetCenterPixel();
	    ++bit;
	  }
      }

//      typename InputImageType::IndexType currentIndex;
//     unsigned int linearindex = 0;
//     for(l=0;l<pieceSize[1];++l)
//       {
// 	for(k=0;k<pieceSize[0];++k)
// 	  {
// 	    currentIndex[0]=pieceIndex[0]+k;
// 	    currentIndex[1]=pieceIndex[1]+l;
// 	    if(inputRegion.IsInside(currentIndex))
// 	      {
// 		inputPiece[linearindex]=bit.GetCenterPixel();
// 		++bit;
// 	      }
// 	    else // reimplement the zero flux neumann boundary condition.
// 	      {
// 		inputPiece[linearindex]=0;
// 	      }
// 	    ++linearindex;
// 	  }
//       }
    
    FFTWProxyType::PlanType filterPlan=FFTWProxyType::Plan_dft_r2c_2d(pieceSize[0],pieceSize[1],resampledFilterPiece,filterPieceFFT,FFTW_ESTIMATE);
		
    FFTWProxyType::Execute(inputPlan);
    FFTWProxyType::Execute(filterPlan);
		
    //memory allocation
    FFTWProxyType::ComplexType* multipliedFFTarray;
    multipliedFFTarray=static_cast<FFTWProxyType::ComplexType*>(fftw_malloc(sizeFFT*sizeof(FFTWProxyType::ComplexType)));

    //inverse FFT

    // memory allocation
    FFTWProxyType::PixelType* inverseFFTpiece;
    inverseFFTpiece=static_cast<FFTWProxyType::PixelType*>(fftw_malloc(pieceNbOfPixel*sizeof(FFTWProxyType::PixelType)));

    FFTWProxyType::PlanType outputPlan=FFTWProxyType::Plan_dft_c2r_2d(pieceSize[0],pieceSize[1],multipliedFFTarray,inverseFFTpiece,FFTW_ESTIMATE);

      for(k=0;k<sizeFFT;k++)
      {
	//complex mutiplication
	multipliedFFTarray[k][0]=inputPieceFFT[k][0]*filterPieceFFT[k][0]-inputPieceFFT[k][1]*filterPieceFFT[k][1];
	multipliedFFTarray[k][1]=inputPieceFFT[k][0]*filterPieceFFT[k][1]+inputPieceFFT[k][1]*filterPieceFFT[k][0];
      }

    FFTWProxyType::Execute(outputPlan);

    norm = itk::NumericTraits<InputRealType>::Zero;
    for(i=0;i<sizeOfFilter[0]*sizeOfFilter[1];i++)
      {
	norm = itk::NumericTraits<InputRealType>::Zero;
	norm += static_cast<InputRealType>(m_Filter(i));
      }
    it.GoToBegin();
    while(!it.IsAtEnd())
      {
	typename InputImageType::IndexType index = it.GetIndex();
	unsigned int linearIndex = (index[1]+sizeOfFilter[1]-1-outputRegion.GetIndex()[1])*pieceSize[0]+index[0]+sizeOfFilter[0]-1-outputRegion.GetIndex()[0];
	if (m_NormalizeFilter)
	      {
		it.Set( static_cast<OutputPixelType>((inverseFFTpiece[linearIndex]/pieceNbOfPixel)/double(norm)) );
	      }
	    else
	      {
		it.Set( static_cast<OutputPixelType>(inverseFFTpiece[linearIndex]/pieceNbOfPixel) );//normalize pixels'values
	      }
	++it;
      }


 //    std::cout<<"Piece region: "<<pieceRegion<<std::endl;
//     std::cout<<"Output region: "<<outputRegion<<std::endl;
    
//     it.GoToBegin();

//     typename InputImageType::IndexType currentIndex;
//     unsigned int linearindex = 0;
//     for(l=0;l<pieceSize[1];++l)
//       {
// 	for(k=0;k<pieceSize[0];++k)
// 	  {
// 	    currentIndex[0]=pieceIndex[0]+k;
// 	    currentIndex[1]=pieceIndex[1]+l;
// 	    if(outputRegion.IsInside(currentIndex))
// 	      {
// // 		std::cout<<currentIndex<<" inside."<<std::endl;
// 		if (m_NormalizeFilter)
// 		  {
// 		    it.Set( static_cast<OutputPixelType>((inverseFFTpiece[linearindex]/pieceNbOfPixel)/double(norm)) );
// 		  }
// 		else
// 		  {
// // 		    std::cout<<currentIndex<<" inside : "<< inverseFFTpiece[linearindex]/pieceNbOfPixel <<std::endl;
// 		    it.Set( static_cast<OutputPixelType>(inverseFFTpiece[linearindex]/pieceNbOfPixel) );//normalize pixels'values
// 		  }
// 		++it;
// 	      }
// 	    ++linearindex;
// 	  }
//       }

		
    //frees memory
    fftw_free(resampledFilterPiece); 
    fftw_free(inputPiece);

    FFTWProxyType::DestroyPlan(inputPlan);
    FFTWProxyType::DestroyPlan(filterPlan);
    FFTWProxyType::DestroyPlan(outputPlan);

    fftw_free(filterPieceFFT);
    fftw_free(inputPieceFFT);		
    fftw_free(multipliedFFTarray);
    fftw_free(inverseFFTpiece);

  }

  template< class TInputImage, class TOutputImage> void OverlapSaveConvolutionImageFilter< TInputImage, TOutputImage>::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,int threadId)
  {
    typename OutputImageRegionType::SizeType size =  outputRegionForThread.GetSize();
    unsigned long minSize = std::min(size[0],size[1]);
    unsigned int tilexdivision = static_cast<unsigned int>(vcl_ceil(static_cast<double>(size[0])/static_cast<double>(minSize)));
    unsigned int tileydivision = static_cast<unsigned int>(vcl_ceil(static_cast<double>(size[1])/static_cast<double>(minSize)));

    std::cout<<"Tile: "<<outputRegionForThread<<std::endl;
    std::cout<<"Min tile size: "<<minSize<<std::endl;
    std::cout<<"tilexdivision: "<<tilexdivision<<std::endl;
    std::cout<<"tileydivision: "<<tileydivision<<std::endl;


    for(unsigned int i = 0;i<tilexdivision;++i)
      {
	for(unsigned int j = 0;j<tileydivision;++j)
	  {
	    OutputImageRegionType newRegion;
	    typename OutputImageRegionType::SizeType newSize;
	    newSize.Fill(minSize);
	    newRegion.SetSize(newSize);
	    typename OutputImageRegionType::IndexType newIndex = outputRegionForThread.GetIndex();
	    newIndex[0]+=i*minSize;
	    newIndex[1]+=j*minSize;
	    newRegion.SetIndex(newIndex);
	    std::cout<<"Processing region: "<<i<<" "<<j<<std::endl;
	    this->ProcessRegion(newRegion,outputRegionForThread);
	  }
      }

//     this->ProcessRegion(outputRegionForThread,outputRegionForThread);
  }

  /** Standard "PrintSelf" method */
  template <class TInputImage, class TOutput> void OverlapSaveConvolutionImageFilter<TInputImage, TOutput>::PrintSelf(std::ostream& os,itk::Indent indent) const
  {
    Superclass::PrintSelf( os, indent );
    os << indent << "Radius: " << m_Radius << std::endl;
  }
} // end namespace otb

#endif
