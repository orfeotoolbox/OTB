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

#ifndef otbScalarImageToPanTexTextureFilter_h
#define otbScalarImageToPanTexTextureFilter_h

#include "otbGreyLevelCooccurrenceIndexedList.h"
#include "itkImageToImageFilter.h"

namespace otb
{
/** \class ScalarImageToPanTexTextureFilter
 *  \brief This class computes a texture derived built-up precense index (PanTex)
 *
 * This class computes a texture-derived built-up presence index (PanTex) from textural
 * characteristics of scalar images.It is the min value of the contrast in 8 directions.
 *
 * Print references:
 *
 * Pesari, M., A. Gerhardinger, F. Kayitakire. 2008.  A robust built-up area precense
 * index by anisotropic rotation-invariant textural measure.
 * IEEE Journal of selected topics in applied earth observations and remote sensing.
 * Vol1, NO3.
 *
 *
 * \sa otb::GreyLevelCooccurrenceIndexedList
 *
 *
 * \ingroup OTBTextures
 */

template<class TInpuImage, class TOutputImage>
class ScalarImageToPanTexTextureFilter : public itk::ImageToImageFilter
  <TInpuImage, TOutputImage>
{
public:
  /** Standard class typedefs */
  typedef ScalarImageToPanTexTextureFilter                  Self;
  typedef itk::ImageToImageFilter<TInpuImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                           Pointer;
  typedef itk::SmartPointer<const Self>                     ConstPointer;

  /** Creation through the object factory */
  itkNewMacro(Self);

  /** RTTI */
  itkTypeMacro(ScalarImageToPanTexTextureFilter, ImageToImageFilter);

  /** Template class typedefs */
  typedef TInpuImage                           InputImageType;
  typedef typename InputImageType::Pointer     InputImagePointerType;
  typedef typename InputImageType::PixelType   InputPixelType;
  typedef typename InputImageType::RegionType  InputRegionType;
  typedef typename InputRegionType::SizeType   SizeType;
  typedef typename InputImageType::OffsetType   OffsetType;
  typedef typename std::vector< OffsetType>  OffsetListType;
  typedef TOutputImage                         OutputImageType;
  typedef typename OutputImageType::Pointer    OutputImagePointerType;
  typedef typename OutputImageType::RegionType OutputRegionType;

  typedef GreyLevelCooccurrenceIndexedList< InputPixelType >   CooccurrenceIndexedListType;
  typedef typename CooccurrenceIndexedListType::Pointer       CooccurrenceIndexedListPointerType;
  typedef typename CooccurrenceIndexedListType::ConstPointer  CooccurrenceIndexedListConstPointerType;
  typedef typename CooccurrenceIndexedListType::IndexType              CooccurrenceIndexType;
  typedef typename CooccurrenceIndexedListType::PixelValueType         PixelValueType;
  typedef typename CooccurrenceIndexedListType::RelativeFrequencyType  RelativeFrequencyType;
  typedef typename CooccurrenceIndexedListType::VectorType             VectorType;

  typedef typename VectorType::iterator                    VectorIteratorType;
  typedef typename VectorType::const_iterator              VectorConstIteratorType;


  /** Set the radius of the window on which textures will be computed */
  itkSetMacro(Radius, SizeType);
  /** Get the radius of the window on which textures will be computed */
  itkGetMacro(Radius, SizeType);

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

protected:
  /** Constructor */
  ScalarImageToPanTexTextureFilter();
  /** Destructor */
  ~ScalarImageToPanTexTextureFilter() override;
  /** Generate the input requested region */
  void GenerateInputRequestedRegion() override;
  /** Parallel textures extraction */
  void ThreadedGenerateData(const OutputRegionType& outputRegion, itk::ThreadIdType threadId) override;

private:
  ScalarImageToPanTexTextureFilter(const Self&) = delete;
  void operator =(const Self&) = delete;

  /** Convenient method to compute union of 2 regions */
  static OutputRegionType RegionUnion(const OutputRegionType& region1, const OutputRegionType& region2);

  /** Radius of the window on which to compute textures */
  SizeType m_Radius;

  /** List of Offsets for co-occurence */
  OffsetListType m_OffsetList;

  /** Number of bins per axis for histogram generation */
  unsigned int m_NumberOfBinsPerAxis;

  /** Input image minimum */
  InputPixelType m_InputImageMinimum;

  /** Input image maximum */
  InputPixelType m_InputImageMaximum;

};
} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbScalarImageToPanTexTextureFilter.hxx"
#endif

#endif
