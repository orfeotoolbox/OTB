/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkExtractROIBase.txx,v $
  Language:  C++
  Date:      $Date: 2005/09/07 21:23:01 $
  Version:   $Revision: 1.19 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _otbExtractROIBase_txx
#define _otbExtractROIBase_txx

#include "otbExtractROIBase.h"
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
ExtractROIBase<TInputImage,TOutputImage>
::ExtractROIBase() : itk::ImageToImageFilter<TInputImage,TOutputImage>(),
                                m_StartX(0),
                                m_StartY(0),
                                m_SizeX(0),
                                m_SizeY(0)
{
}


/**
 *
 */
template <class TInputImage, class TOutputImage>
void 
ExtractROIBase<TInputImage,TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "ExtractionRegion: " << m_ExtractionRegion << std::endl;
  os << indent << "OutputImageRegion: " << m_OutputImageRegion << std::endl;
}


template<class TInputImage, class TOutputImage>
void 
ExtractROIBase<TInputImage,TOutputImage>
::CallCopyOutputRegionToInputRegion(InputImageRegionType &destRegion,
                                    const OutputImageRegionType &srcRegion)
{
  ExtractROIBaseRegionCopierType extractImageRegionCopier;
  extractImageRegionCopier(destRegion, srcRegion, m_ExtractionRegion);
}


template <class TInputImage, class TOutputImage>
void 
ExtractROIBase<TInputImage,TOutputImage>
::SetExtractionRegion(InputImageRegionType extractRegion)
{
  m_ExtractionRegion = extractRegion;

  unsigned int nonzeroSizeCount = 0;
  InputImageSizeType inputSize = extractRegion.GetSize();
  OutputImageSizeType outputSize;
  OutputImageIndexType outputIndex;

  /**
   * check to see if the number of non-zero entries in the extraction region
   * matches the number of dimensions in the output image.  
   **/
  for (unsigned int i = 0; i < InputImageDimension; ++i)
    {
    if (inputSize[i])
      { 
      outputSize[nonzeroSizeCount] = inputSize[i];    
      outputIndex[nonzeroSizeCount] = extractRegion.GetIndex()[i];
      nonzeroSizeCount++;
      }
    }
    
  if (nonzeroSizeCount != OutputImageDimension)
    {
    itkExceptionMacro("Extraction Region not consistent with output image");
    }

  m_OutputImageRegion.SetSize(outputSize);
  m_OutputImageRegion.SetIndex(outputIndex);
  this->Modified();
}



/** 
 * ExtractROIBase can produce an image which is a different resolution
 * than its input image.  As such, ExtractROIBase needs to provide an
 * implementation for GenerateOutputInformation() in order to inform
 * the pipeline execution model.  The original documentation of this
 * method is below.
 *
 * \sa ProcessObject::GenerateOutputInformaton() 
 */
template <class TInputImage, class TOutputImage>
void 
ExtractROIBase<TInputImage,TOutputImage>
::GenerateOutputInformation()
{
        

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


  // do not call the superclass' implementation of this method since
  // this filter allows the input the output to be of different dimensions
 
  // get pointers to the input and output
  typename Superclass::OutputImagePointer      outputPtr = this->GetOutput();
//  typename Superclass::InputImageConstPointer  inputPtr  = this->GetInput();

  if ( !outputPtr || !inputPtr)
    {
    return;
    }

  // Set the output image size to the same value as the extraction region.
  outputPtr->SetLargestPossibleRegion( m_OutputImageRegion );

  // Set the output spacing and origin
  const itk::ImageBase<InputImageDimension> *phyData;

  phyData
    = dynamic_cast<const itk::ImageBase<InputImageDimension>*>(this->GetInput());

  if (phyData)
    {
    // Copy what we can from the image from spacing and origin of the input
    // This logic needs to be augmented with logic that select which
    // dimensions to copy
    unsigned int i;
    const typename InputImageType::SpacingType& 
      inputSpacing = inputPtr->GetSpacing();
    const typename InputImageType::DirectionType&
      inputDirection = inputPtr->GetDirection();
    const typename InputImageType::PointType&
      inputOrigin = inputPtr->GetOrigin();

    typename OutputImageType::SpacingType outputSpacing;
    typename OutputImageType::DirectionType outputDirection;
    typename OutputImageType::PointType outputOrigin;

    if ( static_cast<unsigned int>(OutputImageDimension) > 
         static_cast<unsigned int>(InputImageDimension )    )
      {
      // copy the input to the output and fill the rest of the
      // output with zeros.
      for (i=0; i < InputImageDimension; ++i)
        {
        outputSpacing[i] = inputSpacing[i];
        outputOrigin[i] = inputOrigin[i];
        for (unsigned int dim = 0; dim < InputImageDimension; ++dim)
          {
          outputDirection[i][dim] = inputDirection[i][dim];
          }
        }
      for (; i < OutputImageDimension; ++i)
        {
        outputSpacing[i] = 1.0;
        outputOrigin[i] = 0.0;
        for (unsigned int dim = 0; dim < InputImageDimension; ++dim)
          {
          outputDirection[i][dim] = 0.0;
          }
        outputDirection[i][i] = 1.0;
        }
      }
    else
      {
      // copy the non-collapsed part of the input spacing and origing to the output
      int nonZeroCount = 0;
      for (i=0; i < InputImageDimension; ++i)
        {
        if (m_ExtractionRegion.GetSize()[i])
          {
          outputSpacing[nonZeroCount] = inputSpacing[i];
          outputOrigin[nonZeroCount] = inputOrigin[i];
          for (unsigned int dim = 0; dim < OutputImageDimension; ++dim)
            {
            outputDirection[nonZeroCount][dim] =
              inputDirection[nonZeroCount][dim];
            }
          nonZeroCount++;
          }
        }
      }

    // set the spacing and origin
    outputPtr->SetSpacing( outputSpacing );
    outputPtr->SetDirection( outputDirection );
    outputPtr->SetOrigin( outputOrigin );
    }
  else
    {
    // pointer could not be cast back down
    itkExceptionMacro(<< "itk::ExtractROIBase::GenerateOutputInformation "
                      << "cannot cast input to "
                      << typeid(itk::ImageBase<InputImageDimension>*).name() );
    }
}


} // end namespace otb

#endif
