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

#ifndef otbImageToModulusAndDirectionImageFilter_h
#define otbImageToModulusAndDirectionImageFilter_h

#include "itkImageToImageFilter.h"

namespace otb
{

/** \class ImageToModulusAndDirectionImageFilter
 *
 * \brief Base class for modulus and direction image filters.
 *
 * This is the base class for alls class' generate modulus and
 * directions outputs.
 * GetOutput() method return the output image modulus and
 * GetOutputDirection() return the output image direction
 *
 *
 * \ingroup OTBCommon
 */
template <class TInputImage,
    class TOutputImage,
    class TOutputImageDirection = TOutputImage>
class ITK_EXPORT ImageToModulusAndDirectionImageFilter :  public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /**   Extract dimensions as well of the images of entry of exit. */
  itkStaticConstMacro(InputImageDimension,
                      unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension,
                      unsigned int,
                      TOutputImage::ImageDimension);

  /** typedef for the classes standards. */
  typedef ImageToModulusAndDirectionImageFilter              Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  /** Method for management of the object factory. */
  itkNewMacro(Self);

  /** Return the name of the class. */
  itkTypeMacro(ImageToModulusAndDirectionImageFilter, itk::ImageToImageFilter);

  typedef typename Superclass::InputImageType  InputImageType;
  typedef typename Superclass::OutputImageType OutputImageType;
  typedef TOutputImageDirection                OutputImageDirectionType;

  typedef typename InputImageType::Pointer       InputImagePointer;
  typedef typename InputImageType::ConstPointer  InputImageConstPointer;
  typedef typename InputImageType::RegionType    InputImageRegionType;
  typedef typename OutputImageType::Pointer      OutputImagePointer;
  typedef typename OutputImageType::ConstPointer OutputImageConstPointer;
  typedef typename OutputImageType::RegionType   OutputImageRegionType;

  /** Return the const output image modulus */
  const OutputImageType * GetOutput() const;

  /** Return the output image modulus */
  OutputImageType * GetOutput();

  /** Return the const output image direction */
  const OutputImageDirectionType * GetOutputDirection() const;
  /** Return the output image direction */
  OutputImageDirectionType * GetOutputDirection();

  void GenerateInputRequestedRegion() override;

protected:
  ImageToModulusAndDirectionImageFilter();
  ~ImageToModulusAndDirectionImageFilter() override {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  ImageToModulusAndDirectionImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageToModulusAndDirectionImageFilter.hxx"
#endif

#endif
