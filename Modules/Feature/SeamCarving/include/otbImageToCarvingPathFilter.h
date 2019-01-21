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

#ifndef otbImageToCarvingPathFilter_h
#define otbImageToCarvingPathFilter_h

#include "otbImageToPathFilter.h"
#include "itkImageSliceConstIteratorWithIndex.h"

namespace otb
{
/**
 * \class ImageToCarvingPathFilter
 * \brief This filter find the minimum energy path of an image.
 *
 * This filter find the minimum energy path from top to bottom of
 * the image (if Direction is set to 0) or from left to right
 * (if direction is set to 1).
 *
 * This is one component necessary to apply the seam carving algorithm
 * published by Shai Avidan, Ariel Shamir,
 * Seam Carving for Content-Aware Image Resizing,
 * ACM Transactions on Graphics, Volume 26, Number 3,
 * SIGGRAPH 2007
 *
 * \par INPUTS
 * The input is an image, typically the output of a gradient filter.
 *
 * \par OUTPUTS
 * The output is
 * a polyline parametric path comprising one and only one pixel per line
 * (resp. column) if direction is 0 (resp. 1).
 *
 * \sa RemoveCarvingPathFilter
 *
 * \ingroup PathFilters
 * \ingroup ImageFilters
 *
 * \ingroup OTBSeamCarving
 */

template <class TInputImage, class TOutputPath>
class ITK_EXPORT ImageToCarvingPathFilter
  : public ImageToPathFilter<TInputImage, TOutputPath>
{
public:
  /** standards typedefs */
  typedef ImageToCarvingPathFilter                    Self;
  typedef ImageToPathFilter<TInputImage, TOutputPath> Superclass;
  typedef itk::SmartPointer<Self>                     Pointer;
  typedef itk::SmartPointer<const Self>               ConstPointer;

  /// Creation through the object factory
  itkNewMacro(Self);
  /// Runtime information
  itkTypeMacro(ImageToCarvingPathFilter, ImageToPathFilter);

  /// Template parameters typedef
  typedef typename Superclass::InputImageType        InputImageType;
  typedef typename Superclass::InputImagePointerType InputImagePointerType;
  typedef typename Superclass::OutputPathType        OutputPathType;
  typedef typename Superclass::OutputPathPointerType OutputPathPointerType;

  typedef typename InputImageType::PixelType PixelType;

  /** Set and Get foreground value */
  itkSetMacro(ForegroundValue, PixelType);
  itkGetConstMacro(ForegroundValue, PixelType);

  /** Set and Get direction value */
  itkSetMacro(Direction, unsigned int);
  itkGetConstMacro(Direction, unsigned int);

  /** Get EnergyPerPix value */
  itkGetConstMacro(EnergyPerPix, double);

protected:
  ImageToCarvingPathFilter();
  ~ImageToCarvingPathFilter() override {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;
  void GenerateOutputInformation() override {}  //does nothing
  void GenerateData() override;

private:
  ImageToCarvingPathFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

  PixelType    m_ForegroundValue;
  unsigned int m_Direction;
  double       m_EnergyPerPix;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageToCarvingPathFilter.hxx"
#endif

#endif
