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

#ifndef otbKeyPointDensityImageFilter_h
#define otbKeyPointDensityImageFilter_h

#include "itkImageToImageFilter.h"
#include "otbPointSetToDensityImageFilter.h"
#include "itkNumericTraits.h"

namespace otb
{

/** \class KeyPointDensityImageFilter
 *  \brief This class computes the density of the characteristic
 *  points detected in the image.
 *
 * The density computed is a ratio of the number of keypoints over
 * the total number of pixels within a region.
 * It uses the class PointSetToDensityImageFilter to do so.
 *
 * This class use the key point detector specified in the 3rd argument
 * template to detect the keypoints. Then for every pixel, the density of the key points
 * within its neighborhood is computed.
 *
 * The output is a map of key points density.
 *
 * \ingroup OTBDensity
 */

template <class TInputImage, class TOutputImage, class TDetector>
class ITK_EXPORT KeyPointDensityImageFilter : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{

public:
  /** Standard class typedefs. */
  typedef KeyPointDensityImageFilter Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(KeyPointDensityImageFilter, itk::ImageToImageFilter);

  /** Template parameters typedefs*/
  typedef TInputImage                      InputImageType;
  typedef typename InputImageType::Pointer InputImagePointerType;

  /** OutputImageType typedef support*/
  typedef TOutputImage                      OutputImageType;
  typedef typename OutputImageType::Pointer OutputImagePointerType;

  /** Detector typedef Support*/
  typedef TDetector                                 DetectorType;
  typedef typename DetectorType::OutputPointSetType PointSetType;
  typedef typename DetectorType::Pointer            DetectorPointerType;

  /** PointSetToDensityImageFilter support*/
  typedef otb::PointSetToDensityImageFilter<PointSetType, OutputImageType> PointSetToDensityImageType;
  typedef typename PointSetToDensityImageType::Pointer PointSetToDensityImagePointerType;

  /** Get/Set the radius of the neighborhood over which the
  statistics are evaluated */
  itkSetMacro(NeighborhoodRadius, unsigned int);
  itkGetConstReferenceMacro(NeighborhoodRadius, unsigned int);

  /**Set/Get Descriptor from the otbCountmageFunction*/
  virtual void SetDetector(DetectorType* detector);
  virtual DetectorType* GetDetector();

protected:
  /**
   * Constructor.
   */
  KeyPointDensityImageFilter();
  /**
   * Destructor.
   */
  ~KeyPointDensityImageFilter() override;
  /**
   * Standard PrintSelf method.
   */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;
  /**
   * Main computation method.
   */
  // virtual void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId );
  void GenerateData() override;

private:
  KeyPointDensityImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  DetectorPointerType               m_Detector;
  PointSetToDensityImagePointerType m_PointSetToDensityImageFilter;
  unsigned int                      m_NeighborhoodRadius;
};
}
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbKeyPointDensityImageFilter.hxx"
#endif

#endif
