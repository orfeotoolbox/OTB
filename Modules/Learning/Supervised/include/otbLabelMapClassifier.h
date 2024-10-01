/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbLabelMapClassifier_h
#define otbLabelMapClassifier_h

#include "itkInPlaceLabelMapFilter.h"
#include "otbMachineLearningModel.h"
#include "itkListSample.h"
#include "otbAttributesMapLabelObject.h"

namespace otb
{

/** \class LabelMapClassifier
 * \brief Classify each LabelObject of the input LabelMap in place
 *
 * \sa otb::AttributesMapLabelObject
 * \sa otb::SVMModel
 * \sa itk::InPlaceLabelMapFilter
 *
 * \ingroup OTBSupervised
 */
template <class TInputLabelMap>
class ITK_EXPORT LabelMapClassifier : public itk::InPlaceLabelMapFilter<TInputLabelMap>
{
public:
  /** Standard class typedefs. */
  typedef LabelMapClassifier                         Self;
  typedef itk::InPlaceLabelMapFilter<TInputLabelMap> Superclass;
  typedef itk::SmartPointer<Self>                    Pointer;
  typedef itk::SmartPointer<const Self>              ConstPointer;

  /** Some convenient typedefs. */
  typedef TInputLabelMap                         LabelMapType;
  typedef typename LabelMapType::Pointer         LabelMapPointer;
  typedef typename LabelMapType::ConstPointer    LabelMaponstPointer;
  typedef typename LabelMapType::LabelObjectType LabelObjectType;

  typedef typename LabelObjectType::AttributesValueType AttributesValueType;
  typedef typename LabelObjectType::ClassLabelType      ClassLabelType;

  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputLabelMap::ImageDimension);

  /** Type definitions for the learning model. */
  typedef MachineLearningModel<AttributesValueType, ClassLabelType> ModelType;
  typedef typename ModelType::Pointer         ModelPointer;
  typedef typename ModelType::InputSampleType MeasurementVectorType;
  typedef Functor::AttributesMapMeasurementFunctor<LabelObjectType, MeasurementVectorType> MeasurementFunctorType;

  /** Standard New method. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(LabelMapClassifier, itk::InPlaceLabelMapFilter);

  itkSetObjectMacro(Model, ModelType);

  void SetMeasurementFunctor(const MeasurementFunctorType& functor)
  {
    m_MeasurementFunctor = functor;
  }

  MeasurementFunctorType& GetMeasurementFunctor()
  {
    return m_MeasurementFunctor;
  }

protected:
  LabelMapClassifier();
  ~LabelMapClassifier() override = default;

  void ThreadedProcessLabelObject(LabelObjectType* labelObject) override;

  void ReleaseInputs() override;


private:
  LabelMapClassifier(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** The learning model used for classification */
  ModelPointer m_Model;

  /** The functor used to build the measurement vector */
  MeasurementFunctorType m_MeasurementFunctor;

}; // end of class

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLabelMapClassifier.hxx"
#endif

#endif
