/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbSOMActivationBuilder_h
#define otbSOMActivationBuilder_h

#include "itkImageToImageFilter.h"

namespace otb
{
/** \class SOMActivationBuilder
 *  \brief This class takes a self organizing map and an input vector image, and
 *  produce a map counting the number of activation for each vector in the SOM map.
 *
 * SOM map can be set via the SetInput method, and vector image via the SetVectorSet
 * method.
 *
 * \sa SOMMap
 * \sa SOM
 *
 * \ingroup OTBSOM
 */
template <class TListSample, class TInputMap, class TOutputImage>
class ITK_EXPORT SOMActivationBuilder : public itk::ImageToImageFilter<TInputMap, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef SOMActivationBuilder Self;
  typedef itk::ImageToImageFilter<TInputMap, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Creation through object factory */
  itkNewMacro(Self);

  /** Runtime information macro */
  itkTypeMacro(SOMActivationBuilder, ImageToImageFilter);

  /** Template related typedefs */
  typedef TListSample  ListSampleType;
  typedef TOutputImage OutputImageType;
  typedef TInputMap    InputMapType;

  typedef typename ListSampleType::Pointer  ListSamplePointerType;
  typedef typename OutputImageType::Pointer OutputImagePointerType;
  typedef typename InputMapType::Pointer    InputMapPointerType;

  /** Image Dimension */
  itkStaticConstMacro(ImageDimension, unsigned int, InputMapType::ImageDimension);

  /** ListSample accessor */
  itkSetObjectMacro(ListSample, ListSampleType);
  itkGetObjectMacro(ListSample, ListSampleType);

protected:
  /** Constructor */
  SOMActivationBuilder();
  /** Destructor */
  ~SOMActivationBuilder() override;
  /** Main computation method */
  void GenerateData(void) override;
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  SOMActivationBuilder(const Self&) = delete;
  void operator=(const Self&) = delete;
  /** ListSample object */
  ListSamplePointerType m_ListSample;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSOMActivationBuilder.hxx"
#endif

#endif
