/*
 * Copyright (C) 1999-2011 Insight Software Consortium
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

#ifndef otbImageRegionNonUniformMultidimensionalSplitter_h
#define otbImageRegionNonUniformMultidimensionalSplitter_h

#include "itkRegion.h"
#include "itkObjectFactory.h"
#include "itkIndex.h"
#include "itkSize.h"
#include "itkImageRegionSplitter.h"

namespace otb
{

/** \class ImageRegionNonUniformMultidimensionalSplitter
 * \brief Divide a region into several pieces.
 *
 * ImageRegionNonUniformMultidimensionalSplitter divides an ImageRegion into
 * smaller regions.  ImageRegionNonUniformMultidimensionalSplitter is used by
 * the StreamingImageFilter to divide a requested output region into a
 * series of smaller requests of the pipeline.  This object has two
 * basic methods: GetNumberOfSplits() and GetSplit().
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
 * ImageRegionNonUniformMultidimensionalSplitter class first proceed
 * to the division according to the last dimension, if the number of
 * required division is not reached, it proceed to divide the dimension
 * before the last one and so on.
 *
 * \ingroup ITKSystemObjects
 * \ingroup DataProcessing
 *
 * \ingroup OTBCommon
 */

template <unsigned int VImageDimension>
class ITK_EXPORT ImageRegionNonUniformMultidimensionalSplitter : public itk::ImageRegionSplitter<VImageDimension>
{
public:
  /** Standard class typedefs. */
  typedef ImageRegionNonUniformMultidimensionalSplitter Self;
  typedef itk::ImageRegionSplitter<VImageDimension>     Superclass;
  typedef itk::SmartPointer<Self>                       Pointer;
  typedef itk::SmartPointer<const Self>                 ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageRegionNonUniformMultidimensionalSplitter, ImageRegionSplitter);

  /** Dimension of the image available at compile time. */
  itkStaticConstMacro(ImageDimension, unsigned int, VImageDimension);

  /** Index typedef support. An index is used to access pixel values. */
  typedef itk::Index<VImageDimension> IndexType;

  /** Size typedef support. A size is used to define region bounds. */
  typedef itk::Size<VImageDimension> SizeType;

  /** Region typedef support.   */
  typedef itk::ImageRegion<VImageDimension> RegionType;

  /** How many pieces can the specified region be split? A given region
   * cannot always be divided into the requested number of pieces.  For
   * instance, if the numberOfPieces exceeds the number of pixels along
   * a certain dimensions, then some splits will not be possible. This
   * method returns a number less than or equal to the requested number
   * of pieces.  */
  unsigned int GetNumberOfSplits(const RegionType& region,
                                         unsigned int requestedNumber) override;

  /** Get a region definition that represents the ith piece a specified region.
   * The "numberOfPieces" specified should be less than or equal to what
   * GetNumberOfSplits() returns. */
  RegionType GetSplit(unsigned int i, unsigned int numberOfPieces,
                              const RegionType& region) override;

protected:
  ImageRegionNonUniformMultidimensionalSplitter() {}
  ~ImageRegionNonUniformMultidimensionalSplitter() override {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  ImageRegionNonUniformMultidimensionalSplitter(const Self &) = delete;
  void operator =(const Self&) = delete;

};

} // end namespace otb

// Define instantiation macro for this template.
#define ITK_TEMPLATE_ImageRegionNonUniformMultidimensionalSplitter(_, EXPORT, x, y) namespace otb { \
  _(1 (class EXPORT ImageRegionNonUniformMultidimensionalSplitter<ITK_TEMPLATE_1 x>)) \
  namespace Templates { typedef ImageRegionNonUniformMultidimensionalSplitter<ITK_TEMPLATE_1 x> \
                        ImageRegionNonUniformMultidimensionalSplitter ## y; } \
  }

#if ITK_TEMPLATE_EXPLICIT
# include "Templates/otbImageRegionNonUniformMultidimensionalSplitter+-.h"
#endif

#ifndef ITK_MANUAL_INSTANTIATION
# include "otbImageRegionNonUniformMultidimensionalSplitter.hxx"
#endif

#endif
