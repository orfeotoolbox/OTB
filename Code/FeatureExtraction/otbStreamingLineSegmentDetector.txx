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
#ifndef __otbStreamingLineSegmentDetector_txx
#define __otbStreamingLineSegmentDetector_txx

#include "otbStreamingLineSegmentDetector.h"

#include "otbVectorDataTransformFilter.h"
#include "itkAffineTransform.h"

namespace otb
{

template<class TInputImage>
PersistentStreamingLineSegmentDetector<TInputImage>
::PersistentStreamingLineSegmentDetector()
{
}

template<class TInputImage>
PersistentStreamingLineSegmentDetector<TInputImage>
::~PersistentStreamingLineSegmentDetector()
{
}

template<class TInputImage>
void
PersistentStreamingLineSegmentDetector<TInputImage>
::GenerateInputRequestedRegion()
{
  Superclass::GenerateInputRequestedRegion();

  if (this->GetInput())
    {
    InputImagePointerType input = const_cast<InputImageType *> (this->GetInput());

    typename InputImageType::RegionType region = this->GetOutput()->GetRequestedRegion();

    region.PadByRadius(1);
    region.Crop(input->GetLargestPossibleRegion());

    input->SetRequestedRegion(region);
    }
}

template<class TInputImage>
typename PersistentStreamingLineSegmentDetector<TInputImage>::OutputVectorDataPointerType
PersistentStreamingLineSegmentDetector<TInputImage>
::ProcessTile()
{
  // Apply an ExtractImageFilter to avoid problems with filters asking for the LargestPossibleRegion
  typedef itk::ExtractImageFilter<InputImageType, InputImageType> ExtractImageFilterType;
  typename ExtractImageFilterType::Pointer extract = ExtractImageFilterType::New();
  extract->SetInput( this->GetInput() );
  extract->SetExtractionRegion( this->GetInput()->GetBufferedRegion() );
  // WARNING: itk::ExtractImageFilter does not copy the MetadataDictionnary


  typename LSDType::Pointer lsd = LSDType::New();
  lsd->SetInput(this->GetInput());
  lsd->UpdateOutputInformation();
  lsd->Update();


  // The VectorData in output of the chain is in image index coordinate,
  // and the projection information is lost
  // Apply an affine transform to apply image origin and spacing,
  // and arbitrarily set the ProjectionRef to the input image ProjectionRef

  typedef itk::AffineTransform<typename OutputVectorDataType::PrecisionType, 2> TransformType;
  typedef VectorDataTransformFilter<OutputVectorDataType,OutputVectorDataType> VDTransformType;

  typename TransformType::ParametersType params;
  params.SetSize(6);
  params[0] = this->GetInput()->GetSpacing()[0];
  params[1] = 0;
  params[2] = 0;
  params[3] = this->GetInput()->GetSpacing()[1];
  params[4] = this->GetInput()->GetOrigin()[0];
  params[5] = this->GetInput()->GetOrigin()[1];

  typename TransformType::Pointer transform = TransformType::New();
  transform->SetParameters(params);

  typename VDTransformType::Pointer vdTransform = VDTransformType::New();
  vdTransform->SetTransform(transform);
  vdTransform->SetInput(lsd->GetOutput());
  vdTransform->Update();

  vdTransform->GetOutput()->SetProjectionRef(this->GetInput()->GetProjectionRef());

  return lsd->GetOutput();
}


} // end namespace otb
#endif
