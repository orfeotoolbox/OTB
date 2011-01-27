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
#ifndef __otbDisparityMapMedianFilter_txx
#define __otbDisparityMapMedianFilter_txx

// First make sure that the configuration is available.
// This line can be removed once the optimized versions
// gets integrated into the main directories.
#include "itkConfigure.h"

#ifdef ITK_USE_CONSOLIDATED_MORPHOLOGY
#include "itkOptMedianImageFilter.h"
#else

#include "otbDisparityMapMedianFilter.h"

#include "itkConstNeighborhoodIterator.h"
#include "itkNeighborhoodInnerProduct.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkZeroFluxNeumannBoundaryCondition.h"
#include "itkOffset.h"
#include "itkProgressReporter.h"

#include <vector>
#include <algorithm>

namespace otb
{

template <class TInputImage, class TOutputImage, class TMask>
DisparityMapMedianFilter<TInputImage, TOutputImage, TMask>
::DisparityMapMedianFilter()
{
  this->SetNumberOfRequiredInputs( 2 );
  this->SetNumberOfOutputs(4);
  this->SetNthOutput(1,TMask::New());
  this->SetNthOutput(2,TOutputImage::New());
  this->SetNthOutput(3,TMask::New());
  m_Radius.Fill(3);
  m_IncoherenceThreshold = 1.0;
}

template <class TInputImage, class TOutputImage, class TMask>
void 
DisparityMapMedianFilter<TInputImage, TOutputImage, TMask>
::SetMaskInput( const TMask * inputmask)
{
  // Process object is not const-correct so the const casting is required.
  SetNthInput(1, const_cast<TMask *>( inputmask ));
}


template <class TInputImage, class TOutputImage, class TMask>
const TMask *
DisparityMapMedianFilter<TInputImage, TOutputImage, TMask>
::GetMaskInput()
{
  if (this->GetNumberOfInputs()<2)
  {
    return 0;
  }
  return static_cast<const TMask *>(this->itk::ProcessObject::GetInput(1));
}

template <class TInputImage, class TOutputImage, class TMask>
TMask *
DisparityMapMedianFilter<TInputImage, TOutputImage, TMask>
::GetOutputMask()
{
  if (this->GetNumberOfOutputs()<2)
    {
			return 0;
    }
  return static_cast<TMask *>(this->itk::ProcessObject::GetOutput(1));
}


template <class TInputImage, class TOutputImage, class TMask>
TOutputImage *
DisparityMapMedianFilter<TInputImage, TOutputImage, TMask>
::GetOutputDisparityMap()
{
  if (this->GetNumberOfOutputs()<3)
    {
			return 0;
    }
  return static_cast<TOutputImage *>(this->itk::ProcessObject::GetOutput(2));
}

template <class TInputImage, class TOutputImage, class TMask>
TMask *
DisparityMapMedianFilter<TInputImage, TOutputImage, TMask>
::GetOutputDisparityMask()
{
  if (this->GetNumberOfOutputs()<4)
    {
			return 0;
    }
  return static_cast<TMask *>(this->itk::ProcessObject::GetOutput(3));
}


template <class TInputImage, class TOutputImage, class TMask>
void 
DisparityMapMedianFilter<TInputImage, TOutputImage, TMask>
::GenerateInputRequestedRegion() throw (itk::InvalidRequestedRegionError)
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();
  
  // get pointers to the input and output
  typename Superclass::InputImagePointer inputPtr = const_cast< TInputImage * >( this->GetInput() );
  TMask * inputmaskPtr = const_cast< TMask * >(this->GetMaskInput());
  typename Superclass::OutputImagePointer outputPtr = this->GetOutput();
	TMask * outputmaskPtr = this->GetOutputMask();
  typename Superclass::OutputImagePointer outputdisparitymapPtr = this->GetOutputDisparityMap();
  TMask * outputdisparitymaskPtr = this->GetOutputDisparityMask();
  
  if ( !inputPtr || !outputPtr || !inputmaskPtr|| !outputmaskPtr || !outputdisparitymapPtr || !outputdisparitymaskPtr)
    {
    return;
    }

  // get a copy of the input requested region (should equal the output
  // requested region)
  typename TInputImage::RegionType inputRequestedRegion;
  inputRequestedRegion = inputPtr->GetRequestedRegion();

  // pad the input requested region by the operator radius
  inputRequestedRegion.PadByRadius( m_Radius );

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


template< class TInputImage, class TOutputImage, class TMask>
void
DisparityMapMedianFilter< TInputImage, TOutputImage, TMask>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, int threadId)
{
	// Allocate outputs
	this->AllocateOutputs();


	// Get the image pointers
  typename OutputImageType::Pointer output = this->GetOutput();
  typename InputImageType::ConstPointer input  = this->GetInput();
	typename MaskImageType::ConstPointer inputmaskPtr = this->GetMaskInput();
	TMask * outputmaskPtr = this->GetOutputMask();
	TOutputImage * outputdisparitymapPtr = this->GetOutputDisparityMap();
	TMask * outputdisparitymaskPtr = this->GetOutputDisparityMask();
	

	/** Input iterators */
	itk::ConstNeighborhoodIterator<InputImageType> InputIt(m_Radius, input,outputRegionForThread);
	itk::ConstNeighborhoodIterator<TMask> MaskInputIt(m_Radius, inputmaskPtr,outputRegionForThread);
	InputIt.GoToBegin();
	MaskInputIt.GoToBegin();

	/** Output iterators */
	itk::ImageRegionIteratorWithIndex<OutputImageType> outputIt(output,outputRegionForThread);
	itk::ImageRegionIterator<TMask> outputMaskIt(outputmaskPtr,outputRegionForThread);
	itk::ImageRegionIterator<OutputImageType> outputDisparityMapIt(outputdisparitymapPtr,outputRegionForThread);
	itk::ImageRegionIterator<TMask> outputDisparityMaskIt(outputdisparitymaskPtr,outputRegionForThread);
	outputIt.GoToBegin();
	outputMaskIt.GoToBegin();
	outputDisparityMapIt.GoToBegin();
	outputDisparityMaskIt.GoToBegin();

  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  std::vector<InputPixelType> pixels;
  while (!InputIt.IsAtEnd() && !MaskInputIt.IsAtEnd() && !outputIt.IsAtEnd() && !outputMaskIt.IsAtEnd() && !outputDisparityMapIt.IsAtEnd()  && !outputDisparityMapIt.IsAtEnd())
  {
		if (outputIt.GetIndex()[0] >= m_Radius[0] && outputIt.GetIndex()[0] < input->GetLargestPossibleRegion().GetSize()[0] - m_Radius[0] && outputIt.GetIndex()[1]>=m_Radius[1] && outputIt.GetIndex()[1] < input->GetLargestPossibleRegion().GetSize()[1] - m_Radius[1] )
		{
			// determine pixels in the neighborhood window whose subpixel mask is not equal to 0
			int p=0;
			pixels.clear();
			for (int i=0;i<MaskInputIt.Size();i++)
			{
				if (MaskInputIt.GetPixel(i) != 0)
				{
					p++;
					pixels.push_back(InputIt.GetPixel(i));
				}
			}
			if (p>0)
			{
				//outputMaskIt.Set(itk::NumericTraits<MaskImagePixelType>::max());
				outputMaskIt.Set(1);
				// get the median value
				if (std::fmod((double) p,2)==0) 
				{
					const unsigned int medianPosition_low = (int) p/2 - 1;
					const unsigned int medianPosition_high = (int) p/2;
					const typename std::vector<InputPixelType>::iterator medianIterator_low = pixels.begin() + medianPosition_low;
					const typename std::vector<InputPixelType>::iterator medianIterator_high = pixels.begin() + medianPosition_high;
					std::nth_element(pixels.begin(), medianIterator_low, pixels.end());
					std::nth_element(pixels.begin(), medianIterator_high, pixels.end());
					outputIt.Set( static_cast<typename OutputImageType::PixelType> ((*medianIterator_low + *medianIterator_high)/2) );
				}
				else
				{
					const unsigned int medianPosition = (int) floor(p/2);
					const typename std::vector<InputPixelType>::iterator medianIterator = pixels.begin() + medianPosition;
		      std::nth_element(pixels.begin(), medianIterator, pixels.end());
		      outputIt.Set( static_cast<typename OutputImageType::PixelType> (*medianIterator) );
				}
			}
			else
			{
				outputIt.Set(0.0);
				outputMaskIt.Set(0);
			}
		}
		else 
		{
			outputIt.Set(0.0);
			outputMaskIt.Set(0);
		}

		outputDisparityMapIt.Set( static_cast<typename OutputImageType::PixelType> (InputIt.GetCenterPixel())); // copy the input disparity map
		outputDisparityMaskIt.Set(MaskInputIt.GetCenterPixel());  // copy the input disparity mask

		progress.CompletedPixel();
		++InputIt;
		++MaskInputIt;
		++outputIt;
		++outputMaskIt;
		++outputDisparityMapIt;
		++outputDisparityMaskIt;
  }

 /*Remove incoherences between disparity and median*/
  // creation of the auxilliary image that store positions of incoherences between the median and the input disparity map
  MaskImagePointerType image_aux = MaskImageType::New();
	image_aux->SetRegions(outputRegionForThread);
	image_aux->Allocate();
	image_aux->FillBuffer(0);
	itk::ImageRegionIterator<TMask> image_aux_It(image_aux,outputRegionForThread);
	image_aux_It.GoToBegin();
	InputIt.GoToBegin();
	MaskInputIt.GoToBegin();
	outputIt.GoToBegin();
	outputMaskIt.GoToBegin();
	outputDisparityMapIt.GoToBegin();
	outputDisparityMaskIt.GoToBegin();

	itk::ImageRegionConstIterator<OutputImageType> MedianIt(output,outputRegionForThread);
	MedianIt.GoToBegin();

  while (!InputIt.IsAtEnd() && !MaskInputIt.IsAtEnd() && !outputIt.IsAtEnd() && !outputMaskIt.IsAtEnd())
  {
		if (outputIt.GetIndex()[0] >= m_Radius[0]  && outputIt.GetIndex()[0] < input->GetLargestPossibleRegion().GetSize()[0] - m_Radius[0]  && outputIt.GetIndex()[1]>=m_Radius[1] && outputIt.GetIndex()[1] < input->GetLargestPossibleRegion().GetSize()[1] - m_Radius[1] )
		{
			if (MaskInputIt.GetCenterPixel() != 0 && std::fabs(InputIt.GetCenterPixel() - MedianIt.Get())>m_IncoherenceThreshold)
			{
				outputDisparityMapIt.Set(0.0); /*Remove pixel from disparity map*/
				outputDisparityMaskIt.Set(0);
				image_aux_It.Set(1);
			}
		}
		++MedianIt;
		++image_aux_It;
		++InputIt;
		++MaskInputIt;
		++outputIt;
		++outputMaskIt;
		++outputDisparityMapIt;
		++outputDisparityMaskIt;
	}

//Recompute median where values had been changed 
  // we  use the updated sub pixel disparity map
	itk::ConstNeighborhoodIterator<OutputImageType> updatedDisparityMapIt(m_Radius, outputdisparitymapPtr,outputRegionForThread);
	itk::ConstNeighborhoodIterator<TMask> updateDisparityMaskIt(m_Radius, outputdisparitymaskPtr,outputRegionForThread);

	image_aux_It.GoToBegin();
	outputIt.GoToBegin();
	outputMaskIt.GoToBegin();
	outputDisparityMapIt.GoToBegin();
	outputDisparityMaskIt.GoToBegin();
	updatedDisparityMapIt.GoToBegin();
	updateDisparityMaskIt.GoToBegin();

	while (!updatedDisparityMapIt.IsAtEnd() && !updateDisparityMaskIt.IsAtEnd() && !outputIt.IsAtEnd() && !outputMaskIt.IsAtEnd())
  {
		if (outputIt.GetIndex()[0] >= m_Radius[0] && outputIt.GetIndex()[0] < input->GetLargestPossibleRegion().GetSize()[0] - m_Radius[0] && outputIt.GetIndex()[1]>=m_Radius[1] && outputIt.GetIndex()[1] < input->GetLargestPossibleRegion().GetSize()[1] - m_Radius[1] )
		{
			if (image_aux_It.Get() != 0)
			{
				// determine pixels in the neighborhood window whose subpixel mask is not equal to 0
							int p=0;
							pixels.clear();
							for (int i=0;i<updateDisparityMaskIt.Size();i++)
							{
								if (updateDisparityMaskIt.GetPixel(i) != 0)
								{
									p++;
									pixels.push_back(updatedDisparityMapIt.GetPixel(i));
								}
							}
							if (p>0)
							{
								//outputMaskIt.Set(itk::NumericTraits<MaskImagePixelType>::max());
								outputMaskIt.Set(1);
								// get the median value
								if (std::fmod((double) p,2)==0) 
								{
									const unsigned int medianPosition_low = (int) p/2 - 1;
									const unsigned int medianPosition_high = (int) p/2;
									const typename std::vector<InputPixelType>::iterator medianIterator_low = pixels.begin() + medianPosition_low;
									const typename std::vector<InputPixelType>::iterator medianIterator_high = pixels.begin() + medianPosition_high;
									std::nth_element(pixels.begin(), medianIterator_low, pixels.end());
									std::nth_element(pixels.begin(), medianIterator_high, pixels.end());
									outputIt.Set( static_cast<typename OutputImageType::PixelType> ((*medianIterator_low + *medianIterator_high)/2) );
								}
								else
								{
									const unsigned int medianPosition = (int) floor(p/2);
									const typename std::vector<InputPixelType>::iterator medianIterator = pixels.begin() + medianPosition;
						      std::nth_element(pixels.begin(), medianIterator, pixels.end());
						      outputIt.Set( static_cast<typename OutputImageType::PixelType> (*medianIterator) );
								}
							}
							else
							{
								outputIt.Set(0.0);
								outputMaskIt.Set(0);
							}
			}
		}
		++image_aux_It;
		++outputIt;
		++outputMaskIt;
		++updatedDisparityMapIt;
		++updateDisparityMaskIt;
	}
}


/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TOutput, class TMask>
void
DisparityMapMedianFilter<TInputImage, TOutput, TMask>
::PrintSelf(
  std::ostream& os, 
  itk::Indent indent) const
{
  Superclass::PrintSelf( os, indent );
  os << indent << "Radius: " << m_Radius << std::endl;

}

} // end namespace otb

#endif

#endif
