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
#ifndef __otbImageListToSingleImageFilter_h
#define __otbImageListToSingleImageFilter_h

#include "otbImageListToImageFilter.h"
#include "otbImageList.h"

namespace otb
{
/** \class ImageListToSingleImageFilter
 *  \brief This class aims at converting a image list to one (scalar or vector) image.
 *
 * This class takes a image list image represented as an otb::ImageList and produces a
 * output which contains of scalar/vector images corresponding to the element selected.
 * This filter avoid to break the pipeline after the use of an filter which derive
 * from ImageToImagelistFilter class.
 *
 *
 * \ingroup Streamed
 */

template <class TInputImageType>
class ITK_EXPORT ImageListToSingleImageFilter
  : public ImageListToImageFilter<TInputImageType, TInputImageType>
{
public:
  /** Standard typedefs */
  typedef ImageListToSingleImageFilter              Self;
  typedef ImageListToImageFilter
      <TInputImageType, TInputImageType>            Superclass;
  typedef itk::SmartPointer<Self>                   Pointer;
  typedef itk::SmartPointer<const Self>             ConstPointer;

  typedef TInputImageType                           InputImageType;
  typedef typename InputImageType::Pointer          InputImagePointerType;
  typedef ImageList<InputImageType>                 InputImageListType;
  typedef TInputImageType                           OutputImageType;
  typedef typename OutputImageType::Pointer         OutputImagePointerType;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(ImageListToSingleImageFilter, ImageListToImageFilter);

  /** Set/Get the position of the extracted image from the image list */
  itkGetMacro(ExtractedImagePosition, unsigned int);
  itkSetMacro(ExtractedImagePosition, unsigned int);

  InputImagePointerType GetOutput(void);

protected:
  /** Constructor */
  ImageListToSingleImageFilter();
  /** Destructor */
  virtual ~ImageListToSingleImageFilter() {}

  /** Main computation method */
  virtual void GenerateData();

  /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;


private:
  ImageListToSingleImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  unsigned int m_ExtractedImagePosition;

}; // end of class

} // end of namespace

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageListToSingleImageFilter.txx"
#endif

#endif
