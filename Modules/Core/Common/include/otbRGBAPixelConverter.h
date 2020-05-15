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


#ifndef otbRGBAPixelConverter_h
#define otbRGBAPixelConverter_h

#include "itkProcessObject.h"
#include "itkRGBAPixel.h"
#include "itkRGBPixel.h"

namespace otb
{
/**
 * \class RGBAPixelConverter
 * \brief Convert an itk::RGBApixel<InternalType> into another pixel
 * type
 *
 * To be usable, the desired conversion must be implemented through
 * partial specialisation mechanism.
 *
 *
 * \ingroup OTBCommon
 */


// RGBA to Scalar
template <class TInternalInputPixelType, class TOutputPixelType>
class ITK_EXPORT RGBAPixelConverter : public itk::ProcessObject
{
public:
  /** Standard class typedefs */
  typedef RGBAPixelConverter            Self;
  typedef itk::ProcessObject            Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(RGBAPixelConverter, ProcessObject);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  typedef typename itk::RGBAPixel<TInternalInputPixelType> InputPixelType;
  typedef TOutputPixelType                                 OutputPixelType;

  OutputPixelType Convert(InputPixelType input);

protected:
  RGBAPixelConverter()
  {
  }
  ~RGBAPixelConverter() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override
  {
    Superclass::PrintSelf(os, indent);
  }

private:
  RGBAPixelConverter(const Self&) = delete;
  void operator=(const Self&) = delete;
};

// RGBA to RGBA
template <class TInternalInputPixelType, class TInternalOutputPixelType>
class ITK_EXPORT RGBAPixelConverter<TInternalInputPixelType, typename itk::RGBAPixel<TInternalOutputPixelType>> : public itk::ProcessObject
{
public:
  /** Standard class typedefs */
  typedef RGBAPixelConverter            Self;
  typedef itk::ProcessObject            Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(RGBAPixelConverter, ProcessObject);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  typedef typename itk::RGBAPixel<TInternalInputPixelType>  InputPixelType;
  typedef typename itk::RGBAPixel<TInternalOutputPixelType> OutputPixelType;

  OutputPixelType Convert(InputPixelType input);

protected:
  RGBAPixelConverter()
  {
  }
  ~RGBAPixelConverter() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override
  {
    Superclass::PrintSelf(os, indent);
  }

private:
  RGBAPixelConverter(const Self&) = delete;
  void operator=(const Self&) = delete;
};

// RGBA to RGB
template <class TInternalInputPixelType, class TInternalOutputPixelType>
class ITK_EXPORT RGBAPixelConverter<TInternalInputPixelType, typename itk::RGBPixel<TInternalOutputPixelType>> : public itk::ProcessObject
{
public:
  /** Standard class typedefs */
  typedef RGBAPixelConverter            Self;
  typedef itk::ProcessObject            Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(RGBAPixelConverter, ProcessObject);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  typedef typename itk::RGBAPixel<TInternalInputPixelType> InputPixelType;
  typedef typename itk::RGBPixel<TInternalOutputPixelType> OutputPixelType;

  OutputPixelType Convert(InputPixelType input);

protected:
  RGBAPixelConverter()
  {
  }
  ~RGBAPixelConverter() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override
  {
    Superclass::PrintSelf(os, indent);
  }

private:
  RGBAPixelConverter(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRGBAPixelConverter.hxx"
#endif

#endif
