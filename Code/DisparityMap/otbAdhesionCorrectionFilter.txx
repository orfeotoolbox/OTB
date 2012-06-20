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
#ifndef __otbAdhesionCorrectionFilter_txx
#define __otbAdhesionCorrectionFilter_txx

#include "otbAdhesionCorrectionFilter.h"

#include "itkProgressReporter.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkNormalizedCorrelationImageToImageMetric.h"
#include "itkExceptionObject.h"

namespace otb
{
/**
 * Constructor
 */
template <class TImage,   class TMask>
AdhesionCorrectionFilter<TImage, TMask>
::AdhesionCorrectionFilter()
{
  this->SetNumberOfRequiredInputs( 5 );
  this->SetNumberOfOutputs(3);

  this->SetNthOutput(1,TMask::New());
  this->SetNthOutput(2,TImage::New());

  // Default radius
  m_Radius.Fill(3);
  
  // Default Tolerance
  m_Tolerance = 0.25;
  
  // Default Thresholds
  m_DiscontinuityThreshold  = 10.0;
  m_DiscontinuityHighThreshold = 30.0;
  m_MaxEdgeGap = 100.0;
  m_EdgeThreshold = 100.0;
}

template <class TImage,   class TMask>
void
AdhesionCorrectionFilter<TImage, TMask>
::SetMedianDisparityInput( const TImage * medianmap )
{
  // Process object is not const-correct so the const casting is required.
  SetNthInput(1, const_cast<TImage *>( medianmap ));
}

template <class TImage,   class TMask>
void
AdhesionCorrectionFilter<TImage, TMask>
::SetMedianMaskInput( const TMask * medianmask)
{
  // Process object is not const-correct so the const casting is required.
  SetNthInput(2, const_cast<TMask *>( medianmask ));
}

template <class TImage,   class TMask>
void
AdhesionCorrectionFilter<TImage, TMask>
::SetEdgesDisparityInput( const TImage * cannymedianmap)
{
  // Process object is not const-correct so the const casting is required.
  SetNthInput(3, const_cast<TImage *>( cannymedianmap ));
}

template <class TImage,   class TMask>
void
AdhesionCorrectionFilter<TImage, TMask>
::SetSubPixelMaskInput( const TMask * subpixelmask)
{
  // Process object is not const-correct so the const casting is required.
  SetNthInput(4, const_cast<TMask *>( subpixelmask ));
}

template <class TImage,   class TMask>
const TImage *
AdhesionCorrectionFilter<TImage, TMask>
::GetMedianDisparityInput()
{
  if (this->GetNumberOfInputs()<2)
    {
    return 0;
    }
  return static_cast<const TImage *>(this->itk::ProcessObject::GetInput(1));
}

template <class TImage,   class TMask>
const TMask *
AdhesionCorrectionFilter<TImage, TMask>
::GetMedianMaskInput()
{
  if (this->GetNumberOfInputs()<3)
    {
    return 0;
    }
  return static_cast<const TMask *>(this->itk::ProcessObject::GetInput(2));
}

template <class TImage,   class TMask>
const TImage *
AdhesionCorrectionFilter<TImage, TMask>
::GetEdgesDisparityInput()
{
  if (this->GetNumberOfInputs()<4)
  {
    return 0;
  }
  return static_cast<const TImage *>(this->itk::ProcessObject::GetInput(3));
}

template <class TImage,   class TMask>
const TMask *
AdhesionCorrectionFilter<TImage, TMask>
::GetSubPixelMaskInput()
{
  if (this->GetNumberOfInputs()<5)
    {
    return 0;
    }
  return static_cast<const TMask *>(this->itk::ProcessObject::GetInput(4));
}

template <class TImage,   class TMask>
TMask *
AdhesionCorrectionFilter<TImage, TMask>
::GetOutputMask()
{
  if (this->GetNumberOfOutputs()<2)
    {
    return 0;
    }
  return static_cast<TMask *>(this->itk::ProcessObject::GetOutput(1));
}

template <class TImage,   class TMask>
TImage *
AdhesionCorrectionFilter<TImage, TMask>
::GetOutputRiskEdges()
{
  if (this->GetNumberOfOutputs()<3)
    {
    return 0;
    }
  return static_cast<TImage *>(this->itk::ProcessObject::GetOutput(2));
}

template <class TImage,   class TMask>
void
AdhesionCorrectionFilter<TImage, TMask>
::GenerateOutputInformation()
{
  // Call superclass implementation
  Superclass::GenerateOutputInformation();

  // Retrieve output pointers
  TImage * outputPtr = this->GetOutput();
  TMask * outputMaskPtr = this->GetOutputMask();
  TImage * outputriskedgesPtr = this->GetOutputRiskEdges();

  // Update size and spacing according to grid step
  ImageRegionType largestRegion  = outputPtr->GetLargestPossibleRegion();
  SizeType outputSize       = largestRegion.GetSize();
  m_ImageSize = outputSize;
  SpacingType outputSpacing = outputPtr->GetSpacing();

  // Set spacing
  outputPtr->SetSpacing(outputSpacing);
  outputMaskPtr->SetSpacing(outputSpacing);
  outputriskedgesPtr->SetSpacing(outputSpacing);

  // Set largest region size
  largestRegion.SetSize(outputSize);
  outputPtr->SetLargestPossibleRegion(largestRegion);
  outputMaskPtr->SetLargestPossibleRegion(largestRegion);
  outputriskedgesPtr->SetLargestPossibleRegion(largestRegion);
}

template <class TImage,   class TMask>
void
AdhesionCorrectionFilter<TImage, TMask>
::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();

  // get pointers to the input and output
  TImage * canny_edges  = const_cast< TImage * >( this->GetInput());
  TImage * old_disparityPtr  = const_cast< TImage * >( this->GetMedianDisparityInput());
  TMask * old_maskPtr = const_cast< TMask * >( this->GetMedianMaskInput());
  TImage * canny_disparity = const_cast< TImage * >(this->GetEdgesDisparityInput());
  TMask * subpixelmaskPtr = const_cast< TMask * >(this->GetSubPixelMaskInput());
  TImage * outputPtr = this->GetOutput();
  TMask * outputmaskPtr = this->GetOutputMask();
  TImage * outputriskedgesPtr = this->GetOutputRiskEdges();

  if ( !canny_edges || !old_disparityPtr || !old_maskPtr || !canny_disparity || !subpixelmaskPtr ||
       !outputPtr || !outputmaskPtr || !outputriskedgesPtr)
    {
    return;
    }

  // get a copy of the fixed requested region (should equal the output
  // requested region)
  ImageRegionType RequestedRegion;
  RequestedRegion = outputPtr->GetRequestedRegion();

  SizeType Big_Radius;
  Big_Radius[0] = m_Radius[0] +2;
  Big_Radius[1] = m_Radius[1] +2;
  RequestedRegion.PadByRadius( Big_Radius );

  // crop the  region at the largest possible region
  if ( RequestedRegion.Crop(canny_edges->GetLargestPossibleRegion()))
    {
    canny_edges->SetRequestedRegion( RequestedRegion );
    }
  else
    {
    // Couldn't crop the region (requested region is outside the largest
    // possible region).  Throw an exception.
    // store what we tried to request (prior to trying to crop)
    canny_edges->SetRequestedRegion( RequestedRegion );

    // build an exception
    itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
    itk::OStringStream msg;
    msg << this->GetNameOfClass()
                << "::GenerateInputRequestedRegion()";
    e.SetLocation(msg.str().c_str());
    e.SetDescription("Requested region is (at least partially) outside the largest possible region of image 1.");
    e.SetDataObject(canny_edges);
    throw e;
    }
  
  // crop the  region at the largest possible region
  if ( RequestedRegion.Crop(old_disparityPtr->GetLargestPossibleRegion()))
    {
    old_disparityPtr->SetRequestedRegion( RequestedRegion );
    }
  else
    {
    // Couldn't crop the region (requested region is outside the largest
    // possible region).  Throw an exception.
    // store what we tried to request (prior to trying to crop)
    old_disparityPtr->SetRequestedRegion( RequestedRegion );

    // build an exception
    itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
    itk::OStringStream msg;
    msg << this->GetNameOfClass()
                << "::GenerateInputRequestedRegion()";
    e.SetLocation(msg.str().c_str());
    e.SetDescription("Requested region is (at least partially) outside the largest possible region of image 1.");
    e.SetDataObject(old_disparityPtr);
    throw e;
    }
  
  // crop the  region at the largest possible region
  if ( RequestedRegion.Crop(old_maskPtr->GetLargestPossibleRegion()))
    {
    old_maskPtr->SetRequestedRegion( RequestedRegion );
    }
  else
    {
    // Couldn't crop the region (requested region is outside the largest
    // possible region).  Throw an exception.
    // store what we tried to request (prior to trying to crop)
    old_maskPtr->SetRequestedRegion( RequestedRegion );

    // build an exception
    itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
    itk::OStringStream msg;
    msg << this->GetNameOfClass()
                << "::GenerateInputRequestedRegion()";
    e.SetLocation(msg.str().c_str());
    e.SetDescription("Requested region is (at least partially) outside the largest possible region of image 1.");
    e.SetDataObject(old_maskPtr);
    throw e;
    }
   
  // crop the  region at the largest possible region
  if ( RequestedRegion.Crop(canny_disparity->GetLargestPossibleRegion()))
    {
    canny_disparity->SetRequestedRegion( RequestedRegion );
    }
  else
    {
    // Couldn't crop the region (requested region is outside the largest
    // possible region).  Throw an exception.
    // store what we tried to request (prior to trying to crop)
    canny_disparity->SetRequestedRegion( RequestedRegion );

    // build an exception
    itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
    itk::OStringStream msg;
    msg << this->GetNameOfClass()
                << "::GenerateInputRequestedRegion()";
    e.SetLocation(msg.str().c_str());
    e.SetDescription("Requested region is (at least partially) outside the largest possible region of image 1.");
    e.SetDataObject(canny_disparity);
    throw e;
    }
    
  // crop the  region at the largest possible region
  if ( RequestedRegion.Crop(subpixelmaskPtr->GetLargestPossibleRegion()))
    {
    subpixelmaskPtr->SetRequestedRegion( RequestedRegion );
    }
  else
    {
    // Couldn't crop the region (requested region is outside the largest
    // possible region).  Throw an exception.
    // store what we tried to request (prior to trying to crop)
    subpixelmaskPtr->SetRequestedRegion( RequestedRegion );

    // build an exception
    itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
    itk::OStringStream msg;
    msg << this->GetNameOfClass()
                << "::GenerateInputRequestedRegion()";
    e.SetLocation(msg.str().c_str());
    e.SetDescription("Requested region is (at least partially) outside the largest possible region of image 1.");
    e.SetDataObject(subpixelmaskPtr);
    throw e;
    }

  return;
}

template <class TImage,   class TMask>
void
AdhesionCorrectionFilter<TImage, TMask>
::GenerateData()
{
  // Allocate outputs
  this->AllocateOutputs();

  // get pointers to the input and output
  TImage * canny_edges  = const_cast< TImage * >( this->GetInput());
  TImage * old_disparityPtr  = const_cast< TImage * >( this->GetMedianDisparityInput());
  TMask * old_maskPtr = const_cast< TMask * >( this->GetMedianMaskInput());
  TImage * canny_disparity = const_cast< TImage * >(this->GetEdgesDisparityInput());
  TMask * subpixelmaskPtr = const_cast< TMask * >(this->GetSubPixelMaskInput());
  TImage * outputPtr = this->GetOutput();
  TMask * outputmaskPtr = this->GetOutputMask();
  TImage * outputriskedgesPtr = this->GetOutputRiskEdges();

  outputriskedgesPtr->FillBuffer(0.0);
  
  int win = m_Radius[0]+1; // should be also equal to m_Radius[1]
  int patch_side = 2*win+1;
  int patch_side_small = 2*(win-1)+1;
  int big_dist = 3*win;
  int dif, new_step;
  /* positions around a pixel*/
  IntVectorType ring;
  IntVectorType pix;
  ring.push_back(-1-outputPtr->GetRequestedRegion().GetSize()[0] - outputPtr->GetRequestedRegion().GetIndex()[0]);
  ring.push_back(0-outputPtr->GetRequestedRegion().GetSize()[0] -  outputPtr->GetRequestedRegion().GetIndex()[0]);
  ring.push_back(1-outputPtr->GetRequestedRegion().GetSize()[0]  - outputPtr->GetRequestedRegion().GetIndex()[0]);
  ring.push_back(1);
  ring.push_back(1+outputPtr->GetRequestedRegion().GetSize()[0] + outputPtr->GetRequestedRegion().GetIndex()[0]);
  ring.push_back(outputPtr->GetRequestedRegion().GetSize()[0] + outputPtr->GetRequestedRegion().GetIndex()[0]);
  ring.push_back(-1+outputPtr->GetRequestedRegion().GetSize()[0] + outputPtr->GetRequestedRegion().GetIndex()[0]);
  ring.push_back(-1);
  
//// Compute incoherences (jumps) of the disparity map in the epipolar direction
//// disparity_jump = mask of jumps in the epipolar direction:
//// flags to disparity jump:   1--->jump due to a left border
////        2--->jump due to a right border
////        3--->jump due to a hole. Seems to be a left border
////        4--->jump due to a hole. Seems to be a right border

  AuxImagePointerType disparity_jump = AuxImageType::New();
  disparity_jump->SetRegions(old_disparityPtr->GetRequestedRegion());
  disparity_jump->Allocate();
  disparity_jump->FillBuffer(0);
  
  AuxImagePointerType disparity_jump2 = AuxImageType::New();
  disparity_jump2->SetRegions(old_disparityPtr->GetRequestedRegion());
  disparity_jump2->Allocate();
  disparity_jump2->FillBuffer(0);
  
  AuxImagePointerType aux = AuxImageType::New();
  aux->SetRegions(outputPtr->GetRequestedRegion());
  aux->Allocate();
  aux->FillBuffer(0);
  
  /** input iterators */
  itk::ImageRegionConstIterator<TImage> old_disparityIt(old_disparityPtr,old_disparityPtr->GetRequestedRegion());
  itk::ImageRegionConstIterator<TMask> old_maskIt(old_maskPtr,old_maskPtr->GetRequestedRegion());
  itk::ImageRegionConstIterator<TImage> canny_disparityIt(canny_disparity,canny_disparity->GetRequestedRegion());
  itk::ImageRegionConstIterator<TImage> canny_edgesIt(canny_edges,canny_edges->GetRequestedRegion());

  IndexType index, index2, index_pos, index_pos_actual, index_pos_old, index_pos_new, index_pos0;
  
  /** Output iterators */
  itk::ImageRegionIterator<TImage> new_disparityIt(outputPtr,outputPtr->GetRequestedRegion());
  itk::ImageRegionIterator<TMask> new_maskIt(outputmaskPtr,outputPtr->GetRequestedRegion());
  itk::ImageRegionIterator<AuxImageType> disparity_jumpIt(disparity_jump,outputPtr->GetRequestedRegion());
  itk::ImageRegionIterator<AuxImageType> disparity_jump2It(disparity_jump2,outputPtr->GetRequestedRegion());
  itk::ImageRegionIterator<TImage> risk_edgesIt(outputriskedgesPtr,outputPtr->GetRequestedRegion());
  itk::ImageRegionIterator<AuxImageType> auxIt(aux,outputPtr->GetRequestedRegion());


  new_maskIt.GoToBegin();
  new_disparityIt.GoToBegin();

  /** INITIALISATION */
  while (!new_maskIt.IsAtEnd() && !new_disparityIt.IsAtEnd())
    {
    old_maskIt.SetIndex(new_maskIt.GetIndex());
    old_disparityIt.SetIndex(new_disparityIt.GetIndex());
    new_maskIt.Set(old_maskIt.Get());
    new_disparityIt.Set(old_disparityIt.Get());
    ++old_disparityIt;
    ++new_maskIt;
    ++new_disparityIt;
    ++old_maskIt;
    }

  new_disparityIt.GoToBegin();
  while (new_disparityIt.GetIndex()[1]<outputPtr->GetRequestedRegion().GetSize()[1] + outputPtr->GetRequestedRegion().GetIndex()[1])
    {
    index_pos=new_disparityIt.GetIndex();
    if (new_disparityIt.GetIndex()[1] >= outputPtr->GetRequestedRegion().GetIndex()[1] &&
        new_disparityIt.GetIndex()[1] < m_ImageSize[1] - win)
      {
      old_maskIt.SetIndex(index_pos);
      while (old_maskIt.Get() == 0 &&
        new_disparityIt.GetIndex()[0]<outputPtr->GetRequestedRegion().GetSize()[0] + outputPtr->GetRequestedRegion().GetIndex()[0])
        {
        ++new_disparityIt;
        old_maskIt.SetIndex(new_disparityIt.GetIndex());
        }
      index_pos = new_disparityIt.GetIndex();
      disparity_jumpIt.SetIndex(index_pos);
      disparity_jumpIt.Set(-1); // first disparity in the epipolar line
      int k=0;
      while (new_disparityIt.GetIndex()[0]<outputPtr->GetRequestedRegion().GetSize()[0] + outputPtr->GetRequestedRegion().GetIndex()[0] - win)
        {
        index_pos = new_disparityIt.GetIndex();
        if (old_maskPtr->GetPixel(index_pos) == 0) // holes in the disparity map
          {
          if (k==0)
            {
            k++;
            int i=1;
            index[0]=index_pos[0]+i;
            index[1]=index_pos[1];
            old_maskIt.SetIndex(index);
            new_disparityIt.SetIndex(index);
            while(new_disparityIt.GetIndex()[0]<outputPtr->GetRequestedRegion().GetSize()[0] + outputPtr->GetRequestedRegion().GetIndex()[0] &&
                  old_maskIt.Get()==0)
              {
              i++;
              index[0]=index_pos[0]+i;
              new_disparityIt.SetIndex(index);
              old_maskIt.SetIndex(index);
              }
            if (new_disparityIt.GetIndex()[0] ==  outputPtr->GetRequestedRegion().GetSize()[0] + outputPtr->GetRequestedRegion().GetIndex()[0] - 1)
              {
              disparity_jumpIt.SetIndex(index_pos);
              disparity_jumpIt.Set(4);
              }
            else
              {
              index[0]=index_pos[0]-1;
              index2[0]=index_pos[0]+i;
              index2[1]=index_pos[1];
              if (vcl_abs(old_disparityPtr->GetPixel(index) - old_disparityPtr->GetPixel(index2)) > m_Tolerance)
                {
                if (old_disparityPtr->GetPixel(index) < old_disparityPtr->GetPixel(index2))
                  {
                  index2[0] = index_pos[0] + i -1;
                  disparity_jumpIt.SetIndex(index2);
                  disparity_jumpIt.Set(3);
                  if (i>=win)
                    {
                    disparity_jumpIt.SetIndex(index_pos);
                    disparity_jumpIt.Set(4); // big holes
                    }
                  }
                else
                  {
                  disparity_jumpIt.SetIndex(index_pos);
                  disparity_jumpIt.Set(4);
                  if (i>=win)
                    {
                    index[0]=index_pos[0] + i -1;
                    disparity_jumpIt.SetIndex(index);
                    disparity_jumpIt.Set(3); // big holes
                    }
                  }
                }
              }
            }
          }
        else // Disparity map discontinuities
          {
          if (canny_disparity->GetPixel(index_pos) > m_DiscontinuityThreshold )
            {
            int pp=0;
            
            for(k=0; k<8; k++)
              {
              index[0]=index_pos[0]+ring[k];
              index[1]=index_pos[1];
              if(old_maskPtr->GetPixel(index)==0) pp++;
              }
            if(pp==0)
              {
              index[0]=index_pos[0]-1;
              index[1]=index_pos[1];
              index2[0]=index_pos[0]+1;
              index2[1]=index_pos[1];
              if (old_disparityPtr->GetPixel(index) < old_disparityPtr->GetPixel(index2))
                {
                disparity_jumpIt.SetIndex(index);
                disparity_jumpIt.Set(1);
                }
              if (old_disparityPtr->GetPixel(index) > old_disparityPtr->GetPixel(index2))
                {
                disparity_jumpIt.SetIndex(index2);
                disparity_jumpIt.Set(2);
                }
              }
            }
          k=0;
          }
        new_disparityIt.SetIndex(index_pos);
        ++new_disparityIt;
        }
      }
    index[0]=outputPtr->GetRequestedRegion().GetIndex()[0];
    index[1]=index_pos[1]+1;
    new_disparityIt.SetIndex(index);
    }

  /** Compute jumps of the disparity map in the epipolar direction not detected by Canny */

  new_disparityIt.GoToBegin();
  
  while (new_disparityIt.GetIndex()[1]<outputPtr->GetRequestedRegion().GetSize()[1] + outputPtr->GetRequestedRegion().GetIndex()[1])
    {
    index_pos = new_disparityIt.GetIndex();
    old_maskIt.SetIndex(index_pos);
    while (old_maskIt.Get() == 0 &&
      new_disparityIt.GetIndex()[0] < outputPtr->GetRequestedRegion().GetSize()[0] + outputPtr->GetRequestedRegion().GetIndex()[0] - 1)
      {
      ++new_disparityIt;
      old_maskIt.SetIndex(new_disparityIt.GetIndex());
      }
    index_pos = new_disparityIt.GetIndex();
    old_disparityIt.SetIndex(index_pos);
    double disp = old_disparityIt.Get();
    while (new_disparityIt.GetIndex()[0]<outputPtr->GetRequestedRegion().GetSize()[0] + outputPtr->GetRequestedRegion().GetIndex()[0] - 1)
      {
      index_pos = new_disparityIt.GetIndex();
      index = index_pos;
      index[0] = index_pos[0]+1;
      old_maskIt.SetIndex(index_pos);
      if (old_maskIt.Get() != 0 && old_maskPtr->GetPixel(index) != 0)
        {
        if (vcl_abs(old_disparityPtr->GetPixel(index_pos) - old_disparityPtr->GetPixel(index)) > m_Tolerance)
          {
          if (old_disparityPtr->GetPixel(index) > old_disparityPtr->GetPixel(index_pos) &&
              disparity_jump->GetPixel(index_pos) == 0)
            {
            disparity_jumpIt.SetIndex(index_pos);
            disparity_jumpIt.Set(1);
            }
          if (old_disparityPtr->GetPixel(index) < old_disparityPtr->GetPixel(index_pos)  &&
              disparity_jump->GetPixel(index) == 0)
            {
            disparity_jumpIt.SetIndex(index);
            disparity_jumpIt.Set(2);
            }
          }
        }
      new_disparityIt.SetIndex(index_pos);
      ++new_disparityIt;
      }
    index[0]=outputPtr->GetRequestedRegion().GetIndex()[0];
    index[1]=index_pos[1]+1;
    new_disparityIt.SetIndex(index);
    }
  
  /** Compute intersections between jumps */
  /** disparity_jump2 = mask of jumps in the orthogonal epipolar direction: */

  new_disparityIt.GoToBegin();

  while (new_disparityIt.GetIndex()[1]<outputPtr->GetRequestedRegion().GetSize()[1] + outputPtr->GetRequestedRegion().GetIndex()[1])
    {
    index_pos = new_disparityIt.GetIndex();
    ++new_disparityIt;
    while (new_disparityIt.GetIndex()[0]<outputPtr->GetRequestedRegion().GetSize()[0] + outputPtr->GetRequestedRegion().GetIndex()[0] -1)
      {
      index_pos = new_disparityIt.GetIndex();
      disparity_jumpIt.SetIndex(index_pos);
      if (disparity_jumpIt.Get() == 1 || disparity_jumpIt.Get() == 2 )
        {
        for (int k=0; k<8; k++)
          {
          index=index_pos;
          index[0]=index_pos[0]+ring[k];
          disparity_jumpIt.SetIndex(index);
          if ((disparity_jumpIt.Get() == 1 || disparity_jumpIt.Get() == 2) &&
              disparity_jumpIt.Get() != disparity_jump->GetPixel(index_pos))
            {
            for (int i=-2*win; i<=2*win; i++)
              {
              int l=-2-win;
              index2[0] = index_pos[0] + i;
              index2[1] = index_pos[1] + l;
              while (old_maskPtr->GetPixel(index2) == 0)
                {
                l++;
                index2[1] = index_pos[1] + l;
                }
              if (index2[0]<outputPtr->GetRequestedRegion().GetSize()[0] + outputPtr->GetRequestedRegion().GetIndex()[0] &&
                  index2[1]<outputPtr->GetRequestedRegion().GetSize()[1] + outputPtr->GetRequestedRegion().GetIndex()[1]) // add this condition to avoid seg fault
                {
                disparity_jump2It.SetIndex(index2);
                disparity_jump2It.Set(7);
                }
              }
            }
          }
        }
      new_disparityIt.SetIndex(index_pos);
      ++new_disparityIt;
      }
    index[0]=outputPtr->GetRequestedRegion().GetIndex()[0];
    index[1]=index_pos[1]+1;
    new_disparityIt.SetIndex(index);
    }

  /** Only keep 1 pixel for each discontinuity */
  
  new_disparityIt.GoToBegin();
  
  while (new_disparityIt.GetIndex()[1]<outputPtr->GetRequestedRegion().GetSize()[1] + outputPtr->GetRequestedRegion().GetIndex()[1])
    {
    index_pos = new_disparityIt.GetIndex();
    old_maskIt.SetIndex(index_pos);
    while (old_maskIt.Get() == 0 &&
           new_disparityIt.GetIndex()[0] < outputPtr->GetRequestedRegion().GetSize()[0] + outputPtr->GetRequestedRegion().GetIndex()[0] - 1)
      {
      ++new_disparityIt;
      old_maskIt.SetIndex(new_disparityIt.GetIndex());
      }
    while (new_disparityIt.GetIndex()[0]<outputPtr->GetRequestedRegion().GetSize()[0] + outputPtr->GetRequestedRegion().GetIndex()[0] -1)
      {
      index_pos = new_disparityIt.GetIndex();
      disparity_jumpIt.SetIndex(index_pos);
      if (disparity_jumpIt.Get() == 1)
        {
        int i = 1;
        index = index_pos;
        index[0] = index_pos[0] + i;
        disparity_jumpIt.SetIndex(index);
        while (disparity_jumpIt.Get() == 1 )
          {
          disparity_jumpIt.Set(0);
          i++;
          index[0] = index_pos[0] + i;
          disparity_jumpIt.SetIndex(index);
          }
        }
      disparity_jumpIt.SetIndex(index_pos);
      if (disparity_jumpIt.Get() == 2)
        {
        int i=-1;
        index = index_pos;
        index[0] = index_pos[0] + i;
        disparity_jumpIt.SetIndex(index);
        while (disparity_jumpIt.Get() == 2 )
          {
          disparity_jumpIt.Set(0);
          i--;
          index[0] = index_pos[0] + i;
          disparity_jumpIt.SetIndex(index);
          }
        }
      new_disparityIt.SetIndex(index_pos);
      ++new_disparityIt;
      }
    index[0]=outputPtr->GetRequestedRegion().GetIndex()[0];
    index[1]=index_pos[1]+1;
    new_disparityIt.SetIndex(index);
    }

  /** Risk zone = pixels to the right for jumps with flag 1 or 3 and  pixels to the left for jumps with flag 1 or 3 */
  /** Inside the risk zone, we look for edges which may cause the adhesion--->risk_edges */

  // Exploring the left-right epipolar direction
  new_disparityIt.GoToBegin();
  while (new_disparityIt.GetIndex()[1]<outputPtr->GetRequestedRegion().GetSize()[1] + outputPtr->GetRequestedRegion().GetIndex()[1])
    {
    index_pos = new_disparityIt.GetIndex();
    while (new_disparityIt.GetIndex()[0]<outputPtr->GetRequestedRegion().GetSize()[0] + outputPtr->GetRequestedRegion().GetIndex()[0] -1)
      {
      index_pos = new_disparityIt.GetIndex();
      disparity_jumpIt.SetIndex(index_pos);
      double value_max = m_EdgeThreshold;
      int index_max = -100; // Canny edges should be larger than m_EdgeThreshold
      if (disparity_jumpIt.Get() == 1)
        {
        int l=1;
        while (index_pos[0]+l<outputPtr->GetRequestedRegion().GetSize()[0] + outputPtr->GetRequestedRegion().GetIndex()[0] &&
               l<=big_dist &&
               index_pos[0]+l<outputPtr->GetRequestedRegion().GetSize()[0] + outputPtr->GetRequestedRegion().GetIndex()[0] &&
               index_pos[1]<outputPtr->GetRequestedRegion().GetSize()[1] + outputPtr->GetRequestedRegion().GetIndex()[1])
          {
          index = index_pos;
          index[0] = index_pos[0] + l;
          if (canny_edges->GetPixel(index)>value_max)
            {
            value_max = canny_edges->GetPixel(index);
            index_max = index_pos[0] + l;
            }
          l++;
          }
        }
      if (disparity_jumpIt.Get() == 3)
        {
        int l=1;
        while (index_pos[0]+l<outputPtr->GetRequestedRegion().GetSize()[0] + outputPtr->GetRequestedRegion().GetIndex()[0] &&
               l<=big_dist &&
               index_pos[0]+l<outputPtr->GetRequestedRegion().GetSize()[0] + outputPtr->GetRequestedRegion().GetIndex()[0] &&
               index_pos[1]<outputPtr->GetRequestedRegion().GetSize()[1] + outputPtr->GetRequestedRegion().GetIndex()[1] )
          {
          index = index_pos;
          index[0] = index_pos[0] + l;
          if (canny_edges->GetPixel(index)>value_max)
            {
            value_max = canny_edges->GetPixel(index);
            index_max = index_pos[0] + l;
            }
          l++;
          }
        }
      if (disparity_jumpIt.Get() == 2)
        {
        int l=-1;
        while (index_pos[0]+l>=outputPtr->GetRequestedRegion().GetIndex()[0] &&
               l>=-big_dist &&
               index_pos[0]+l<outputPtr->GetRequestedRegion().GetSize()[0] + outputPtr->GetRequestedRegion().GetIndex()[0] &&
               index_pos[1]<outputPtr->GetRequestedRegion().GetSize()[1] + outputPtr->GetRequestedRegion().GetIndex()[1])
          {
          index = index_pos;
          index[0] = index_pos[0] + l;
          if (canny_edges->GetPixel(index)>value_max)
            {
            value_max = canny_edges->GetPixel(index);
            index_max = index_pos[0] + l;
            }
          l--;
          }
        }
      if (disparity_jumpIt.Get() == 4)
        {
        int l=-1;
        while (index_pos[0]+l>=outputPtr->GetRequestedRegion().GetIndex()[0] &&
               l>=-big_dist &&
               index_pos[0]+l<outputPtr->GetRequestedRegion().GetSize()[0] + outputPtr->GetRequestedRegion().GetIndex()[0] &&
               index_pos[1]<outputPtr->GetRequestedRegion().GetSize()[1] + outputPtr->GetRequestedRegion().GetIndex()[1])
          {
          index = index_pos;
          index[0] = index_pos[0] + l;
          if (canny_edges->GetPixel(index)>value_max)
            {
            value_max = canny_edges->GetPixel(index);
            index_max = index_pos[0] + l;
            }
          l--;
          }
        }
      if(index_max !=-100)
        {
        index = index_pos;
        index[0] = index_max;
        risk_edgesIt.SetIndex(index);
        risk_edgesIt.Set(1);  // flag= 1 for risk edges
        }
      new_disparityIt.SetIndex(index_pos);
      ++new_disparityIt;
      }
    index[0]=outputPtr->GetRequestedRegion().GetIndex()[0];
    index[1]=index_pos[1]+1;
    new_disparityIt.SetIndex(index);
    }

  /** Find extreme of risk edges : extreme points in the edges have flag 2, an the other edge points have flag = 1 */
  
  new_disparityIt.GoToBegin();

  while (new_disparityIt.GetIndex()[1]<outputPtr->GetRequestedRegion().GetSize()[1] + outputPtr->GetRequestedRegion().GetIndex()[1])
    {
    index_pos = new_disparityIt.GetIndex();
    while (new_disparityIt.GetIndex()[0]<outputPtr->GetRequestedRegion().GetSize()[0] + outputPtr->GetRequestedRegion().GetIndex()[0]-1)
      {
      index_pos = new_disparityIt.GetIndex();
      risk_edgesIt.SetIndex(index_pos);
      if (risk_edgesIt.Get() == 1)
        {
        int pp=0;
        for (int k=0; k<8; k++)
          {
          index = index_pos;
          index[0] = index_pos[0] + ring[k];
          risk_edgesIt.SetIndex(index);
          if(risk_edgesIt.Get() > 0) pp++;
          }
        risk_edgesIt.SetIndex(index_pos);
        if (pp == 0) risk_edgesIt.Set(0); //remove isolate points
        if (pp == 1) risk_edgesIt.Set(2);
        if(pp==2)
          {
          int l=0;
          pix.clear();
          for(int k=0; k<8; k++)
            {
            index = index_pos;
            index[0] = index_pos[0] + ring[k];
            risk_edgesIt.SetIndex(index);
            if(risk_edgesIt.Get() > 0)
              {
              pix.push_back(k);
              l++;
              }
            }
          dif = (int) vcl_abs(pix[0]-pix[1]);
          risk_edgesIt.SetIndex(index_pos);
          if (dif==1 || dif==7) risk_edgesIt.Set(2);
          }
        }
      new_disparityIt.SetIndex(index_pos);
      ++new_disparityIt;
      }
    index[0]=outputPtr->GetRequestedRegion().GetIndex()[0];
    index[1]=index_pos[1]+1;
    new_disparityIt.SetIndex(index);
    }

  /** extend extreme edges if necessary :
   *  extreme points in the edges have flag 2, an the other edge points have flag = 1
   */

  new_disparityIt.GoToBegin();

  while (new_disparityIt.GetIndex()[1]<outputPtr->GetRequestedRegion().GetSize()[1] + outputPtr->GetRequestedRegion().GetIndex()[1] -1)
    {
    index_pos = new_disparityIt.GetIndex();
    ++new_disparityIt;
    while (new_disparityIt.GetIndex()[0]<outputPtr->GetRequestedRegion().GetSize()[0] + outputPtr->GetRequestedRegion().GetIndex()[0] -1)
      {
      index_pos = new_disparityIt.GetIndex();
      risk_edgesIt.SetIndex(index_pos);
      if (risk_edgesIt.Get() == 2)
        {
        new_step = 1;
        index_pos_old = index_pos;
        while (new_step == 1)
          {
          new_step = 0;
          for(int k=0; k<8; k++)
            {//Ignore merging cases
            index_pos_actual = index_pos_old;
            index_pos_actual[0] = index_pos_old[0] +ring[k];
            double m_max = 0;
            // if the next pixel is the continuation of the border,we extend the risk edge //
            // The extend the border, it has to be as prevalent as before //
            canny_edgesIt.SetIndex(index_pos_actual);
            if (canny_edgesIt.Get()>m_DiscontinuityHighThreshold &&
                vcl_abs(canny_edgesIt.Get() - canny_edges->GetPixel(index_pos_old)) < m_MaxEdgeGap)
              {
              if (outputriskedgesPtr->GetPixel(index_pos_actual) ==0 && canny_edges->GetPixel(index_pos_actual) > m_max)
                {
                index_pos_new = index_pos_actual;
                new_step = 1;
                }
              }
            }
          if (new_step ==1)
            {
            risk_edgesIt.SetIndex(index_pos_old);
            risk_edgesIt.Set(1);
            risk_edgesIt.SetIndex(index_pos_new);
            risk_edgesIt.Set(2);
            auxIt.SetIndex(index_pos_new);
            auxIt.Set(1);
            index_pos_old = index_pos_new;
            //steps++;
            }
          }
        }
      new_disparityIt.SetIndex(index_pos);
      ++new_disparityIt;
      }
    index[0]=outputPtr->GetRequestedRegion().GetIndex()[0];
    index[1]=index_pos[1]+1;
    new_disparityIt.SetIndex(index);
    }

  /** Cut risk edges
   *  remove pixel edges when the associated patch has the same disparity
   * Ie: the patch is in a planar surfarce without jumps
   */

  int Count;
  int big_win = win + 1;
  int half_big_win = (2*big_win +1)*(2*big_win +1) /2;
  double Tol2 = m_Tolerance/2;
  
  new_disparityIt.GoToBegin();

  while (new_disparityIt.GetIndex()[1]<outputPtr->GetRequestedRegion().GetSize()[1] + outputPtr->GetRequestedRegion().GetIndex()[1])
    {
    index_pos = new_disparityIt.GetIndex();
    if (index_pos[1]>=big_win + outputPtr->GetRequestedRegion().GetIndex()[1])
      {
      while (new_disparityIt.GetIndex()[0]<outputPtr->GetRequestedRegion().GetSize()[0] + outputPtr->GetRequestedRegion().GetIndex()[0] -1)
        {
        index_pos = new_disparityIt.GetIndex();
        auxIt.SetIndex(index_pos);
        if( auxIt.Get() !=0 )
          {
          double m_max = -10000000.;
          double m_min = -m_max;
          int Count=0;
          for(int j= -big_win; j<= big_win; j++)
            {
            for(int i= -big_win; i<= big_win; i++)
              {
              if (index_pos[0] + i >= old_disparityPtr->GetRequestedRegion().GetIndex()[0] &&
                  index_pos[0] + i < old_disparityPtr->GetRequestedRegion().GetSize()[0] + old_disparityPtr->GetRequestedRegion().GetIndex()[0] &&
                  index_pos[1] + j >= old_disparityPtr->GetRequestedRegion().GetIndex()[1] &&
                  index_pos[1] + j < old_disparityPtr->GetRequestedRegion().GetSize()[1] + old_disparityPtr->GetRequestedRegion().GetIndex()[1])
                {
                index_pos0[0] = index_pos[0] + i;
                index_pos0[1] = index_pos[1] + j;
                old_maskIt.SetIndex(index_pos0);
                if (old_maskIt.Get() == 1)
                  {
                  if (old_disparityPtr->GetPixel(index_pos0) > m_max) m_max = old_disparityPtr->GetPixel(index_pos0);
                  if (old_disparityPtr->GetPixel(index_pos0) < m_min) m_min = old_disparityPtr->GetPixel(index_pos0);
                  Count++;
                  }
                new_disparityIt.SetIndex(index_pos);
                }
              }
            }
          /// If we have ~ the same disparity in a patch of radius big_win and we known the disparity for more the half pixels in this patch //
          risk_edgesIt.SetIndex(index_pos);
          if ((m_max-m_min)<Tol2 && Count>0) risk_edgesIt.Set(0);
          else
            {
            for(int j= -win; j<= win; j++)
              {
              for(int i= -win; i<= win; i++)
                {
                if (index_pos[0] + i >= outputPtr->GetRequestedRegion().GetIndex()[0] &&
                    index_pos[0] + i < outputPtr->GetRequestedRegion().GetSize()[0] + outputPtr->GetRequestedRegion().GetIndex()[0] &&
                    index_pos[1] + j >= outputPtr->GetRequestedRegion().GetIndex()[1] &&
                    index_pos[1] + j < outputPtr->GetRequestedRegion().GetSize()[1] + outputPtr->GetRequestedRegion().GetIndex()[1])
                  {
                  index_pos0[0] = index_pos[0] + i;
                  index_pos0[1] = index_pos[1] + j;
                  new_disparityIt.SetIndex(index_pos0);
                  new_maskIt.SetIndex(index_pos0);
                  new_disparityIt.Set(0);
                  new_maskIt.Set(0);
                  }
                }
              }
            }
          }
        new_disparityIt.SetIndex(index_pos);
        ++new_disparityIt;
        index_pos = new_disparityIt.GetIndex();
        }
      }
    index[0]=outputPtr->GetRequestedRegion().GetIndex()[0];
    index[1]=index_pos[1]+1;
    new_disparityIt.SetIndex(index);
    }

  /** Remove edges with only 1 or 2 pixels */
  
  new_disparityIt.GoToBegin();
  
  while (new_disparityIt.GetIndex()[1]<outputPtr->GetRequestedRegion().GetSize()[1] + outputPtr->GetRequestedRegion().GetIndex()[1]- 1)
    {
    ++new_disparityIt;
    while (new_disparityIt.GetIndex()[0]<outputPtr->GetRequestedRegion().GetSize()[0] + outputPtr->GetRequestedRegion().GetIndex()[0]-1)
      {
      index_pos = new_disparityIt.GetIndex();
      risk_edgesIt.SetIndex(index_pos);
      if (risk_edgesIt.Get() == 2)
        {
        int pp=0;
        for(int k=0; k<8; k++)
          {
          index[0]=index_pos[0]+ring[k];
          index[1]=index_pos[1];
          risk_edgesIt.SetIndex(index);
          if (risk_edgesIt.Get() == 2)
            {// 2pix edge
            risk_edgesIt.Set(0);
            risk_edgesIt.SetIndex(index_pos);
            risk_edgesIt.Set(0);
            }
          risk_edgesIt.SetIndex(index);
          if (risk_edgesIt.Get() == 0) pp++;
          }
        risk_edgesIt.SetIndex(index_pos);
        if (pp == 8) risk_edgesIt.Set(0); // 1pix edge
        }
      ++new_disparityIt;
      ++disparity_jumpIt;
      }
    index[0]=outputPtr->GetRequestedRegion().GetIndex()[0];
    index[1]=index_pos[1]+1;
    new_disparityIt.SetIndex(index);
    }
  
  /** Remove pixels risking adhesion */
  
  new_disparityIt.GoToBegin();

  while (new_disparityIt.GetIndex()[1]<outputPtr->GetRequestedRegion().GetSize()[1] + outputPtr->GetRequestedRegion().GetIndex()[1] - 1)
    {
    index_pos = new_disparityIt.GetIndex();
    old_maskIt.SetIndex(index_pos);
    while (old_maskIt.Get() == 0 &&
      new_disparityIt.GetIndex()[0] < outputPtr->GetRequestedRegion().GetSize()[0] + outputPtr->GetRequestedRegion().GetIndex()[0]- 1)
      {
      ++new_disparityIt;
      old_maskIt.SetIndex(new_disparityIt.GetIndex());
      }
    index_pos = new_disparityIt.GetIndex();
    while (new_disparityIt.GetIndex()[0]<outputPtr->GetRequestedRegion().GetSize()[0] + outputPtr->GetRequestedRegion().GetIndex()[0] - 1)
      {
      index_pos = new_disparityIt.GetIndex();
      int pp=0;
      int pp2=0;
      if (outputriskedgesPtr->GetPixel(index_pos) != 0)
        {// left
        int l=-patch_side;
        index[0]=index_pos[0]+l;
        index[1]=index_pos[1];
        while ((new_disparityIt.GetIndex()[0] + l) >= outputPtr->GetRequestedRegion().GetIndex()[0] &&
                l<=0 &&
                disparity_jump->GetPixel(index) !=1 )
          {
          l++;
          index[0]=index_pos[0]+l;
          }
        index[0]=index_pos[0]+l;
        if (disparity_jump->GetPixel(index) == 1)
          {
          pp++;
          for (int i=l+1; i<=0; i++)
            {
            index[0]=index_pos[0]+i;
            new_disparityIt.SetIndex(index);
            new_disparityIt.Set(0);
            new_maskIt.SetIndex(index);
            new_maskIt.Set(0);
            }
          int i=1;
          index[0]=index_pos[0]+i;
          while (disparity_jump->GetPixel(index) !=1 && i<=win)
            {
            index[0]=index_pos[0]+i;
            new_disparityIt.SetIndex(index);
            new_disparityIt.Set(0);
            new_maskIt.SetIndex(index);
            new_maskIt.Set(0);
            i++;
            index[0]=index_pos[0]+i;
            }
          pp2=0;
          index[0]=index_pos[0]+i;
          index[1]=index_pos[1];
          if(disparity_jump->GetPixel(index) == 1)
            {//2a adhesion
            for(int j=1; j<=win; j++)
              {
              index2[0]=index_pos[0]+i+j;
              index2[1]=index_pos[1];
              risk_edgesIt.SetIndex(index2);
              if (risk_edgesIt.Get() != 0) pp2++;
              }
            index[0]=index_pos[0]+i;
            disparity_jumpIt.SetIndex(index);
            if (pp2==0) disparity_jumpIt.Set(0);
            }
          index[0]=index_pos[0]+i+1;
          disparity_jumpIt.SetIndex(index);
          if(disparity_jumpIt.Get() == 1)
            {
            for(int j=1; j<=win; j++)
              {
              index2[0]=index_pos[0]+i+1+j;
              index2[1]=index_pos[1];
              risk_edgesIt.SetIndex(index2);
              if (risk_edgesIt.Get() != 0) pp2++;
              }
            index[0]=index_pos[0]+i+1;
            disparity_jumpIt.SetIndex(index);
            if (pp2==0) disparity_jumpIt.Set(0);
            }
          }
        l=-big_dist;
        index[0]=index_pos[0]+l;
        index[1]=index_pos[1];
        while ((new_disparityIt.GetIndex()[0] + l) >= outputPtr->GetRequestedRegion().GetIndex()[0] &&
                l<=0 &&
                disparity_jump->GetPixel(index) != 3)
          {
          l++;
          index[0]=index_pos[0]+l;
          }
        index[0]=index_pos[0]+l;
        if(disparity_jump->GetPixel(index) == 3)
          {
          pp++;
          for(int i=l+1; i<=0; i++)
            {
            index[0]=index_pos[0]+i;
            new_disparityIt.SetIndex(index);
            new_disparityIt.Set(0);
            new_maskIt.SetIndex(index);
            new_maskIt.Set(0);
            }
          int i=1;
          index[0]=index_pos[0]+i;
          while (disparity_jump->GetPixel(index) !=1 && i<=win)
            {
            index[0]=index_pos[0]+i;
            new_disparityIt.SetIndex(index);
            new_disparityIt.Set(0);
            new_maskIt.SetIndex(index);
            new_maskIt.Set(0);
            i++;
            index[0]=index_pos[0]+i;
            }
          pp2=0;
          index[0]=index_pos[0]+i;
          if (disparity_jump->GetPixel(index)==1)
            {//2a adhesion
            for (int j=1; j<=win; j++)
              {
              index2[0]=index_pos[0]+i+j;
              index2[1]=index_pos[1];
              if(outputriskedgesPtr->GetPixel(index2) != 0) pp2++;
              }
            index[0]=index_pos[0]+i;
            disparity_jumpIt.SetIndex(index);
            if (pp2 == 0) disparity_jumpIt.Set(0);
            }
          index[0]=index_pos[0]+i+1;
          if (disparity_jump->GetPixel(index)==1)
            {
            for (int j=1; j<=win; j++)
              {
              index2[0]=index_pos[0]+i+1+j;
              index2[1]=index_pos[1];
              if(outputriskedgesPtr->GetPixel(index2) != 0) pp2++;
              }
            index[0]=index_pos[0]+i+1;
            disparity_jumpIt.SetIndex(index);
            if (pp2 == 0) disparity_jumpIt.Set(0);
            }
          }
        // right
        l=patch_side;
        index[0]=index_pos[0]+l;
        index[1]=index_pos[1];
        while ((new_disparityIt.GetIndex()[0] + l) < outputPtr->GetRequestedRegion().GetSize()[0] + outputPtr->GetRequestedRegion().GetIndex()[0]- 1 &&
                l>=0 &&
                disparity_jump->GetPixel(index) != 2)
          {
          l--;
          index[0]=index_pos[0]+l;
          }
        index[0]=index_pos[0]+l;
        if(disparity_jump->GetPixel(index)==2)
          {
          pp++;
          for (int i=l-1; i>=0; i--)
            {
            index[0]=index_pos[0]+i;
            new_disparityIt.SetIndex(index);
            new_disparityIt.Set(0);
            new_maskIt.SetIndex(index);
            new_maskIt.Set(0);
            }
          int i=-1;
          index[0]=index_pos[0]+i;
          while (disparity_jump->GetPixel(index) != 2 && i>=-win)
            {
            index[0]=index_pos[0]+i;
            new_disparityIt.SetIndex(index);
            new_disparityIt.Set(0);
            new_maskIt.SetIndex(index);
            new_maskIt.Set(0);
            i--;
            index[0]=index_pos[0]+i;
            }
          pp2 = 0;
          index[0]=index_pos[0]+i;
          if (disparity_jump->GetPixel(index) == 2)
            {//2a adhesion
            for (int j=-1; j>=-win; j--)
              {
              index2[0]=index_pos[0]+i+j;
              index2[1]=index_pos[1];
              if (outputriskedgesPtr->GetPixel(index2) !=0 ) pp2++;
              }
            index[0]=index_pos[0]+i;
            disparity_jumpIt.SetIndex(index);
            if (pp2==0) disparity_jumpIt.Set(0);
            }
          index[0]=index_pos[0]+i-1;
          if (disparity_jump->GetPixel(index) == 2)
            {
            for (int j=-1; j>=-win; j--)
              {
              index2[0]=index_pos[0]+i-1+j;
              index2[1]=index_pos[1];
              if (outputriskedgesPtr->GetPixel(index2) !=0 ) pp2++;
              }
            index[0]=index_pos[0]+i-1;
            disparity_jumpIt.SetIndex(index);
            if (pp2==0) disparity_jumpIt.Set(0);
            }
          }
        l=big_dist;
        index[0]=index_pos[0]+l;
        index[1]=index_pos[1];
        while ((new_disparityIt.GetIndex()[0] + l) < outputPtr->GetRequestedRegion().GetSize()[0] + outputPtr->GetRequestedRegion().GetIndex()[0]- 1 &&
                l>=0 &&
                disparity_jump->GetPixel(index) != 4)
          {
          l--;
          index[0]=index_pos[0]+l;
          }
        index[0]=index_pos[0]+l;
        if (disparity_jump->GetPixel(index) == 4)
          {
          pp++;
          for (int i=l-1; i>=0; i--)
            {
            index[0]=index_pos[0]+i;
            new_disparityIt.SetIndex(index);
            new_disparityIt.Set(0);
            new_maskIt.SetIndex(index);
            new_maskIt.Set(0);
            }
          int i=-1;
          index[0]=index_pos[0]+i;
          while (disparity_jump->GetPixel(index) != 2 && i>=-win)
            {
            index[0]=index_pos[0]+i;
            new_disparityIt.SetIndex(index);
            new_disparityIt.Set(0);
            new_maskIt.SetIndex(index);
            new_maskIt.Set(0);
            i--;
            index[0]=index_pos[0]+i;
            }
          pp2=0;
          index[0]=index_pos[0]+i;
          if (disparity_jump->GetPixel(index) == 2)
            {//2a adhesion
            for (int j=-1; j>=-win; j--)
              {
              index2[0]=index_pos[0]+i+j;
              index2[1]=index_pos[1];
              if (outputriskedgesPtr->GetPixel(index2) !=0 ) pp2++;
              }
            index[0]=index_pos[0]+i;
            disparity_jumpIt.SetIndex(index);
            if (pp2==0) disparity_jumpIt.Set(0);
            }
          index[0]=index_pos[0]+i-1;
          if (disparity_jump->GetPixel(index) == 2)
            {
            for (int j=-1; j>=-win; j--)
              {
              index2[0]=index_pos[0]+i-1+j;
              index2[1]=index_pos[1];
              if (outputriskedgesPtr->GetPixel(index2) !=0 ) pp2++;
              }
            index[0]=index_pos[0]+i-1;
            disparity_jumpIt.SetIndex(index);
            if (pp2==0) disparity_jumpIt.Set(0);
            }
          }
        // if(pp==0) /// remove around the risk_edge if no disparity jump have been found
        // unique difference between new_adhesion and new_adhesion_2
        // the version 2 commented the previous line
        for (int i=-win+1; i<=win-1; i++)
          {
          for (int k=-win+1; k<=win-1; k++)
            {
            index2[0]=index_pos[0]+i;
            index2[1]=index_pos[1]+k;
            if (index2[0]>=outputPtr->GetRequestedRegion().GetIndex()[0] &&
                index2[1]>=outputPtr->GetRequestedRegion().GetIndex()[1] &&
                index2[0]<outputPtr->GetRequestedRegion().GetSize()[0] + outputPtr->GetRequestedRegion().GetIndex()[0] &&
                index2[1]<outputPtr->GetRequestedRegion().GetSize()[1] + outputPtr->GetRequestedRegion().GetIndex()[1])
              {
              new_disparityIt.SetIndex(index2);
              new_maskIt.SetIndex(index2);
              new_disparityIt.Set(0);
              new_maskIt.Set(0);
              }
            }
          }
        }
      new_disparityIt.SetIndex(index_pos);
      ++new_disparityIt;
      }
    index[0]=outputPtr->GetRequestedRegion().GetIndex()[0];
    index[1]=index_pos[1]+1;
    new_disparityIt.SetIndex(index);
    }

  /** remove around the  disparity jump if no risk_edge have been found */
  
  new_disparityIt.GoToBegin();
  
  while (new_disparityIt.GetIndex()[1]<outputPtr->GetRequestedRegion().GetSize()[1] + outputPtr->GetRequestedRegion().GetIndex()[1])
    {
    index_pos = new_disparityIt.GetIndex();
    old_maskIt.SetIndex(index_pos);
    while (old_maskIt.Get() == 0 &&
      new_disparityIt.GetIndex()[0] < outputPtr->GetRequestedRegion().GetSize()[0] + outputPtr->GetRequestedRegion().GetIndex()[0] - 1)
      {
      ++new_disparityIt;
      old_maskIt.SetIndex(new_disparityIt.GetIndex());
      }
    while (new_disparityIt.GetIndex()[0]<outputPtr->GetRequestedRegion().GetSize()[0] + outputPtr->GetRequestedRegion().GetIndex()[0] - 1)
      {
      index_pos = new_disparityIt.GetIndex();
      index[0]=index_pos[0]+1;
      index[1]=index_pos[1];
      new_maskIt.SetIndex(index);
      risk_edgesIt.SetIndex(index);
      if ((disparity_jump->GetPixel(index_pos)==1 || disparity_jump->GetPixel(index_pos)==3) &&
          new_maskIt.Get() !=0 &&
          risk_edgesIt.Get() ==0 )
        {
        int i=1;
        index[0]=index_pos[0]+i;
        disparity_jumpIt.SetIndex(index);
        while (i<=patch_side_small &&
          disparity_jumpIt.GetIndex()[0]<outputPtr->GetRequestedRegion().GetSize()[0] + outputPtr->GetRequestedRegion().GetIndex()[0] - 1 &&
          disparity_jumpIt.Get()==0)
          {
          new_disparityIt.SetIndex(index);
          new_maskIt.SetIndex(index);
          new_disparityIt.Set(0);
          new_maskIt.Set(0);
          i++;
          index[0]=index_pos[0]+i;
          disparity_jumpIt.SetIndex(index);
          }
        }
      index[0]=index_pos[0]-1;
      index[1]=index_pos[1];
      new_maskIt.SetIndex(index);
      risk_edgesIt.SetIndex(index);
      if ((disparity_jump->GetPixel(index_pos)==2 || disparity_jump->GetPixel(index_pos)==4) &&
          new_maskIt.Get() !=0 &&
          risk_edgesIt.Get() ==0 )
        {
        int i=-1;
        index[0]=index_pos[0]+i;
        disparity_jumpIt.SetIndex(index);
        while (i>=-patch_side_small &&
          disparity_jumpIt.GetIndex()[0]<outputPtr->GetRequestedRegion().GetSize()[0] + outputPtr->GetRequestedRegion().GetIndex()[0] - 1 &&
          disparity_jumpIt.Get()==0 )
          {
          new_disparityIt.SetIndex(index);
          new_maskIt.SetIndex(index);
          new_disparityIt.Set(0);
          new_maskIt.Set(0);
          i--;
          index[0]=index_pos[0]+i;
          disparity_jumpIt.SetIndex(index);
          }
        }
      new_disparityIt.SetIndex(index_pos);
      ++new_disparityIt;
      }
    index[0]=outputPtr->GetRequestedRegion().GetIndex()[0];
    index[1]=index_pos[1]+1;
    new_disparityIt.SetIndex(index);
    }

  /**   Vertical lines (perpendicular to epipolar lines) */

  /** Jumps of the disaprity map in the other diection
   *  disparity_jump_2:     flag 7---> special case: intersections
   *        flag 5---> jump due to an upper border
   *        flag 6---> jump due to an lower border
   *        flag 8---> jump due to an upper border + intersection
   *        flag 9---> jump due to an lower border + intersection
   */

  new_disparityIt.GoToBegin();

  while (new_disparityIt.GetIndex()[0]<outputPtr->GetRequestedRegion().GetSize()[0] + outputPtr->GetRequestedRegion().GetIndex()[0] - 1)
    {
    index2 = new_disparityIt.GetIndex();
    index=index2;
    old_maskIt.SetIndex(index2);
    while (old_maskIt.Get() == 0 &&
      new_disparityIt.GetIndex()[1] < outputPtr->GetRequestedRegion().GetSize()[1] + outputPtr->GetRequestedRegion().GetIndex()[1] - 1)
      {
      index[1]=index[1]+1;
      old_maskIt.SetIndex(index);
      new_disparityIt.SetIndex(index);
      }
    double disp=old_disparityPtr->GetPixel(index);
    int k=0;
    while (new_disparityIt.GetIndex()[1]<outputPtr->GetRequestedRegion().GetSize()[1] + outputPtr->GetRequestedRegion().GetIndex()[1] -1)
      {
      index_pos = new_disparityIt.GetIndex();
      old_maskIt.SetIndex(index_pos);
      if (old_maskIt.Get() == 0) k++;
      else
        {
        if (vcl_abs(old_disparityPtr->GetPixel(index_pos)-disp)>m_Tolerance && k<=win)
          {//only small holes
          disparity_jump2It.SetIndex(index_pos);
          if (old_disparityPtr->GetPixel(index_pos)>disp)  disparity_jump2It.Set(5);
          index[0]=index_pos[0]-1;
          index[1]=index_pos[1];
          disparity_jump2It.SetIndex(index);
          if (old_disparityPtr->GetPixel(index_pos)<disp)  disparity_jump2It.Set(6);
          }
        k=0;
        disp=old_disparityPtr->GetPixel(index_pos);
        }
      index[0]=index_pos[0];
      index[1]=index_pos[1]+1;
      new_disparityIt.SetIndex(index);
      }
    new_disparityIt.SetIndex(index2);
    ++new_disparityIt;
    }

  /** Vertical lines (perpendicular to epipolar lines) part2 */

  new_disparityIt.GoToBegin();
  
  while (new_disparityIt.GetIndex()[0]<outputPtr->GetRequestedRegion().GetSize()[0] + outputPtr->GetRequestedRegion().GetIndex()[0] -1)
    {
    int k=0;
    index2 = new_disparityIt.GetIndex();
    index=index2;
    while (new_disparityIt.GetIndex()[1]<outputPtr->GetRequestedRegion().GetSize()[1] + outputPtr->GetRequestedRegion().GetIndex()[1] - 1)
      {
      index_pos = new_disparityIt.GetIndex();
      if (disparity_jump2->GetPixel(index_pos) == 7)
        {
        double disp=old_disparityPtr->GetPixel(index_pos);
        for (int i=0; i<=big_dist; i++)
          {
          if (index_pos[1]+i < outputPtr->GetRequestedRegion().GetSize()[1] + outputPtr->GetRequestedRegion().GetIndex()[1])
            {
            index_pos0[0]=index_pos[0];
            index_pos0[1]=index_pos[1]+i;
            if (old_maskPtr->GetPixel(index_pos0)==0) k++;
            else
              {
              if (vcl_abs(old_disparityPtr->GetPixel(index_pos0)-disp)>m_Tolerance/2 && k<=win)
                {//only small holes
                disparity_jump2It.SetIndex(index_pos0);
                if (old_disparityPtr->GetPixel(index_pos0)>disp)  disparity_jump2It.Set(8);
                index[0]=index_pos0[0]-1;
                index[1]=index_pos0[1];
                disparity_jump2It.SetIndex(index);
                if (old_disparityPtr->GetPixel(index_pos0)<disp)  disparity_jump2It.Set(9);
                }
              k=0;
              disp=old_disparityPtr->GetPixel(index_pos0);
              }
            }
          }
        }
      index[0]=index_pos[0];
      index[1]=index_pos[1]+1;
      new_disparityIt.SetIndex(index);
      }
    new_disparityIt.SetIndex(index2);
    ++new_disparityIt;
    }

  /** remove neighborhood if there is an edge near the first disparities */

  new_disparityIt.GoToBegin();
  
  while (new_disparityIt.GetIndex()[1]<outputPtr->GetRequestedRegion().GetSize()[1] + outputPtr->GetRequestedRegion().GetIndex()[1]-1)
    {
    index_pos = new_disparityIt.GetIndex();
    old_maskIt.SetIndex(index_pos);
    while (old_maskIt.Get() == 0 &&
      new_disparityIt.GetIndex()[0] < outputPtr->GetRequestedRegion().GetSize()[0] + outputPtr->GetRequestedRegion().GetIndex()[0] - 1)
      {
      ++new_disparityIt;
      old_maskIt.SetIndex(new_disparityIt.GetIndex());
      }
    index_pos = new_disparityIt.GetIndex();
    for (int l=patch_side; l>0; l--)
      {
      index[0]=index_pos[0]+l;
      index[1]=index_pos[1];
      if (disparity_jump->GetPixel(index) != 0 || disparity_jump2->GetPixel(index) != 0 || outputriskedgesPtr->GetPixel(index) != 0)
        {
        for (int i=-win; i<=win; i++)
          {
          for (int k=-win; k<=win; k++)
            {
            index2[0]=index_pos[0]+l+i;
            index2[1]=index_pos[1]+k;
            if (index2[0]>=outputPtr->GetRequestedRegion().GetIndex()[0] &&
                index2[1]>=outputPtr->GetRequestedRegion().GetIndex()[1] &&
                index2[0]<outputPtr->GetRequestedRegion().GetSize()[0] + outputPtr->GetRequestedRegion().GetIndex()[0] &&
                index2[1]<outputPtr->GetRequestedRegion().GetSize()[1] + outputPtr->GetRequestedRegion().GetIndex()[1])
              {
              new_disparityIt.SetIndex(index2);
              new_maskIt.SetIndex(index2);
              new_disparityIt.Set(0);
              new_maskIt.Set(0);
              }
            }
          }
        }
      }
    index[0]=outputPtr->GetRequestedRegion().GetIndex()[0];
    index[1]=index_pos[1]+1;
    new_disparityIt.SetIndex(index);
    }

  /** Remove pixels risking adhesion in the vertical direction(perpendicular to epipolar) */

  new_disparityIt.GoToBegin();
  while (new_disparityIt.GetIndex()[0]<outputPtr->GetRequestedRegion().GetSize()[0] + outputPtr->GetRequestedRegion().GetIndex()[0] -1)
    {
    index2 = new_disparityIt.GetIndex();
    while (new_disparityIt.GetIndex()[1]<outputPtr->GetRequestedRegion().GetSize()[1] + outputPtr->GetRequestedRegion().GetIndex()[1] - 1)
      {
      index_pos = new_disparityIt.GetIndex();
      if (outputriskedgesPtr->GetPixel(index_pos) != 0)
        {
        int l=0-big_dist;
        index[0]=index_pos[0];
        index[1]=index_pos[1]+l;
        while(l<=0 && disparity_jump2->GetPixel(index) != 5)
          {
          l++;
          index[1]=index_pos[1]+l;
          }
        index[1]=index_pos[1]+l;
        disparity_jump2It.SetIndex(index);
        if(disparity_jump2It.Get() ==5 && l != 0) disparity_jump2It.Set(0);
        for(int i=l; i<=0; i++)
          {
          index[1]=index_pos[1]+i;
          new_disparityIt.SetIndex(index);
          new_maskIt.SetIndex(index);
          new_disparityIt.Set(0);
          new_maskIt.Set(0);
          }
        l=big_dist;
        index[0]=index_pos[0];
        index[1]=index_pos[1]+l;
        while(l>=0 && disparity_jump2->GetPixel(index) != 6)
          {
          l--;
          index[1]=index_pos[1]+l;
          }
        index[1]=index_pos[1]+l;
        disparity_jump2It.SetIndex(index);
        if(disparity_jump2It.Get() == 6 && l != 0) disparity_jump2It.Set(0);
        for(int i=l; i>=0; i--)
          {
          index[1]=index_pos[1]+i;
          new_disparityIt.SetIndex(index);
          new_maskIt.SetIndex(index);
          new_disparityIt.Set(0);
          new_maskIt.Set(0);
          }
        l=-big_dist;
        index[0]=index_pos[0];
        index[1]=index_pos[1]+l;
        while(l<=0 && disparity_jump2->GetPixel(index) != 8)
          {
          l++;
          index[1]=index_pos[1]+l;
          }
        index[1]=index_pos[1]+l;
        disparity_jump2It.SetIndex(index);
        if(disparity_jump2It.Get() == 8 && l != 0) disparity_jump2It.Set(0);
        for(int i=l; i<=0; i++)
          {
          index[1]=index_pos[1]+i;
          new_disparityIt.SetIndex(index);
          new_maskIt.SetIndex(index);
          new_disparityIt.Set(0);
          new_maskIt.Set(0);
          }
        l=big_dist;
        index[0]=index_pos[0];
        index[1]=index_pos[1]+l;
        while(l>=0 && disparity_jump2->GetPixel(index) != 9)
          {
          l--;
          index[1]=index_pos[1]+l;
          }
        index[1]=index_pos[1]+l;
        disparity_jump2It.SetIndex(index);
        if(disparity_jump2It.Get() == 9 && l != 0) disparity_jump2It.Set(0);
        for(int i=l; i>=0; i--)
          {
          index[1]=index_pos[1]+i;
          new_disparityIt.SetIndex(index);
          new_maskIt.SetIndex(index);
          new_disparityIt.Set(0);
          new_maskIt.Set(0);
          }
        }
      index[0]=index_pos[0];
      index[1]=index_pos[1]+1;
      new_disparityIt.SetIndex(index);
      }
    new_disparityIt.SetIndex(index2);
    ++new_disparityIt;
    }

  /** remove pixels around disparity jumps with no risk edges */

  new_disparityIt.GoToBegin();
  while (new_disparityIt.GetIndex()[0]<outputPtr->GetRequestedRegion().GetSize()[0] + outputPtr->GetRequestedRegion().GetIndex()[0] -1)
    {
    index2 = new_disparityIt.GetIndex();
    while (new_disparityIt.GetIndex()[1]<outputPtr->GetRequestedRegion().GetSize()[1] + outputPtr->GetRequestedRegion().GetIndex()[1] - 1)
      {
      index_pos = new_disparityIt.GetIndex();
      if (disparity_jump2->GetPixel(index_pos) == 5 || disparity_jump2->GetPixel(index_pos) == 8)
        {
        int l=0;
        index[0]=index_pos[0];
        index[1]=index_pos[1]+l;
        while (l<=patch_side_small &&
          index_pos[1]+l<outputPtr->GetRequestedRegion().GetSize()[1] + outputPtr->GetRequestedRegion().GetIndex()[1] -1 &&
          disparity_jump2->GetPixel(index) != 6)
          {
          new_disparityIt.SetIndex(index);
          new_maskIt.SetIndex(index);
          new_disparityIt.Set(0);
          new_maskIt.Set(0);
          l++;
          index[1]=index_pos[1]+l;
          }
        }
      if (disparity_jump2->GetPixel(index_pos) == 6 || disparity_jump2->GetPixel(index_pos) == 9)
        {
        int l=0;
        index[0]=index_pos[0];
        index[1]=index_pos[1]+l;
        while (l>=-patch_side_small &&
          index_pos[1]+l>=outputPtr->GetRequestedRegion().GetIndex()[1] &&
          disparity_jump2->GetPixel(index) != 5)
          {
          new_disparityIt.SetIndex(index);
          new_maskIt.SetIndex(index);
          new_disparityIt.Set(0);
          new_maskIt.Set(0);
          l--;
          index[1]=index_pos[1]+l;
          }
        }
      index[0]=index_pos[0];
      index[1]=index_pos[1]+1;
      new_disparityIt.SetIndex(index);
      }
    new_disparityIt.SetIndex(index2);
    ++new_disparityIt;
    }

  /** Reject isolated disparities
   *  ie: In the patch is the only meaningful match
   */
   
  int nb_disp =3;
  new_disparityIt.GoToBegin();

  while (new_disparityIt.GetIndex()[1]<outputPtr->GetRequestedRegion().GetSize()[1] + outputPtr->GetRequestedRegion().GetIndex()[1] -1)
    {
    index_pos = new_disparityIt.GetIndex();
    while (new_disparityIt.GetIndex()[0] <outputPtr->GetRequestedRegion().GetSize()[0] + outputPtr->GetRequestedRegion().GetIndex()[0] - 1)
      {
      index_pos = new_disparityIt.GetIndex();
      new_maskIt.SetIndex(index_pos);
      if (new_maskIt.Get() != 0 && subpixelmaskPtr->GetPixel(index_pos) != 0)
        {
        nb_disp = 0;
        for (int j= -win+1; j<= win-1; j++)
          {
          for (int i= -win+1; i<= win-1; i++)
            {
            index_pos0[0]=index_pos[0]+i;
            index_pos0[1]=index_pos[1]+j;
            new_maskIt.SetIndex(index_pos0);
            if(new_maskIt.Get() != 0 && subpixelmaskPtr->GetPixel(index_pos0) != 0) nb_disp++;
            }
          }
        }
      if(nb_disp<=2)
        {
        new_maskIt.SetIndex(index_pos);
        new_disparityIt.SetIndex(index_pos);
        new_maskIt.Set(0);
        new_disparityIt.Set(0);
        }
      ++new_disparityIt;
      }
    index[0]=outputPtr->GetRequestedRegion().GetIndex()[0];
    index[1]=index_pos[1]+1;
    new_disparityIt.SetIndex(index);
    }

}

} // end namespace otb

#endif
