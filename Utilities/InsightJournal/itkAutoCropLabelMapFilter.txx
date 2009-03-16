/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkAutoCropLabelMapFilter.txx,v $
  Language:  C++
  Date:      $Date: 2004/07/11 14:56:39 $
  Version:   $Revision: 1.7 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkAutoCropLabelMapFilter_txx
#define _itkAutoCropLabelMapFilter_txx
#include "itkAutoCropLabelMapFilter.h"


namespace itk
{

template <class TInputImage>
AutoCropLabelMapFilter<TInputImage>
::AutoCropLabelMapFilter()
{
  m_CropBorder.Fill( 0 );
}


template <class TInputImage>
void 
AutoCropLabelMapFilter<TInputImage>
::GenerateOutputInformation()
{

  const InputImageType * input = this->GetInput();

  if( !(input->GetMTime() > m_CropTimeStamp) && !(this->GetMTime() > m_CropTimeStamp) )
    {
    // early exit, crop sizes already computed
    // std::cout << "Don't recompute the output size again." << std::endl;
    // std::cout << "LargestPossibleRegion: " << this->GetOutput()->GetLargestPossibleRegion() << std::endl;
    // std::cout << "BufferedRegion: " << this->GetOutput()->GetBufferedRegion() << std::endl;
    // std::cout << "RequestedRegion: " << this->GetOutput()->GetRequestedRegion() << std::endl;
    return;
    }
    
  // update the input if needed
  if( input->GetSource() )
    {
    ProcessObject * upstream = input->GetSource();
    if (upstream)
      {
      // this->SetInput(NULL);
      // std::cout << "Update the input (again?)." << std::endl;
      upstream->Update();
      // this->SetInput(input);
      }
    }

  // Prefetch image region and size
  InputImageRegionType cropRegion = input->GetLargestPossibleRegion();

  // just find the bounding box of the objects
  IndexType mins;
  mins.Fill( NumericTraits< long >::max() );
  IndexType maxs;
  maxs.Fill( NumericTraits< long >::NonpositiveMin() );
  // iterate over all the lines
  typename InputImageType::LabelObjectContainerType container = this->GetInput()->GetLabelObjectContainer();
  for( typename InputImageType::LabelObjectContainerType::const_iterator loit = container.begin();
    loit != container.end();
    loit++ )
    {
    const LabelObjectType * labelObject = loit->second;
    typename LabelObjectType::LineContainerType::const_iterator lit;
    const typename LabelObjectType::LineContainerType & lineContainer = labelObject->GetLineContainer();
    for( lit = lineContainer.begin(); lit != lineContainer.end(); lit++ )
      {
      const IndexType & idx = lit->GetIndex();
      unsigned long length = lit->GetLength();
  
      // update the mins and maxs
      for( int i=0; i<ImageDimension; i++)
        {
        if( idx[i] < mins[i] )
          {
          mins[i] = idx[i];
          }
        if( idx[i] > maxs[i] )
          {
          maxs[i] = idx[i];
          }
        }
      // must fix the max for the axis 0
      if( idx[0] + (long)length > maxs[0] )
        {
        maxs[0] = idx[0] + length - 1;
        }
      }
    }
  // final computation
  SizeType regionSize;
  for( int i=0; i<ImageDimension; i++ )
    {
    regionSize[i] = maxs[i] - mins[i] + 1;
    }
  cropRegion.SetIndex( mins );
  cropRegion.SetSize( regionSize );
  
  // pad by the crop border, but take care to not be larger than the largest
  // possible region of the input image
  cropRegion.PadByRadius( m_CropBorder );
  cropRegion.Crop( input->GetLargestPossibleRegion() );

  // finally set that region as the largest output region
  this->SetRegion(cropRegion);
  m_CropTimeStamp.Modified();
  
  Superclass::GenerateOutputInformation();

  // std::cout << "LargestPossibleRegion: " << this->GetOutput()->GetLargestPossibleRegion() << std::endl;
  // std::cout << "BufferedRegion: " << this->GetOutput()->GetBufferedRegion() << std::endl;
  // std::cout << "RequestedRegion: " << this->GetOutput()->GetRequestedRegion() << std::endl;
}    


} // end namespace itk

#endif
