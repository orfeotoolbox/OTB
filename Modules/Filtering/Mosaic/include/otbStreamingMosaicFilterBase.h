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
#ifndef __StreamingMosaicFilterBase_H
#define __StreamingMosaicFilterBase_H

#include "itkImageToImageFilter.h"
#include "itkNearestNeighborInterpolateImageFunction.h"
#include "otbStreamingTraits.h"

// No data
#include "otbNoDataHelper.h"

namespace otb
{
/** \class StreamingMosaicFilterBase
 *  \brief Base class for mosaic filters. Computes the total extent of
 *         multiple inputs, and provide routines to compute whether or
 *         not an input image region must be requested
 *
 * The otb::StreamingMosaicFilterBase allows to compute what the final
 * mosaic of multiple input of otb::VectorImage is, and provide the requested
 * regions of input images. There is a shift-scale mode which allows
 * to shift-scale input images, which can be useful e.g. color or
 * radiometric harmonization of input set of images. The output
 * spacing (SetOutputSpacing()), the start index (SetOutputIndex()) and
 * the  interpolator (SetInterpolator()) and the origin (SetOrigin())
 * can be set using the method between brackets.
 *
 *
 * \ingroup OTBMosaic
 *
 **/

template <class TInputImage, class TOutputImage = TInputImage, class TInternalValueType = double>
class ITK_EXPORT StreamingMosaicFilterBase : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedef */
  typedef StreamingMosaicFilterBase Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Runtime information support. */
  itkTypeMacro(StreamingMosaicFilterBase, ImageToImageFilter);

  /** Input image typedefs.  */
  typedef TInputImage                                InputImageType;
  typedef typename InputImageType::ConstPointer      InputImagePointer;
  typedef typename InputImageType::RegionType        InputImageRegionType;
  typedef typename InputImageType::PixelType         InputImagePixelType;
  typedef typename InputImageType::PointType         InputImagePointType;
  typedef typename InputImageType::IndexType         InputImageIndexType;
  typedef typename InputImageType::SizeType          InputImageSizeType;
  typedef typename InputImageType::SpacingType       InputImageSpacingType;
  typedef typename InputImageType::InternalPixelType InputImageInternalPixelType;

  /** Output image typedefs typedefs.  */
  typedef TOutputImage                                OutputImageType;
  typedef typename OutputImageType::Pointer           OutputImagePointer;
  typedef typename OutputImageType::RegionType        OutputImageRegionType;
  typedef typename OutputImageType::IndexType         OutputImageIndexType;
  typedef typename OutputImageType::PointType         OutputImagePointType;
  typedef typename OutputImageType::SizeType          OutputImageSizeType;
  typedef typename OutputImageType::SpacingType       OutputImageSpacingType;
  typedef typename OutputImageType::PixelType         OutputImagePixelType;
  typedef typename OutputImageType::InternalPixelType OutputImageInternalPixelType;

  /** Internal computing typedefs */
  typedef TInternalValueType InternalValueType;
  typedef typename itk::ContinuousIndex<double, 2>                         ContinuousIndexType;
  typedef itk::InterpolateImageFunction<InputImageType, InternalValueType> InterpolatorType;
  typedef typename InterpolatorType::Pointer InterpolatorPointerType;
  typedef itk::NearestNeighborInterpolateImageFunction<InputImageType, InternalValueType> DefaultInterpolatorType;
  typedef typename otb::VectorImage<InternalValueType, 2>                                 InternalImageType;
  typedef typename InternalImageType::PixelType          InternalPixelType;
  typedef itk::ImageRegionIterator<OutputImageType>      IteratorType;
  typedef itk::ImageRegionConstIterator<OutputImageType> ConstIteratorType;
  typedef otb::StreamingTraits<OutputImageType>          StreamingTraitsType;

  /** Typedef for input images indices
   * TODO maybe use a itk class instead of std::vector ?*/
  typedef std::vector<unsigned int> IndicesListType;

  /** Typedef for matrices */
  typedef vnl_matrix<InternalValueType> MatrixType;

  /** Set/Get the interpolator function. */
  itkSetObjectMacro(Interpolator, InterpolatorType);
  itkGetObjectMacro(Interpolator, InterpolatorType);

  /** Set/Get the output no data value */
  itkSetMacro(NoDataOutputPixel, OutputImagePixelType);
  itkGetMacro(NoDataOutputPixel, OutputImagePixelType);

  /** Set/Get the input no data value */
  itkSetMacro(NoDataInputPixel, InputImagePixelType);
  itkGetMacro(NoDataInputPixel, InputImagePixelType);

  /** Set/Get output spacing */
  itkSetMacro(OutputSpacing, OutputImageSpacingType);
  itkGetMacro(OutputSpacing, OutputImageSpacingType);

  /** Set/Get output origin */
  itkSetMacro(OutputOrigin, OutputImagePointType);
  itkGetMacro(OutputOrigin, OutputImagePointType);

  /** Set/Get output size */
  itkSetMacro(OutputSize, OutputImageSizeType);
  itkGetMacro(OutputSize, OutputImageSizeType);

  /** Set/Get automatic compute output parameters mode */
  itkSetMacro(AutomaticOutputParametersComputation, bool);
  itkGetMacro(AutomaticOutputParametersComputation, bool);

  /** Set shift-scale mode */
  itkSetMacro(ShiftScaleInputImages, bool);
  itkGetMacro(ShiftScaleInputImages, bool);
  itkBooleanMacro(ShiftScaleInputImages);

  /** Set the shift-scale matrices */
  virtual void SetShiftMatrix(MatrixType shiftMatrix)
  {
    m_ShiftMatrix = MatrixType(shiftMatrix);
  }

  virtual void SetScaleMatrix(MatrixType scaleMatrix)
  {
    m_ScaleMatrix = MatrixType(scaleMatrix);
  }

  virtual MatrixType GetShiftMatrix()
  {
    return m_ShiftMatrix;
  }

  virtual MatrixType GetScaleMatrix()
  {
    return m_ScaleMatrix;
  }

protected:
  StreamingMosaicFilterBase();
  ~StreamingMosaicFilterBase()
  {
  }

  /**
   * Methods
   */
  void GenerateOutputInformation(void) override;

  void GenerateInputRequestedRegion(void) override;

  void AfterThreadedGenerateData() override;

  void BeforeThreadedGenerateData() override;

  /** Image extent computing */
  virtual void ImageToExtent(InputImageType* image, InputImagePointType& extentInf, InputImagePointType& extentSup);

  /** Requested region computing */
  virtual bool OutputRegionToInputRegion(const OutputImageRegionType& mosaicRegion, InputImageRegionType& inputRegion, InputImageType*& inputImage);

  /** normalize pixel value, according to numeric type **/
  virtual void NormalizePixelValue(InternalValueType& pixelValue);

  /** Check if the given pixel is empty */
  virtual bool IsPixelNotEmpty(InputImagePixelType& inputPixel);

  /** Get the used input images indices */
  virtual void AddUsedInputImageIndex(unsigned int index)
  {
    usedInputIndices.push_back(index);
  }

  virtual unsigned int GetUsedInputImageIndice(unsigned int i)
  {
    return usedInputIndices[i];
  }

  virtual unsigned int GetNumberOfUsedInputImages()
  {
    return usedInputIndices.size();
  }

  /** Compute output mosaic parameters (size, spacing, origin, ...) */
  virtual void ComputeOutputParameters();

  /** Get the number of input images */
  virtual unsigned int GetNumberOfInputImages()
  {
    return this->GetNumberOfInputs();
  }

  /** Get number of bands */
  virtual unsigned int GetNumberOfBands()
  {
    return nbOfBands;
  }

  /** Compute the requested region of a given input image */
  virtual void ComputeRequestedRegionOfInputImage(unsigned int inputImageIndex);

  /** Shift-Scale a value */
  virtual void ShiftScaleValue(InternalValueType& value, const unsigned int& imageIndex, unsigned int& band)
  {
    value *= m_ScaleMatrix[imageIndex][band];
    value += m_ShiftMatrix[imageIndex][band];
  }

  /** Check the shift scale matrices */
  virtual void CheckShiftScaleMatrices();

  /** Prepare interpolators, valid regions, and input images pointers */
  virtual void PrepareImageAccessors(typename std::vector<InputImageType*>& image, typename std::vector<InterpolatorPointerType>& interpolator);

private:
  StreamingMosaicFilterBase(const Self&); // purposely not implemented
  void operator=(const Self&);            // purposely not implemented

  InterpolatorPointerType m_Interpolator;      // Interpolator
  OutputImagePixelType    m_NoDataOutputPixel; // No data (output)
  InputImagePixelType     m_NoDataInputPixel;  // No data (input)

  OutputImageSpacingType m_OutputSpacing; // Output spacing
  OutputImagePointType   m_OutputOrigin;  // Output origin
  OutputImageSizeType    m_OutputSize;    // Output size

  bool m_AutomaticOutputParametersComputation; // Output parameters auto on/off
  bool m_ShiftScaleInputImages;                // Shift-scale mode on/off

  MatrixType m_ShiftMatrix; // matrix of shifts
  MatrixType m_ScaleMatrix; // matrix of scales

  /** Internal */
  unsigned int      nbOfBands;          // number of bands
  unsigned int      interpolatorRadius; // interpolator padding radius
  IndicesListType   usedInputIndices;   // requested input image indices
  InternalValueType minOutputPixelValue;
  InternalValueType maxOutputPixelValue;

}; // end of class

} // end namespace itk

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStreamingMosaicFilterBase.hxx"
#endif

#endif
