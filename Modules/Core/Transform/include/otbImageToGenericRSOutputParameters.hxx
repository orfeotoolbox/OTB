/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbImageToGenericRSOutputParameters_hxx
#define otbImageToGenericRSOutputParameters_hxx


#include "otbImageToGenericRSOutputParameters.h"
#include "itkMacro.h"
#include "itkContinuousIndex.h"

namespace otb
{

template <class TImage>
ImageToGenericRSOutputParameters<TImage>::ImageToGenericRSOutputParameters()
{
  m_Transform                = GenericRSTransformType::New();
  m_ForceSpacing             = false;
  m_ForceSize                = false;
  m_EstimateIsotropicSpacing = false;
}

/**
 * Trigger the output image information computation
 */
template <class TImage>
void ImageToGenericRSOutputParameters<TImage>::Compute()
{
  // Do some checks : exceptions if Null or empty projectionRef and empty keywordlist
  if (m_Input.IsNull())
    itkExceptionMacro(<< "The input is null , please set a non null input image");

  if (m_Input->GetProjectionRef().empty() && m_Input->GetImageKeywordlist().GetSize() == 0)
    itkExceptionMacro(<< "No information in the metadata, please set an image with non empty metadata");

  // First Call to UpdateTransform : Initialize with the input image
  // information
  this->UpdateTransform();

  // Estimate the Output image Extent
  this->EstimateOutputImageExtent();

  // Estimate the Output Spacing
  if (!m_ForceSpacing)
    this->EstimateOutputSpacing();

  // Finally Estimate the Output Size
  this->EstimateOutputSize();

  // Estimate the Output Origin
  this->EstimateOutputOrigin();
}


template <class TImage>
void ImageToGenericRSOutputParameters<TImage>::UpdateTransform()
{
  m_Transform->SetOutputDictionary(this->GetInput()->GetMetaDataDictionary());
  m_Transform->SetOutputProjectionRef(this->GetInput()->GetProjectionRef());
  m_Transform->SetOutputKeywordList(this->GetInput()->GetImageKeywordlist());
  m_Transform->InstantiateTransform();
}


/**
 * The extent is the projection of the 4 image corner in the final
 * projection system.
 */
template <class TImage>
void ImageToGenericRSOutputParameters<TImage>::EstimateOutputImageExtent()
{
  // Get the inverse transform again : used later
  GenericRSTransformPointerType invTransform = GenericRSTransformType::New();
  m_Transform->GetInverse(invTransform);

  // Compute the 4 corners in the cartographic coordinate system
  std::vector<itk::ContinuousIndex<double, 2>> vindex;
  std::vector<PointType> voutput;

  itk::ContinuousIndex<double, 2> index1(m_Input->GetLargestPossibleRegion().GetIndex());
  index1[0] += -0.5;
  index1[1] += -0.5;
  itk::ContinuousIndex<double, 2> index2(index1);
  itk::ContinuousIndex<double, 2> index3(index1);
  itk::ContinuousIndex<double, 2> index4(index1);

  // Image size
  SizeType size = m_Input->GetLargestPossibleRegion().GetSize();

  // project the 4 corners
  index2[0] += size[0];
  index3[0] += size[0];
  index3[1] += size[1];
  index4[1] += size[1];

  vindex.push_back(index1);
  vindex.push_back(index2);
  vindex.push_back(index3);
  vindex.push_back(index4);

  for (unsigned int i = 0; i < vindex.size(); ++i)
  {
    PointType physicalPoint;
    m_Input->TransformContinuousIndexToPhysicalPoint(vindex[i], physicalPoint);
    voutput.push_back(invTransform->TransformPoint(physicalPoint));
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
  m_OutputExtent.maxX = maxX;
  m_OutputExtent.minX = minX;
  m_OutputExtent.maxY = maxY;
  m_OutputExtent.minY = minY;
}


/**
 * Method used to estimate the Origin using the extent of the image
 *
 */
template <class TImage>
void ImageToGenericRSOutputParameters<TImage>::EstimateOutputOrigin()
{
  // Set the output origin in carto
  // projection
  PointType origin;
  origin[0] = m_OutputExtent.minX + 0.5 * this->GetOutputSpacing()[0];
  origin[1] = m_OutputExtent.maxY + 0.5 * this->GetOutputSpacing()[1];
  this->SetOutputOrigin(origin);
}

/**
 * Method used to estimate the spacing using the extent of the image
 *
 */
template <class TImage>
void ImageToGenericRSOutputParameters<TImage>::EstimateOutputSpacing()
{
  // Compute the output size
  double sizeCartoX = std::abs(m_OutputExtent.maxX - m_OutputExtent.minX);
  double sizeCartoY = std::abs(m_OutputExtent.minY - m_OutputExtent.maxY);

  PointType o, oX, oY;
  o[0] = m_OutputExtent.minX;
  o[1] = m_OutputExtent.maxY;

  oX = o;
  oY = o;

  oX[0] += sizeCartoX;
  oY[1] -= sizeCartoY;

  // Transform back into the input image
  PointType io  = m_Transform->TransformPoint(o);
  PointType ioX = m_Transform->TransformPoint(oX);
  PointType ioY = m_Transform->TransformPoint(oY);

  // Transform to indices
  IndexType ioIndex, ioXIndex, ioYIndex;
  m_Input->TransformPhysicalPointToIndex(io, ioIndex);
  m_Input->TransformPhysicalPointToIndex(ioX, ioXIndex);
  m_Input->TransformPhysicalPointToIndex(ioY, ioYIndex);

  // Evaluate Ox and Oy length in number of pixels
  double OxLength, OyLength;

  OxLength = std::sqrt(std::pow((double)ioIndex[0] - (double)ioXIndex[0], 2) + std::pow((double)ioIndex[1] - (double)ioXIndex[1], 2));

  OyLength = std::sqrt(std::pow((double)ioIndex[0] - (double)ioYIndex[0], 2) + std::pow((double)ioIndex[1] - (double)ioYIndex[1], 2));

  // Evaluate spacing
  SpacingType outputSpacing;


  if (m_EstimateIsotropicSpacing)
  {
    double isotropicSpacing = std::min(sizeCartoX / OxLength, sizeCartoY / OyLength);
    outputSpacing[0]        = isotropicSpacing;
    outputSpacing[1]        = -isotropicSpacing;
  }
  else
  {
    outputSpacing[0] = sizeCartoX / OxLength;
    outputSpacing[1] = -sizeCartoY / OyLength;
  }

  this->SetOutputSpacing(outputSpacing);
}

/**
 * Method used to estimate the size using the output size of the image
 *
 */
template <class TImage>
void ImageToGenericRSOutputParameters<TImage>::EstimateOutputSize()
{
  // Compute the output size
  double sizeCartoX = std::abs(m_OutputExtent.maxX - m_OutputExtent.minX);
  double sizeCartoY = std::abs(m_OutputExtent.minY - m_OutputExtent.maxY);

  // Evaluate output size
  SizeType outputSize;
  outputSize[0] = static_cast<unsigned int>(std::floor(std::abs(sizeCartoX / this->GetOutputSpacing()[0])));
  outputSize[1] = static_cast<unsigned int>(std::floor(std::abs(sizeCartoY / this->GetOutputSpacing()[1])));

  // if ForceSizeTo used don't update the output size with the value
  // computed : the value is computed to update the spacing knowing
  // the forced size and the computed one.
  if (!m_ForceSize)
    this->SetOutputSize(outputSize);
  else
  {
    // Compute the spacing knowing the size
    SpacingType outputSpacing;
    outputSpacing[0] = this->GetOutputSpacing()[0] * outputSize[0] / this->GetOutputSize()[0];
    outputSpacing[1] = this->GetOutputSpacing()[1] * outputSize[1] / this->GetOutputSize()[1];
    this->SetOutputSpacing(outputSpacing);
  }
  this->UpdateTransform();
}
}

#endif
