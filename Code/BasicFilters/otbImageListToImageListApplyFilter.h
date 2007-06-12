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
#ifndef _otbImageListToImageListApplyFilter_h
#define _otbImageListToImageListApplyFilter_h

#include "otbImageListToImageListFilter.h"


namespace otb
{
/** \class ImageListToImageListApplyFilter
 *  \brief 
 *
 * \ingroup 
 * \ingroup
 */
template <class TInputImageList, class TOutputImageList, class TFilter>
class ITK_EXPORT ImageListToImageListApplyFilter
  : public ImageListToImageListFilter<typename TInputImageList::ImageType, 
  typename TOutputImageList::ImageType>
{
 public:
  /** Standard typedefs */
  typedef ImageListToImageListApplyFilter            Self;
  typedef ImageListToImageListFilter<typename TInputImageList::ImageType, 
    typename TOutputImageList::ImageType> Superclass;
  typedef itk::SmartPointer<Self>           Pointer;
  typedef itk::SmartPointer<const Self>     ConstPointer;
  
  /** Type macro */
  itkNewMacro(Self);
  
  /** Creation through object factory macro */
  itkTypeMacro(ImageListToImageListApplyFilter, ImageListToImageListFilter);
  
  /** Template parameters typedefs */
  typedef TInputImageList InputImageListType;
  typedef typename InputImageListType::ConstPointer InputImageListConstPointerType;
  typedef typename InputImageListType::ImageType InputImageType;
  typedef TOutputImageList OutputImageListType;
  typedef typename OutputImageListType::Pointer OutputImageListPointerType;
  typedef typename OutputImageListType::ImageType OutputImageType;
  typedef TFilter FilterType;
  typedef typename FilterType::Pointer FilterPointerType;

  /** Accessors */
  itkSetObjectMacro(Filter,FilterType);
  itkGetObjectMacro(Filter,FilterType);

  itkGetMacro(OutputIndex,unsigned int);
  itkSetMacro(OutputIndex,unsigned int);

  /** Generate output information for the ImageList and for each image
      in the list. */
  virtual void GenerateOutputInformation(void);

  /** Generate input requested region for each image in the list. */
  virtual void GenerateInputRequestedRegion(void);

protected:
  /** Main computation method */
  virtual void GenerateData(void);
  /** Constructor */
  ImageListToImageListApplyFilter();
  /** Destructor */
  virtual ~ImageListToImageListApplyFilter() {};
 /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  ImageListToImageListApplyFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** The processing filter */
  FilterPointerType m_Filter;
  /** The index of the output of the filter to use */
  unsigned int m_OutputIndex;
};
}// End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageListToImageListApplyFilter.txx"
#endif

#endif
