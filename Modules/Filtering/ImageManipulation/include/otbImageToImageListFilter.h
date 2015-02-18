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
#ifndef __otbImageToImageListFilter_h
#define __otbImageToImageListFilter_h

#include "otbImageListSource.h"

namespace otb
{
/** \class ImageToImageListFilter
 *
 *  \brief Base class for all the filters taking an image input to
 *  produce an image list.
 *
 * \ingroup Images
 * \ingroup Lists
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT ImageToImageListFilter
  : public ImageListSource<TOutputImage>
{
public:
  /** Standard typedefs */
  typedef ImageToImageListFilter        Self;
  typedef ImageListSource<TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  /** Type macro */
  itkNewMacro(Self);
  /** Creation through object factory macro */
  itkTypeMacro(ImageToImageListFilter, ImageListSource);
  /** Template parameters typedefs */
  typedef          TInputImage                  InputImageType;
  typedef typename InputImageType::ConstPointer InputImagePointer;
  typedef typename InputImageType::RegionType   InputImageRegionType;
  typedef typename InputImageType::PixelType    InputImagePixelType;
  typedef typename InputImageType::SizeType     SizeType;
  typedef typename InputImageType::ValueType    ValueType;
  /** Derived typedefs */
  typedef typename Superclass::OutputImageType        OutputImageType;
  typedef typename Superclass::OutputImageListType    OutputImageListType;
  typedef typename Superclass::OutputImagePointerType OutputImagePointerType;
  /** InputImage dimension constant */
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  /** Overiding the SetInput() and GetInput() methods */
  virtual void SetInput(const InputImageType * image);
  InputImageType * GetInput(void);

protected:
  /** Constructor */
  ImageToImageListFilter();
  /** Destructor */
  virtual ~ImageToImageListFilter() {}
  /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  ImageToImageListFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageToImageListFilter.txx"
#endif

#endif
