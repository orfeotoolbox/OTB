/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbImageListToImageFilter_h
#define __otbImageListToImageFilter_h

#include "itkImageSource.h"
#include "otbImageList.h"

namespace otb
{
/** \class ImageListToImageFilter
 *
 *  \brief Base class for all the filters taking an images list as input to
 *  produce an image.
 *
 * \ingroup Images
 * \ingroup Lists
 *
 * \ingroup OTBObjectList
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT ImageListToImageFilter
  : public itk::ImageSource<TOutputImage>
{
public:
  /** Standard typedefs */
  typedef ImageListToImageFilter         Self;
  typedef itk::ImageSource<TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;
  /** Type macro */
  itkNewMacro(Self);
  /** Creation through object factory macro */
  itkTypeMacro(ImageListToImageFilter, ImageSource);
  /** Template parameters typedefs */
  typedef          TInputImage                      InputImageType;
  typedef typename InputImageType::ConstPointer     InputImagePointer;
  typedef typename InputImageType::RegionType       InputImageRegionType;
  typedef typename InputImageType::PixelType        InputImagePixelType;
  typedef typename InputImageType::SizeType         SizeType;
  typedef typename InputImageType::ValueType        ValueType;
  typedef ImageList<InputImageType>                 InputImageListType;
  typedef typename InputImageListType::ConstPointer InputImageListConstPointerType;

  /** Derived typedefs */
  typedef typename Superclass::OutputImageType OutputImageType;
  typedef typename OutputImageType::Pointer    OutputImagePointerType;

  /** InputImage dimension constant */
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  /** Overiding the SetInput() and GetInput() methods */
  using Superclass::SetInput;
  virtual void SetInput(const InputImageListType * image);
  InputImageListType * GetInput(void);

protected:
  /** Constructor */
  ImageListToImageFilter();
  /** Destructor */
  ~ImageListToImageFilter() ITK_OVERRIDE {}
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;

private:
  ImageListToImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageListToImageFilter.txx"
#endif

#endif
