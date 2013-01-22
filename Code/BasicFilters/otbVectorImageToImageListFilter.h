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
#ifndef __otbVectorImageToImageListFilter_h
#define __otbVectorImageToImageListFilter_h

#include "otbImageToImageListFilter.h"

namespace otb
{
/** \class VectorImageToImageListFilter
 *  \brief This class aims at converting a multi-band image to a list of scalar images.
 *
 * This class takes a multi-band image represented as an otb::VectorImage and produces a list
 * of scalar images corresponding to each band of the input image.
 *
 * Casting is done through standard cast operation.
 *
 * The output of this filter didn't support the use of iterator on it, use a selector filter
 * to avoid break the output
 *
 * \ingroup Streamed
 */
template <class TVectorImageType, class TImageList>
class ITK_EXPORT VectorImageToImageListFilter
  : public ImageToImageListFilter<TVectorImageType, typename TImageList::ImageType>
{
public:
  /** Standard typedefs */
  typedef VectorImageToImageListFilter Self;
  typedef ImageToImageListFilter<TVectorImageType,
      typename TImageList::ImageType>         Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(VectorImageToImageListFilter, ImageToImageListFilter);

  /** Template parameters typedefs */
  typedef TVectorImageType                        InputVectorImageType;
  typedef typename InputVectorImageType::Pointer  InputVectorImagePointerType;
  typedef TImageList                              OutputImageListType;
  typedef typename OutputImageListType::Pointer   OutputImageListPointerType;
  typedef typename OutputImageListType::ImageType OutputImageType;
  typedef typename OutputImageType::Pointer       OutputImagePointerType;

  /** Generate the input requested region from the first element in the list. */
  virtual void GenerateInputRequestedRegion(void);

  /** Generate the output information by building the output list. */
  virtual void GenerateOutputInformation(void);

protected:

  /** Main computation method */
  virtual void GenerateData(void);

  /** Constructor */
  VectorImageToImageListFilter() {};
  /** Destructor */
  virtual ~VectorImageToImageListFilter() {}
  /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  VectorImageToImageListFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorImageToImageListFilter.txx"
#endif

#endif
