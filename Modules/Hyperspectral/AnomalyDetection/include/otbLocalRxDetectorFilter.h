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


#ifndef otbLocalRxDetectorFilter_h
#define otbLocalRxDetectorFilter_h

#include "itkImageToImageFilter.h"

#include "itkListSample.h"
#include "itkCovarianceSampleFilter.h"
#include "itkConstNeighborhoodIterator.h"
#include "otbVectorImage.h"

namespace otb
{

/** \class LocalRxDetectionFunctor
 * \brief This functor computes a local Rx score on an input neighborhood. Pixel of the neighborhood
 * inside the internal radius are not considered during the computation of local statistics.
 *
 * \ingroup ImageFilters
 *
 * \ingroup OTBAnomalyDetection
 */
namespace Functor
{
template<typename TInput, typename TOutput =TInput> 
class LocalRxDetectionFunctor
{
public:

  /** typedef */
  typedef typename itk::Neighborhood<itk::VariableLengthVector<TInput>>::OffsetType OffsetType;
  typedef typename itk::VariableLengthVector<TInput>                         VectorMeasurementType;
  typedef itk::Statistics::ListSample<VectorMeasurementType>            ListSampleType;
  typedef itk::Statistics::CovarianceSampleFilter<ListSampleType>       CovarianceCalculatorType;
  typedef typename CovarianceCalculatorType::MeasurementVectorRealType  MeasurementVectorRealType;
  typedef typename CovarianceCalculatorType::MatrixType                 MatrixType;

private:
  // Internal radius along the X axis
  unsigned int m_InternalRadiusX;

  // Internal radius along the Y axis
  unsigned int m_InternalRadiusY;

public:
  LocalRxDetectionFunctor() : m_InternalRadiusX(1), m_InternalRadiusY(1) {};

  void SetInternalRadius(const unsigned int internalRadiusX, const unsigned int internalRadiusY)
  {
    m_InternalRadiusX = internalRadiusX;
    m_InternalRadiusY = internalRadiusY;
  };

  int GetInternalRadiusX()
  {
    return m_InternalRadiusX;
  };

  int GetInternalRadiusY()
  {
    return m_InternalRadiusY;
  };

  auto operator()(const itk::ConstNeighborhoodIterator<otb::VectorImage<TInput>> & in) const
  {
    // Create a list sample with the pixels of the neighborhood located between
    // the two radius.
    typename ListSampleType::Pointer listSample = ListSampleType::New();

    // The pixel on whih we will compute the Rx score, we load it now to get the input vector size.
    auto centerPixel = in.GetCenterPixel();
    listSample->SetMeasurementVectorSize(centerPixel.Size());

    OffsetType off;

    auto externalRadius = in.GetRadius();
    
    // Cache radiuses attributes for threading performances
    const int internalRadiusX = m_InternalRadiusX;
    const int internalRadiusY = m_InternalRadiusY;
    
    // Cache radiuses attributes for threading performances
    const int externalRadiusX = static_cast<int>(externalRadius[0]);
    const int externalRadiusY = static_cast<int>(externalRadius[0]);

    for (int y = -externalRadiusY; y <= externalRadiusY; y++)
      {
      off[1] = y;
      for (int x = -externalRadiusX; x <= externalRadiusX; x++)
        {
        off[0] = x;
        if ((abs(x) > internalRadiusX) || (abs(y) > internalRadiusY))
          {
            listSample->PushBack(in.GetPixel(off));
          }
        }
      }

    // Compute mean & inverse covariance matrix
    typename CovarianceCalculatorType::Pointer covarianceCalculator = CovarianceCalculatorType::New();
    covarianceCalculator->SetInput(listSample);
    covarianceCalculator->Update();

    MeasurementVectorRealType meanVector = covarianceCalculator->GetMean();
    
    VectorMeasurementType meanVec(meanVector.GetNumberOfElements());
    for(unsigned int i = 0; i < meanVector.GetNumberOfElements(); ++i)
      {
      meanVec.SetElement(i, meanVector.GetElement(i));
      }

    MatrixType covarianceMatrix = covarianceCalculator->GetCovarianceMatrix();
    typename MatrixType::InternalMatrixType invCovMat = covarianceMatrix.GetInverse();

    typename MatrixType::InternalMatrixType centeredTestPixMat(meanVector.GetNumberOfElements(), 1);
    
    for (unsigned int i = 0; i < centeredTestPixMat.rows(); ++i)
      {
      centeredTestPixMat.put(i, 0, (centerPixel.GetElement(i) - meanVector.GetElement(i)));
      }

    // Rx score computation
    typename MatrixType::InternalMatrixType rxValue 
      = centeredTestPixMat.transpose() * invCovMat * centeredTestPixMat;

    return static_cast<TOutput> (rxValue.get(0, 0));
  }

};

} // end namespace functor
} // end namespace otb

#endif
