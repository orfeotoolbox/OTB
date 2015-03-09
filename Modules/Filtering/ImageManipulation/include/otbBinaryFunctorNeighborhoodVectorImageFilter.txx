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
#ifndef __otbBinaryFunctorNeighborhoodVectorImageFilter_txx
#define __otbBinaryFunctorNeighborhoodVectorImageFilter_txx

#include "otbBinaryFunctorNeighborhoodVectorImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkProgressReporter.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage1, class TInputImage2,
    class TOutputImage, class TFunction>
BinaryFunctorNeighborhoodVectorImageFilter<TInputImage1, TInputImage2, TOutputImage, TFunction>
::BinaryFunctorNeighborhoodVectorImageFilter()
{
  this->SetNumberOfRequiredInputs(2);
  this->InPlaceOff();
  m_Radius = 3;
}

/**
 * Connect one of the operands for neighborhood-wise operation
 */
template <class TInputImage1, class TInputImage2,
    class TOutputImage, class TFunction>
void
BinaryFunctorNeighborhoodVectorImageFilter<TInputImage1, TInputImage2, TOutputImage, TFunction>
::SetInput1(const TInputImage1 * image1)
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(0, const_cast<TInputImage1 *>(image1));
}

/**
 * Connect one of the operands for neighborhood-wise operation
 */
template <class TInputImage1, class TInputImage2,
    class TOutputImage, class TFunction>
void
BinaryFunctorNeighborhoodVectorImageFilter<TInputImage1, TInputImage2, TOutputImage, TFunction>
::SetInput2(const TInputImage2 * image2)
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(1, const_cast<TInputImage2 *>(image2));
}

/**
 * Connect the interval of radius
 */
template <class TInputImage1, class TInputImage2,
    class TOutputImage, class TFunction>
void
BinaryFunctorNeighborhoodVectorImageFilter<TInputImage1, TInputImage2, TOutputImage, TFunction>
::SetRadius(const unsigned char& min, const unsigned char& max)
{
  this->SetRadius(max);
  GetFunctor().SetRadius(min, max);
}

/**
 * Generate the output information
 */
template <class TInputImage1, class TInputImage2, class TOutputImage, class TFunction>
void
BinaryFunctorNeighborhoodVectorImageFilter<TInputImage1, TInputImage2, TOutputImage, TFunction>
::GenerateOutputInformation(void)
{
  Superclass::GenerateOutputInformation();

  int nbComponents = static_cast<int>(m_Functor.GetRadiusMax()) + 1
                     - static_cast<int>(m_Functor.GetRadiusMin());

  this->GetOutput()->SetNumberOfComponentsPerPixel(nbComponents);
}

/**
 * ThreadedGenerateData Performs the neighborhood-wise operation
 */
template <class TInputImage1, class TInputImage2, class TOutputImage, class TFunction>
void
BinaryFunctorNeighborhoodVectorImageFilter<TInputImage1, TInputImage2, TOutputImage, TFunction>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                       itk::ThreadIdType threadId)
{

  itk::ZeroFluxNeumannBoundaryCondition<TInputImage1> nbc1;
  itk::ZeroFluxNeumannBoundaryCondition<TInputImage2> nbc2;

  Input1ImagePointer inputPtr1 = dynamic_cast<const TInputImage1 *>(ProcessObjectType::GetInput(0));
  Input2ImagePointer inputPtr2 = dynamic_cast<const TInputImage2 *>(ProcessObjectType::GetInput(1));

  RadiusType1 r1;
  r1.Fill(m_Radius);
  NeighborhoodIteratorType1 neighInputIt1;

  RadiusType2 r2;
  r2.Fill(m_Radius);
  NeighborhoodIteratorType2 neighInputIt2;

  // This is the main difference from BinaryFunctorNeighborhoodImageFilter<TInputImage1, TInputImage2, TOutputImage, TFunction>::ThreadedGenerateData( const OutputImageRegionType &outputRegionForThread, itk::ThreadIdType threadId)
  OutputImagePointer outputPtr = this->GetOutput();
  outputPtr->SetNumberOfComponentsPerPixel(m_Functor.GetNumberOfComponentsPerPixel());
  outputPtr->Allocate();

  itk::ImageRegionIterator<TOutputImage> outputIt;

  // Find the data-set boundary "faces"
  typedef typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<TInputImage1> TypeFace1;
  typename TypeFace1::FaceListType::iterator fit1;
  typename TypeFace1::FaceListType           faceList1;
  TypeFace1                                  bC1;
  faceList1 = bC1(inputPtr1, outputRegionForThread, r1);

  typedef typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<TInputImage2> TypeFace2;
  typename TypeFace2::FaceListType::iterator fit2;
  typename TypeFace2::FaceListType           faceList2;
  TypeFace2                                  bC2;
  faceList2 = bC2(inputPtr2, outputRegionForThread, r2);

  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  // Process each of the boundary faces.
  // Center first and then left, right, up, down borders
  for (fit1 = faceList1.begin(), fit2 = faceList2.begin();
       fit1 != faceList1.end() && fit2 != faceList2.end();
       ++fit1, ++fit2)
    {
    neighInputIt1 = itk::ConstNeighborhoodIterator<TInputImage1> (r1, inputPtr1, *fit1);
    neighInputIt1.OverrideBoundaryCondition(&nbc1);
    neighInputIt1.GoToBegin();

    neighInputIt2 = itk::ConstNeighborhoodIterator<TInputImage2> (r2, inputPtr2, *fit2);
    neighInputIt2.OverrideBoundaryCondition(&nbc2);
    neighInputIt2.GoToBegin();

    outputIt = itk::ImageRegionIterator<TOutputImage> (outputPtr, *fit1);
    outputIt.GoToBegin();

    while (!outputIt.IsAtEnd())
      {
      outputIt.Set(m_Functor(neighInputIt1, neighInputIt2));

      ++neighInputIt1;
      ++neighInputIt2;
      ++outputIt;

      progress.CompletedPixel();
      }
    }
}

} // end namespace otb

#endif
