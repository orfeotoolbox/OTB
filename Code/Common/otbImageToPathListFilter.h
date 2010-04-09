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
#ifndef __otbImageToPathListFilter_h
#define __otbImageToPathListFilter_h

#include "otbPathListSource.h"
namespace otb
{
/** \class ImageToPathListFilter
 * \brief Base class used to implement filters that have a image as input and a paths list as output.
 *
 *
 * \ingroup PathListFilters
 * \ingroup PathLists
 */

template <class TInputImage, class TOutputPath>
//class ITK_EXPORT ImageToPathListFilter : public itk::PathSource<TOutputPath>
class ITK_EXPORT ImageToPathListFilter : public PathListSource<TOutputPath>
{
public:
  /** Standard class typedefs. */
  typedef ImageToPathListFilter         Self;
  typedef PathListSource<TOutputPath>   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef itk::ProcessObject ProcessObjectType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageToPathListFilter, PathListSource);

  /** Some convenient typedefs. */
  typedef          TInputImage                  InputImageType;
  typedef typename InputImageType::ConstPointer InputImagePointer;
  typedef typename InputImageType::RegionType   InputImageRegionType;
  typedef typename InputImageType::PixelType    InputImagePixelType;
  typedef typename InputImageType::SizeType     SizeType;
  typedef typename InputImageType::ValueType    ValueType;

  typedef typename Superclass::OutputPathType        OutputPathType;
  typedef typename Superclass::OutputPathListType    OutputPathListType;
  typedef typename Superclass::OutputPathPointerType OutputPathPointerType;

  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);

  /** Set/Get the image input of this process object. */
  virtual void SetInput(const InputImageType * image);
  const InputImageType * GetInput(void);

protected:
  ImageToPathListFilter();
  virtual ~ImageToPathListFilter() {}

  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  ImageToPathListFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageToPathListFilter.txx"
#endif

#endif
