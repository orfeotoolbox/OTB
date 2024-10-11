/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbLabelImageRegionPruningFilter_h
#define otbLabelImageRegionPruningFilter_h

#include "otbImage.h"
#include "otbVectorImage.h"
#include "itkImageToImageFilter.h"
#include "itkNumericTraits.h"

#include <set>

namespace otb
{

/** \class LabelImageRegionPruningFilter
 *
 *
 * This class merges regions in the input label image according to the input
 * image of spectral values and the RangeBandwidth parameter.
 *
 *
 * \ingroup ImageSegmentation
 *
 * \ingroup OTBConversion
 */
template <class TInputLabelImage, class TInputSpectralImage, class TOutputLabelImage = TInputLabelImage, class TOutputClusteredImage = TInputSpectralImage>
class ITK_EXPORT LabelImageRegionPruningFilter : public itk::ImageToImageFilter<TInputLabelImage, TOutputLabelImage>
{
public:
  /** Standard class typedef */
  typedef LabelImageRegionPruningFilter Self;
  typedef itk::ImageToImageFilter<TInputLabelImage, TOutputLabelImage> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  typedef double                        RealType;

  /** Type macro */
  itkTypeMacro(LabelImageRegionPruningFilter, ImageToImageFilter);
  itkNewMacro(Self);

  /** Template parameters typedefs */
  typedef TInputLabelImage                        InputLabelImageType;
  typedef typename InputLabelImageType::PixelType InputLabelType;

  typedef TInputLabelImage                        InputImageType;
  typedef typename InputImageType::Pointer        InputImagePointerType;
  typedef typename InputImageType::PixelType      InputPixelType;
  typedef typename InputImageType::IndexType      InputIndexType;
  typedef typename InputImageType::SizeType       InputSizeType;
  typedef typename InputImageType::IndexValueType InputIndexValueType;
  typedef typename InputImageType::PointType      PointType;
  typedef typename InputImageType::RegionType     RegionType;
  typedef typename InputImageType::SizeType       SizeType;

  typedef TInputSpectralImage                     InputSpectralImageType;
  typedef typename TInputSpectralImage::PixelType SpectralPixelType;

  typedef TOutputLabelImage                        OutputLabelImageType;
  typedef typename OutputLabelImageType::PixelType OutputLabelType;

  typedef TOutputLabelImage                    OutputImageType;
  typedef typename OutputImageType::Pointer    OutputImagePointerType;
  typedef typename OutputImageType::PixelType  OutputPixelType;
  typedef typename OutputImageType::RegionType OutputRegionType;

  typedef TOutputClusteredImage OutputClusteredImageType;

  itkStaticConstMacro(ImageDimension, unsigned int, InputLabelImageType::ImageDimension);

  /** Typedefs for region adjacency map */
  typedef InputLabelType                           LabelType;
  typedef std::set<LabelType>                      AdjacentLabelsContainerType;
  typedef std::vector<AdjacentLabelsContainerType> RegionAdjacencyMapType;

  itkSetMacro(MinRegionSize, RealType);
  itkGetConstMacro(MinRegionSize, RealType);

  /** Returns the const image of region labels */
  const OutputLabelImageType* GetLabelOutput() const;
  /** Returns the image of region labels */
  OutputLabelImageType* GetLabelOutput();
  /** Returns the const clustered output image, with one spectral value per region  */
  const OutputClusteredImageType* GetClusteredOutput() const;
  /** Returns the clustered output image, with one spectral value per region  */
  OutputClusteredImageType* GetClusteredOutput();

  /** Sets the input image where the value of a pixel is the region id */
  void SetInputLabelImage(const InputLabelImageType* labelImage);
  /** Sets the input image representing spectral values */
  void SetInputSpectralImage(const InputSpectralImageType* spectralImage);
  /** Returns input label image */
  InputLabelImageType* GetInputLabelImage();
  /** Returns input spectral image */
  InputSpectralImageType* GetInputSpectralImage();

protected:
  void EnlargeOutputRequestedRegion(itk::DataObject* output) override;

  void GenerateOutputInformation(void) override;

  void GenerateData() override;

  /** Constructor */
  LabelImageRegionPruningFilter();

  /** Destructor */
  ~LabelImageRegionPruningFilter() override;

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /** Method to build a map of adjacent regions */
  RegionAdjacencyMapType LabelImageToRegionAdjacencyMap(typename OutputLabelImageType::Pointer inputLabelImage);

private:
  LabelImageRegionPruningFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** Number of components per pixel in the input image */
  unsigned int m_NumberOfComponentsPerPixel;
  unsigned int m_MinRegionSize;
  /** This contains the label to which each label will be merged */
  std::vector<LabelType> m_CanonicalLabels;
  /** Contains the spectral value for each region */
  std::vector<SpectralPixelType> m_Modes;
  /** Number of points in each region */
  std::vector<unsigned int> m_PointCounts;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLabelImageRegionPruningFilter.hxx"
#endif

#endif
