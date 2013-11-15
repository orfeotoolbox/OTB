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
#ifndef __otbNeighborhoodScalarProductFilter_txx
#define __otbNeighborhoodScalarProductFilter_txx

#include "otbNeighborhoodScalarProductFilter.h"
#include "itkImageRegionIterator.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkProgressReporter.h"

#include "otbMath.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputModulus, class TOutputDirection>
NeighborhoodScalarProductFilter<TInputImage, TOutputModulus, TOutputDirection>
::NeighborhoodScalarProductFilter()
{}

template <class TInputImage, class TOutputModulus, class TOutputDirection>
void
NeighborhoodScalarProductFilter<TInputImage, TOutputModulus, TOutputDirection>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId)
{
  // some typedefs
  typedef itk::ConstNeighborhoodIterator<InputImageType>                           NeighborhoodIteratorType;
  typedef typename NeighborhoodIteratorType::RadiusType                            RadiusType;
  typedef typename NeighborhoodIteratorType::OffsetType                            OffsetType;
  typedef itk::ZeroFluxNeumannBoundaryCondition<InputImageType>                    BoundaryConditionType;
  typedef itk::ImageRegionIterator<OutputModulusType>                              OutputIteratorType;
  typedef itk::ImageRegionIterator<OutputDirectionType>                            OutputDirectionIteratorType;
  typedef itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType> BoundaryFacesCalculatorType;
  typedef typename BoundaryFacesCalculatorType::FaceListType                       FaceListType;
  typedef typename FaceListType::iterator                                          FaceListIteratorType;

  // Pointers on inputs/outputs
  InputImageType *           inputPtr = const_cast<InputImageType *>(this->GetInput());
  OutputModulusPointerType   outputPtr = this->GetOutput();
  OutputDirectionPointerType outputDirPtr = this->GetOutputDirection();

  // Neighborhood radius
  RadiusType r;
  r.Fill(1);

  // Find the data-set boundary "faces"
  BoundaryFacesCalculatorType bC;
  FaceListType                faceList = bC(inputPtr, outputRegionForThread, r);
  FaceListIteratorType        fit;

  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  // Process each of the boundary faces.  These are N-d regions which border
  // the edge of the buffer.
  for (fit = faceList.begin(); fit != faceList.end(); ++fit)
    {
    NeighborhoodIteratorType neighInputIt(r, inputPtr, *fit);
    OutputIteratorType outputIt(outputPtr, *fit);
    OutputDirectionIteratorType outputDirIt(outputDirPtr, *fit);
    neighInputIt.GoToBegin();
    outputIt.GoToBegin();
    outputDirIt.GoToBegin();

    while ((!neighInputIt.IsAtEnd()) && (!outputIt.IsAtEnd()) && (!outputDirIt.IsAtEnd()))
      {
      // local variable intialization
      int    neighborhoodNumberMax = 0;
      double scalarMaxValue = 0;
      //TODO for a more general algorithm this might be replaced by:
      //double scalarMaxValue = itk::NumericTraits<double>::NonpositiveMin();
      int flagPosNegDirection = 0;

      // walk through each case
      for (int neighborhoodNumber = 0; neighborhoodNumber < 4; ++neighborhoodNumber)
        {
        double     scalarCurrentValue = 0.0;
        OffsetType offset1;
        OffsetType offset2;
        switch (neighborhoodNumber)
          {
          case 0:
            offset1[0] = 1;
            offset1[1] = -1;
            offset2[0] = -1;
            offset2[1] = 1;
            break;
          case 1:
            offset1[0] = 1;
            offset1[1] = 0;
            offset2[0] = -1;
            offset2[1] = 0;
            break;
          case 2:
            offset1[0] = 1;
            offset1[1] = 1;
            offset2[0] = -1;
            offset2[1] = -1;
            break;
          case 3:
            offset1[0] = 0;
            offset1[1] = 1;
            offset2[0] = 0;
            offset2[1] = -1;
            break;
          }
        // Get the gradient values
        InputPixelType pixel1 = neighInputIt.GetPixel(offset1);
        InputPixelType pixel2 = neighInputIt.GetPixel(offset2);

        // Compute the scalar product
        scalarCurrentValue = -(pixel1[0] * pixel2[0] + pixel1[1] * pixel2[1]);

        // If the value is higher than the current max value
        if (scalarCurrentValue > scalarMaxValue)
          {
          // keep this configuration
          scalarMaxValue = scalarCurrentValue;
          neighborhoodNumberMax = neighborhoodNumber;

          // Also keep the direction
          if (pixel1[0] < 0)
            {
            flagPosNegDirection = 1;
            }
          else
            {
            flagPosNegDirection = 0;
            }

          }
        }
      // Compute the direction
      double angle = static_cast<double>((1 + neighborhoodNumberMax)) * static_cast<double>(CONST_PI_4);
      if (flagPosNegDirection)
        {
        angle -= CONST_PI;
        }

      // Set the ouptut values
      outputIt.Set(scalarMaxValue);
      outputDirIt.Set(angle);
      ++neighInputIt;
      ++outputIt;
      ++outputDirIt;
      progress.CompletedPixel();
      }
    }
}
/**
 * PrintSelf Method
 */
template <class TInputImage, class TOutputModulus, class TOutputDirection>
void
NeighborhoodScalarProductFilter<TInputImage, TOutputModulus, TOutputDirection>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
