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

#ifndef otbBinaryImageToDensityImageFilter_h
#define otbBinaryImageToDensityImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkDataObject.h"
#include "itkConstNeighborhoodIterator.h"

namespace otb
{
/** \class BinaryImageToDensityImageFilter
 *  \brief This class performs a streaming isotropic shrinking operation without smoothing.
 *
 *
 * \ingroup OTBImageManipulation
 */
template <class TInputImage, class TOutputImage, class TCountFunction>
class ITK_EXPORT BinaryImageToDensityImageFilter : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef BinaryImageToDensityImageFilter Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Creation through object factory macro */
  itkNewMacro(Self);

  /** Type macro */
  itkTypeMacro(BinaryImageToDensityImageFilter, itk::ImageToImageFilter);

  /** Template parameter typedefs */
  typedef TInputImage                         InputImageType;
  typedef typename InputImageType::RegionType InputImageRegionType;
  typedef typename InputImageType::Pointer    InputImagePointerType;
  typedef typename InputImageType::SizeType   InputImageSizeType;

  typedef TOutputImage                      OutputImageType;
  typedef typename OutputImageType::Pointer OutputImagePointerType;

  typedef TCountFunction                      CountFunctionType;
  typedef typename CountFunctionType::Pointer CountFunctionPointerType;

  typedef itk::ConstNeighborhoodIterator<TInputImage>   NeighborhoodIteratorType;
  typedef typename NeighborhoodIteratorType::RadiusType RadiusType;

  /** Shrink factor accessor */
  itkSetMacro(NeighborhoodRadius, RadiusType);
  itkGetMacro(NeighborhoodRadius, RadiusType);

  void SetNeighborhoodRadius(unsigned int rad)
  {
    m_NeighborhoodRadius.Fill(rad);
    this->Modified();
  }

  /** Main computation method */
  void ThreadedGenerateData(const InputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId) override;
  void BeforeThreadedGenerateData() override;
  void GenerateInputRequestedRegion() override;

protected:
  /** Constructor */
  BinaryImageToDensityImageFilter();
  /** Destructor */
  ~BinaryImageToDensityImageFilter() override;
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  BinaryImageToDensityImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  CountFunctionPointerType m_CountFunction;

  /** The shrink factor */
  RadiusType m_NeighborhoodRadius;
};
} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbBinaryImageToDensityImageFilter.hxx"
#endif

#endif
