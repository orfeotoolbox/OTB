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
#ifndef __otbDrawPathFilter_h
#define __otbDrawPathFilter_h

#include "itkImageAndPathToImageFilter.h"

namespace otb
{
/** \class DrawPathFilter
 * \brief This class can be used to draw a single path on an image.
 *
 * It encapsulates the use of the otb::DrawPathListFilter by internally creating
 * an otb::ObjectList<TInputPath> of size one and passing it to a DrawPathListFilter.
 *
 * Please note that iterating this filter to draw several path is NOT efficient. If you
 * have several path to draw, consider using the otb::DrawPathListFilter.
 *
 * \sa DrawPathListFilter
 *
 * \ingroup PathFilters
 * \ingroup ImageFilters
 *
 * \ingroup OTBPath
 */

template <class TInputImage, class TInputPath, class TOutputImage>
class ITK_EXPORT DrawPathFilter : public itk::ImageAndPathToImageFilter<TInputImage, TInputPath, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef DrawPathFilter                                                        Self;
  typedef itk::ImageAndPathToImageFilter<TInputImage, TInputPath, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                                               Pointer;
  typedef itk::SmartPointer<const Self>                                         ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(DrawPathFilter, itk::ImageAndPathToImageFilter);

  /** Some convenient typedefs. */
  typedef          TInputImage                   InputImageType;
  typedef typename InputImageType::Pointer       InputImagePointer;
  typedef typename InputImageType::ConstPointer  InputImageConstPointer;
  typedef typename InputImageType::RegionType    InputImageRegionType;
  typedef typename InputImageType::PixelType     InputImagePixelType;
  typedef          TInputPath                    InputPathType;
  typedef typename InputPathType::Pointer        InputPathPointer;
  typedef typename InputPathType::ConstPointer   InputPathConstPointer;
  typedef typename InputPathType::InputType      InputPathInputType;
  typedef typename InputPathType::OutputType     InputPathOutputType;
  typedef typename InputPathType::IndexType      InputPathIndexType;
  typedef typename InputPathType::OffsetType     InputPathOffsetType;
  typedef          TOutputImage                  OutputImageType;
  typedef typename OutputImageType::Pointer      OutputImagePointer;
  typedef typename OutputImageType::ConstPointer OutputImageConstPointer;

  typedef typename OutputImageType::RegionType OutputImageRegionType;
  typedef typename OutputImageType::PixelType  OutputImagePixelType;
  typedef typename OutputImageType::ValueType  OutputImageValueType;

  itkSetMacro(Value, OutputImagePixelType);
  itkGetConstReferenceMacro(Value, OutputImagePixelType);

  /** Set/Get the path input of this process object.
   Note: these methods are adding to get the proper name */
  void SetInputPath(const TInputPath * path);

  const TInputPath * GetInputPath(void);

protected:
  DrawPathFilter();
  ~DrawPathFilter() ITK_OVERRIDE {}

  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;
  void GenerateData() ITK_OVERRIDE;

private:
  DrawPathFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  OutputImagePixelType m_Value;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbDrawPathFilter.txx"
#endif

#endif
