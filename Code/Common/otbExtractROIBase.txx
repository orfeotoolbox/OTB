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
#ifndef __otbExtractROIBase_txx
#define __otbExtractROIBase_txx

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
 //  ExtractROIBaseRegionCopierType extractImageRegionCopier;
//   extractImageRegionCopier(destRegion, srcRegion, m_ExtractionRegion);

  destRegion = srcRegion;

  OutputImageIndexType index = destRegion.GetIndex();

  for (unsigned int i = 0; i < InputImageDimension; ++i)
    {
      index[i]+=m_ExtractionRegion.GetIndex()[i];
    }
  destRegion.SetIndex(index);
}


template <class TInputImage, class TOutputImage>
void
ExtractROIBase<TInputImage,TOutputImage>
::SetInternalExtractionRegion(InputImageRegionType extractRegion)
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
      outputIndex[nonzeroSizeCount] =0;
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


template <class TInputImage, class TOutputImage>
void
ExtractROIBase<TInputImage,TOutputImage>
::SetExtractionRegion(InputImageRegionType roi)
{
  m_SizeX = roi.GetSize()[0];
  m_SizeY = roi.GetSize()[1];
  m_StartX = roi.GetIndex()[0];
  m_StartY = roi.GetIndex()[1];

  this->Modified();
}

template <class TInputImage, class TOutputImage>
void
ExtractROIBase<TInputImage,TOutputImage>
::GenerateInputRequestedRegion()
{
  Superclass::GenerateInputRequestedRegion();

  typename Superclass::InputImagePointer  inputPtr  = const_cast<InputImageType*>(this->GetInput());
  typename Superclass::OutputImagePointer      outputPtr = this->GetOutput();

  if ( !outputPtr || !inputPtr)
    {
      return;
    }
  InputImageRegionType requestedRegion = outputPtr->GetRequestedRegion();
  InputImageIndexType index = requestedRegion.GetIndex();
  InputImageIndexType offset = m_ExtractionRegion.GetIndex();

  for (unsigned int i=0; i < InputImageDimension; ++i)
        {
    index[i]+=offset[i];
  }
  requestedRegion.SetIndex(index);
  inputPtr->SetRequestedRegion(requestedRegion);
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

	// Check if input exists or not before doing anything
	if(!inputPtr)
	  {
	  return;
	  }

        // Recupere Region de l'image d'entree
//        const InputImageRegionType& inputRegion = inputPtr->GetRequestedRegion();
        const InputImageRegionType& inputRegion = inputPtr->GetLargestPossibleRegion();

        if ( (m_SizeX == 0) || (m_SizeX > (inputRegion.GetSize()[0] - m_StartX)) )
        {
                m_SizeX = inputRegion.GetSize()[0] - m_StartX;
        }
        if ( (m_SizeY == 0) || (m_SizeY > (inputRegion.GetSize()[1] - m_StartY)) )
        {
                m_SizeY = inputRegion.GetSize()[1] - m_StartY;
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
        this->SetInternalExtractionRegion( desiredRegion );


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
        outputOrigin[i] = inputOrigin[i]+static_cast<double>(m_ExtractionRegion.GetIndex()[i])*outputSpacing[i];
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
          outputOrigin[nonZeroCount] = inputOrigin[i]+static_cast<double>(m_ExtractionRegion.GetIndex()[i])*outputSpacing[i];
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
// THOMAS : dans ITK ce code est present, mais pas dans notre cas, car le nombre de composantes/pixel depend des canaux selectionnes par l'utilisateur
//          ce parametre est renseignes dans les classes sous-jacentes
//    outputPtr->SetNumberOfComponentsPerPixel(
//       inputPtr->GetNumberOfComponentsPerPixel() );
    }
  else
    {
    // pointer could not be cast back down
    itkExceptionMacro(<< "otb::ExtractROIBase::GenerateOutputInformation "
                      << "cannot cast input to "
                      << typeid(itk::ImageBase<InputImageDimension>*).name() );
    }


}


} // end namespace otb

#endif
