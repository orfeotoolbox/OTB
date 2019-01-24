/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbMorphologicalPyramidSynthesisFilter_h
#define otbMorphologicalPyramidSynthesisFilter_h

#include "otbImageToImageListFilter.h"

namespace otb
{
/**
 * \class MorphologicalPyramidSynthesisFilter
 * \brief Recomposition of the multiscale analysis performed with
 * a MorphologicalPyramidAnalyseFilter.
 *
 * Once a morphological pyramidal decompisition has been made, it is
 * possible to rebuild without any loss of information the original
 * image at full resolution. The MorphologicalPyramidsynthesisFilter
 * provides such a functionality. At each iteration, it performs the
 * up-sampling of the current image using the
 * otb::morphologicalPyramid::Resampler filter, the addition of
 * the details lost in down-sampling, and the addition of the details
 * lost in smoothing.
 *
 * \ingroup MultiScale
 * \sa MorphologicalPyramidAnalyseFilter
 * \sa MorphologicalPyramidResampler
 *
 * \ingroup OTBMorphologicalPyramid
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT MorphologicalPyramidSynthesisFilter
  : public ImageToImageListFilter<TInputImage, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef MorphologicalPyramidSynthesisFilter               Self;
  typedef ImageToImageListFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                           Pointer;
  typedef itk::SmartPointer<const Self>                     ConstPointer;
  /** Creation through object factory macro */
  itkNewMacro(Self);
  /** Type macro */
  itkTypeMacro(MorphologicalPyramidSynthesisFilter, MorphologicalPyramidSynthesisFilter);
  /** Input image dimension constant macro */
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  /** Output related typedefs */
  typedef typename Superclass::OutputImageType                      OutputImageType;
  typedef typename Superclass::OutputImageListType                  OutputImageListType;
  typedef typename OutputImageListType::Pointer                     OutputImageListPointerType;
  typedef typename Superclass::OutputImagePointerType               OutputImagePointerType;
  typedef typename Superclass::OutputImageListType::Iterator        ImageListIterator;
  typedef typename Superclass::OutputImageListType::ReverseIterator ImageListReverseIterator;
  /** Input related typedefs */
  typedef typename Superclass::InputImageType       InputImageType;
  typedef typename Superclass::InputImageRegionType InputImageRegionType;
  typedef typename InputImageType::Pointer          InputImagePointer;
  typedef typename InputImageType::ConstPointer     InputImageConstPointer;
  typedef typename InputImageType::SizeType         SizeType;
  typedef typename InputImageType::ValueType        ValueType;
  typedef typename InputImageType::PixelType        PixelType;
  typedef typename InputImageType::SpacingType      SpacingType;
  typedef otb::ImageList<InputImageType>            InputImageListType;
  typedef typename InputImageListType::Pointer      InputImageListPointerType;
  /** Size vector typedefs */
  typedef std::vector<SizeType>                     SizeVectorType;
  typedef typename SizeVectorType::iterator         SizeIterator;
  typedef typename SizeVectorType::reverse_iterator SizeReverseIterator;
  /**
   * Set The SupFilter details
   * \param imageList The brighter details extracted from the filtering operation.
   */
  void SetSupFilter(InputImageListType * imageList);
  /**
   * Set The InfFilter details
   * \param imageList The darker details extracted from the filtering operation.
   */
  void SetInfFilter(InputImageListType * imageList);
  /**
   * Set The SupDeci details
   * \param imageList The brighter details extracted from the filtering operation.
   */
  void SetSupDeci(InputImageListType * imageList);
  /**
   * Set The InfDeci details
   * \param imageList The darker details extracted from the filtering operation.
   */
  void SetInfDeci(InputImageListType * imageList);
  /**
   * Get The SupFilter details
   * \return The brighter details extracted from the filtering operation.
   */
  InputImageListType* GetSupFilter(void);
  /**
   * Get The InfFilter details
   * \return The darker details extracted from the filtering operation.
   */
  InputImageListType* GetInfFilter(void);
  /**
   * Get The SupDeci details
   * \return The brighter details extracted from the resampling operation.
   */
  InputImageListType* GetSupDeci(void);
  /**
   * Get The InfDeci details
   * \return The brighter details extracted from the resampling operation.
   */
  InputImageListType* GetInfDeci(void);

protected:
  /** Constructor */
  MorphologicalPyramidSynthesisFilter();
  /** Destructor */
  ~MorphologicalPyramidSynthesisFilter() override;
  void GenerateOutputInformation() override {}  // does nothing
  /** Main computation method */
  void GenerateData() override;
  /** Printself method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;
};
} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMorphologicalPyramidSynthesisFilter.hxx"
#endif

#endif
