/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbDrawPathFilter_h
#define otbDrawPathFilter_h

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
  ~DrawPathFilter() override {}

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;
  void GenerateData() override;

private:
  DrawPathFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

  OutputImagePixelType m_Value;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbDrawPathFilter.hxx"
#endif

#endif
