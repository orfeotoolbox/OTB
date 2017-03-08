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

#ifndef otbSVMPointSetModelEstimator_h
#define otbSVMPointSetModelEstimator_h

#include "vcl_deprecated_header.h"

#include "otbSVMModelEstimator.h"

namespace otb
{

/** \class SVMPointSetModelEstimator

 * \brief Class for SVM model
 * estimation from PointSets used for classification.
 *
 *
 * The basic functionality of the SVMPointSetModelEstimator is to
 * generate the models used in SVM classification. It
 * requires one input poinset and a training pointset to be provided by the
 * user. This object supports data handling of multiband data. The
 * object accepts the input image as a VectorImage only, where each
 * pixel is a vector and each element of the vector corresponds to an
 * entry from 1 particular band of a multiband dataset. A single band
 * data set is treated as a vector with a single element for every
 * vector. The classified data is treated as a single band scalar
 * data.
 *
 * \deprecated
 *
 * \sa MachineLearningModel
 * \sa LibSVMMachineLearningModel
 * \sa ImageClassificationFilter
 *
 * \ingroup ClassificationFilters
 *
 * \ingroup OTBSVMLearning
 */
template <class TInputPointSet, class TTrainingPointSet>
class ITK_EXPORT SVMPointSetModelEstimator :
  public SVMModelEstimator<typename TInputPointSet::PixelType::value_type, typename TTrainingPointSet::PixelType>
{
public:
  /** Standard class typedefs. */
  typedef SVMPointSetModelEstimator Self;
  typedef SVMModelEstimator<typename TInputPointSet::PixelType::value_type,
      typename TTrainingPointSet::PixelType> Superclass;
  typedef itk::SmartPointer<Self>
  Pointer;
  typedef itk::SmartPointer<const Self>
  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SVMPointSetModelEstimator, SVMModelEstimator);

  /** Type definition for the input image. */
  typedef typename TInputPointSet::Pointer InputPointSetPointer;

  /** Type definitions for the training image. */
  typedef typename TTrainingPointSet::Pointer TrainingPointSetPointer;

  /** Type definition for the vector associated with
   * input image pixel type. */
  typedef typename TInputPointSet::PixelType InputPointSetPixelType;
  typedef typename TInputPointSet::PointType InputPointType;

  /** Type definitions for the vector holding
   * training image pixel type. */
  typedef typename TTrainingPointSet::PixelType TrainingPointSetPixelType;

  /** Type definitions for the iterators for the input and training images. */
  typedef typename
  TInputPointSet::PointsContainerConstIterator InputPointSetIteratorType;
  typedef typename
  TTrainingPointSet::PointsContainerConstIterator TrainingPointSetIteratorType;

  /** Set the input image */
  void SetInputPointSet(const TInputPointSet * inputPointSet);

  /** Set the training image */
  void SetTrainingPointSet(const TTrainingPointSet * trainingPointSet);

  /** Get the input image */
  const TInputPointSet * GetInputPointSet();

  /** Get the training image */
  const TTrainingPointSet * GetTrainingPointSet();

protected:
  /** Constructor */
  SVMPointSetModelEstimator();
  /** Destructor */
  ~SVMPointSetModelEstimator() ITK_OVERRIDE;
  /** PrintSelf */
  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;
  /** Prepare data */
  void PrepareData() ITK_OVERRIDE;

private:
  SVMPointSetModelEstimator(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

}; // class SVMPointSetModelEstimator

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSVMPointSetModelEstimator.txx"
#endif

#endif
