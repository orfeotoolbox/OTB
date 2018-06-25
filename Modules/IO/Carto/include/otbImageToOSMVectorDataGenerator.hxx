/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbImageToOSMVectorDataGenerator_hxx
#define otbImageToOSMVectorDataGenerator_hxx

#include "otbImageToOSMVectorDataGenerator.h"
#include "otbGeoInformationConversion.h"
#include "otbGenericRSTransform.h"

namespace otb {

// constructor
template < class TImage >
ImageToOSMVectorDataGenerator<TImage>
::ImageToOSMVectorDataGenerator()
{
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredOutputs(1);

  m_ImageExtent.minX = 0;
  m_ImageExtent.maxX = 0;
  m_ImageExtent.minY = 0;
  m_ImageExtent.maxY = 0;
}

template < class TImage >
void
ImageToOSMVectorDataGenerator<TImage>
::SetInput( const ImageType * input  )
{
  // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(0,
                                   const_cast< ImageType* >( input  ) );
}

// Method to get the SampleList as DataObject
template < class TImage>
const typename ImageToOSMVectorDataGenerator<TImage>
::ImageType *
ImageToOSMVectorDataGenerator<TImage>
::GetInput() const
{
  if (this->GetNumberOfInputs() < 1)
    {
    return nullptr;
    }

  return static_cast<const  ImageType* >
    (this->itk::ProcessObject::GetInput(0) );
}


template < class TImage>
void
ImageToOSMVectorDataGenerator<TImage>
::GenerateData()
{
  // Get the extent of the image
  this->EstimateImageExtent();

  this->SetWest(m_ImageExtent.minX);
  this->SetSouth(m_ImageExtent.minY);
  this->SetEast(m_ImageExtent.maxX);
  this->SetNorth(m_ImageExtent.maxY);

  Superclass::GenerateData();
}

template < class TImage>
void
ImageToOSMVectorDataGenerator<TImage>
::EstimateImageExtent()
{
  // Get the input image
  typename ImageType::ConstPointer input = this->GetInput();

  // Local generic RS Transform to project the 4 corners to WGS84
  typedef otb::GenericRSTransform<>           TransformType;
  typename TransformType::Pointer transform = TransformType::New();
  transform->SetInputKeywordList(input->GetImageKeywordlist());
  transform->SetInputProjectionRef(input->GetProjectionRef());
  transform->SetOutputProjectionRef(otb::GeoInformationConversion::ToWKT(4326));
  transform->InstantiateTransform();

  // Compute the 4 corners in the cartographic coordinate system
  std::vector< IndexType> vindex;
  std::vector<PointType> voutput;

  IndexType index1, index2, index3, index4;
  SizeType  size;

  // Image size
  size = input->GetLargestPossibleRegion().GetSize();

  // project the 4 corners
  index1 = input->GetLargestPossibleRegion().GetIndex();
  index2 = input->GetLargestPossibleRegion().GetIndex();
  index3 = input->GetLargestPossibleRegion().GetIndex();
  index4 = input->GetLargestPossibleRegion().GetIndex();

  index2[0] += size[0] - 1;
  index3[0] += size[0] - 1;
  index3[1] += size[1] - 1;
  index4[1] += size[1] - 1;

  vindex.push_back(index1);
  vindex.push_back(index2);
  vindex.push_back(index3);
  vindex.push_back(index4);

  for (unsigned int i = 0; i < vindex.size(); ++i)
    {
    PointType physicalPoint;
    input->TransformIndexToPhysicalPoint(vindex[i], physicalPoint);
    otbMsgDevMacro( << " physical point "<<  physicalPoint << " --> Transform "
                    << transform->TransformPoint(physicalPoint));
    voutput.push_back(transform->TransformPoint(physicalPoint));
    }

  // Compute the boundaries
  double minX = voutput[0][0];
  double maxX = voutput[0][0];
  double minY = voutput[0][1];
  double maxY = voutput[0][1];

  for (unsigned int i = 0; i < voutput.size(); ++i)
    {
    // Origins
    if (minX > voutput[i][0])
      minX = voutput[i][0];
    if (minY > voutput[i][1])
      minY = voutput[i][1];

    // Sizes
    if (maxX < voutput[i][0])
      maxX = voutput[i][0];

    if (maxY < voutput[i][1])
      maxY = voutput[i][1];
    }

  // Edit the output image extent type
  m_ImageExtent.maxX =  maxX;
  m_ImageExtent.minX =  minX;
  m_ImageExtent.maxY =  maxY;
  m_ImageExtent.minY =  minY;
}


} // End namespace otb

#endif
