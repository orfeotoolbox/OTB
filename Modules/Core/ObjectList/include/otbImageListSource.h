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

#ifndef otbImageListSource_h
#define otbImageListSource_h

#include "itkProcessObject.h"
#include "otbImageList.h"

namespace otb
{
/** \class ImageListSource
 *
 *  \brief Base class for all the filters producing an otbImageList
 *
 * \ingroup DataSources
 * \ingroup Images
 * \ingroup Lists
 *
 * \ingroup OTBObjectList
 */
template <class TOutputImage>
class ITK_EXPORT ImageListSource
  : public itk::ProcessObject
{
public:
  /** Standard typedefs */
  typedef ImageListSource               Self;
  typedef itk::ProcessObject            Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  /** Type macro */
  itkNewMacro(Self);
  /** Creation through object factory macro */
  itkTypeMacro(ImageListSource, itk::ProcessObject);
  /** Data object pointer type */
  typedef itk::DataObject::Pointer DataObjectPointer;
  /** Template parameter typedef*/
  typedef TOutputImage                               OutputImageType;
  typedef typename OutputImageType::Pointer          OutputImagePointerType;
  typedef ImageList<OutputImageType>                 OutputImageListType;
  typedef typename OutputImageListType::Pointer      OutputImageListPointerType;
  typedef typename OutputImageListType::ConstPointer OutputImageListConstPointerType;
  /** Overriding of the GetOutput() method */
  virtual OutputImageListType * GetOutput(void);

protected:
  /** Constructor */
  ImageListSource();
  /** Destructor */
  ~ImageListSource() override {}
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  ImageListSource(const Self &) = delete;
  void operator =(const Self&) = delete;
};
} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageListSource.hxx"
#endif
#endif
