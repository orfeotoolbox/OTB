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

#ifndef otbNormalBayesMachineLearningModel_h
#define otbNormalBayesMachineLearningModel_h

#include "otbRequiresOpenCVCheck.h"

#include "itkLightObject.h"
#include "itkFixedArray.h"
#include "otbMachineLearningModel.h"

#include "otbOpenCVUtils.h"

namespace otb
{
template <class TInputValue, class TTargetValue>
class ITK_EXPORT NormalBayesMachineLearningModel : public MachineLearningModel<TInputValue, TTargetValue>
{
public:
  /** Standard class typedefs. */
  typedef NormalBayesMachineLearningModel Self;
  typedef MachineLearningModel<TInputValue, TTargetValue> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef typename Superclass::InputValueType       InputValueType;
  typedef typename Superclass::InputSampleType      InputSampleType;
  typedef typename Superclass::InputListSampleType  InputListSampleType;
  typedef typename Superclass::TargetValueType      TargetValueType;
  typedef typename Superclass::TargetSampleType     TargetSampleType;
  typedef typename Superclass::TargetListSampleType TargetListSampleType;
  typedef typename Superclass::ConfidenceValueType  ConfidenceValueType;
  typedef typename Superclass::ProbaSampleType      ProbaSampleType;
  /** Run-time type information (and related methods). */
  itkNewMacro(Self);
  itkTypeMacro(NormalBayesMachineLearningModel, MachineLearningModel);

  /** Train the machine learning model */
  void Train() override;

  /** Save the model to file */
  void Save(const std::string& filename, const std::string& name = "") override;

  /** Load the model from file */
  void Load(const std::string& filename, const std::string& name = "") override;

  /**\name Classification model file compatibility tests */
  //@{
  /** Is the input model file readable and compatible with the corresponding classifier ? */
  bool CanReadFile(const std::string&) override;

  /** Is the input model file writable and compatible with the corresponding classifier ? */
  bool CanWriteFile(const std::string&) override;
  //@}

protected:
  /** Constructor */
  NormalBayesMachineLearningModel();

  /** Destructor */
  ~NormalBayesMachineLearningModel() override = default;

  /** Predict values using the model */
  TargetSampleType DoPredict(const InputSampleType& input, ConfidenceValueType* quality = nullptr, ProbaSampleType* proba = nullptr) const override;

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  NormalBayesMachineLearningModel(const Self&) = delete;
  void operator=(const Self&) = delete;
  cv::Ptr<cv::ml::NormalBayesClassifier> m_NormalBayesModel;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbNormalBayesMachineLearningModel.hxx"
#endif

#endif
