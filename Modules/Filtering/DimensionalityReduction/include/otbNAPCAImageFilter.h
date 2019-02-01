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

#ifndef otbNAPCAImageFilter_h
#define otbNAPCAImageFilter_h

#include "otbMNFImageFilter.h"

namespace otb {

/** \class NAPCAImageFilter
 * \brief Performs a Noise Adjusted PCA analysis of a vector image.
 *
 * The internal structure of this filter is a filter-to-filter like structure.
 * The estimation of the covariance matrix has persistent capabilities...
 *
 * The high pass filter which has to be used for the noise estimation is templated
 * for a better scalability...
 *
 * This class is very similar to the MNFImageFilter one since only the projection
 * matrix definition (through GenerateTransformationMatrix function)
 * differs.
 *
 * TODO? Use a 2nd input to give a noise image directly?
 *
 * \sa otbStreamingStatisticsVectorImageFilter
 * \sa MNFImageFilter
 *
 * \ingroup OTBDimensionalityReduction
 */
template <class TInputImage, class TOutputImage,
            class TNoiseImageFilter,
            Transform::TransformDirection TDirectionOfTransformation >
class ITK_EXPORT NAPCAImageFilter
  : public MNFImageFilter< TInputImage, TOutputImage, TNoiseImageFilter, TDirectionOfTransformation >
{
public:
  typedef NAPCAImageFilter Self;
  typedef MNFImageFilter< TInputImage, TOutputImage,
            TNoiseImageFilter, TDirectionOfTransformation > Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(NAPCAImageFilter, MNFImageFilter);

  /** Template parameters typedefs */
  typedef typename Superclass::InputImageType InputImageType;
  typedef typename Superclass::OutputImageType OutputImageType;

  typedef typename Superclass::CovarianceEstimatorFilterType CovarianceEstimatorFilterType;
  typedef typename Superclass::CovarianceEstimatorFilterPointerType CovarianceEstimatorFilterPointerType;

  typedef typename Superclass::RealType RealType;
  typedef typename Superclass::VectorType VectorType;
  typedef typename Superclass::MatrixObjectType MatrixObjectType;
  typedef typename Superclass::MatrixType MatrixType;
  typedef typename Superclass::InternalMatrixType InternalMatrixType;
  typedef typename Superclass::MatrixElementType MatrixElementType;

  typedef typename Superclass::TransformFilterType TransformFilterType;
  typedef typename Superclass::TransformFilterPointerType TransformFilterPointerType;

  typedef typename Superclass::NoiseImageFilterType NoiseImageFilterType;
  typedef typename Superclass::NoiseImageFilterPointerType NoiseImageFilterPointerType;

  typedef typename Superclass::NormalizeFilterType NormalizeFilterType;
  typedef typename Superclass::NormalizeFilterPointerType NormalizeFilterPointerType;

protected:
  NAPCAImageFilter() { }
  ~NAPCAImageFilter () override { }

  /** Specific functionality of NAPCA */
  void GenerateTransformationMatrix() override;
}; // end of class

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbNAPCAImageFilter.hxx"
#endif

#endif

