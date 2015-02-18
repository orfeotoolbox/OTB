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
#ifndef __otbDrawPathListFilter_h
#define __otbDrawPathListFilter_h

#include "itkImageToImageFilter.h"
#include "otbObjectList.h"

namespace otb
{
/** \class DrawPathListFilter
 * \brief This class can be used to draw a list of path on an image.
 *
 * This filter first copy the input image to the output, with default casting operators.
 * It then uses the otb::PolyLineImageIterator to draw each polyline. This iterator uses
 * the general Bresenham algorithm known to be efficient in segment drawing.
 *
 * If the UsePathInternalValue is toggled, the filter check if the metadata dictionary of
 * the input path has a "Value" key. If it is the case, it will use this value to draw the
 * Path instead of the default value. If not, it will use the default value.
 *
 * \sa PolyLineParametricPathWithValue
 * \sa MetaDataDictionary
 *
 * \ingroup PathListFilters
 * \ingroup PathLists
 */

template <class TInputImage, class TInputPath, class TOutputImage>
class ITK_EXPORT DrawPathListFilter : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef DrawPathListFilter                                 Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(DrawPathListFilter, ImageToImageFilter);

  /** Some convenient typedefs. */
  typedef          TInputImage                  InputImageType;
  typedef typename InputImageType::Pointer      InputImagePointerType;
  typedef typename InputImageType::ConstPointer InputImageConstPointerType;
  typedef typename InputImageType::RegionType   InputImageRegionType;
  typedef typename InputImageType::PixelType    InputImagePixelType;
  typedef typename InputImageType::SizeType     InputImageSizeType;
  typedef typename InputImageType::ValueType    InputImageValueType;

  typedef          TInputPath                      InputPathType;
  typedef typename InputPathType::Pointer          InputPathPointerType;
  typedef otb::ObjectList<InputPathType>           InputPathListType;
  typedef typename InputPathListType::ConstPointer InputPathListConstPointerType;
  typedef typename InputPathListType::Pointer      InputPathListPointerType;

  typedef          TOutputImage                  OutputImageType;
  typedef typename OutputImageType::Pointer      OutputImagePointerType;
  typedef typename OutputImageType::ConstPointer OutputImageConstPointerType;
  typedef typename OutputImageType::RegionType   OutputImageRegionType;
  typedef typename OutputImageType::PixelType    OutputImagePixelType;
  typedef typename OutputImageType::SizeType     OutputImageSizeType;
  typedef typename OutputImageType::ValueType    OutputImageValueType;
  typedef typename OutputImageType::IndexType    OutputImageIndexType;

  typedef itk::ProcessObject ProcessObjectType;

  itkGetMacro(UseInternalPathValue, bool);
  itkSetMacro(UseInternalPathValue, bool);
  itkBooleanMacro(UseInternalPathValue);

  itkGetMacro(AddValue, bool);
  itkSetMacro(AddValue, bool);
  itkBooleanMacro(AddValue);

  itkSetMacro(PathValue, OutputImagePixelType);
  itkGetMacro(PathValue, OutputImagePixelType);

  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);

  /** Set/Get the path input of this process object. */
  virtual void SetInputPath(InputPathListType *path);
  const InputPathListType * GetInputPath(void);

protected:
  /** Constructor */
  DrawPathListFilter();
  /** Desctructor */
  virtual ~DrawPathListFilter() {}
  /** Printself method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;
  /** Main computation method */
  virtual void GenerateData();

private:
  DrawPathListFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** Default value to draw */
  OutputImagePixelType m_PathValue;

  /** If set to true, the algorithm try to use path internal metadata dictionary value */
  bool m_UseInternalPathValue;

  bool m_AddValue;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbDrawPathListFilter.txx"
#endif

#endif
