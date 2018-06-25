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

#ifndef otbEdgeDetectorImageFilter_h
#define otbEdgeDetectorImageFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include "itkImageToImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"

namespace otb
{

/** \class EdgeDetectorImageFilter
 *  \brief This composite filter binaries a edge detection image output.
 *  The used edge detection filter is given as template of the class.
 *  The class only supports Image.
 *
 *
 * \ingroup OTBEdge
 */

template <class TInputImage, class TOutputImage, class TEdgeDetection>
class ITK_EXPORT EdgeDetectorImageFilter
  : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{

public:

  /** Standard class typedefs. */
  typedef EdgeDetectorImageFilter                            Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(EdgeDetectorImageFilter, itk::ImageToImageFilter);

  /** Template parameters typedefs*/
  typedef TInputImage                          InputImageType;
  typedef TOutputImage                         OutputImageType;
  typedef typename InputImageType::Pointer     InputImagePointerType;
  typedef typename InputImageType::PixelType   InputImagePixelType;
  typedef typename InputImageType::SizeType    InputImageSizeType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;
  typedef typename OutputImageType::Pointer    OutputImagePointerType;
  typedef typename OutputImageType::PixelType  OutputImagePixelType;

  /** Edge Detection */
  typedef TEdgeDetection                  DetectionType;
  typedef typename DetectionType::Pointer DetectionPointerType;

  /** Thresholder filter : Otsu */
  typedef itk::BinaryThresholdImageFilter<InputImageType, InputImageType> BinaryFilterType;
  typedef typename BinaryFilterType::Pointer                              BinaryFilterPointerType;

  /**Set/Get detector */
  itkSetObjectMacro(Detector, DetectionType);
  itkGetObjectMacro(Detector, DetectionType);
  itkGetConstObjectMacro(Detector, DetectionType);
  /**Set/Get binary filter */
  itkSetObjectMacro(BinaryFilter, BinaryFilterType);
  itkGetObjectMacro(BinaryFilter, BinaryFilterType);
  //itkGetObjectMacro(DetectorImageFilter, DetectorImageType);

  /** Set lower threshold value. */
  void SetLowerThreshold(InputImagePixelType val)
  {
    m_BinaryFilter->SetLowerThreshold(val);
    this->Modified();
  }
  /** Set upper threshold value. */
  void SetUpperThreshold(InputImagePixelType val)
  {
    m_BinaryFilter->SetUpperThreshold(val);
    this->Modified();
  }
  /** Set Inside value. */
  void SetInsideValue(InputImagePixelType val)
  {
    m_BinaryFilter->SetInsideValue(val);
    this->Modified();
  }
  /** Set Outside value. */
  void SetOutsideValue(InputImagePixelType val)
  {
    m_BinaryFilter->SetOutsideValue(val);
    this->Modified();
  }
protected:

  EdgeDetectorImageFilter();
  ~EdgeDetectorImageFilter() override;
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;
  void GenerateData() override;

private:

  EdgeDetectorImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

  DetectionPointerType    m_Detector;
  BinaryFilterPointerType m_BinaryFilter;
};
}
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbEdgeDetectorImageFilter.hxx"
#endif

#endif
