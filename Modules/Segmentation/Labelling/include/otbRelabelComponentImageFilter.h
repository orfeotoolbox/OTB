/*
 * Copyright (C) 1999-2011 Insight Software Consortium
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbRelabelComponentImageFilter_h
#define otbRelabelComponentImageFilter_h

#include "itkRelabelComponentImageFilter.h"
#include <vector>

namespace otb
{

/**
 * \class RelabelComponentImageFilter
 * \brief Relabel the components in an image such that consecutive labels are used.
 *
 * RelabelComponentImageFilter remaps the labels associated with the
 * objects in an image (as from the output of
 * ConnectedComponentImageFilter) such that the label numbers are
 * consecutive with no gaps between the label numbers used.  By
 * default, the relabeling will also sort the labels based on the size
 * of the object: the largest object will have label #1, the second
 * largest will have label #2, etc.
 *
 * Label #0 is assumed to be background is left unaltered by the
 * relabeling.
 *
 * RelabelComponentImageFilter is typically used on the output of the
 * ConnectedComponentImageFilter for those applications that want to
 * extract the largest object or the "k" largest objects. Any
 * particular object can be extracted from the relabeled output using
 * a BinaryThresholdImageFilter. A group of objects can be extracted
 * from the relabled output using a ThresholdImageFilter.
 *
 * Once all the objects are relabeled, the application can query the
 * number of objects and the size of each object. Object sizes are
 * returned in a vector. The size of the background is not
 * calculated. So the size of object #1 is
 * GetSizeOfObjectsInPixels()[0], the size of object #2 is
 * GetSizeOfObjectsInPixels()[1], etc.
 *
 * If user sets a minimum object size, all objects with fewer pixelss
 * than the minimum will be discarded, so that the number of objects
 * reported will be only those remaining. The
 * GetOriginalNumberOfObjects method can be called to find out how
 * many objects were present before the small ones were discarded.
 *
 * RelabelComponentImageFilter can be run as an "in place" filter,
 * where it will overwrite its output.  The default is run out of
 * place (or generate a separate output).  "In place" operation can be
 * controlled via methods in the superclass,
 * InPlaceImageFilter::InPlaceOn() and InPlaceImageFilter::InPlaceOff().
 *
 * \sa ConnectedComponentImageFilter, BinaryThresholdImageFilter, ThresholdImageFilter
 *
 * \ingroup Singlethreaded
 *
 * \ingroup OTBLabelling
 */

template <class TInputImage, class TOutputImage>
class ITK_EXPORT RelabelComponentImageFilter : public itk::RelabelComponentImageFilter<TInputImage, TOutputImage>
{
public:
  /**
   * Standard "Self" & Superclass typedef.
   */
  typedef RelabelComponentImageFilter Self;
  typedef itk::RelabelComponentImageFilter<TInputImage, TOutputImage> Superclass;

  /**
   * Types from the Superclass
   */
  typedef typename Superclass::InputImagePointer InputImagePointer;

  /**
   * Extract some information from the image types.  Dimensionality
   * of the two images is assumed to be the same.
   */
  typedef typename TOutputImage::PixelType         OutputPixelType;
  typedef typename TOutputImage::InternalPixelType OutputInternalPixelType;
  typedef typename TInputImage::PixelType          InputPixelType;
  typedef typename TInputImage::InternalPixelType  InputInternalPixelType;
  itkStaticConstMacro(ImageDimension, unsigned int, TOutputImage::ImageDimension);
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);

  /**
   * Image typedef support
   */
  typedef TInputImage                       InputImageType;
  typedef TOutputImage                      OutputImageType;
  typedef typename TInputImage::IndexType   IndexType;
  typedef typename TInputImage::SizeType    SizeType;
  typedef typename TOutputImage::RegionType RegionType;

  /**
   * Smart pointer typedef support
   */
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /**
   * Run-time type information (and related methods)
   */
  itkTypeMacro(RelabelComponentImageFilter, ImageToImageFilter);

  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);

  /** Type used as identifier for the different component labels. */
  typedef unsigned long int LabelType;

  /** Type used to count number of pixels in objects. */
  typedef unsigned long int ObjectSizeType;

protected:
  RelabelComponentImageFilter();
  virtual ~RelabelComponentImageFilter()
  {
  }

  void GenerateInputRequestedRegion();

  void EnlargeOutputRequestedRegion(itk::DataObject*){};

private:
  RelabelComponentImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // end namespace otb

#ifndef ITK_MANUAL_INSTANTIATION
#include "otbRelabelComponentImageFilter.hxx"
#endif

#endif
