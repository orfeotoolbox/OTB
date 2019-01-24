/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbImageToLineSpatialObjectListFilter_h
#define otbImageToLineSpatialObjectListFilter_h


#include "itkProcessObject.h"
#include "otbLineSpatialObjectList.h"

namespace otb
{
/** \class ImageToLineSpatialObjectListFilter
 *  \brief Base class for all process objects that output LineSpatialObjectList.
 *
 * ImageToLineSpatialObjectListFilter is the base class for all process objects
 * that output list of LineSpatialObject.
 *
 *
 * \ingroup OTBSpatialObjects
 */

template <class TInputImage>
class ITK_EXPORT ImageToLineSpatialObjectListFilter : public itk::ProcessObject
{
public:

  /**   Extract dimensions as well of the images of entry of exit. */
  itkStaticConstMacro(InputImageDimension,
                      unsigned int,
                      TInputImage::ImageDimension);

  typedef TInputImage InputImageType;

  /** Standard class typedefs. */
  typedef ImageToLineSpatialObjectListFilter Self;
  typedef itk::ProcessObject                 Superclass;
  typedef itk::SmartPointer<Self>            Pointer;
  typedef itk::SmartPointer<const Self>      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageToLineSpatialObjectListFilter, itk::ProcessObject);

  /** Some convenient typedefs. */
  typedef LineSpatialObjectList            LinesListType;
  typedef typename LinesListType::LineType LineType;

  /** Definition of the input and output images */
  typedef typename InputImageType::PixelType InputPixelType;

  /** Definition of the size of the images. */
  typedef typename InputImageType::SizeType SizeType;

  typedef itk::ProcessObject ProcessObjectType;

  /** Set/Get the input image */
  using Superclass::SetInput;
  void SetInput(const InputImageType *image);
  const InputImageType * GetInput(void);

  /** Set/Get the list of LineSpatialObject of this process object.  */
  using Superclass::SetOutput;
  void SetOutput(const LinesListType *list);
  LinesListType * GetOutput(void);

protected:
  ImageToLineSpatialObjectListFilter();
  ~ImageToLineSpatialObjectListFilter() override {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  ImageToLineSpatialObjectListFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageToLineSpatialObjectListFilter.hxx"
#endif

#endif
