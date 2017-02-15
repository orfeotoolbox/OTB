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
#ifndef otbImageListToImageListApplyFilter_h
#define otbImageListToImageListApplyFilter_h

#include "otbImageListToImageListFilter.h"

namespace otb
{
/** \class ImageListToImageListApplyFilter
 *  \brief This filter applies the filter given as template to the input image list.
 *
 *  It produces one output image in the output list for one input image in the output list.
 *
 *  It uses the given filter methods to generate output information and input requested region.
 *
 *  The filter can be set up via the GetFilter() accessor.
 *
 *  It also support streaming when the templated filter support it.
 *
 *  No type checking is done : it is up to the user to check the adequation between the input images,
 *  output images and filter IO types.
 *
 * \ingroup Streamed
 *
 * \ingroup OTBObjectList
 */
template <class TInputImageList, class TOutputImageList, class TFilter>
class ITK_EXPORT ImageListToImageListApplyFilter
  : public ImageListToImageListFilter<typename TInputImageList::ImageType,
      typename TOutputImageList::ImageType>
{
public:
  /** Standard typedefs */
  typedef ImageListToImageListApplyFilter Self;
  typedef ImageListToImageListFilter<typename TInputImageList::ImageType,
      typename TOutputImageList::ImageType> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(ImageListToImageListApplyFilter, ImageListToImageListFilter);

  /** Template parameters typedefs */
  typedef TInputImageList                         InputImageListType;
  typedef typename InputImageListType::Pointer    InputImageListPointerType;
  typedef typename InputImageListType::ImageType  InputImageType;
  typedef TOutputImageList                        OutputImageListType;
  typedef typename OutputImageListType::Pointer   OutputImageListPointerType;
  typedef typename OutputImageListType::ImageType OutputImageType;
  typedef TFilter                                 FilterType;
  typedef typename FilterType::Pointer            FilterPointerType;

  /** Accessors */
  itkSetObjectMacro(Filter, FilterType);
  itkGetObjectMacro(Filter, FilterType);

  itkGetMacro(OutputIndex, unsigned int);
  itkSetMacro(OutputIndex, unsigned int);

  /** Generate output information for the ImageList and for each image
      in the list. */
  void GenerateOutputInformation(void) ITK_OVERRIDE;

  /** Generate input requested region for each image in the list. */
  void GenerateInputRequestedRegion(void) ITK_OVERRIDE;

protected:
  /** Main computation method */
  void GenerateData(void) ITK_OVERRIDE;
  /** Constructor */
  ImageListToImageListApplyFilter();
  /** Destructor */
  ~ImageListToImageListApplyFilter() ITK_OVERRIDE {}
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;

private:
  ImageListToImageListApplyFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** The processing filter */
  FilterPointerType m_Filter;
  /** The index of the output of the filter to use */
  unsigned int m_OutputIndex;
};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageListToImageListApplyFilter.txx"
#endif

#endif
