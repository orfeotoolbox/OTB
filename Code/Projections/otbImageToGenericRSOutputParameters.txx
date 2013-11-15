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
#ifndef __otbImageToGenericRSOutputParameters_txx
#define __otbImageToGenericRSOutputParameters_txx


#include "otbImageToGenericRSOutputParameters.h"
#include "itkMacro.h"

namespace otb {

template<class TImage>
ImageToGenericRSOutputParameters<TImage>
::ImageToGenericRSOutputParameters()
{
  m_Transform   = GenericRSTransformType::New();
  m_ForceSpacing = false;
  m_ForceSize    = false;
  m_EstimateIsotropicSpacing = false;
}

/**
 * Trigger the output image information computation
 */
template<class TImage>
void
ImageToGenericRSOutputParameters<TImage>
::Compute()
{
  // Do some checks : exceptions if Null or empty projectionRef and empty keywordlist
  if(m_Input.IsNull())
    itkExceptionMacro(<<"The input is null , please set a non null input image");

  if(m_Input->GetProjectionRef().empty() && m_Input->GetImageKeywordlist().GetSize() == 0)
    itkExceptionMacro(<<"No information in the metadata, please set an image with non empty metadata");

  // First Call to UpdateTransform : Initialize with the input image
  // information
  this->UpdateTransform();

  // Estimate the Output image Extent
  this->EstimateOutputImageExtent();

  // Estimate the Output Origin
  this->EstimateOutputOrigin();

  // Estimate the Output Spacing
  if(!m_ForceSpacing)
    this->EstimateOutputSpacing();

  // Finally Estimate the Output Size
  this->EstimateOutputSize();
}


template<class TImage>
void
ImageToGenericRSOutputParameters<TImage>
::UpdateTransform()
{
  m_Transform->SetOutputDictionary(this->GetInput()->GetMetaDataDictionary());
  m_Transform->SetOutputProjectionRef(this->GetInput()->GetProjectionRef());
  m_Transform->SetOutputKeywordList(this->GetInput()->GetImageKeywordlist());
  m_Transform->InstanciateTransform();
}


/**
 * The extent is the projection of the 4 image corner in the final
 * projection system.
 */
template <class TImage>
void
ImageToGenericRSOutputParameters<TImage>
::EstimateOutputImageExtent()
{
  // Get the inverse transform again : used later
  GenericRSTransformPointerType invTransform = GenericRSTransformType::New();
  m_Transform->GetInverse(invTransform);

  // Compute the 4 corners in the cartographic coordinate system
  std::vector<IndexType>       vindex;
  std::vector<PointType> voutput;

  IndexType index1, index2, index3, index4;
  SizeType  size;

  // Image size
  size = m_Input->GetLargestPossibleRegion().GetSize();

  // project the 4 corners
  index1 = m_Input->GetLargestPossibleRegion().GetIndex();
  index2 = m_Input->GetLargestPossibleRegion().GetIndex();
  index3 = m_Input->GetLargestPossibleRegion().GetIndex();
  index4 = m_Input->GetLargestPossibleRegion().GetIndex();

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
    m_Input->TransformIndexToPhysicalPoint(vindex[i], physicalPoint);
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
  m_OutputExtent.maxX =  maxX;
  m_OutputExtent.minX =  minX;
  m_OutputExtent.maxY =  maxY;
  m_OutputExtent.minY =  minY;
}


/**
 * Method used to estimate the Origin using the extent of the image
 *
 */
template <class TImage>
void
ImageToGenericRSOutputParameters<TImage>
::EstimateOutputOrigin()
{
  // Set the output orgin in carto
  // projection
  PointType   origin;
  origin[0] = m_OutputExtent.minX;
  origin[1] = m_OutputExtent.maxY;
  this->SetOutputOrigin(origin);
}

/**
 * Method used to estimate the spacing using the extent of the image
 *
 */
template <class TImage>
void
ImageToGenericRSOutputParameters<TImage>
::EstimateOutputSpacing()
{
  // Compute the output size
  double sizeCartoX = vcl_abs(m_OutputExtent.maxX - m_OutputExtent.minX);
  double sizeCartoY = vcl_abs(m_OutputExtent.minY - m_OutputExtent.maxY);

  PointType o, oX, oY;
  o[0] = this->GetOutputOrigin()[0];
  o[1] = this->GetOutputOrigin()[1];

  oX = o;
  oY = o;

  oX[0] += sizeCartoX;
  oY[1] += sizeCartoY;

  // Transform back into the input image
  PointType io = m_Transform->TransformPoint(o);
  PointType ioX = m_Transform->TransformPoint(oX);
  PointType ioY = m_Transform->TransformPoint(oY);

  // Transform to indices
  IndexType ioIndex, ioXIndex, ioYIndex;
  m_Input->TransformPhysicalPointToIndex(io, ioIndex);
  m_Input->TransformPhysicalPointToIndex(ioX, ioXIndex);
  m_Input->TransformPhysicalPointToIndex(ioY, ioYIndex);

  // Evaluate Ox and Oy length in number of pixels
  double OxLength, OyLength;

  OxLength = vcl_sqrt(vcl_pow((double) ioIndex[0] - (double) ioXIndex[0], 2)
                      +  vcl_pow((double) ioIndex[1] - (double) ioXIndex[1], 2));

  OyLength = vcl_sqrt(vcl_pow((double) ioIndex[0] - (double) ioYIndex[0], 2)
                      +  vcl_pow((double) ioIndex[1] - (double) ioYIndex[1], 2));

  // Evaluate spacing
  SpacingType outputSpacing;


  if(m_EstimateIsotropicSpacing)
    {
    double isotropicSpacing = std::min(sizeCartoX / OxLength, sizeCartoY / OyLength);
    outputSpacing[0] = isotropicSpacing;
    outputSpacing[1] = -isotropicSpacing;
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
void
ImageToGenericRSOutputParameters<TImage>
::EstimateOutputSize()
{
  // Compute the output size
  double sizeCartoX = vcl_abs(m_OutputExtent.maxX - m_OutputExtent.minX);
  double sizeCartoY = vcl_abs(m_OutputExtent.minY - m_OutputExtent.maxY);

  // Evaluate output size
  SizeType outputSize;
  outputSize[0] = static_cast<unsigned int>(vcl_floor(vcl_abs(sizeCartoX / this->GetOutputSpacing()[0])));
  outputSize[1] = static_cast<unsigned int>(vcl_floor(vcl_abs(sizeCartoY / this->GetOutputSpacing()[1])));

  // if ForceSizeTo used don't update the output size with the value
  // computed : the value is computed to update the spacing knowing
  // the forced size and the computed one.
  if(!m_ForceSize)
    this->SetOutputSize(outputSize);
  else
    {
    // Compute the spacing knowing the size
    SpacingType outputSpacing;
    outputSpacing[0] = this->GetOutputSpacing()[0] * outputSize[0]/this->GetOutputSize()[0];
    outputSpacing[1] = this->GetOutputSpacing()[1] * outputSize[1]/this->GetOutputSize()[1];
    this->SetOutputSpacing(outputSpacing);
    }
  this->UpdateTransform();
}


}

#endif
