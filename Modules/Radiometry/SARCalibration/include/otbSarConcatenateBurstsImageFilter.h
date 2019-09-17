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


#ifndef otbSarConcatenateBurstsImageFilter_h
#define otbSarConcatenateBurstsImageFilter_h

#include "itkImageToImageFilter.h"
#include "otbTileImageFilter.h"
#include "otbImageKeywordlist.h"

namespace otb
{
/** \class SarConcatenateBurstsImageFilter
 *  \brief This filter allows making a spatial mosaic from a set of bursts
 *
 *  This filter derives from TileImagefilter and produces a spatial mosaic from a set of bursts
 *  from Sentinel-1 IW products . The layout parameter allows setting up of the images will
 *  be patched together: it is a 2D array containing the number of images in the horizontal
 *  direction and vertical direction respectively.
 *
 *  Images can be set using the PushBackInput() or SetInput(unsigned
 *  int, Image *) methods. Please note that input images are supposed
 *  to be set in the lexicographical order.
 *
 *  This filter does not support missing images: the number of input
 *  images must match exactly layout[0]*layout[1]. Additional
 *  consistency checks are that the number of components and spacing
 *  must be consistent for all images, and the size of the images must
 *  match with each others.
 *
 *  Metadata are adapted in order to correspond to the contatenated SAR image.
 *
 * \ingroup OTBSARCalibration
 * \ingroup MultiThreaded
 *
 * \ingroup OTBImageManipulation
 */
template <class TImage>
class ITK_EXPORT SarConcatenateBurstsImageFilter : public TileImageFilter<TImage>
{
public:
  /** Standard class typedef */
  typedef SarConcatenateBurstsImageFilter Self;
  typedef TileImageFilter<TImage>         Superclass;
  typedef itk::SmartPointer<Self>         Pointer;
  typedef itk::SmartPointer<const Self>   ConstPointer;

  /** Helper typedefs */
  typedef TImage                         ImageType;
  typedef typename ImageType::Pointer    ImagePointerType;
  typedef typename ImageType::SizeType   SizeType;
  typedef typename ImageType::RegionType RegionType;
  typedef typename ImageType::PointType  PointType;

  typedef std::pair<unsigned long, unsigned long> RecordType;
  typedef std::vector<RecordType> LinesRecordVectorType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SarConcatenateBurstsImageFilter, TileImageFilter);

  // Setter
  void SetSLCImageKeyWorList(ImageKeywordlist sarImageKWL);

  bool getDeburstLinesAndSamples(LinesRecordVectorType& linesRecord, LinesRecordVectorType& samplesRecord, unsigned int first_burstInd,
                                 bool inputWithInvalidPixels);

protected:
  /** Constructor */
  SarConcatenateBurstsImageFilter() = default;

  /** Destructor */
  ~SarConcatenateBurstsImageFilter() = default;

  /** Generate input requested region method */
  void GenerateOutputInformation() override;


private:
  SarConcatenateBurstsImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  // SLC KeywordList
  ImageKeywordlist m_SLCImageKWL;

  // Deburst SLC KeywordList
  ImageKeywordlist m_DeburstSLCImageKWL;

  unsigned int m_Offset_OriginL;
};

} // end namespace itk

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSarConcatenateBurstsImageFilter.hxx"
#endif

#endif
