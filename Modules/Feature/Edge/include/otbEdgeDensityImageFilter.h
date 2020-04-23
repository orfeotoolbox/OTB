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

#ifndef otbEdgeDensityImageFilter_h
#define otbEdgeDensityImageFilter_h

#include "otbBinaryImageToDensityImageFilter.h"
#include "itkProcessObject.h"


namespace otb
{

/** \class EdgeDensityImageFilter
 *  \brief This composite filter computes the density of the edges around a pixel.
 *
 *
 *
 * \ingroup OTBEdge
 */

template <class TInputImage, class TOutputImage, class TEdgeDetector, class TDensityCount>
class ITK_EXPORT EdgeDensityImageFilter : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{

public:
  /** Standard class typedefs. */
  typedef EdgeDensityImageFilter Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(EdgeDensityImageFilter, itk::ImageToImageFilter);

  /** Template parameters typedefs*/
  typedef TInputImage                        InputImageType;
  typedef typename InputImageType::Pointer   InputImagePointerType;
  typedef typename InputImageType::PixelType InputImagePixelType;
  typedef typename InputImageType::SizeType  InputImageSizeType;

  /** OutputImageType typedef support*/
  typedef TOutputImage                         OutputImageType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;
  typedef typename OutputImageType::Pointer    OutputImagePointerType;

  /** Edge Detector typedef Support*/
  typedef TEdgeDetector                  DetectorType;
  typedef typename DetectorType::Pointer DetectorPointerType;

  /** Count Density Function typedef support*/
  typedef TDensityCount DensityCountFunctionType;

  /** PointSetToDensityImageFilter support*/
  typedef otb::BinaryImageToDensityImageFilter<InputImageType, OutputImageType, DensityCountFunctionType> DensityImageType;

  typedef typename DensityImageType::Pointer DensityImagePointerType;

  /** Get/Set the radius of the neighborhood over which the
  statistics are evaluated */
  itkSetMacro(NeighborhoodRadius, InputImageSizeType);
  itkGetConstReferenceMacro(NeighborhoodRadius, InputImageSizeType);
  void SetNeighborhoodRadius(unsigned int rad)
  {
    m_NeighborhoodRadius.Fill(rad);
    this->Modified();
  }

  /**Set/Get detector */
  itkSetObjectMacro(Detector, DetectorType);
  itkGetObjectMacro(Detector, DetectorType);
  itkGetObjectMacro(DensityImageFilter, DensityImageType);

protected:
  /**
   * Constructor.
   */
  EdgeDensityImageFilter();
  /**
   * Destructor.
   */
  ~EdgeDensityImageFilter() override;
  /**
   * Standard PrintSelf method.
   */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;
  /**
   * Main computation method.
   */
  void GenerateData() override;

private:
  EdgeDensityImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  DetectorPointerType     m_Detector;
  DensityImagePointerType m_DensityImageFilter;
  InputImageSizeType      m_NeighborhoodRadius;
};
}
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbEdgeDensityImageFilter.hxx"
#endif

#endif
