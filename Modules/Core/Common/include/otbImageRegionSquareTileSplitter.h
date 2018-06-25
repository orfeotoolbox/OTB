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


#ifndef otbImageRegionSquareTileSplitter_h
#define otbImageRegionSquareTileSplitter_h

#include "itkRegion.h"
#include "itkImageRegionSplitter.h"
#include "itkObjectFactory.h"
#include "itkIndex.h"
#include "itkSize.h"

namespace otb
{

/** \class ImageRegionSquareTileSplitter
   * \brief Divide a region into several pieces.
   *
   * ImageRegionSquareTileSplitter divides an ImageRegion into smaller regions.
   * ImageRegionSquareTileSplitter is used by the StreamingImageFilter to divide a
   * requested output region into a series of smaller requests of the
   * pipeline.  This object has two basic methods: GetNumberOfSplits()
   * and GetSplit().
   *
   * GetNumberOfSplits() is used to determine how may subregions a given
   * region can be divided.  You call GetNumberOfSplits with an argument
   * that is the number of subregions you want.  If the image region can
   * support that number of subregions, that number is returned.
   * Otherwise, the maximum number of splits a region can support will
   * be returned.  For example, if a region splitter class only divides
   * a region into horizontal slabs, then the maximum number of splits
   * will be the number of rows in the region.
   *
   * GetSplit() returns the ith of N subregions (as an ImageRegion object).
   *
   * This ImageRegionSquareTileSplitter class divides a region along the outermost
   * dimension. If the outermost dimension has size 1 (i.e. a volume
   * with a single slice), the ImageRegionSquareTileSplitter will divide the
   * region along the next outermost dimension. If that dimension has size 1,
   * the process continues with the next outermost dimension.
   *
   * Regions obtained by the ImageRegionSquareTileSplitter are aligned on a grid
   * with width of 256. Divisions can occur only at line defined as k*256.
   *
   * Other ImageRegionSquareTileSplitter subclasses could divide an image into
   * more uniform shaped regions instead of slabs.
   *
   * \sa ImageRegionMultidimensionalSplitter
   *
   * \ingroup ITKSystemObjects
   * \ingroup DataProcessing
 *
 * \ingroup OTBCommon
 */

template <unsigned int VImageDimension>
class ITK_EXPORT ImageRegionSquareTileSplitter : public itk::ImageRegionSplitter<VImageDimension>
{
public:
  /** Standard class typedefs. */
  typedef ImageRegionSquareTileSplitter                Self;
  typedef itk::ImageRegionSplitter<VImageDimension> Superclass;
  typedef itk::SmartPointer<Self>                   Pointer;
  typedef itk::SmartPointer<const Self>             ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageRegionSquareTileSplitter, itk::Object);

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

  /** How many pieces can the specified region be split? A given region
   *  cannot always be divided into the requested number of pieces.  For
   *  instance, if the numberOfPieces exceeds the number of pixels along
   *  a certain dimensions, then some splits will not be possible.
   */
  unsigned int GetNumberOfSplits(const RegionType& region,
                                         unsigned int requestedNumber) override;

  /** Get a region definition that represents the ith piece a specified region.
   * The "numberOfPieces" specified should be less than or equal to what
   * GetNumberOfSplits() returns. */
  RegionType GetSplit(unsigned int i, unsigned int numberOfPieces,
                              const RegionType& region) override;

  itkGetMacro(TileSizeAlignment, unsigned int);
  itkSetMacro(TileSizeAlignment, unsigned int);

  itkGetMacro(TileDimension, unsigned int);

protected:
  ImageRegionSquareTileSplitter() : m_SplitsPerDimension(0U), m_TileDimension(0), m_TileSizeAlignment(16) {}
  ~ImageRegionSquareTileSplitter() override {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  ImageRegionSquareTileSplitter(const ImageRegionSquareTileSplitter &) = delete;
  void operator =(const ImageRegionSquareTileSplitter&) = delete;

  itk::FixedArray<unsigned int, VImageDimension> m_SplitsPerDimension;
  unsigned int m_TileDimension;
  unsigned int m_TileSizeAlignment;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
# include "otbImageRegionSquareTileSplitter.hxx"
#endif

#endif
