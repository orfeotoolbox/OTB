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


#ifndef otbImageRegionAdaptativeSplitter_h
#define otbImageRegionAdaptativeSplitter_h

#include "itkRegion.h"
#include "itkImageRegionSplitter.h"
#include "itkIndex.h"
#include "itkSize.h"
#include "itkFastMutexLock.h"
#include "OTBCommonExport.h"

namespace otb
{

/** \class ImageRegionSquareTileSplitter
   * \brief Divide a region into several pieces.
   *
   * This region splitter tries to adapt to the tiling scheme of the
   * input image using the TileHint parameter. It aims at
   * synchronizing the streaming with the tiling scheme (in a JPEG
   * 2000 situation for example) so as to avoid reading the same tile
   * multiple times in the standard pixel-based processing scheme.
   *
   * If the requested number of splits is lower than the number of
   * tiles in the image region, then the splitter will derive splits
   * that combine several tiles in one split. If the requested number
   * of splits is greater than the number of tiles in the image
   * region, the splitter will derive splits that divide exactly one
   * tile. All splits from one input tiles will be spawned before
   * changing to a new tile, ensuring the former tile will be only
   * read once.
   *
   * If the TileHint is empty, or is VImageDimension is not 2, the
   * splitter falls back to the behaviour of
   * otb::ImageRegionSquareTileSplitter.
   *
   * \sa ImageRegionSquareTileSplitter
   *
   * \ingroup ITKSystemObjects
   * \ingroup DataProcessing
 *
 * \ingroup OTBCommon
 */

template <unsigned int VImageDimension>
class OTBCommon_EXPORT_TEMPLATE ImageRegionAdaptativeSplitter : public itk::ImageRegionSplitter<VImageDimension>
{
public:
  /** Standard class typedefs. */
  typedef ImageRegionAdaptativeSplitter             Self;
  typedef itk::ImageRegionSplitter<VImageDimension> Superclass;
  typedef itk::SmartPointer<Self>                   Pointer;
  typedef itk::SmartPointer<const Self>             ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageRegionAdaptativeSplitter, itk::Object);

  /** Dimension of the image available at compile time. */
  itkStaticConstMacro(ImageDimension, unsigned int, VImageDimension);

  /** Dimension of the image available at run time. */
  static unsigned int GetImageDimension()
  {
    return VImageDimension;
  }

  /** Index typedef support. An index is used to access pixel values. */
  typedef itk::Index<VImageDimension>        IndexType;
  typedef typename IndexType::IndexValueType IndexValueType;

  /** Size typedef support. A size is used to define region bounds. */
  typedef itk::Size<VImageDimension>       SizeType;
  typedef typename SizeType::SizeValueType SizeValueType;

  /** Region typedef support.   */
  typedef itk::ImageRegion<VImageDimension> RegionType;

  typedef std::vector<RegionType> StreamVectorType;

  /** Set the TileHint parameter */
  itkSetMacro(TileHint, SizeType);

  /** Get the TileHint parameter */
  itkGetConstReferenceMacro(TileHint, SizeType);

  /** Set the ImageRegion parameter */
  itkSetMacro(ImageRegion, RegionType);

  /** Get the ImageRegion parameter */
  itkGetConstReferenceMacro(ImageRegion, RegionType);

  /** Set the requested number of splits parameter */
  itkSetMacro(RequestedNumberOfSplits, unsigned int);

  /** Get the requested number of splits parameter */
  itkGetConstReferenceMacro(RequestedNumberOfSplits, unsigned int);

  /**
   * Calling this method will set the image region and the requested
   * number of splits, and call the EstimateSplitMap() method if
   * necessary.
   */
  unsigned int GetNumberOfSplits(const RegionType& region,
                                         unsigned int requestedNumber) override;

  /** Calling this method will set the image region and the requested
   * number of splits, and call the EstimateSplitMap() method if
   * necessary. */
  RegionType GetSplit(unsigned int i, unsigned int numberOfPieces,
                              const RegionType& region) override;

  /** Make the Modified() method update the IsUpToDate flag */
  void Modified() const override
  {
    // Call superclass implementation
    Superclass::Modified();

    // Invalidate up-to-date
    m_IsUpToDate = false;
  }

protected:
  ImageRegionAdaptativeSplitter() : m_TileHint(),
                                    m_ImageRegion(),
                                    m_RequestedNumberOfSplits(0),
                                    m_StreamVector(),
                                    m_IsUpToDate(false)
                                      {}

  ~ImageRegionAdaptativeSplitter() override {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  /** This methods actually estimate the split map and stores it in a
  vector */
  void EstimateSplitMap();

  ImageRegionAdaptativeSplitter(const ImageRegionAdaptativeSplitter &) = delete;
  void operator =(const ImageRegionAdaptativeSplitter&) = delete;

  // This reflects the input image tiling
  SizeType   m_TileHint;

  // This contains the ImageRegion that is currently being split
  RegionType m_ImageRegion;

  // This contains the requested number of splits
  unsigned int m_RequestedNumberOfSplits;

  // This is a vector of all regions which will be split
  StreamVectorType m_StreamVector;

  // Is the splitter up-to-date ?
  mutable bool m_IsUpToDate;

  // Lock to ensure thread-safety
  itk::SimpleFastMutexLock m_Lock;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
# include "otbImageRegionAdaptativeSplitter.hxx"
#endif

namespace otb {

extern template class OTBCommon_EXPORT_TEMPLATE ImageRegionAdaptativeSplitter<2>;

}

#endif
