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
#ifndef otbStreamingLineSegmentDetector_txx
#define otbStreamingLineSegmentDetector_txx

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
  extract->Update();

  // WARNING: itk::ExtractImageFilter does not copy the MetadataDictionnary
  // but LSD filter need the projection ref if available
  extract->GetOutput()->SetMetaDataDictionary(this->GetInput()->GetMetaDataDictionary());

  typename LSDType::Pointer lsd = LSDType::New();
  lsd->SetInput(extract->GetOutput());
  lsd->UpdateOutputInformation();
  lsd->Update();

  // return the LSD VectorData in image physical coordinates
  return lsd->GetOutput();
}


} // end namespace otb
#endif
