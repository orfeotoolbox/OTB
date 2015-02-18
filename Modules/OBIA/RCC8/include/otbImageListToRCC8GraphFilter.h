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
#ifndef __otbImageListToRCC8GraphFilter_h
#define __otbImageListToRCC8GraphFilter_h

#include "otbRCC8GraphSource.h"
#include "otbImageList.h"

namespace otb
{
/** \class ImageListToRCC8GraphFilter
 * \brief Base class for filters taking an images list as input to
 * produce a RCC8 graph.
 */
template <class TInputImage, class TOutputGraph>
class ITK_EXPORT ImageListToRCC8GraphFilter
  : public RCC8GraphSource<TOutputGraph>
{
public:
  /** Standard typedefs */
  typedef ImageListToRCC8GraphFilter    Self;
  typedef RCC8GraphSource<TOutputGraph> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  /** Type macro */
  itkNewMacro(Self);
  /** Creation through object factory macro */
  itkTypeMacro(ImageListToRCC8GraphFilter, RCC8GraphSource);
  /** Template input parameters typedefs */
  typedef          TInputImage                      InputImageType;
  typedef typename InputImageType::ConstPointer     InputImagePointer;
  typedef typename InputImageType::RegionType       InputImageRegionType;
  typedef typename InputImageType::PixelType        InputImagePixelType;
  typedef typename InputImageType::SizeType         SizeType;
  typedef typename InputImageType::ValueType        ValueType;
  typedef ImageList<InputImageType>                 InputImageListType;
  typedef typename InputImageListType::Pointer      InputImageListPointerType;
  typedef typename InputImageListType::ConstPointer InputImageListConstPointer;
  /** Template output parameters typedefs */
  typedef TOutputGraph                                OutputGraphType;
  typedef typename Superclass::OutputGraphPointerType OutputGraphPointerType;
  typedef typename OutputGraphType::VertexType        VertexType;
  typedef typename VertexType::Pointer                VertexPointerType;
  typedef typename OutputGraphType::RCC8ValueType     RCC8ValueType;
  /** InputImage dimension constant */
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  /** Overiding the SetInput() and GetInput() methods */
  virtual void SetInput(const InputImageListType * imageList);
  virtual InputImageListType * GetInput(void);

protected:
  /** Constructor */
  ImageListToRCC8GraphFilter();
  /** Destructor */
  virtual ~ImageListToRCC8GraphFilter() {}
  /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  ImageListToRCC8GraphFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageListToRCC8GraphFilter.txx"
#endif

#endif
