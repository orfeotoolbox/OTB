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

#ifndef otbScalarImageToHigherOrderTexturesFilter_h
#define otbScalarImageToHigherOrderTexturesFilter_h

#include "itkImageToImageFilter.h"
#include "itkScalarImageToRunLengthFeaturesFilter.h"

namespace otb
{
/** \class ScalarImageToHigherOrderTexturesFilter
 *  \brief This class compute 11 local higher order statistics textures coefficients
 *  based on the grey level run-length matrix
 *
 *  This filter computes the following Haralick textures over a sliding window with
 *  user defined radius:
 *  (where \f$ p(i, j) \f$ is the element in
 *  cell i, j of a normalized Run Length Matrix,
 *  \f$n_r\f$ is the total number of runs
 *   and \f$n_p\f$ is the total number of pixels ):
 *
 * "Short Run Emphasis" \f$ = SRE = \frac{1}{n_r} \sum_{i, j}\frac{p(i, j)}{j^2} \f$
 *
 * "Long Run Emphasis" \f$ = LRE =  \frac{1}{n_r} \sum_{i, j}p(i, j) * j^2 \f$
 *
 * "Grey-Level Nonuniformity" \f$ = GLN = \frac{1}{n_r} \sum_{i} \left( \sum_{j}{p(i, j)} \right)^2 \f$
 *
 * "Run Length Nonuniformity" \f$ = RLN = \frac{1}{n_r} \sum_{j} \left( \sum_{i}{p(i, j)} \right)^2 \f$
 *
 * "Low Grey-Level Run Emphasis" \f$ = LGRE = \frac{1}{n_r} \sum_{i, j}\frac{p(i, j)}{i^2} \f$
 *
 * "High Grey-Level Run Emphasis" \f$ = HGRE = \frac{1}{n_r} \sum_{i, j}p(i, j) * i^2  \f$
 *
 * "Short Run Low Grey-Level Emphasis" \f$ = SRLGE = \frac{1}{n_r} \sum_{i, j}\frac{p(i, j)}{i^2 j^2} \f$
 *
 * "Short Run High Grey-Level Emphasis" \f$ = SRHGE = \frac{1}{n_r} \sum_{i, j}\frac{p(i, j) * i^2}{j^2} \f$
 *
 * "Long Run Low Grey-Level Emphasis" \f$ = LRLGE = \frac{1}{n_r} \sum_{i, j}\frac{p(i, j) * j^2}{i^2} \f$
 *
 * "Long Run High Grey-Level Emphasis" \f$ = LRHGE = \frac{1}{n_r} \sum_{i, j} p(i, j) i^2 j^2 \f$
 *
 * Neighborhood size can be set using the SetRadius() method. Offset for co-occurence estimation
 * is set using the SetOffset() method.
 *
 * \sa otb::MaskedScalarImageToGreyLevelRunLengthMatrixGenerator
 * \sa itk::GreyLevelRunLengthMatrixTextureCoefficientsCalculator
 *
 * \ingroup Streamed
 * \ingroup Threaded
 *
 *
 * \ingroup OTBTextures
 */
template<class TInpuImage, class TOutputImage>
class ScalarImageToHigherOrderTexturesFilter : public itk::ImageToImageFilter
  <TInpuImage, TOutputImage>
{
public:
  /** Standard class typedefs */
  typedef ScalarImageToHigherOrderTexturesFilter            Self;
  typedef itk::ImageToImageFilter<TInpuImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                           Pointer;
  typedef itk::SmartPointer<const Self>                     ConstPointer;

  /** Creation through the object factory */
  itkNewMacro(Self);

  /** RTTI */
  itkTypeMacro(ScalarImageToHigherOrderTexturesFilter, ImageToImageFilter);

  /** Template class typedefs */
  typedef TInpuImage                           InputImageType;
  typedef typename InputImageType::Pointer     InputImagePointerType;
  typedef typename InputImageType::PixelType   InputPixelType;
  typedef typename InputImageType::RegionType  InputRegionType;
  typedef typename InputRegionType::SizeType   SizeType;
  typedef TOutputImage                         OutputImageType;
  typedef typename OutputImageType::Pointer    OutputImagePointerType;
  typedef typename OutputImageType::RegionType OutputRegionType;

  /** Co-occurence matrix and textures calculator */
/*  typedef otb::MaskedScalarImageToGreyLevelRunLengthMatrixGenerator
    <InputImageType>                                           RunLengthMatrixGeneratorType;
  typedef typename RunLengthMatrixGeneratorType::Pointer       RunLengthMatrixGeneratorPointerType;
  typedef typename RunLengthMatrixGeneratorType::OffsetType    OffsetType;
  typedef typename RunLengthMatrixGeneratorType::HistogramType HistogramType;
  typedef itk::Statistics::GreyLevelRunLengthMatrixTextureCoefficientsCalculator
    <HistogramType>                                            TextureCoefficientsCalculatorType;
  typedef typename TextureCoefficientsCalculatorType::Pointer  TextureCoefficientsCalculatorPointerType;
  */

  typedef itk::Statistics::ScalarImageToRunLengthFeaturesFilter
    <InputImageType> ScalarImageToRunLengthFeaturesFilterType;
  //typedef typename ImageType::PixelType                PixelType;
  typedef typename InputImageType::OffsetType          OffsetType;
  typedef itk::VectorContainer< unsigned char, OffsetType > OffsetVector;
  typedef typename OffsetVector::Pointer               OffsetVectorPointer;
  typedef typename OffsetVector::ConstPointer          OffsetVectorConstPointer;


  /** Set the radius of the window on which textures will be computed */
  itkSetMacro(Radius, SizeType);
  /** Get the radius of the window on which textures will be computed */
  itkGetMacro(Radius, SizeType);

  /** Get/Set the offset or offsets over which the co-occurrence pairs will be computed.
      Calling either of these methods clears the previous offsets. */
  itkSetConstObjectMacro(Offsets, OffsetVector);
  itkGetConstObjectMacro(Offsets, OffsetVector);

  void SetOffset(const OffsetType offset);

  /** Set the number of bin per axis for histogram generation */
  itkSetMacro(NumberOfBinsPerAxis, unsigned int);

  /** Get the number of bin per axis for histogram generation */
  itkGetMacro(NumberOfBinsPerAxis, unsigned int);

  /** Set the input image minimum */
  itkSetMacro(InputImageMinimum, InputPixelType);

  /** Get the input image minimum */
  itkGetMacro(InputImageMinimum, InputPixelType);

  /** Set the input image maximum */
  itkSetMacro(InputImageMaximum, InputPixelType);

  /** Get the input image maximum */
  itkGetMacro(InputImageMaximum, InputPixelType);

  /* Enable/Disable fast calculation */
  itkGetConstMacro(FastCalculations, bool);
  itkSetMacro(FastCalculations, bool);
  itkBooleanMacro(FastCalculations);

  /** Set the sub-sampling factor */
  itkSetMacro(SubsampleFactor, SizeType);

  /** Get the sub-sampling factor */
  itkGetMacro(SubsampleFactor, SizeType);

  /** Set the sub-sampling offset */
  itkSetMacro(SubsampleOffset, OffsetType);

  /** Get the sub-sampling offset */
  itkGetMacro(SubsampleOffset, OffsetType);

  /** Get the Short Run Emphasis output image */
  OutputImageType * GetShortRunEmphasisOutput();

  /** Get the Long Run Emphasis output image */
  OutputImageType * GetLongRunEmphasisOutput();

  /** Get the Grey-Level Nonuniformity output image */
  OutputImageType * GetGreyLevelNonuniformityOutput();

  /** Get the Run Length Nonuniformity output image */
  OutputImageType * GetRunLengthNonuniformityOutput();

  /** Get the Run Percentage output image */
  OutputImageType * GetRunPercentageOutput();

  /** Get the Low Grey-Level Run Emphasis output image */
  OutputImageType * GetLowGreyLevelRunEmphasisOutput();

  /** Get the High Grey-Level Run Emphasis output image */
  OutputImageType * GetHighGreyLevelRunEmphasisOutput();

  /** Get the Short Run Low Grey-Level Emphasis output image */
  OutputImageType * GetShortRunLowGreyLevelEmphasisOutput();

  /** Get the Short Run High Grey-Level Emphasis output image */
  OutputImageType * GetShortRunHighGreyLevelEmphasisOutput();

  /** Get the Long Run Low Grey-Level Emphasis output image */
  OutputImageType * GetLongRunLowGreyLevelEmphasisOutput();

  /** Get the Long Run High Grey-Level Emphasis output image */
  OutputImageType * GetLongRunHighGreyLevelEmphasisOutput();

protected:
  /** Constructor */
  ScalarImageToHigherOrderTexturesFilter();
  /** Destructor */
  ~ScalarImageToHigherOrderTexturesFilter() override;
  /** Generate the output information */
  void GenerateOutputInformation() override;
  /** Generate the input requested region */
  void GenerateInputRequestedRegion() override;
  /** Parallel textures extraction */
  void ThreadedGenerateData(const OutputRegionType& outputRegion, itk::ThreadIdType threadId) override;

private:
  ScalarImageToHigherOrderTexturesFilter(const Self&) = delete;
  void operator =(const Self&) = delete;

  /** Convenient method to compute union of 2 regions */
  static OutputRegionType RegionUnion(const OutputRegionType& region1, const OutputRegionType& region2);

  /** Radius of the window on which to compute textures */
  SizeType m_Radius;

  /** Offset for co-occurence */
  OffsetVectorConstPointer m_Offsets;

  /** Number of bins per axis for histogram generation */
  unsigned int m_NumberOfBinsPerAxis;

  /** Input image minimum */
  InputPixelType m_InputImageMinimum;

  /** Input image maximum */
  InputPixelType m_InputImageMaximum;

  /** Fast calculation */
  bool m_FastCalculations;

  /** Sub-sampling factor */
  SizeType m_SubsampleFactor;

  /** Sub-sampling offset */
  OffsetType m_SubsampleOffset;
};
} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbScalarImageToHigherOrderTexturesFilter.hxx"
#endif

#endif
