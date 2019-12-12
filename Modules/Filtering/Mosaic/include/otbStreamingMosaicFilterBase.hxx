/*
 * Copyright (C) 1999-2011 Insight Software Consortium
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 * Copyright (C) 2016-2019 IRSTEA
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
#ifndef __StreamingMosaicFilterBase_hxx
#define __StreamingMosaicFilterBase_hxx

#include "otbStreamingMosaicFilterBase.h"

namespace otb
{

template <class TInputImage, class TOutputImage, class TInternalValueType>
StreamingMosaicFilterBase<TInputImage, TOutputImage, TInternalValueType>::StreamingMosaicFilterBase()
{

  m_OutputSpacing.Fill(0);
  m_OutputOrigin.Fill(0);
  m_OutputSize.Fill(0);
  m_ShiftScaleInputImages                = false;
  m_AutomaticOutputParametersComputation = true;
  Superclass::SetCoordinateTolerance(itk::NumericTraits<double>::max());
  Superclass::SetDirectionTolerance(itk::NumericTraits<double>::max());
  interpolatorRadius = 0;
  nbOfBands          = 0;
}

/**
  Compute the input image extent i.e. corners inf & sup
 */
template <class TInputImage, class TOutputImage, class TInternalValueType>
void StreamingMosaicFilterBase<TInputImage, TOutputImage, TInternalValueType>::ImageToExtent(InputImageType* image, InputImagePointType& extentInf,
                                                                                             InputImagePointType& extentSup)
{

  // Get index of the last pixel
  InputImageIndexType imageLastIndex;

  for (unsigned int dim = 0; dim < OutputImageType::ImageDimension; ++dim)
  {
    imageLastIndex[dim] = image->GetLargestPossibleRegion().GetSize()[dim];
  }

  // Compute extent
  InputImagePointType  imageOrigin = image->GetOrigin();
  OutputImagePointType imageEnd;
  image->TransformIndexToPhysicalPoint(imageLastIndex, imageEnd);
  for (unsigned int dim = 0; dim < OutputImageType::ImageDimension; ++dim)
  {
    extentInf[dim] = vnl_math_min(imageOrigin[dim], imageEnd[dim]);
    extentSup[dim] = vnl_math_max(imageOrigin[dim], imageEnd[dim]);
  }
}

/**
  Compute the region of the input image which correspond to the given output requested region
  Return true if the region exists, false if not

  TODO: Pad the Image's LargestPossibleRegion with the interpolator radius to avoid
  side effects near the Image's border?
 */
template <class TInputImage, class TOutputImage, class TInternalValueType>
bool StreamingMosaicFilterBase<TInputImage, TOutputImage, TInternalValueType>::OutputRegionToInputRegion(const OutputImageRegionType& outputRegion,
                                                                                                         InputImageRegionType& inputRegion,
                                                                                                         InputImageType*&      inputImage)
{

  // Mosaic Region Start & End (mosaic image index)
  OutputImageIndexType outIndexStart = outputRegion.GetIndex();
  OutputImageIndexType outIndexEnd   = outputRegion.GetUpperIndex();

  // Mosaic Region Start & End (geo)
  OutputImagePointType outPointStart, outPointEnd;
  this->GetOutput()->TransformIndexToPhysicalPoint(outIndexStart, outPointStart);
  this->GetOutput()->TransformIndexToPhysicalPoint(outIndexEnd, outPointEnd);

  // Mosaic Region Start & End (input image index)
  InputImageIndexType defIndexStart, defIndexEnd;
  inputImage->TransformPhysicalPointToIndex(outPointStart, defIndexStart);
  inputImage->TransformPhysicalPointToIndex(outPointEnd, defIndexEnd);

  // Compute input image region
  InputImageSizeType  defRequestedSize;
  InputImageIndexType defRequestedIndex;
  for (unsigned int dim = 0; dim < OutputImageType::ImageDimension; ++dim)
  {
    defRequestedIndex[dim] = vnl_math_min(defIndexStart[dim], defIndexEnd[dim]);
    defRequestedSize[dim]  = vnl_math_max(defIndexStart[dim], defIndexEnd[dim]) - defRequestedIndex[dim] + 1;
  }
  InputImageRegionType computedInputRegion(defRequestedIndex, defRequestedSize);

  // avoid extrapolation
  computedInputRegion.PadByRadius(1);

  // padding for interpolator
  computedInputRegion.PadByRadius(interpolatorRadius);

  inputRegion = computedInputRegion;

  // crop the input requested region at the input's largest possible region
  return inputRegion.Crop(inputImage->GetLargestPossibleRegion());
}

template <class TInputImage, class TOutputImage, class TInternalValueType>
void StreamingMosaicFilterBase<TInputImage, TOutputImage, TInternalValueType>::ComputeRequestedRegionOfInputImage(unsigned int inputImageIndex)
{

  // Output requested region
  const OutputImageRegionType outRegion = this->GetOutput()->GetRequestedRegion();

  // Get the input image pointer
  InputImageType*      inputTile = static_cast<InputImageType*>(Superclass::ProcessObject::GetInput(inputImageIndex));
  InputImageRegionType inRegion;

  // Compute the requested region
  if (OutputRegionToInputRegion(outRegion, inRegion, inputTile))
  {
    // Image does overlap requested region: add the image to the used input
    // images list
    itkDebugMacro(<< "Image #" << inputImageIndex << "\n" << inRegion << " is inside the requested region");
    AddUsedInputImageIndex(inputImageIndex);
  }
  else
  {
    // Image does not overlap requested region: set requested region to null
    itkDebugMacro(<< "Image #" << inputImageIndex << "\n" << inRegion << " is outside the requested region");
    inRegion.GetModifiableSize().Fill(0);
    inRegion.GetModifiableIndex().Fill(0);
  }
  inputTile->SetRequestedRegion(inRegion);
}

/**
 * Generate the input requested region and the used input images list
 */
template <class TInputImage, class TOutputImage, class TInternalValueType>
void StreamingMosaicFilterBase<TInputImage, TOutputImage, TInternalValueType>::GenerateInputRequestedRegion(void)
{
  usedInputIndices.clear();

  // For each image, get the requested region
  for (unsigned int i = 0; i < this->GetNumberOfInputs(); ++i)
  {
    ComputeRequestedRegionOfInputImage(i);
  }
}

/** Check if scales and shifts are good */
template <class TInputImage, class TOutputImage, class TInternalValueType>
void StreamingMosaicFilterBase<TInputImage, TOutputImage, TInternalValueType>::CheckShiftScaleMatrices()
{

  const unsigned int numberOfInputImages = this->GetNumberOfInputImages();

  // Check shift-scale matrix dimensions
  if (m_ShiftScaleInputImages)
  {
    if (m_ShiftMatrix.rows() == 0)
    {
      m_ShiftMatrix.set_size(numberOfInputImages, nbOfBands);
      m_ShiftMatrix.fill(itk::NumericTraits<InternalValueType>::Zero);
    }
    if (m_ScaleMatrix.rows() == 0)
    {
      m_ScaleMatrix.set_size(numberOfInputImages, nbOfBands);
      m_ScaleMatrix.fill(itk::NumericTraits<InternalValueType>::One);
    }
    if (m_ShiftMatrix.cols() != nbOfBands || m_ScaleMatrix.cols() != nbOfBands)
    {
      itkWarningMacro(<< "Shift-Scale matrices should have " << nbOfBands << " cols (1 col/band)"
                      << " instead of " << m_ShiftMatrix.cols());
    }
    if (m_ShiftMatrix.rows() != numberOfInputImages || m_ScaleMatrix.rows() != numberOfInputImages)
    {
      itkWarningMacro(<< "Shift-Scale matrices must have " << numberOfInputImages << " rows (1 row/input image)"
                      << " instead of " << m_ShiftMatrix.rows());
    }
  }
}

/*
 * Compute output parameters:
 * -number of component per pixel
 * -spacing
 * -size
 * -origin
 */
template <class TInputImage, class TOutputImage, class TInternalValueType>
void StreamingMosaicFilterBase<TInputImage, TOutputImage, TInternalValueType>::ComputeOutputParameters(void)
{
  itkDebugMacro(<< "Computing output parameters of the mosaic image");

  // Initialize extent and spacing
  OutputImagePointType extentInf, extentSup;
  m_OutputSpacing.Fill(itk::NumericTraits<double>::max());
  extentInf.Fill(itk::NumericTraits<double>::max());
  extentSup.Fill(itk::NumericTraits<double>::NonpositiveMin());

  // Compute output spacing, size, and origin
  for (unsigned int imageIndex = 0; imageIndex < this->GetNumberOfInputs(); imageIndex++)
  {
    // Read image (cast is required since filter can have multiple input images
    // types)
    InputImageType* currentImage = static_cast<InputImageType*>(Superclass::ProcessObject::GetInput(imageIndex));

    // Update mosaic spacing (keep the nearest of 0)
    for (unsigned int dim = 0; dim < OutputImageType::ImageDimension; ++dim)
    {
      if (vcl_abs(currentImage->GetSignedSpacing()[dim]) < vcl_abs(m_OutputSpacing[dim]))
      {
        m_OutputSpacing[dim] = currentImage->GetSignedSpacing()[dim];
      }
    }

    // Update mosaic image extent
    InputImagePointType currentInputImageExtentInf, currentInputImageExtentSup;
    ImageToExtent(currentImage, currentInputImageExtentInf, currentInputImageExtentSup);
    for (unsigned int dim = 0; dim < OutputImageType::ImageDimension; ++dim)
    {
      extentInf[dim] = vnl_math_min(currentInputImageExtentInf[dim], extentInf[dim]);
      extentSup[dim] = vnl_math_max(currentInputImageExtentSup[dim], extentSup[dim]);
    }
  }

  // Set final size
  m_OutputSize[0] = vcl_floor((extentSup[0] - extentInf[0]) / vcl_abs(m_OutputSpacing[0])) + 1;
  m_OutputSize[1] = vcl_floor((extentSup[1] - extentInf[1]) / vcl_abs(m_OutputSpacing[1])) + 1;

  // Set final origin (Coordinate of the upper left pixel center)
  m_OutputOrigin[0] = extentInf[0];
  m_OutputOrigin[1] = extentSup[1];
}

/**
 * Generates output information:
 * size, spacing, etc.
 * Default behavior: the output spacing is the min or all input images
 */
template <class TInputImage, class TOutputImage, class TInternalValueType>
void StreamingMosaicFilterBase<TInputImage, TOutputImage, TInternalValueType>::GenerateOutputInformation(void)
{
  itkDebugMacro(<< "Generate output informations");
  Superclass::GenerateOutputInformation();

  // check interpolator and put a default interpolator if empty
  if (!m_Interpolator)
  {
    itkDebugMacro(<< "No interpolator specified. Using default");
    typename DefaultInterpolatorType::Pointer interp = DefaultInterpolatorType::New();
    m_Interpolator                                   = static_cast<InterpolatorType*>(interp.GetPointer());
  }

  // buffer padding radius
  interpolatorRadius = StreamingTraitsType::CalculateNeededRadiusForInterpolator(m_Interpolator);

  // If AutomaticOutputParametersComputation mode is TRUE
  if (m_AutomaticOutputParametersComputation)
  {
    ComputeOutputParameters(); // compute output size, spacing, origin
  }

  // Update/Check following things:
  // -number of component per pixel
  // -projection
  nbOfBands                 = 0;
  std::string projectionRef = "";
  for (unsigned int imageIndex = 0; imageIndex < this->GetNumberOfInputs(); imageIndex++)
  {
    itkDebugMacro(<< "Input image #" << imageIndex);

    // Read image (cast is required since filter can have multiple input images
    // types)
    InputImageType* currentImage = static_cast<InputImageType*>(Superclass::ProcessObject::GetInput(imageIndex));

    itkDebugMacro(<< "\tSize   : " << currentImage->GetLargestPossibleRegion().GetSize());
    itkDebugMacro(<< "\tSpacing: " << currentImage->GetSignedSpacing());
    itkDebugMacro(<< "\tOrigin : " << currentImage->GetOrigin());

    // Update number of bands (keep the max)
    nbOfBands = vnl_math_max(nbOfBands, currentImage->GetNumberOfComponentsPerPixel());

    // Check projection
    itk::MetaDataDictionary metaData = currentImage->GetMetaDataDictionary();
    std::string             currentProjectionRef;
    if (metaData.HasKey(otb::MetaDataKey::ProjectionRefKey))
    {
      itk::ExposeMetaData<std::string>(metaData, static_cast<std::string>(otb::MetaDataKey::ProjectionRefKey), currentProjectionRef);
    }

    if (projectionRef.size() != 0)
    {
      if (currentProjectionRef.size() != 0)
      {
        if (currentProjectionRef.compare(projectionRef) != 0)
        {
          itkWarningMacro(<< "Input images may have not the same projection");
        }
      }
    }
    else
      projectionRef = currentProjectionRef;
  }
  itk::MetaDataDictionary mosaicMetaData;
  itk::EncapsulateMetaData<std::string>(mosaicMetaData, static_cast<std::string>(otb::MetaDataKey::ProjectionRefKey), projectionRef);

  // check no data pixels
  if (m_NoDataOutputPixel.GetSize() != nbOfBands || m_NoDataInputPixel.GetSize() != nbOfBands)
  {
    if (m_NoDataOutputPixel.GetSize() != 0)
      itkWarningMacro(<< "Specified NoDataOutputPixel has not " << nbOfBands << " components. Using default (zeros)");
    if (m_NoDataInputPixel.GetSize() != 0)
      itkWarningMacro(<< "Specified NoDataInputPixel has not " << nbOfBands << " components. Using default (zeros)");

    m_NoDataOutputPixel.SetSize(nbOfBands);
    m_NoDataInputPixel.SetSize(nbOfBands);
    m_NoDataOutputPixel.Fill(itk::NumericTraits<OutputImageInternalPixelType>::Zero);
    m_NoDataInputPixel.Fill(itk::NumericTraits<InputImageInternalPixelType>::Zero);
  }

  // Write no data flags
  std::vector<bool>   noDataValueAvailable;
  std::vector<double> noDataValues1;
  for (unsigned int band = 0; band < nbOfBands; band++)
  {
    noDataValueAvailable.push_back(true);
    noDataValues1.push_back(static_cast<double>(m_NoDataOutputPixel[band]));
  }
  otb::WriteNoDataFlags(noDataValueAvailable, noDataValues1, this->GetOutput()->GetMetaDataDictionary());

  // Get min & max values from output pixel type
  minOutputPixelValue = static_cast<InternalValueType>(itk::NumericTraits<OutputImageInternalPixelType>::NonpositiveMin());
  maxOutputPixelValue = static_cast<InternalValueType>(itk::NumericTraits<OutputImageInternalPixelType>::max());

  // Output parameters
  OutputImageIndexType outputRegionStart;
  outputRegionStart.Fill(0);
  OutputImageRegionType outputRegion(outputRegionStart, m_OutputSize);
  OutputImageType*      outputPtr = this->GetOutput();
  outputPtr->SetOrigin(m_OutputOrigin);
  outputPtr->SetSignedSpacing(m_OutputSpacing);
  outputPtr->SetNumberOfComponentsPerPixel(nbOfBands);
  outputPtr->SetLargestPossibleRegion(outputRegion);

  itkDebugMacro(<< "Output mosaic parameters:"
                << "\n\tBands  : " << nbOfBands << "\n\tOrigin : " << m_OutputOrigin << "\n\tSize   : " << m_OutputSize << "\n\tSpacing: " << m_OutputSpacing);

  // Check shift-scale matrices
  if (m_ShiftScaleInputImages)
  {
    CheckShiftScaleMatrices();
  }
}

/*
 * Check if the pixel is empty
 */
template <class TInputImage, class TOutputImage, class TInternalValueType>
bool StreamingMosaicFilterBase<TInputImage, TOutputImage, TInternalValueType>::IsPixelNotEmpty(InputImagePixelType& pixelValue)
{
  bool isDataInCurrentInputPixel = false;

  for (unsigned int band = 0; band < nbOfBands; band++)
  {
    isDataInCurrentInputPixel = isDataInCurrentInputPixel || (pixelValue[band] != m_NoDataInputPixel[band]);
  }
  return isDataInCurrentInputPixel;
}

/*
 * Clipping
 */
template <class TInputImage, class TOutputImage, class TInternalValueType>
void StreamingMosaicFilterBase<TInputImage, TOutputImage, TInternalValueType>::NormalizePixelValue(InternalValueType& pixelValue)
{
  if (pixelValue < minOutputPixelValue)
    pixelValue = minOutputPixelValue;
  if (pixelValue > maxOutputPixelValue)
    pixelValue = maxOutputPixelValue;
}

/**
 * Setup state of filter before multi-threading.
 * InterpolatorType::SetInputImage is not thread-safe and hence
 * has to be setup before ThreadedGenerateData
 */
template <class TInputImage, class TOutputImage, class TInternalValueType>
void StreamingMosaicFilterBase<TInputImage, TOutputImage, TInternalValueType>::BeforeThreadedGenerateData()
{
  if (!m_Interpolator)
  {
    itkExceptionMacro(<< "Interpolator not set");
  }
}

/**
 * Setup state of filter after multi-threading.
 */
template <class TInputImage, class TOutputImage, class TInternalValueType>
void StreamingMosaicFilterBase<TInputImage, TOutputImage, TInternalValueType>::AfterThreadedGenerateData()
{
  // Disconnect input image from interpolator
  m_Interpolator->SetInputImage(NULL);
}

template <class TInputImage, class TOutputImage, class TInternalValueType>
void StreamingMosaicFilterBase<TInputImage, TOutputImage, TInternalValueType>::PrepareImageAccessors(
    typename std::vector<InputImageType*>& image, typename std::vector<InterpolatorPointerType>& interpolator)
{

  // Get number of used input images
  const unsigned int n = GetNumberOfUsedInputImages();

  interpolator.reserve(n);
  image.reserve(n);

  // Loop on input images
  for (unsigned int i = 0; i < n; i++)
  {
    // Input image i
    image.push_back(const_cast<InputImageType*>(this->GetInput(usedInputIndices.at(i))));

    // Interpolator i
    interpolator.push_back(static_cast<InterpolatorType*>((m_Interpolator->CreateAnother()).GetPointer()));
    interpolator[i]->SetInputImage(image[i]);
  }
}

} // end namespace otb

#endif
