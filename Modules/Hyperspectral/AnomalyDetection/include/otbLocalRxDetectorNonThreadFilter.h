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


#ifndef otbLocalRxDetectorNonThreadFilter_h
#define otbLocalRxDetectorNonThreadFilter_h

#include "itkImageToImageFilter.h"

#include "itkConstShapedNeighborhoodIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkImageRegionIterator.h"
#include "itkListSample.h"
#include "itkCovarianceSampleFilter.h"

namespace otb
{

/** \class otbLocalRxDetectorNonThreadFilter
 * \brief Local-RX detector algorithm with multichannel VectorImage data as input
 *
 *
 * \ingroup ImageFilters
 *
 * \ingroup OTBAnomalyDetection
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT LocalRxDetectorNonThreadFilter:
    public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:

  /** Standard class typedefs. */
  typedef LocalRxDetectorNonThreadFilter                         Self;
  typedef itk::ImageToImageFilter< TInputImage, TOutputImage >   Superclass;
  typedef itk::SmartPointer<Self>                                Pointer;
  typedef itk::SmartPointer<const Self>                          ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(LocalRxDetectorNonThreadFilter, ImageToImageFilter);

  /** typedef related to input and output images */
  typedef TInputImage                                InputImageType;
  typedef typename InputImageType::Pointer           InputPointerType;
  typedef typename InputImageType::ConstPointer      InputConstPointerType;
  typedef typename InputImageType::IndexType         InputIndexType;
  typedef typename InputImageType::SizeType          InputSizeType;

  typedef TOutputImage                               OutputImageType;
  typedef typename OutputImageType::Pointer          OutputPointerType;
  typedef typename OutputImageType::IndexType        OutputIndexType;
  typedef typename OutputImageType::OffsetType       OutputOffsetType;
  typedef typename OutputImageType::SizeType         OutputSizeType;
  typedef typename OutputImageType::RegionType       OutputImageRegionType;

  /** typedef related to iterators */
  typedef itk::ConstShapedNeighborhoodIterator<InputImageType>                       ConstShapedNeighborhoodIteratorType;
  typedef itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType>   VectorFaceCalculatorType;
  typedef itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<OutputImageType>  FaceCalculatorType;
  typedef itk::ImageRegionIterator<OutputImageType>                                  ImageRegionIteratorType;

  /** typedef related to statistics */
  typedef typename InputImageType::PixelType                      VectorMeasurementType;
  typedef itk::Statistics::ListSample<VectorMeasurementType>      ListSampleType;
  typedef itk::Statistics::CovarianceSampleFilter<ListSampleType> CovarianceCalculatorType;
  typedef typename CovarianceCalculatorType::MeasurementVectorRealType    MeasurementVectorRealType;
  typedef typename CovarianceCalculatorType::MatrixType                   MatrixType;

  /** Getter and Setter */
  itkSetMacro(InternalRadius, int);
  itkGetMacro(InternalRadius, int);
  itkSetMacro(ExternalRadius, int);
  itkGetMacro(ExternalRadius, int);

  /** Main computation method */
  void GenerateInputRequestedRegion() override;
  void GenerateData() override;
//       virtual void BeforeThreadedGenerateData();
//       virtual void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);


protected:
  LocalRxDetectorNonThreadFilter();
  ~LocalRxDetectorNonThreadFilter() override {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  LocalRxDetectorNonThreadFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  int m_InternalRadius;
  int m_ExternalRadius;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLocalRxDetectorNonThreadFilter.hxx"
#endif

#endif
