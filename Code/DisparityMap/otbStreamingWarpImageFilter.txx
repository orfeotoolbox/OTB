/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbStreamingWarpImageFilter_txx
#define __otbStreamingWarpImageFilter_txx

#include "otbStreamingWarpImageFilter.h"
#include "otbStreamingTraits.h"

namespace otb
{

template<class TInputImage, class TOutputImage, class TDeformationField>
StreamingWarpImageFilter<TInputImage,TOutputImage,TDeformationField>
::StreamingWarpImageFilter()
{
  // Fill the default maximum deformation
  m_MaximumDeformation.Fill(1);
}


template<class TInputImage, class TOutputImage, class TDeformationField>
void
StreamingWarpImageFilter<TInputImage,TOutputImage,TDeformationField>
::GenerateInputRequestedRegion()
{
  // First, ca the superclass implementation
  Superclass::GenerateInputRequestedRegion();

  // Get the input and deformation field pointers
  InputImageType * inputPtr = const_cast<InputImageType *>(this->GetInput());
  DeformationFieldType * deformationPtr = const_cast<DeformationFieldType*>(this->GetDeformationField());

  // Check if the input and the deformation field exist
  if(!inputPtr || !deformationPtr)
    {
      return;
    }

  // Compute the security margin radius
  typename InputImageType::SizeType radius;
  typename InputImageType::SpacingType spacing = inputPtr->GetSpacing();

  // Add the padding due to the interpolator
  unsigned int interpolatorRadius = StreamingTraits<typename Superclass::InputImageType>::CalculateNeededRadiusForInterpolator(this->GetInterpolator());

  // Compute the margin due to the maximum deformation value and interpolator radius
  for(unsigned int i = 0; i<InputImageType::ImageDimension;++i)
    {
      radius[i]= interpolatorRadius + static_cast<unsigned int>(vcl_ceil(m_MaximumDeformation[i]/vcl_abs(spacing[i])));
    }

  otbMsgDevMacro(<<"WarpImageFilter: MaximumDeformation: "<<m_MaximumDeformation<<", interpolator radius: "<<interpolatorRadius<<", total radius:  "<<radius);

  // get a copy of the input requested region (should equal the output
  // requested region)
  typename DeformationFieldType::RegionType inputRequestedRegion;
  inputRequestedRegion = deformationPtr->GetRequestedRegion();

  // pad the input requested region by the operator radius
  inputRequestedRegion.PadByRadius( radius);

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

template<class TInputImage, class TOutputImage, class TDeformationField>
void
StreamingWarpImageFilter<TInputImage,TOutputImage,TDeformationField>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os << indent << "Maximum deformation: " << m_MaximumDeformation<<std::endl;
}


}// end namespace otb
#endif
