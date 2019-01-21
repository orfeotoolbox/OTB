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

#ifndef otbBinaryImageMinimalBoundingRegionCalculator_h
#define otbBinaryImageMinimalBoundingRegionCalculator_h

#include "itkImageToImageFilter.h"
#include "itkObjectFactory.h"

namespace otb
{
/**
 * \class BinaryImageMinimalBoundingRegionCalculator
 * \brief This class compute the smallest region of the image
 * containing every pixel with the foreground value.
 *
 * This class is used for instance in the RCC8 calculator filter,
 * where the input region used for computation has to be the smallest possible
 * for costs reasons. The Pad arg allows the user to get a region of pad  pixel larger
 * at each bound in case a security margin has to be kept.
 *
 * \sa ImageToImageRCC8Calculator
 *
 * \ingroup OTBImageManipulation
 */
template <class TInputImage>
class ITK_EXPORT BinaryImageMinimalBoundingRegionCalculator
  : public itk::ImageToImageFilter<TInputImage, TInputImage>
{
public:
  /** Standard typedefs */
  typedef BinaryImageMinimalBoundingRegionCalculator        Self;
  typedef itk::ImageToImageFilter<TInputImage, TInputImage> Superclass;
  typedef itk::SmartPointer<Self>                           Pointer;
  typedef itk::SmartPointer<const Self>                     ConstPointer;
  /** Creation through object factory macro */
  itkNewMacro(Self);
  /** Type macro */
  itkTypeMacro(BinaryImageMinimalBoundingRegionCalculator, ImageToImageFilter);

  typedef TInputImage                         InputImageType;
  typedef typename InputImageType::PixelType  PixelType;
  typedef typename InputImageType::RegionType RegionType;
  typedef typename InputImageType::Pointer    InputImagePointerType;
  /** Toogle the pad option */
  itkGetMacro(Region, RegionType);
  itkSetMacro(InsideValue, PixelType);
  itkGetMacro(InsideValue, PixelType);
  itkSetMacro(Pad, unsigned int);
  itkGetMacro(Pad, unsigned int);

protected:
  /** Constructor */
  BinaryImageMinimalBoundingRegionCalculator();
  /** Destructor */
  ~BinaryImageMinimalBoundingRegionCalculator() override {}
  /** Main computation method */
  void GenerateData(void) override;
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;
private:
  BinaryImageMinimalBoundingRegionCalculator(const Self &) = delete;
  void operator =(const Self&) = delete;
  /** The computed region */
  RegionType m_Region;
  /** Toogle if pad wanted */
  unsigned int m_Pad;
  /** Inside value */
  PixelType m_InsideValue;
};
} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbBinaryImageMinimalBoundingRegionCalculator.hxx"
#endif

#endif
