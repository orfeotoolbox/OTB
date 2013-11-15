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
#ifndef __otbLeeImageFilter_txx
#define __otbLeeImageFilter_txx

#include "otbLeeImageFilter.h"

#include "itkDataObject.h"
#include "itkMacro.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkNeighborhoodInnerProduct.h"
#include "itkImageRegionIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkZeroFluxNeumannBoundaryCondition.h"
#include "itkOffset.h"
#include "itkProgressReporter.h"

namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputImage>
LeeImageFilter<TInputImage, TOutputImage>::LeeImageFilter()
{
  m_Radius.Fill(1);
  SetNbLooks(1.0);
}

template <class TInputImage, class TOutputImage>
void LeeImageFilter<TInputImage, TOutputImage>::GenerateInputRequestedRegion() throw (itk::InvalidRequestedRegionError)
  {
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();

  // get pointers to the input and output
  typename Superclass::InputImagePointer  inputPtr   =  const_cast<TInputImage *>(this->GetInput());
  typename Superclass::OutputImagePointer outputPtr = this->GetOutput();

  if (!inputPtr || !outputPtr)
    {
    return;
    }

  // get a copy of the input requested region (should equal the output
  // requested region)
  typename TInputImage::RegionType inputRequestedRegion;
  inputRequestedRegion = inputPtr->GetRequestedRegion();

  // pad the input requested region by the operator radius
  inputRequestedRegion.PadByRadius(m_Radius);

  // crop the input requested region at the input's largest possible region
  if (inputRequestedRegion.Crop(inputPtr->GetLargestPossibleRegion()))
    {
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
    std::ostringstream msg;
    msg << static_cast<const char *>(this->GetNameOfClass())
        << "::GenerateInputRequestedRegion()";
    e.SetLocation(msg.str().c_str());
    e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
    e.SetDataObject(inputPtr);
    throw e;
    }
  }

template<class TInputImage, class TOutputImage>
void LeeImageFilter<TInputImage, TOutputImage>::ThreadedGenerateData(
  const OutputImageRegionType&     outputRegionForThread,
  itk::ThreadIdType threadId
  )
{
  unsigned int                                          i;
  itk::ZeroFluxNeumannBoundaryCondition<InputImageType> nbc;

  itk::ConstNeighborhoodIterator<InputImageType> bit;
  itk::ImageRegionIterator<OutputImageType>      it;

  // Allocate output
  typename OutputImageType::Pointer     output = this->GetOutput();
  typename InputImageType::ConstPointer input  = this->GetInput();

  // Find the data-set boundary "faces"
  typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType>::FaceListType           faceList;
  typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType>::FaceListType::iterator fit;

  itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType> bC;
  faceList = bC(input, outputRegionForThread, m_Radius);

  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  //  InputRealType pixel;
  InputRealType sum;
  InputRealType sum2;

  double Cr2, Cv2, E_I, I, Var_I, dPixel;

  //Compute the ratio using the number of looks
  Cv2 = 1. / (vcl_sqrt(m_NbLooks));
  Cv2 *= Cv2;

  // Process each of the boundary faces.  These are N-d regions which border
  // the edge of the buffer.
  for (fit = faceList.begin(); fit != faceList.end(); ++fit)
    {
    bit = itk::ConstNeighborhoodIterator<InputImageType>(m_Radius, input, *fit);
    const unsigned int neighborhoodSize = bit.Size();
    it = itk::ImageRegionIterator<OutputImageType>(output, *fit);
    bit.OverrideBoundaryCondition(&nbc);
    bit.GoToBegin();

    while (!bit.IsAtEnd())
      {
      sum = itk::NumericTraits<InputRealType>::Zero;
      sum2 = itk::NumericTraits<InputRealType>::Zero;
      //Parcours du voisinage
      for (i = 0; i < neighborhoodSize; ++i)
        {
        dPixel = static_cast<double>(bit.GetPixel(i));
        sum += dPixel;
        sum2 += dPixel * dPixel;
        }
      E_I   = sum / static_cast<double>(neighborhoodSize);
      Var_I = sum2 / static_cast<double>(neighborhoodSize) - E_I * E_I;
      I = static_cast<double>(bit.GetCenterPixel());

      const double epsilon = 0.0000000001;
      if (vcl_abs(E_I) < epsilon)
        {
        dPixel = itk::NumericTraits<OutputPixelType>::Zero;
        }
      else
        {
        Cr2    = Var_I / (E_I * E_I);
        dPixel = E_I + ((I - E_I) * (Cr2)) / (Cr2 + Cv2);

        }
      // get the mean value
      it.Set(static_cast<OutputPixelType>(dPixel));

      ++bit;
      ++it;
      progress.CompletedPixel();
      }
    }
}

/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TOutput>
void
LeeImageFilter<TInputImage, TOutput>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Radius: " << m_Radius << std::endl;
}

} // end namespace otb

#endif
