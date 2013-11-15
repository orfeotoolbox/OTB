/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkRelabelComponentImageFilter.h, v $
  Language:  C++
  Date:      $Date: 2009-04-27 22:58:48 $
  Version:   $Revision: 1.17 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbRelabelComponentImageFilter_h
#define __otbRelabelComponentImageFilter_h

#include "itkRelabelComponentImageFilter.h"
#include "itkImage.h"
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
 */

template <class TInputImage, class TOutputImage>
class ITK_EXPORT RelabelComponentImageFilter :
    public itk::RelabelComponentImageFilter< TInputImage, TOutputImage >
{
public:
  /**
   * Standard "Self" & Superclass typedef.
   */
  typedef RelabelComponentImageFilter                     Self;
  typedef itk::RelabelComponentImageFilter < TInputImage, TOutputImage > Superclass;

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
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TOutputImage::ImageDimension);
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);

  /**
   * Image typedef support
   */
  typedef TInputImage                             InputImageType;
  typedef TOutputImage                            OutputImageType;
  typedef   typename TInputImage::IndexType       IndexType;
  typedef   typename TInputImage::SizeType        SizeType;
  typedef   typename TOutputImage::RegionType     RegionType;

  /**
   * Smart pointer typedef support
   */
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /**
   * Run-time type information (and related methods)
   */
  itkTypeMacro(RelabelComponentImageFilter, ImageToImageFilter);

  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);

  /** Type used as identifier for the different component labels. */
  typedef unsigned long int    LabelType;

  /** Type used to count number of pixels in objects. */
  typedef unsigned long int    ObjectSizeType;

protected:

  RelabelComponentImageFilter();
  virtual ~RelabelComponentImageFilter() {}

  void GenerateInputRequestedRegion();

  void EnlargeOutputRequestedRegion(itk::DataObject *){};

private:
  RelabelComponentImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace otb

#ifndef ITK_MANUAL_INSTANTIATION
#include "otbRelabelComponentImageFilter.txx"
#endif

#endif
