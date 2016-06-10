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
#ifndef __otbImageListToVectorImageFilter_h
#define __otbImageListToVectorImageFilter_h

#include "otbImageListToImageFilter.h"

namespace otb
{
/** \class ImageListToVectorImageFilter
 *  \brief Converts an ImageList to a VectorImage.
 *
 * The output VectorImage has n band, where n is the number of images in the image list.
 *
 * This filter assumes that the images in the input ImageList have all the same size.
 *
 * Casting is done through standard cast operation.
 *
 * \ingroup Streamed
 *
 * \ingroup OTBObjectList
 */
template <class TImageList, class TVectorImage>
class ITK_EXPORT ImageListToVectorImageFilter
  : public ImageListToImageFilter<typename TImageList::ImageType, TVectorImage>
{
public:
  /** Standard typedefs */
  typedef ImageListToVectorImageFilter Self;
  typedef ImageListToImageFilter<typename TImageList::ImageType,
      TVectorImage>        Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(ImageListToVectorImageFilter, ImageToImageListFilter);

  /** Template parameters typedefs */
  typedef TVectorImage                            OutputVectorImageType;
  typedef typename OutputVectorImageType::Pointer OutputVectorImagePointerType;
  typedef TImageList                              InputImageListType;
  typedef typename InputImageListType::Pointer    InputImageListPointerType;
  typedef typename InputImageListType::ImageType  InputImageType;
  typedef typename InputImageType::Pointer        InputImagePointerType;

protected:

  /** Main computation method */
  void GenerateData(void) ITK_OVERRIDE;

  /** GenerateOutputInformation
   * Set the number of bands of the output.
   * Copy information from the first image of the list if existing.
   **/
  void GenerateOutputInformation(void) ITK_OVERRIDE;

  /**
   * GenerateInputRequestedRegion
   * Set the requested region of each image in the list.
   */
  void GenerateInputRequestedRegion(void) ITK_OVERRIDE;

  /** Constructor */
  ImageListToVectorImageFilter() {};
  /** Destructor */
  ~ImageListToVectorImageFilter() ITK_OVERRIDE {}
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;

private:
  ImageListToVectorImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageListToVectorImageFilter.txx"
#endif

#endif
