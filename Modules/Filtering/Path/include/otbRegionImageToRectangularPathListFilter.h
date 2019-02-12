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


// Adapted from otbImageToPathListAlignFilter.h
#ifndef otbRegionImageToRectangularPathListFilter_h
#define otbRegionImageToRectangularPathListFilter_h

#include "itkImageSource.h"
#include "itkConceptChecking.h"
#include "otbImage.h"
#include "otbImageToPathListFilter.h"
#include <vector>
#include "otbVectorDataSource.h"
#include "otbVectorData.h"

namespace otb
{

/** \class RegionImageToRectangularPathListFilter
 * \brief Base class used to implement filter to model regions by rectangles.
 *
 *
 * \ingroup OTBPath
 */
template <class TInputImage, class TOutputPath>
class ITK_EXPORT RegionImageToRectangularPathListFilter : public ImageToPathListFilter<TInputImage, TOutputPath>
{
public:
  /** Standard class typedefs. */
  typedef RegionImageToRectangularPathListFilter          Self;
  typedef ImageToPathListFilter<TInputImage, TOutputPath> Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(RegionImageToRectangularPathListFilter, ImageToPathListFilter);
//  itkTypeMacro(ImageToPathListAlignFilter, itk::ImageSource);

  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);

  /** Some convenient typedefs. */
  typedef typename Superclass::OutputPathListType OutputPathListType;

  typedef typename Superclass::InputImageType       InputImageType;
  typedef typename Superclass::InputImageRegionType InputImageRegionType;
  typedef typename InputImageType::Pointer          InputImagePointer;
  typedef typename InputImageType::ConstPointer     InputImageConstPointer;

  typedef typename InputImageType::SizeType  SizeType;
  typedef typename InputImageType::ValueType ValueType;
  typedef typename InputImageType::PixelType PixelType;

  typedef typename Superclass::OutputPathType OutputPathType;
  // typedef typename Superclass::OutputPathListType     OutputPathListType;
  typedef typename Superclass::OutputPathPointerType OutputPathPointerType;

//  typedef          float                                        RealType;
//typedef typename itk::NumericTraits<PixelType>::RealType       RealType;
  typedef double RealType;
//  typedef typename itk::Image<RealType, InputImageDimension>      RealImageType;
  typedef typename otb::Image<RealType, InputImageDimension> RealImageType;
  typedef typename RealImageType::Pointer                    RealImageTypePointer;
  typedef typename RealImageType::IndexType                  RealImageTypeIndexType;

  /** The minimum rectangular fit of returned rectangles. */
  itkSetMacro(MinimumFit, double);
  itkGetConstReferenceMacro(MinimumFit, double);

  /** The minimum size of returned rectangles. */
  itkSetMacro(MinimumSize, double);
  itkGetConstReferenceMacro(MinimumSize, double);

  //jgc
  /** Dimension of the image the iterator walks.  This constant is needed so
   * that functions that are templated over image iterator type (as opposed to
   * being templated over pixel type and dimension) can have compile time
   * access to the dimension of the image that the iterator walks. */
  itkStaticConstMacro(NDimensions, unsigned int, TInputImage::ImageDimension);

protected:
  RegionImageToRectangularPathListFilter();
  ~RegionImageToRectangularPathListFilter() override;

  void GenerateOutputInformation() override{}  // do nothing
  void GenerateData() override;

  double m_MinimumFit;
  double m_MinimumSize;
  int    m_CrossTermFormula;

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  //jgc
  typedef Image<unsigned char, itkGetStaticConstMacro(NDimensions)> MarkerImageType;
  typename MarkerImageType::Pointer markerImage;

private:
  RegionImageToRectangularPathListFilter(const Self &) = delete;
  void operator =(const Self&) = delete;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRegionImageToRectangularPathListFilter.hxx"
#endif

#endif
